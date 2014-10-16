/*************************************************************
 *	ゆいちゃっとＣ言語版 1.0y1
 *	Japanese	version--sjis表示専用
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

#include "chat.h"
#include <stdarg.h>

#if	YUIBOT == 1

char	botname[MAX_NAME], botmessage[MAX_MESSAGE], botwrite[MAX_MESSAGE], botcolor[MAX_NAME], bothost[MAX_NAME];
#if YUIBOT_KEYCONV == 1
char	botmessage1[MAX_MESSAGE], botmessage2[MAX_MESSAGE];
#endif
#if YUI_CUTE == 1
char	botfacedir[MAX_NAME], botfacefile[MAX_NAME];
int		botcharanumber = -1;
#endif

int		bot_learn(void);
int		omikuji(void);
int		toyui(char *str, ...);
int		bot(void);
void	bot2(void);
int		getlinedic(char *file);

FILE	*fp_bot;
char	*botkey;
BOT		current;

void	currentcopy(void) {
#if YUI_CUTE == 1
	int		argc = 0;
	char	*argv[MAX_BOTFACE];
#endif
	strcpy(botcolor, current.BOTCOLOR);
	strcpy(botname, current.BOTNAME1);
	strcpy(bothost, current.BOTHOST);
#if YUI_CUTE == 1
	strcpy(botfacedir, current.FACE_DIR);
	argc = parse(current.FACE_FILE, ",", argv);
	trim(botfacefile, argv[botcharanumber == -1 ? (botcharanumber = dice(argc)) : botcharanumber]);
#endif
}

#if OMIKUJI_ONLY == 0
int		nowbotmembers = 0;

int		botsanka(int n) {
	int		i, j;

	for (i = 0, j = n; botmember[i].BOTNAME1 != NULL; i++) {
		current = botmember[i];
		if (current.BOT_FILE != NULL) {
			members[n].site = current.BOTHOST;
			members[n].name = current.BOTNAME1;
			members[n].color = current.BOTCOLOR;
			members[n].time = current_time;
			n++;
			nowbotmembers++;
		}
	}
	return n; 
}

#if BOT_KIOKU == 1
void	kioku(char *key, char *res) {
	if ((fp_bot = fopen(current.BOT_FILE, "a")) != NULL) {
		fprintf(fp_bot, "%s%s%s\n", key, YUIBOT_DIC, res);
		fclose(fp_bot);
	}
}

int		bot_learn(void) {
	char	*buf, *s;
	/* 学習 */
	if ((botkey = strstr(message, LEARNSTR)) != NULL) {
		buf = malloc(LINE_LENGTH);
		*botkey = '\0';
		botkey += strlen(LEARNSTR);
		if ((s = strstr(botkey, TOYUI)) != NULL)
			*s = '\0';
		if (strlen(message) <= KEYLENGTH)
			strcpy(buf, current.MSG_SHORT);
		else if (strcspn(message, YUIBOT_DIC) == 0 || strcspn(botkey, YUIBOT_DIC) == 0
			|| strcmp(message, current.BOTNAME1) == 0 || strcmp(message, current.BOTNAME2) == 0)
			strcpy(buf, current.MSG_ERROR);
		else {
			kioku(message, botkey);
			strcpy(buf, current.MSG_KIOKU);
			currentcopy();
			insertparm(buf, yuibot_value);
		}
		strcpy(message, buf);
		free(buf);
		return 2;
	}
	return 0;
}

#endif

#if YUIBOT_KEYCONV == 1
/* 全角ひらがな－＞カタカナ */
unsigned char	*zh2zk(unsigned char *p) {
	unsigned char	*q=p;
	unsigned int	i ,j = 0;

	for(i = 0; *(p+i) != '\0'; i++){
		if(iskanji1st(*(p+i))) {
			/* ひらがなをカタカナに */
			if((*(p+i) == 0x82) && (*(p+i+1) >= 0x9f) && (*(p+i+1) <= 0xf1)) {
				*(q+j) = 0x83;
				j++;

				if(*(p+i+1) <= 0xdd)
					*(q+j) = *(p+i+1)-0x5f;
				else
					*(q+j) = *(p+i+1)-0x5e;
			} else{
				/* 他の全角文字の保持 */
				*(q + j) = *(p + i);
				j++;
				*(q + j) = *(p + i + 1);
			}
			i++;
		} else{
/* --- 半角文字の保持 --- */
			if(((*(p + i) >= (unsigned char)0x00) && (*(p + i) <= (unsigned char)0x7e))||
				((*(p + i) >= (unsigned char)0xa1) && (*(p + i) <= (unsigned char)0xdf)))
				*(q + j) = *(p + i);
		}
		j++;
	}
	*(q+j)='\0';
	return(q);
}

