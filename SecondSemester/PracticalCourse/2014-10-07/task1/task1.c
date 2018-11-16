#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

void task1()
{
	cJSON * item;
	char * json = 
		"{"\
		"\"a\" : true, "\
		"\"b\" : { "\
		"\"c\":\"stro\\\"ka\""\
		"}"\
		"}";

	item = cJSON_Parse(json);


	cJSON_Delete(item);
}


int main(int argc, char ** argv)
{
	task1();

	return 0;
}

