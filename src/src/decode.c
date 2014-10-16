/*************************************************************
 *	‚ä‚¢‚¿‚á‚Á‚Æ‚bŒ¾Œê”Å 1.0y1
 *	Japanese	version--sjis•\¦ê—p
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

#include"chat.h"

int decode(char *str, int henkanmode)
//char *str;
//int henkanmode;   /* 1: to JIS   2: to EUC  3: to SJIS 0: not convert*/
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
		}else if(buf[i] == '%' && isxdigit((int)(buf[i+1])) && isxdigit((int)(buf[i+2]))){
			sprintf(hex, "0x%c%c", buf[i+1],buf[i+2]);
			ch = strtoul(hex, (char **)NULL, 16);
			i += 2;
		}else{
			ch = buf[i];
		}
#if NO_TAG == 1
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


char	*encode(char *dest, char *src) {
	int		i, j;
	char	c;

	i = 0; j = 0;
	while((c = src[i]) != '\0') {
		if (c == (char)0x20) {
			dest[j] = '+';
		} else 	if (c >= (char)0x7b || c <= (char)0x2f || c == ',' || (c >= 0x3a && c <= 0x3f)) {
			sprintf(&dest[j], "%c%02x", '%', c & 255);
			j += 2;
		} else dest[j] = c;
		i++;
		j++;
	}

	dest[j] = '\0';
	return(dest);
}