#endif

int		toyui(char *str, ...) {
	va_list	args;
	char	*s, *va;
	int		ret = 0;

	va_start(args, str);
	if ((s = strstr(message, TOYUI)) != NULL) {
		va = str;
		do {
			if (strcmp(s + 2, va) == 0) {
				ret = 2;
				break;
			}
		} while((va = va_arg(args, char *)) != NULL);
	}
	va_end(args);
	return ret;
}

char	*readyuibot(int *cnt) {
	char	*s;

	fgets(botmessage, MAX_MESSAGE, fp_bot);
	if (ferror(fp_bot))
		return NULL;

	if ((s = strchr(botmessage, '\n')) != NULL)
		*s = '\0';

	if ((s = strstr(botmessage, YUIBOT_DIC)) != NULL) {
		/* 対応する言葉がある場合は */
		*s = '\0';
#if YUIBOT_KEYCONV == 1
//		zenhan(botmessage, botmessage1);
		strcpy(botmessage1, botmessage);
		zh2zk(botmessage1);
		strcpy(botmessage2, message);
//		zenhan(message1, botmessage);
		zh2zk(botmessage2);
		if (re(botmessage1, botmessage2, 0) != NULL) {
#else
		if (re(botmessage, message, 0) != NULL) {
#endif
//printf("%s(%s):%d<BR>\n", botmessage, s+strlen(YUIBOT_DIC), *cnt);
//printf("read %s:%s:%s:%d<BR>\n", botmessage1, botmessage2, s+strlen(YUIBOT_DIC), *cnt);
			(*cnt)++;
		}
	}
	return s + strlen(YUIBOT_DIC);
}

int		bot() {
	int		i = 0, cnt = 0, rand;
	char	*s, *c;

	if ((fp_bot = fopen(current.BOT_FILE, "r")) == NULL)
		return;
	/* 可能な回答パターンの検索 */
	while(!feof(fp_bot)) {
		readyuibot(&cnt);
		i++;
	}
	/* 回答パターンがあれば */
	if (cnt) {
		fseek(fp_bot, 0, 0);
		rand = dice(cnt);
		cnt = 0;
		while(!feof(fp_bot) && cnt <= rand) {
			s = readyuibot(&cnt);
		}
		fclose(fp_bot);
		strreplace(s, BOTKEY_NAME, name, 0);
		strreplace(s, BOTKEY_HOST, host, 0);
		if ((c = strstr(s, BOTKEY_FACE)) != NULL) {
			*c = '\0';
#if YUI_CUTE == 1
			c += strlen(BOTKEY_FACE);
			if (!(botcharanumber = atoi(c)))
				botcharanumber = -1;
#endif
		}
		strcpy(botwrite, s);
		return 1;
	} else {
		/* 回答パターンがなければ */
		fclose(fp_bot);
		if (dice(10) > OSHIETE_RATIO) {
			strcpy(message3, message);
			if ((s = strstr(message, TOYUI)) != NULL)
				*s = '\0';
			getlinedic(current.AGO_FILE);
			strcpy(botwrite, botmessage);
			strcpy(message, message3);
		} else {
			/* 教えてモード */
			strcpy(message3, message);
			if ((s = strstr(message, TOYUI)) != NULL)
				*s = '\0';
			strcpy(botwrite, current.MSG_OSHIETE);
			insertparm(botwrite, chatline);
			strcpy(message, message3);
		}
		return 0;
	}
}

void	bot2() {
	if (getlinedic(current.BOKE_FILE) == 0) {
		strcpy(botwrite, current.MSG_BOKE);
		insertparm(botwrite, yuibot_value);
	}
}

#endif

int		getlinedic(char *file) {
	int		cnt = 0, rand;
	char	*s;

	if ((fp_bot = fopen(file, "r")) == NULL)
		return -1;

	/* 行数を数える */
	while(!feof(fp_bot)) {
		fgets(botmessage, MAX_MESSAGE, fp_bot);
		if (ferror(fp_bot)) {
			fclose(fp_bot);
			return -2;
		}
		cnt++;
	}
	if (cnt) {
		fseek(fp_bot, 0, 0);
		rand = dice(cnt);
		cnt = 0;
		while(!feof(fp_bot) && cnt <= rand) {
			fgets(botmessage, MAX_MESSAGE, fp_bot);
			if (ferror(fp_bot)) {
				fclose(fp_bot);
				return -2;
			}
			if ((s = strchr(botmessage, '\n')) != NULL)
				*s = '\0';
			cnt++;
		}
	}
	fclose(fp_bot);
}

int		omikuji(void) {
	int		i;

	for (i = 0; botmember[i].BOTNAME1 != NULL; i++) {
		current = botmember[i];
		if (current.OMIKUJISTR != NULL) {
			if (strcmp(message, current.OMIKUJISTR) == 0) {
				if (getlinedic(current.KUJI_FILE) == 0) {
					strcpy(botwrite, current.MSG_KUJI);
					insertparm(botwrite, yuibot_value);
					return 1;
				}
			}
		}
	}
	return 0;
}

void	bot_write() {
	if (*botwrite) {
		logsread();
		strcpy(message, botwrite);
		strcpy(color, botcolor);
		strcpy(name, botname);
		strcpy(host, bothost);
#if YUI_CUTE == 1
		FACE_DIR = botfacedir;
		strcpy(chara, botfacefile);
#endif
		*email = '\0';
		*emoji = '\0';
		logswrite();
	}
}

void	yuibot(int count) {
	int		i, r = -1, tmp;
	botmessage[0] = '\0';
	if (!botmembercount) return;

	/* おみくじ */
	if (omikuji())
		return;

	/* 誰宛？ */
	for (i = 0; botmember[i].BOTNAME1 != NULL; i++) {
		if (botmember[i].BOT_FILE != NULL) {
			current = botmember[i];
			if ((tmp = toyui(current.BOTNAME1, current.BOTNAME2, NULL)) == 2) {
				r = i;
				break;
			}
		}
	}

	/* ALL指定 */
	if (toyui(ALL1, ALL2, ALL3, ALL4, ALL5, ALL6, ALL7, ALL8, ALL9, ALL10, NULL) == 2) {
		r = -2;
	}

	/* 学習 */
#if BOT_KIOKU == 1
	if (strstr(message, LEARNSTR) != NULL) {
		/* 学習相手が決まってない場合、一番最初を選択 */
		if (r < 0) {
			for (i = 0; botmember[i].BOTNAME1 != NULL; i++) {
				if (botmember[i].BOT_FILE != NULL) {
					current = botmember[i];
					r = i;
					break;
				}
			}
		}
		if (r < 0)	/* それでも決まらなければエラー */
			return;

		bot_learn();

	/* bot反応 */
	} else {
#endif
		if ((r == -2) || (r == -1 && count <= BOT_TALK)) {
			while(botmember[(r = dice(BOT_MEMBERS))].BOT_FILE == NULL);
			current = botmember[r];
			/* 各botの反応率による */
			if (current.MEMBERS_TALK[(count > 10 ? 10 : count) - 1] > dice(100) || r == -2) {
				if (dice(100) > BOKE_RATIO) {
					/* 辞書がひけるかnowbotmembers×2回までまで繰り返す */
//					if (nowbotmembers > 1) {
//						for (i = 0; bot() == 0 && i < nowbotmembers * 2; i++) {
						for (i = 0; bot() == 0 && i < BOT_MEMBERS * 2; i++) {
							while(botmember[(r = dice(BOT_MEMBERS))].BOT_FILE == NULL);
							current = botmember[r];
						}
//					}
				} else {
					bot2();	/* つっこみ */
				}
			}
		} else if (r >= 0) {
			if (dice(100) > BOKE_RATIO) {
				bot();	/* 辞書応答 */
			} else {
				bot2();	/* つっこみ */
			}
		}
#if BOT_KIOKU == 1
	}
#endif
	currentcopy();
}

#if MONO_TALK == 1	/* ひとりごと */

int	bot4check(void) {
	long	mtime;
	struct	stat	sb;
	if (!botmembercount) return 0;
	stat(CHAT_LOG, &sb);
	if (sb.st_mtime < current_time - MONO_TALK_TIME)
		return 1;
	return 0;
}

void	bot4() {
	int		r;
	if (!botmembercount) return;

	while(botmember[(r = dice(BOT_MEMBERS))].BOT_FILE == NULL);
	current = botmember[r];
	getlinedic(current.MONO_FILE);
	strcpy(botwrite, botmessage);
	currentcopy();
	bot_write();
}

#endif
#endif
