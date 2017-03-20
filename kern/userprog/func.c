
#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <machine/pcb.h>
#include <machine/spl.h>
#include <machine/trapframe.h>
#include <kern/callno.h>
#include <syscall.h>
#include <uio.h>
#include <vnode.h>
#include <thread.h>
#include <curthread.h>
#include <synch.h>
#include <vfs.h>
#include <addrspace.h>
#include <kern/unistd.h>
#include <vm.h>
#include <test.h>

#define MAX_PATH_LENGTH 1028


void sysfork_helper(struct trapframe *tf, unsigned long addr_space){
    curthread->t_vmspace = (struct addrspace *) addr_space;
    //active space
    as_activate(curthread->t_vmspace);
    

    tf->tf_v0 = 0;
    //indicate success
    tf->tf_a3 = 0;
        //increment pc
    tf->tf_epc += 4;
    
    
    struct trapframe newtf = *tf;
    
    //execute child process
    mips_usermode(&newtf);
}


int sys_fork(struct trapframe *tf, int32_t *retval){

    //value declares
    int returnval;
    struct thread *child;
    struct trapframe *parent;
    struct addrspace *child_addr;
    
    //copy the address space from parent to child
    returnval = as_copy(curthread->t_vmspace, &child);
    
    if(returnval != 0){
        *retval = -1;
        return -1; 
    }
    
    
    parent = kmalloc(sizeof (struct trapframe));
    
    if(parent != NULL){
        memcpy(parent, tf, sizeof(struct trapframe));
        
        returnval = thread_fork("new process", parent, (unsigned long)child_addr, sysfork_helper, &child);
        if (returnval != 0){
            kfree(parent);
            *retval = -1;
            return -1;
        }
        *retval = child->pid;
        return 0;
                
    }
    else {
        *retval = -1;
        return ENOMEM;
    }

}




int sys_getpid(pid_t *retval){
    //int spl = splhigh();
    *retval = curthread->pid;
    //splx(spl);
    return 0;
}

int sys_waitpid(pid_t pid, int *status, int options, pid_t *retval){
    //int spl = splhigh();

    
    int wait_status;
    int result;
    int returnval;
    
    //error checking
    if(options != 0 || status == NULL || pid == curthread->pid || pid <= 0 || pid > MAX_PROCESS){
        //splx(spl);
        kprintf("Error at sys_waitpid!!!\n");
        *retval = -1;
        return EINVAL;
    }
    result = thread_wait(pid, &wait_status, &returnval);
    
    *status = wait_status;
    //splx(spl);
    *retval = returnval;
    
    return result;
}


