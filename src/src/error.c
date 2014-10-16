#include<stdio.h>
#include"chat.h"

int error(code)
int code;
{

	if(code == FILE_ERROR){
		printf("<H1> Error </H1>\n");
		printf("Please mail to me.<BR>\n");
		printf("<A HREF=\"mailto:%s\">Mailto:%s</A><BR>\n",EMAIL,EMAIL);
	}else if(code == TAG_ERROR){
		printf("<H1> Error </H1>\n");
		printf("TAG Error\n");
	}else if(code == TIME_OUT){
		printf( "Content-type: text/html\n\n");
		printf("<H1> Error </H1>\n");
		printf("Server is busy.<BR>\n");
		printf("Please retry.\n");
	}
	printf("</BODY>\n");
	exit(0);
}
