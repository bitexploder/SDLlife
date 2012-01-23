#include <stdlib.h>
#include <assert.h>
#include "bench.h"

Bench *Bench_MakeBench()
{
    Bench *new_bench = malloc(sizeof(Bench));
    assert(new_bench != NULL);
    return new_bench;
}

void Bench_DestroyBench(Bench *bench)
{
    free(bench);
}

void Bench_Start(Bench *bench)
{
    gettimeofday(&bench->start, NULL);
}
void Bench_End(Bench *bench)
{
    gettimeofday(&bench->end, NULL);
}

void Bench_CalcElapsed(Bench *bench)
{
    bench->elapsed = ((bench->end.tv_sec - bench->start.tv_sec) * 1000 + (bench->end.tv_usec - bench->start.tv_usec)/1000.0);
}
