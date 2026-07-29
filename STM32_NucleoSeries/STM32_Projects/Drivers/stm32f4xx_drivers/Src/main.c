/*
 * stm32f407xx.h
 *
 *  Created on: Dec 15, 2025
 *      Author: caxel
 *
 *      - Check video no.74
 *      - Instead of creating a normal project, set:
 *      	- Targeted Project Type: Empty (since we write our own Driver)
 *      - We created the folder "drivers" and it's subfolders, it didn't exist at first
 *      - Store under "src" and "inc" the files for the driver
 *      - "Project Properties ---> C/C++ Build ---> Untick "Exclude resource from buil"
 *
 *
 */


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "stm32f407xx.h"

int main(void)
{
	for(;;);
}
