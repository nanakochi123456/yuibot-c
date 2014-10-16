#include<string.h>
#include<stdio.h>
#include"chat.h"

int decode(str,henkanmode)
char *str;
int henkanmode;   /* 1: to JIS   2: to EUC  3: to SJIS 0: not convert*/
{
	int i,len,c;
	char buf[MAX_MESSAGE],hex[5];
	unsigned char ch;
	int ret = NORMAL;

	len = strlen(str);
	if(len > MAX_MESSAGE-1){
		len = MAX_MESSAGE-1;
	}
	strncpy(buf, str, len+1);
	c=0;

	for(i=0 ; i<len ; i++){
		if( buf[i] == '+'){
			ch = ' ';
		}else if(buf[i] == '%' && isxdigit(buf[i+1]) && isxdigit(buf[i+2])){
			sprintf(hex, "0x%c%c\0", buf[i+1],buf[i+2]);
			ch = strtoul(hex, NULL, 16);
			i += 2;
		}else{
			ch = buf[i];
		}
#ifdef NO_TAG
/*erace HTTP TAG */
		if(ch == '<'){
			sprintf(&str[c], "&lt;");
			c += 4;continue;
		}else if(ch == '>'){
			sprintf(&str[c], "&gt;");
			c += 4;continue;
		}
#endif
		if(ch != 13 && ch != 10){
			str[c]=ch;
			c++;
		}else{
			sprintf(&str[c], " ");
			/*str[c] = '\0';*/
			c++;
		/*	c += 4;*/
		}
	}
	str[c] = '\0';

	if( henkanmode != NOT_CNVT){
			jc(str,henkanmode);/* KANJI CODE CONVERT */
	}

	return(ret);
}

