#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

void task2()
{
	cJSON * item;
	char * text;

	item = cJSON_Parse("{"\
		"\"name\" : \"Ivanov\","\
		"\"age\" : 25"\
		"}"
		);

	text = cJSON_PrintUnformatted(item);

	puts(text);

	free(text);
	cJSON_Delete(item);
}

int main(int argc, char ** argv)
{
	task2();

	return 0;
}

