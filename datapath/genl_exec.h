
#ifndef GENL_EXEC_H
#define GENL_EXEC_H 1

typedef int (*genl_exec_func_t)(void *data);
int genl_exec(genl_exec_func_t func, void *data);
int genl_exec_init(void);
void genl_exec_exit(void);

#endif /* genl_exec.h */
