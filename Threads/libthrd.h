#ifndef H_LIBTHRD
#define H_LIBTHRD


void *recup_param_Thread(void *arg); 
int lanceThread(void* (*fonction) (void*), void *param1, int param2); 



#endif


