#!/bin/sh
gcc -c -fpic liboldpigpio.c
gcc -o rsl main_rsl.c -loldpigpio
gcc -c -fpic main_rsl.c
gcc -shared -o liboldpigpio.so liboldpigpio.o main_rsl.o
cp liboldpigpio.so /usr/lib
