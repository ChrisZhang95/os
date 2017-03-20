#ifndef _FUNC_H_
#define _FUNC_H_


int sys_fork(struct trapframe *tf, int32_t retval);

void sysfork_helper(struct trapframe *tf, unsigned long addr_space);

int sys_getpid(pid_t *retval);

int sys_waitpid(pid_t pid, int *status, int options, pid_t *retval);

int sys_exit(int exitcode);

//int sys_execv(int exitcode);



#endif /* _FUNC_H_ */
