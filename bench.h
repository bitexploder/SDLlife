#include <sys/time.h>
#ifndef _bench_h
#define _bench_h

typedef struct {
   struct timeval start;
   struct timeval end;
   unsigned long elapsed;
} Bench;

Bench *Bench_MakeBench();
void Bench_DestroyBench(Bench *bench);
void Bench_Start(Bench *bench);
void Bench_End(Bench *bench);
void Bench_CalcElapsed(Bench *bench);

#endif
