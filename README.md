Raspberry PI direct GPIO access using C / C# programming language
===

Raspberry Pi Public code

Direct GPIO access for Raspberry 2, 3 and 4
C Library and C# Wrapper Library

The Periphal Base Register depends on PI Model

Comment/uncomment based on your RPI model

// RPI Model B+ V1.2 (2014)

//#define BCM_PERI_BASE 0x20000000

// RPI 3 Model B+ (2017)

//#define BCM_PERI_BASE 0x3F000000

// RPI 4 Model B (2018)

#define BCM_PERI_BASE 0xFE000000