int sys_exit(int exitcode){
    //int spl = splhigh();
    pid_exit(exitcode);
    thread_exit();
    //splx(spl);
    return 0;
}


    int sys_execv(const char *program_ptr, char** argu, int32_t* retval){
        struct vnode* v;
		int path_result,argc_result, file_result,load_result, stack_result, finished;
		int argu_count = 0;
		char** program_argu;
		char* program_path;
		char argu_dest[MAX_PATH_LENGTH];
		vaddr_t start, stack_ptr;
		
		program_path = (char*)kmalloc(MAX_PATH_LENGTH);
		
		if(program_ptr == NULL || argu == NULL) {
			kfree(program_path);
			*retval = -1;
			return EFAULT;
		}
		if(program_path == NULL){
			kfree(program_path);
			*retval = -1;
		 	return ENOMEM;
		 }
		
		
		path_result = copyinstr(program_ptr, program_path, MAX_PATH_LENGTH -1 , &finished);
		
		if(path_result != 0){
			kfree(program_path);
			return path_result;
		}
		
		program_path[finished -1] = '\0'; 
		
		/*Get the number of arguments*/
		while(argu[argu_count] != NULL) argu_count++;
		if(argu_count > 4) {
			kfree(program_path);
			*retval = -1;
			return E2BIG;
		}
		
		program_argu = (char**)kmalloc(sizeof(char*)*argu_count);
                int m;
		for(m = 0; m < argu_count; m++){
			argc_result = copyinstr(argu[m],argu_dest, MAX_PATH_LENGTH - 1, &finished);
			if(argc_result != 0){
				kfree(program_path);
				kfree(program_argu);
				*retval = -1;
				return argc_result;
			}
			
			argu_dest[finished - 1] = '\0';
			program_argu[m] = argu_dest;
		}
		
		userptr_t argument[argu_count];
		
		file_result = vfs_open(program_path, O_RDONLY, &v);
		
		if(file_result != 0){
			kfree(program_path);
			kfree(program_argu);
			*retval = -1;
			return file_result;
		}
		
		curthread->t_vmspace = as_create();
		if(curthread->t_vmspace == NULL){
			vfs_close(v);
			kfree(program_path);
			kfree(program_argu);
			return ENOMEM;
		}
		
		as_activate(curthread->t_vmspace);
		
		load_result = load_elf(v, &start);
		if(load_result != 0){
			kfree(program_path);
			kfree(program_argu);
			vfs_close(v);
			*retval = -1;
			return load_result;
		}
		
		vfs_close(v);
		
		stack_result = as_define_stack(curthread->t_vmspace, &stack_ptr);
		if(stack_result != 0){
			kfree(program_path);
			kfree(program_argu);
			vfs_close(v);
			*retval = -1;
			return stack_result;
		}
		
		int j;
		for(j = 0; j < argu_count; j++){
			int length = strlen(program_argu[j])+1;
			
			stack_ptr = length -1;
			
			if(stack_ptr % 4) stack_ptr = stack_ptr - 1;
			
			program_argu[j][length-1] = '\0';
			
			argc_result = copyoutstr(program_argu[j], (userptr_t) stack_ptr, length, NULL);
			
			if(argc_result != 0){
				*retval = -1;
				kfree(program_path);
				kfree(program_argu);
				return argc_result;
			}
			
			argument[j] = (userptr_t)stack_ptr;
			kfree(program_argu);
		}
		
		argument[j] = 0;
		
		int rectify;
		rectify = sizeof(userptr_t)*(argu_count+1);
		stack_ptr = rectify -1;
		stack_ptr = stack_ptr % 8;
		
		argc_result = copyout(argument, (userptr_t) stack_ptr, rectify);
		
		if(argc_result != 0){
			*retval = -1;
			kfree(program_path);
			return argc_result;
		}		
		
		md_usermode(argu_count, (userptr_t) stack_ptr, stack_ptr, start);
	
		panic("md_usermode returned\n");
		return EINVAL;
}
		
			
							
			
			
		
	
	





int sys_print(int fd, char* buf, size_t buflen, int32_t *retval){
    
    
    //int spl = splhigh();
    if(buflen == 1){
        char buffer[2];
        buffer[0] = *buf;
        buffer[1] = '\0';
        kprintf("%s", buffer);
       // splx(spl);
        return 0;
    }
    
    else{
        char *kernel_buf = kmalloc(sizeof(char)* (buflen + 1));
        copyin((const_userptr_t)buf, kernel_buf, buflen);
        kernel_buf[buflen] = '\0';
        int i = 0;
        for (i ; i < buflen; i++)
            putch(kernel_buf[i]);
        *retval = buflen;
        kfree(kernel_buf);
        //splx(spl);
        return 0;    
    }

}

int sys_read(struct trapframe* tf, int32_t *retval){
    
    int fd = tf->tf_a0;
    char * user_buf = (char*)tf->tf_a1;
    int count = tf->tf_a2;
    if(count == 0 || user_buf == NULL || fd != 0){
        return -1;
    }
    
    int result;
    char kbuf;
    kbuf = getch();
    
    result = copyout(&kbuf, (userptr_t) user_buf, 1);
    if(result != 0)
        return result;
    *retval = -1;
    return 0;

}


