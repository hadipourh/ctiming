#include "timing.h" // It is needed to measure clock cycles
#include <stdio.h>  // It is needed to use printf 
#include <stdint.h> // It is needed to use uint64_t
#include <sys/random.h>

/*
Include the ciphers
In this example we compare the speed of two different implementations of AES block cipher.
One implementation is based on lookup tables and another one employs AES New Instructions (Intel AES-NI)
*/

#include "aesni.h"

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
    printf("Timing overhead: %lu clock cycles\n", timing_overhead);

    /*
    Compute the length of processed data 
    */
    int byte_length_of_processed_data = 16;
    double rate;

    /*
    Measure the speed of table-based implementation of aes
    */
    uint8_t master_key[16];
    uint8_t plaintext[16];
    uint8_t ciphertext[16];
    for (int i = 0; i < 16; i++){
        master_key[i] = 0xff & 0xff;
        plaintext[i] = 0xff & 0xff;       
    }
    __m128i key_schedule[20];
    aes128_load_key(master_key, key_schedule);
    int status;
    status = aes128_self_test();
    if (status != 0){
        printf("%s\n", "AES-NI does not work correctly!");
        return 0;
    } else {
        printf("%s\n", "AES-NI works correctly");
    }
    uint64_t *cycles = (uint64_t *)malloc(NUM_TIMINGS * sizeof(uint64_t));
    uint64_t temp;
    for (uint64_t i = 0; i < NUM_TIMINGS; i++){
        temp = start_timer();
        aes128_enc(key_schedule, plaintext, ciphertext);
        temp = end_timer() - temp;
        cycles[i] = temp;
    }    
    qsort(cycles, NUM_TIMINGS, sizeof(uint64_t), compare_u64);
    rate = (double)(cycles[NUM_TIMINGS / 2] - timing_overhead) / byte_length_of_processed_data;
    printf("Speed of AES-NI: %0.02f [Clock cycles]/[Byte]\n", rate);
    free(cycles);
    return 0;
}
