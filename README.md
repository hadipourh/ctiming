# Performance Measurement in C/C++

This repository provides a brief introduction to the performance measurement of encryption 
algorithms in C/C++.

---
- [Performance Measurement in C/C++](#performance-measurement-in-cc)
  - [Measuring CPU Cycles](#measuring-cpu-cycles)
  - [Hardware/Software Setup](#hardwaresoftware-setup)
  - [AES](#aes)
  - [AES-NI](#aes-ni)
  - [References](#references)
  
## Measuring CPU Cycles

To get the number of clock cycles required for running a process, we can use the [Time Stamp Counter (TSC)](https://en.wikipedia.org/wiki/Time_Stamp_Counter). TSC counts the number of CPU clock cycles since its last reset. Measuring the performance based on CPU clock cycles is more accurate than measuring based on time since it is independent of CPU clock frequency.

In C language, we can use `RDTSC` (and `RDTSCP`) to get the value of TSC. To see the more detail see [timing.h](aesni/timing.h).

Modern processors have multiple CPU cores. The operating system (OS) assigns each task to a specific core. 
Many processes are running when OS is up, and OS may move the execution of a single process from one core to another while the corresponding program is running. 
This arises a problem for programs that use the time stamp counter as each core has its counter, which is not synchronized with the other CPU cores. 
To prevent this issue, we can lock a program to a single core while measuring the performance. 
To this end, we can use the `taskset` command in Linux. 
For example, to lunch program `./test` on CPU core 0, we use the following command:

```sh
taskset 0x1 ./test
```

The power-saving mode in modern processors is another feature that may influence the accuracy of our performance measurement. Thanks to this feature, the CPU reduces the clock frequency when there is a low CPU load. Thus, we would prefer to disable the power-saving feature of a particular core while we employ it to measure the performance of our algorithm. For example, in Linux, we can disable the power-saving mode of the CPU core 0 using the following command:

```sh
echo performance > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
```

To show how we measure the performance of an encryption algorithm, we have chosen two implementations of AES. One of them follows the table-based implementation approach, and another employs the AES New Instructions (AES-NI).

## Hardware/Software Setup

We used a laptop with the following configurations to perform our experiments.

**CPU**

```sh
lscpu
...
Model name:                      11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz
...
```

**OS**

```sh
uname
Linux ipn012 5.15.5-051505-generic #202111250933 SMP Thu Nov 25 09:37:51 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux
```

## AES

This example is a table-based implementation of AES in C language borrowed from [crypto-algorithms](https://github.com/B-Con/crypto-algorithms).

To build and run the performance measurement, navigate into [aes](aes) folder run the following command:

```
make
taskset 0x1 ./test
```

Output:

```
[x] PRNG initialized by 4 random bytes: 0x251C1250
Timing overhead: 22
Speed of AES: 47.69 [Clock cycles]/[Byte]
```

## AES-NI

This example which is borrowed from [aes-brute-force](https://github.com/sebastien-riou/aes-brute-force/blob/master/include/aes_ni.h), is an implementation of AES in C language that employs the [AES New Instructions (AES-NI)](library/Gueron2009_Chapter_IntelSNewAESInstructionsForEnh.pdf).

To build and run the performance measurement, navigate into [aesni](aesni) folder run the following command:

```
make
taskset 0x1 ./test
```

Output:

```
[x] PRNG initialized by 4 random bytes: 0x9001BE8E
Timing overhead: 25 clock cycles
AES-NI works correctly
Speed of AES-NI: 1.25 [Clock cycles]/[Byte]
```

It can be seen that employing AES-NI results in a way better performance.

## References

[1]- [Analysis of Recent Attacks on AES](library/137389_gstir_david_2012.pdf)

[2]- [Breakthrough AES Performance with Intel® AES New Instructions](library/10tb24-breakthrough-aes-performance-with-intel-aes-new-instructions-final-secure-165940.pdf)

[3]- [Intel’s New AES Instructions for Enhanced
Performance and Security](library/Gueron2009_Chapter_IntelSNewAESInstructionsForEnh.pdf)

