#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
void child_fork(struct trapframe *tf, unsigned long addr_space);

int sys_fork(struct trapframe *tf, int32_t *retval);

int sys_getpid(pid_t *retval);

int sys_waitpid(pid_t pid, int *status, int options, pid_t *retval);

int sys_exit(int exitcode);

int sys_execv(const char *program, char **args, int32_t *retval);

int sys_print(int fd, char* buf, size_t buflen, int32_t *retval);
int sys_read(struct trapframe *tf, int32_t *retval);

int sys_sbrk(intptr_t amount, int32_t *retval);







#endif /* _SYSCALL_H_ */
