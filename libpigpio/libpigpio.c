// Reading material:
// http://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html
// http://elinux.org/RPi_Low-level_peripherals#Code_examples
// http://www.mono-project.com/Interop_with_Native_Libraries

// gcc -c -fpic libpigpio.c
// gcc -shared -o libpigpio.so libpigpio.o
// sudo cp libpigpio.so /lib

// Periphal Base Address depends on RPI Model

// RPI Model B+ V1.2 (2014)
//#define BCM2708_PERI_BASE 0x20000000

// RPI 3 Model B+ (2017)
//#define BCM2708_PERI_BASE 0x3F000000

// RPI 4 Model B (2018)
#define BCM2708_PERI_BASE 0xFE000000

#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_mem, *gpio_map;

volatile unsigned *gpio; // I/O access

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define GET_GPIO(g) (*(gpio + 13) & ( 1 << g )) >> g
//#define GET_GPIO *(gpio + 13)

#define GPIO_SET *(gpio+7)  // sets bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

int setup_io();
void set_in(int pin);
void set_out(int pin);
void switch_gpio(int val, int pin);
int check_gpio(int pin);


// set pin as input pin

void set_in(int pin)
{
    printf("\n LIPGPIO set_in pin %d",pin);
	INP_GPIO(pin);

} 

// set pin as output pin

void set_out(int pin)
{
  //printf("\n LIPGPIO set_out pin %d",pin);
  INP_GPIO(pin); // must use INP_GPIO before OUT_GPIO
  OUT_GPIO(pin);
}


// set output pin to val

void switch_gpio(int val, int pin)
{
  //nanosleep(200);
  if (val) GPIO_SET = 1<<pin;
  else GPIO_CLR = 1<<pin;
  //nanosleep(1);
} // switch_gpio


// read pin state

int check_gpio(int pin)
{
  return GET_GPIO(pin);
} // check_gpio


// Set up memory regions to access GPIO

int setup_io()
{

   printf("\n LIPGPIO setup-io()");

   printf("\n gpio = %x",gpio);

   if (gpio!=0)
   {
     printf("\n LIPGPIO setup-io already done. Return \n");
	 return(0);
   }

   errno = 0;  // clear error 

   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
	 return(-1);
   }

   /* mmap GPIO */

   // Allocate MAP block
   if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf(" LIPGPIO allocation error \n");
	  return(-1);
   }

   printf("\n gpio_mem = %x",gpio_mem);

   // Make sure pointer is on 4K boundary
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   // Now map it
   gpio_map = (unsigned char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   //if ((long)gpio_map < 0) {
   //   printf("\nLIPGPIO mmap error number %d\n", errno);
   //   printf("\nLIPGPIO mmap error %x\n", (int)gpio_map);
	  //return(errno);
   //}

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;

   printf("\n gpio = %x",gpio);
   return(errno);
   
} // setup_io


// release memory for gpio access

void close_io()
{
    printf("\n LIPGPIO close-io() \n");
	munmap((void*)gpio,BLOCK_SIZE);
	close(mem_fd);

}
