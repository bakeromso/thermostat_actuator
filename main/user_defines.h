/*
 * user_defines.h
 *
 *  Created on: Mar 18, 2020
 *      Author: Bakeromso
 */

#ifndef MAIN_USER_DEFINES_H_
#define MAIN_USER_DEFINES_H_

/**
 * Brief:
 *
 *
 * GPIO status:
 * D0 / GPIO16: output
 * D3 / GPIO0: 	output
 */

#define GPIO_OUTPUT_IO_D0   16
#define GPIO_OUTPUT_IO_D3   0
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_D0) | (1ULL<<GPIO_OUTPUT_IO_D3))


#endif /* MAIN_USER_DEFINES_H_ */
