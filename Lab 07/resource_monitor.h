#include <pthread.h>

#ifndef RESOURCE_MONITOR_H
#define RESOURCE_MONITOR_H

/* --- monitor operations --- */
void initMonitor(int buffersize);
void printfBuffer();
void destroyMonitor();
void put(int value,int posid);
int take(int pos);

#endif
