/*************************************************************
 *	Ç‰Ç¢ÇøÇ·Ç¡Ç∆ÇbåæåÍî≈ 1.0y1
 *	Japanese	version--sjisï\é¶êÍóp
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

#include"chat.h"
void	content_check() {
	if (!content) {
		Content();
	}
}

int error(int code, char *str) {
	switch(code) {
		case FILE_ERROR: {
			content_check();
			fprintf(chat_out, "<H1>Error</H1>\n");
			fprintf(chat_out, "%s not found\n<HR><A HREF=mailto:%s>%s</A>\n"
				, str, EMAIL, EMAIL);
			break;
		}
		case FILE_WRITE_ERROR: {
			content_check();
			fprintf(chat_out, "<H1>Error</H1>\n");
			fprintf(chat_out, "%s can't write\n<HR><A HREF=mailto:%s>%s</A>\n"
				, str, EMAIL, EMAIL);
			break;
		}
		case TIME_OUT: {
			content_check();
			fprintf(chat_out, "<H1>Error</H1>\n");
			fprintf(chat_out, "Server is busy.<BR>Please retry.<HR><A HREF=mailto:%s>%s</A>\n"
				, EMAIL, EMAIL);
			break;
		}
		case TOOLONG_STRING: {
			content_check();
			fprintf(chat_out, "<H1>Error</H1>\n");
			fprintf(chat_out, "Method toolong.<HR><A HREF=mailto:%s>%s</A>\n"
				, EMAIL, EMAIL);
			break;
		}
		case ACCESS_FORMAT: {
			content_check();
			fprintf(chat_out, "<H1>Error</H1>\n");
			fprintf(chat_out, "Access Format Error.<HR><A HREF=mailto:%s>%s</A>\n"
				, EMAIL, EMAIL);
			break;
		}
		case INI_FILE_ERROR: {
			content_check();
			fprintf(chat_out, "<H1>Error</H1>\n");
			fprintf(chat_out, "Ini file Error.<HR><A HREF=mailto:%s>%s</A>\n"
				, EMAIL, EMAIL);
			break;
		}
		case FORBIDDEN: {
			if (*WARNING != '\0') {
				printf("Content-type: text/html\n\n");
				strcpy(message3, WARNING);
				insertparm(message3, chatline);
				puts(message3);
			} else {
				printf("Status: 404\n\n");
			}
			exit(0);
		}
		case TAG_ERROR: {
			content_check();
			if (*TAGWARNING != '\0') {
				strcpy(message3, TAGWARNING);
				insertparm(message3, chatline);
				fprintf(chat_out, "%s\n", message3);
			} else {
				fprintf(chat_out, "TAG Error\n");
			}
			break;
		}
		default: {
			content_check();
			fprintf(chat_out, "<H1>Error</H1>\n");
			fprintf(chat_out, "Unknown error<HR><A HREF=mailto:%s>%s</A>\n"
				, EMAIL, EMAIL);
		}
	}
	printf("</BODY>\n");
	exit(0);
}

void	htmlerror(char *s) {
	printf("Content-type: text/html\n\n%s\n\n", s);
	return;
}
