/**********************************************************************
* Filename    : timer555.c
* Description : Count the pluses procude by NE555.
* Author      : Robot
* E-mail      : support@sunfounder.com
* website     : www.sunfounder.com
* Date        : 2014/08/27
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#define  Pin0  0

static volatile int globalCounter = 0 ;
static unsigned long lastTick = 0;
static volatile int globalCounterVecchio = 0;
static volatile int stats = 0;
struct timeval tv;


void exInt0_ISR(void)  //GPIO0 interrupt service routine 
{
	++globalCounter;
        gettimeofday(&tv,NULL);
	unsigned long curTime = 1000000 * tv.tv_sec + tv.tv_usec;
	unsigned long diffTime = curTime - lastTick;        

	if(diffTime >= 1000000){
		printf("Frequenza: %d\n", globalCounter - globalCounterVecchio);	

                piLock(0);
                globalCounterVecchio =  globalCounter;
		lastTick = curTime;
		stats++;
	        piUnlock(0);
	        if((stats % 5) == 0){
        	        while(digitalRead(Pin0) == 1){}
	                gettimeofday(&tv,NULL);
	                unsigned long startLowStat = 1000000 * tv.tv_sec + tv.tv_usec;
        	        while(digitalRead(Pin0) == 0){}
	                gettimeofday(&tv,NULL);
	                unsigned long stopLowStat = 1000000 * tv.tv_sec + tv.tv_usec;
	                printf("periodo: %lu\n", stopLowStat - curTime);
        	        printf("Up: %lu\n", startLowStat - curTime);
	                printf("Down: %lu\n", stopLowStat - startLowStat);
	        }
	}
}

int main (void)
{
  if(wiringPiSetup() < 0){
  	fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
	return 1;
  }

  gettimeofday(&tv,NULL);
  lastTick =  1000000 * tv.tv_sec + tv.tv_usec; 
  wiringPiISR(Pin0, INT_EDGE_RISING, &exInt0_ISR);

  while(1){
//	printf("Current pluse number is : %d\n", globalCounter);
  }

  return 0;
}
