/*************************************************************
 *	Ç‰Ç¢ÇøÇ·Ç¡Ç∆ÇbåæåÍî≈ 1.0y1
 *	Japanese	version--sjisï\é¶êÍóp
 *	Copyright(c)	1998	yui@cup.com
 *	Modified		2000	yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *	http://www.yuibot.com/
 *************************************************************/


/*
 *	jc -- Japanese code Converter
 */

/*
#ifdef	lint
static char rcsid[] = "$Header: jc.c,v 1.1 89/02/07 23:56:19 onoe Exp $";
#endif	lint*/
	
#include"chat.h"
	
#define TRUE	1
#define FALSE	0

#ifndef DEFMODE
#define DEFMODE JIS
#endif
	
#define DEFJIN	'B'
#define DEFJOUT 'B'
#define ESC	'\033'

#define KT_JIN	0x01
#define KT_JOUT 0x02
#define KT_ESC	0x04
#define KT_JIS	0x08
#define KT_SFT1 0x10
#define KT_SFT2 0x20
#define KT_EUC1 0x40
#define KT_EUC2 0x80

int ktype[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*00*/
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*10*/
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, /*	!"#$%&' */
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, /* ()*+,-./ */
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, /* 01234567 */
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, /* 89:;<=>? */
	0x29, 0x28, 0x2b, 0x28, 0x28, 0x28, 0x28, 0x28, /* @ABCDEFG */
	0x2a, 0x28, 0x2a, 0x28, 0x28, 0x28, 0x28, 0x28, /* HIJKLMNO */
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, /* PQRSTUVW */
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, /* XYZ[\]^_ */
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, /*	abcdefg */
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, /* hijklmno */
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, /* pqrstuvw */
	0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x00, /* xyz{|}~	*/
	0x20, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, /*80*/
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, /*90*/
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x20, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, /*A0*/
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, /*B0*/
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, /*C0*/
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, /*D0*/
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, /*E0*/
	0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
	0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, /*F0*/
	0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xc0, 0xc0, 0x00,
};
int mode = DEFMODE;
int nocr = FALSE; /*TRUE;*/
char jin = DEFJIN;
char jout = DEFJOUT;
char buf[MAX_MESSAGE];
int c = 0;

kanjiIn()
{
	if(mode == JIS){
		sprintf(&buf[c],"\033$%c", jin) ;c+=3;
	}
}

kanjiOut()
{
	if(mode == JIS){
		sprintf(&buf[c],"\033(%c", jout) ;c+=3;
	}
}

void	jc(char *str, int code) {
	char ch;
	int flag,i=0;
	register unsigned int ch1, ch2 = 0;
	int kanji = 0, k_default = KT_EUC1;

	flag = 0;
	mode = code;
	c=0;

	while ( *(str+i) != '\0') {
		ch = *(str+i);
		i++;
		ch1 = (unsigned char) ch;
		if (ch == '\033') {
			flag = KT_ESC;
			continue;
		}
		if (flag & KT_ESC) {
			if (ch == '$')
				flag = KT_JIN;
			else if (ch == '(')
				flag = KT_JOUT;
			else {
				flag = 0;
				if (kanji && !(kanji & KT_JIN)) {
					kanjiOut();
					kanji = 0;
				}
				buf[c] = '\033';
				c++;
				buf[c] = ch;
				c++;
			}
			continue;
		}

		if (flag & KT_JIN) {
			if (ktype[ch1] & KT_JIN) {
				kanji = KT_JIN | KT_JOUT;
				kanjiIn();
				flag = 0;
				continue;
			}
		} else if (flag & KT_JOUT) {
			if (ktype[ch1] & KT_JOUT) {
				kanji = 0;
				kanjiOut();
				flag = 0;
				continue;
			}
		}

		if (flag & KT_SFT1) {
			flag = 0;
			if (ktype[ch1] & KT_SFT2) {
				ch2 -= (ch2 <= 0x9f) ? 0x71 : 0xb1;
				ch2 = ch2 * 2 + 1;
				if (ch1 > 0x7f)
					ch1 -= 1;
				if (ch1 >= 0x9e) {
					ch1 -= 0x7d;
					ch2++;
				}
				else
					ch1 -= 0x1f;
				if (!(kanji & KT_JOUT)) {
					kanjiIn();
					kanji |= KT_JOUT;
				}
				putKanji(ch2, ch1, c);
				k_default = KT_SFT1;
				continue;
			}
		}

		if (flag & KT_EUC1) {
			flag = 0;
			if (ktype[ch1] & KT_EUC2) {
				if (!(kanji & KT_JOUT)) {
					kanjiIn();
					kanji |= KT_JOUT;
				}
				putKanji(ch2 & 0x7f, ch1 & 0x7f, c);
				k_default = KT_EUC1;
				continue;
			} else {
				buf[c] = ch2;
				c++;
			}
		}

		if (ktype[ch1] & KT_JIS) {
			if (kanji & KT_JIN) {
				if (flag & KT_JIS) {
					putKanji(ch2, ch1, c);
					flag = 0;
				} else {
					ch2 = ch1;
					flag = KT_JIS;
				}
			} else {
				if (kanji & KT_JOUT) {
					kanjiOut();
					kanji &= ~ KT_JOUT;
				}
				buf[c]=ch;
				c++;
			}
			continue;
		}

		if (ktype[ch1] & KT_EUC1) {
			if (k_default == KT_EUC1 || !(ktype[ch1] & KT_SFT1)) {
				flag = KT_EUC1;
				ch2 = ch1;
				continue;
			}
		}

		if (ktype[ch1] & KT_SFT1) {
			flag = KT_SFT1;
			ch2 = ch1;
			continue;
		}

		if (ch == '\n' && mode == JIS && kanji & KT_JOUT) {
			kanjiOut();
			buf[c] = '\n';
			c++;
			kanjiIn();
		} else {
			if (kanji) {
				kanjiOut();
				kanji = 0;
			}
			if (ch != '\r' || !nocr)
				buf[c] = ch;
			c++;
		}
	}
	
	if (kanji)
		kanjiOut();
	if (flag)
		buf[c++] = ch2;
	buf[c] = '\0';
	strcpy(str,buf);
}

putKanji(hi, lo)
   unsigned int hi, lo;
{
	unsigned int k;
	switch (mode) {
	case EUC:
	k = hi | 0x80;
	buf[c] = k;
	c++;
	k = lo | 0x80;
	buf[c] = k;
	c++;
	break;
	case SJIS:
	if (hi & 1)
		lo += 0x1f;
	else
		lo += 0x7d;
	if (lo >= 0x7f)
		lo++;
	hi = (hi - 0x21 >> 1) + 0x81;
	if (hi > 0x9f)
		hi += 0x40;
	/* THROUGH DOWN */
	case JIS:
	buf[c] = hi;
	c++;
	buf[c] = lo;
	c++;
	break;
	}
}
