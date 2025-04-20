#include <stdio.h>
#include <stdlib.h>

#include <pigpio.h>

/*
   gcc -pthread -o mcp3008 mcp3008.c -lpigpio
*/

int main(int argc, char *argv[])
{
   int i;
   int h;
   int v;
   int loops;
   int speed;
   double start, diff, sps;
   unsigned char buf[3];

   if (argc > 1) loops = atoi(argv[1]);
   else loops = 1000000;

   if (argc > 2) speed = atoi(argv[2]);
   else speed = 1000000;

   if (gpioInitialise() < 0) return 1;

   h = spiOpen(0, speed, 0);

   if (h < 0) return 2;

   start = time_time();

   for (i=0; i<loops; i++)
   {
      buf[0] = 1;
      buf[1] = 128;
      buf[2] = 0;

      spiXfer(h, buf, buf, 3);

      v = ((buf[1]&3)<<8) | buf[2];

      printf("%d\n", v);
   }

   diff = time_time() - start;

   fprintf(stderr, "sps=%.1f @ %d bps (%d/%.1f)\n",
      (double)loops / diff, speed, loops, diff);

   spiClose(h);

   gpioTerminate();

   return 0;
}


