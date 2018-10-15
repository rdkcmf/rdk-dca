#ifndef DCATR181_H
#define DCATR181_H

int ccsp_handler_init(void);
void ccsp_handler_exit(void);
int get_tr181param_value( const char * path_name, char* parm_value, int len);
#endif
