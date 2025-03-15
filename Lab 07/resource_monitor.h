#include <pthread.h>

#ifndef RESOURCE_MONITOR_H
#define RESOURCE_MONITOR_H

/* --- monitor operations --- */
void initMonitor(int size);
void printfBuffer();
void destroyMonitor();
void put(int value);
int take(int pos);

#endif
