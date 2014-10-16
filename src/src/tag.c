/*************************************************************
 *	ゆいちゃっとＣ言語版 1.0y1
 *	Japanese	version--sjis表示専用
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

#include"chat.h"

#if	NO_TAG == 0

/* 許可タグリスト */
char	*allowtaglist[] = {
	"</.*>", "<font.*>", "<i>", "<b>", "<s>", "<u>","<!",
	"<sub.*>", "<sup.*>", "<br>",
	 NULL
};

char	*allowtaglistlv2[] = {
	"</.*>", "<a.*href=.*>", "<font.*>", "<img.*>", "<image.*> ",
	"<i>", "<b>", "<s>", "<u>",
	"<select.*", "<option.*",
	"<sup.*>", "<sub.*>", "<!", NULL
};

char	*allowtaglistlv3[] = {
	"</.*>", "<a.*href=.*>", "<font.*>", "<img.*>", "<image.*> ",
	"<i>", "<b>", "<s>", "<u>",
	"<select.*", "<option.*",
	"<sup.*>", "<sub.*>", "<!", NULL
};

/* 禁止タグリスト（注：上記 */
char	*denytaglist[] = {
	"<.* .*ona.*>", "<.* .*onb.*>", "<.* .*onc.*>", "<.* .*ond.*>", 
	"<.* .*one.*>", "<.* .*onf.*>", "<.* .*onk.*>", "<.* .*onl.*>", 
	"<.* .*onm.*>", "<.* .*onr.*>", "<.* .*ons.*>", "<.* .*onu.*>", 
/* on...って、こうしないとうまくいかないみたい；； */
	"<!--.*>", "<.*font-weight.*>", "<.*point-size.*>",
	"<.*style.*>", "<.*font-size.*>", 
	"<.*height.*>", "<.*width.*>", "<.*cols.*>", "<.*rows.*>",
	"<.*file:.*>", "<.*mailbox.*>", "<i.*mail.*>",
	"<font.*size.*=.*7.*>", /* 不完全なfont size=7の規制 */
	"<font.*size.*=.*+.*>", /* 無理やりfont size=7にさせない規制 */
	NULL
};

/* 禁止タグリスト（注：上記 */
char	*denytaglistlv2[] = {
	"<.* .*ona.*>", "<.* .*onb.*>", "<.* .*onc.*>", "<.* .*ond.*>", 
	"<.* .*one.*>", "<.* .*onf.*>", "<.* .*onk.*>", "<.* .*onl.*>", 
	"<.* .*onm.*>", "<.* .*onr.*>", "<.* .*ons.*>", "<.* .*onu.*>", 
/* on...って、こうしないとうまくいかないみたい；； */
	"<!--.*>",
	"<.*style.*>", "<.*font-size.*>", 
	"<.*height.*>", "<.*width.*>", "<.*cols.*>", "<.*rows.*>",
	"<.*file:.*>", "<.*mailbox.*>", "<i.*mail.*>",
	NULL
};

/* 変換するタグ */
char	*converttaglist[] = {
	"href=", "target=_blank href=",
//	"src=", "src=",
	NULL
};

#define	ENCODE_TAG		"__TAG__"

int			tagstatus = 0;

