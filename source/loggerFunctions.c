/*
 * @file loggerFunctions.c
 * @brief Source file that contains the logger print functions
 *
 * This source file has the logger print functions, which can print
 * strings, integers and data at memory locations
 *
 * @authors Rahul Ramaprasad, Prayag Milan Desai
 * @date November 1 2019
 * @verison 1.0
 */

#include "loggerFunctions.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


void log_enable(void)
{
	loggerEnable = true;	//Enable the logger
}


void log_disable(void)
{
	loggerEnable = false;	//Disable the logger
}


bool log_status(void)
{
	return(loggerEnable);	//Give status of the logger
}



void log_data(uint32_t *inAddress, size_t length)
{
	//Log data at a certain address till length
	if(loggerEnable){
		for(size_t i = 0; i < length; i++){
			printf("%p: ", inAddress + i);
			log_integer((inAddress + i));
		}
		printf("\n");
	}
}

void log_message(int logLevel, const char *functionName, char *message)
{
	if(loggerEnable){
#ifdef debug
		if(logLevel == 0){
			printf("DEBUG: ");
			printf("%s: %s\n", functionName, message);
		}
#endif

#ifdef test
		if (logLevel == 1){
			printf("TEST: ");
			printf("%s: %s\n", functionName, message);
		}
#endif

#ifdef normal
		if (logLevel == 2){
			printf("NORMAL: ");
			printf("%s: %s\n", functionName, message);
		}
#endif
		//
		//		if(logLevel == 0){
		//			printf("DEBUG: ");
		//			printf("%s: %s\n", functionName, message);
		//		}
		//		else if (logLevel == 1){
		//			printf("TEST: ");
		//			printf("%s: %s\n", functionName, message);
		//		}
		//		else if (logLevel == 2){
		//			printf("NORMAL: ");
		//			printf("%s: %s\n", functionName, message);
		//		}

	}
}


void log_string(char *inString)
{
	if(loggerEnable)
		printf("%s\n", inString);	//Print a string
}


void log_integer(uint32_t *inAddress)
{
	if(loggerEnable)
		printf("%d\n", *inAddress);	//Print an integer
}
