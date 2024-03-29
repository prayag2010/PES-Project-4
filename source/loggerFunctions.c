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
	if(loggerEnable)
	{
		for(size_t i = 0; i < length; i++)
		{
			printf("%p: ", inAddress + i);
			log_integer((inAddress + i));
		}
		printf("\n");
	}
}

//Print a message in a particular format based on build config
void log_message(enum loggerMode logLevel, const char *functionName, char *message)
{
	if(loggerEnable){
		//log if debug build config is selected
#ifdef debug
		if(logLevel == DEBUG)
		{
			printf("DEBUG: ");
			printf("%s: %s\n", functionName, message);
		}
#endif

#ifdef test
		//log if test build config is selected
		if (logLevel == TEST){
			printf("TEST: ");
			printf("%s: %s\n", functionName, message);
		}
#endif

#ifdef normal
		//log if normal build config is selected
		if (logLevel == NORMAL)
		{
			printf("NORMAL: ");
			printf("%s: %s\n", functionName, message);
		}
#endif

	}
}

//Print an integer in a particular format based on build config
void log_message_int(enum loggerMode logLevel, const char *functionName, char* message, int number)
{
	if(loggerEnable){
#ifdef debug
		//log if debug build config is selected
		if(logLevel == DEBUG){
			printf("DEBUG: ");
			printf("%s: %s %d\n", functionName, message, number);
		}
#endif

#ifdef test
		//log if test build config is selected
		if (logLevel == TEST){
			printf("TEST: ");
			printf("%s: %d\n", functionName, message);
		}
#endif

#ifdef normal
		//log if normal build config is selected
		if (logLevel == NORMAL){
			printf("NORMAL: ");
			printf("%s: %s %d\n", functionName, message, number);
		}
#endif

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