int	tagparce(char *chat, char *allowtaglist[], char *denytaglist[], char *converttaglist[]) {
	register	char	*s, *s2, *s3;
	register	char	c;
	int					i, j, k, len;

	*message2 = *message3 = '\0';
	for (s = chat, s2 = NULL, s3 = message3; *s != '\0'; s++) {
		switch(c = *s) {
			case '<': {
				s2 = message2;
				*s2++ = c;
				break;
			}
			case '!': {// 特例
#define	COMMENT_TAG	"<!"
				if (strcmp(s - 1, COMMENT_TAG) == 0) {
					strcpy(s3, COMMENT_TAG);
					s2 = NULL;
//					s3 += strlen(COMMENT_TAG);
					s3 += 2;
				}
				break;
			}
			case '>': {
				if (s2 != NULL) {
					*s2++ = c;
					*s2 = '\0';
					s2 = NULL;
					strreplace(message2, ENCODE_TAG, "&lt;", 1);
					/* 禁止タグ */
					for (i = 0; denytaglist[i] != NULL; i++) {
						if (re(denytaglist[i], message2, 1) != NULL) {
							strreplace(message2, "<", "&lt;", 0);
							tagstatus = 1;
						}
					}
					/* 許可タグ */
					for (i = 0; allowtaglist[i] != NULL; i++) {
						if (re(allowtaglist[i], message2, 1) != NULL)
							strreplace(message2, "<", ENCODE_TAG, 0);
					}
					if (strchr(message2, '<') != NULL)
						tagstatus = 1;
					/* 許可されなかったタグ、許可されたタグの変換 */
					strreplace(message2, "<", "&lt;", 0);
					strreplace(message2, ENCODE_TAG, "<", 0);

					/* 許可されたタグの整形 */
					for (i = 0; converttaglist[i] != NULL; i += 2) {
						if (re(converttaglist[i], message2, 1) != NULL) {
							strreplace(message2, converttaglist[i], converttaglist[i+1], 3);
						}
					}
					/* ” と ’の文字の数があっているか確認 */
					for (i = 0, j = 0, k = 0; (c = message2[i]) != '\0'; i++) {
						j += (c == '\"' ? 1 : 0);
						k += (c == '\'' ? 1 : 0);
					}
					if (j & 1|| k & 1)
						strreplace(message2, "<", "&lt;", 0);
					/* タグを結合 */
					*s3 = '\0';
					strcat(s3, message2);
					s3 += strlen(message2);
				} else *s3++ = c;
				break;
			}
			default: {
				if (s2 == NULL)
					*s3++ = c;
				else
					*s2++ = c;
			}
		}
	}
	*s3 = '\0';
	strcpy(chat, message3);
	return tagstatus;
}

#if TAG_AUTOLINK == 1
void	autolink(char *chat, char *proto) {
	char	*s;
	int		i;

	if (strchr(chat, '<') != NULL)
		return;

	for (s = chat; (s = strstr(s, proto)) != NULL; s++) {
		strncpy(message2, s, MAX_MESSAGE - 1);
		for (i = strlen(message2) - 1; i >= 0; i--) {
			if (strchr(";?:/@~&=+-$.%#_",message2[i]) == NULL
				&& !isalnum((int)message2[i]))
				message2[i] = '\0';
		}
		if (strcmp(message2, proto)) {
			strcpy(message3, AUTOLINK);
			insertparm(message3, chatline);
			strreplace(chat, message2, message3, 0);
			s += strlen(message3) - 1;
		} else s += strlen(message2) - 1;
	}
}

#endif

STACK	*tagstack = NULL;;

/* 自動タグ閉じ */
void	tagclose(char *chat) {
	char	*s1, *s2;

	if (strchr(chat, '<') == NULL)
		return;

	if (tagstack == NULL)
		stack_init(&tagstack, MAX_MESSAGE);
	strcpy(stack_push(&tagstack, 3), "*");

	for (s1 = chat; *s1 != '\0'; s1++) {
		if (*s1 != '<')
			continue;
		s1++;
		for (s2 = message2; *s1 != '>' && *s1 != '<' && *s1 != ' ' && *s1 != '\0' && *s1 != '='; s1++, s2++)
+			*s2 = *s1;
		*s2 = '\0';
		if (*message2 != '/')
			strcpy(stack_push(&tagstack, strlen(s2) + 2), message2);
	}
	strcpy(stack_push(&tagstack, 3), "");
	while(strcmp(s1 = stack_pop(&tagstack), "*") != 0) {
		strcat(chat, "</");
		strcat(chat, s1);
		strcat(chat, ">");
	}
}

#if NO_NAMETAG == 0
void	NameTag() {
#if 0
	if (tagparce(name, allowtaglist, denytaglist, converttaglist))
		error(TAG_ERROR, "");
#endif
}
#endif

void	Tag() {
#if 0
	int             count = 0;
	int             i, len, c = 0;
	char           *p = message2;
	if (tagparce(message, allowtaglist, denytaglist, converttaglist))
		error(TAG_ERROR, "");

#if TAG_AUTOLINK == 1
	autolink(message, "http://");
	autolink(message, "https://");
	autolink(message, "ftp://");
	autolink(message, "mailto:"); /* 注：メールアドレスだけでは無理 */
#endif
	tagclose(message);
#endif
}
#endif
