#ifndef TIMING_H
#define TIMING_H

#include <stdint.h> // needed to use uint64_t
#include <stdlib.h> // needed to use qsort()

#define NUM_TIMINGS (1LLU << 25)
#define OVERHEAD_LOOPS 1000000

uint64_t start_timer(void);
uint64_t end_timer(void);
uint64_t measure_overhead(void);

uint64_t start_timer(void)
{
   int a, d;

   __asm__ volatile("CPUID\n\t"
                    "RDTSC\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t"
                    : "=r" (d), "=r" (a)             // output operands
                    :                                // input operands (empty in this case)
                    : "%rax", "%rbx", "%rcx", "%rdx" // clobbered register
                    );

   return ((uint64_t)a | (((uint64_t)d) << 32));
}

uint64_t end_timer(void)
{
   int a, d;

   __asm__ volatile("RDTSCP\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t"
                    "CPUID\n\t"
                    : "=r" (d), "=r" (a)             // output operands
                    :                                // input operands (empty in this case)
                    : "%rax", "%rbx", "%rcx", "%rdx" // clobbered register
                    );

   return ((uint64_t)a | ((uint64_t)d << 32));
}

int compare_u64(const void *x, const void *y)
{
  double xx = *(uint64_t*)x, yy = *(uint64_t*)y;
  if (xx < yy) return -1;
  if (xx > yy) return  1;
  return 0;
}

// compute the measurement overhead
uint64_t measure_overhead(){
    uint64_t *cycles = (uint64_t *)malloc(OVERHEAD_LOOPS * sizeof(uint64_t));
    uint64_t temp;
    for (uint64_t i = 0; i < OVERHEAD_LOOPS; i++){
        temp = start_timer();
        temp = end_timer() - temp;
        cycles[i] = temp;
    }
    qsort(cycles, OVERHEAD_LOOPS, sizeof(uint64_t), compare_u64);
    uint64_t output = cycles[OVERHEAD_LOOPS / 2];
    free(cycles);
    return output;
}

#endif