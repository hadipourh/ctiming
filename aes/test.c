#include "timing.h" // It is needed to measure clock cycles
#include <stdio.h>  // It is needed to use printf 
#include <stdint.h> // It is needed to use uint64_t
#include <sys/random.h>

/*
Include the ciphers
In this example we compare the speed of two different implementations of AES block cipher.
One implementation is based on lookup tables and another one employs AES New Instructions (Intel AES-NI)
*/

#include "aes.h"

void initialize_prng() {
    unsigned int initial_seed = -1;
    ssize_t temp;
    temp = getrandom(&initial_seed, sizeof(initial_seed), 0);
    srand(initial_seed);   // Initialization, should only be called once. int r = rand();
	printf("[x] PRNG initialized by %lu random bytes: 0x%08X\n", temp, initial_seed);
}

int main(){
    /*
    Initialize PRNG
    */
    initialize_prng();
    
    /*
    Measure the overhead of timing
    */
    uint64_t timing_overhead;
    timing_overhead = measure_overhead();
    printf("Timing overhead: %lu\n", timing_overhead);

    /*
    Compute the length of processed data 
    */
    uint64_t byte_length_of_processed_data = 16;
    double rate;

    /*
    Measure the speed of table-based implementation of aes
    */
    WORD key_schedule[60];
	BYTE enc_buf[128];
	BYTE plaintext[16];
	BYTE key[32];
    for (int i = 0; i < 16; i++) {
        plaintext[i] = rand() && 0xff;
        key[i] = rand() && 0xff;
        key[i + 16] = rand() & 0xff;
    }
    aes_key_setup(key, key_schedule, 128);

    uint64_t *cycles = (uint64_t *)malloc(NUM_TIMINGS * sizeof(uint64_t));    
    uint64_t temp;
    for (uint64_t i = 0; i < NUM_TIMINGS; i++){
        temp = start_timer();
        aes_encrypt(plaintext, enc_buf, key_schedule, 128);
        temp = end_timer() - temp;
        cycles[i] = temp;
    }
    qsort(cycles, NUM_TIMINGS, sizeof(uint64_t), compare_u64);
    rate = (double)(cycles[NUM_TIMINGS / 2] - timing_overhead) / byte_length_of_processed_data;
    printf("Speed of AES: %0.02f [Clock cycles]/[Byte]\n", rate);
    return 0;
}