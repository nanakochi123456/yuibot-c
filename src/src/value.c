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

char			*htmlheader, *htmlfooter, *allfooter, *htmlautoreload, *htmlreload, *noframehtml;

char	*DEFAULTCONFIGNAME = "config";
char	*CONFIGNAME = "";
char	*CHAT_LOG	= "";
#if	DOUBLELOG == 1
char	*CHAT_LOG2	= "";
#endif
char	*CHAT_S		= "";
char	*CHAT_R		= "";
char	*CGI_URL	= "";
#if KILL == 1
char	*KILLFILE	= "";
#endif
#if REFERERCHECK != 0
char	*REFERER	= "";
#endif
char	*EMAIL		= "";
char	*END_PAGE	= "";
char	*CHAT_TITLE = "";
char	*BODY		= "";
char	*MESSAGE	= "";
char	*MESSAGE_MAIL = "";
char	*MESSAGE_EXIT = "";
#if YUI_CUTE == 1
char	*MESSAGE_CUTE	= "";
char	*MESSAGE_MAIL_CUTE = "";
#endif
#if	DOUBLELOG == 1
char	*MESSAGE_DOUBLELOG = "";
#endif
#if TAG_AUTOLINK == 1
char	*AUTOLINK = "";
#endif

#if MODE_SANKA == 1
char	*SANKA_ONLY = "";
#endif
char	*SANKA_ALL = "";
char	*SANKA_MEMBER = "";
char	*CLEAR_STR = "";
char	*CLEAR_CMD = "";
char	*CUT_CMD = "";
char	*ADMIN_CMD = "";

char	*DEFAULTWINDOW = "10";
char	*DEFAULTRELOAD = "30";
char	*DEFAULTCOLOR = "red";

char	*WARNING = "";
char	*TAGWARNING = "";
char	*METHOD = "POST";	/* POSTにしておきましょう（汗）*/

int		botmembercount = 0;

#if YUI_CUTE == 1
char	*FACE_DIR = "";
#endif
#if YUIBOT == 1
BOT		botmember[BOT_MEMBERS];
#endif

char	*getconfig(char **s, FILE *fp, char *section, char *name) {
	char	*buf;

	buf = malloc(LINE_LENGTH);
	if (getini(buf, fp, section, name) != NULL) {
		*s = AllocAndCopy(buf);
		return *s;
	} else {
		return NULL;
	}
}

void	configread(char	*configname) {
	FILE	*fp;
	int		i;

	char	*s;

	if (configname == NULL || *configname == '\0')
		CONFIGNAME = DEFAULTCONFIGNAME;
	else
		CONFIGNAME = configname;

	/* セキュリティーホール予防 */
	if (dircheck(CONFIGNAME))
		exit(1);

	sprintf(message3, "./%s.%s", CONFIGNAME, CONFIGEXTENTION);

	if ((fp = fopen(message3, "r")) != NULL) {
		getconfig(&CHAT_LOG,     fp, "CHAT", "CHAT_LOG");
#if DOUBLELOG == 1
		getconfig(&CHAT_LOG2,    fp, "CHAT", "CHAT_LOG2");
#endif
		getconfig(&CHAT_S,       fp, "CHAT", "CHAT_S");
		getconfig(&CHAT_R,       fp, "CHAT", "CHAT_R");
		getconfig(&CGI_URL,      fp, "CHAT", "CGI_URL");
		getconfig(&END_PAGE,     fp, "CHAT", "END_PAGE");
		getconfig(&CHAT_TITLE,   fp, "CHAT", "CHAT_TITLE");
#if REFERERCHECK != 0
		getconfig(&REFERER,      fp, "CHAT", "REFERER");
#endif
#if KILL == 1
		getconfig(&KILLFILE,     fp, "CHAT", "KILLFILE");
#endif
		getconfig(&BODY,         fp, "CHAT", "BODY");
		getconfig(&MESSAGE,      fp, "CHAT", "MESSAGE");
#if DOUBLELOG == 1
		getconfig(&MESSAGE_DOUBLELOG,fp, "CHAT", "DOUBLELOG");
#endif
		getconfig(&MESSAGE_MAIL, fp, "CHAT", "MESSAGE_MAIL");
		getconfig(&MESSAGE_EXIT, fp, "CHAT", "MESSAGE_EXIT");
#if YUI_CUTE == 1
		getconfig(&MESSAGE_CUTE,      fp, "CHAT", "MESSAGE_CUTE");
		getconfig(&MESSAGE_MAIL_CUTE, fp, "CHAT", "MESSAGE_MAIL_CUTE");
#endif
#if MODE_SANKA == 1
		getconfig(&SANKA_ONLY,   fp, "CHAT", "SANKA_ONLY");
#endif
		getconfig(&SANKA_ALL,    fp, "CHAT", "SANKA_ALL");
		getconfig(&SANKA_MEMBER, fp, "CHAT", "SANKA_MEMBER");
		getconfig(&CLEAR_STR,    fp, "CHAT", "CLEAR_STR");
		getconfig(&CLEAR_CMD,    fp, "CHAT", "CLEAR_CMD");
		getconfig(&CUT_CMD,      fp, "CHAT", "CUT_CMD");
		getconfig(&ADMIN_CMD,    fp, "CHAT", "ADMIN_CMD");
		getconfig(&DEFAULTCOLOR, fp, "CHAT", "DEFAULTCOLOR");
		getconfig(&DEFAULTWINDOW,fp, "CHAT", "DEFAULTWINDOW");
		getconfig(&DEFAULTRELOAD,fp, "CHAT", "DEFAULTRELOAD");
		getconfig(&WARNING,      fp, "CHAT", "WARNING");
		getconfig(&TAGWARNING,   fp, "CHAT", "TAGWARNING");
#if TAG_AUTOLINK == 1
		getconfig(&AUTOLINK,     fp, "CHAT", "AUTOLINK");
#endif
#if YUI_CUTE == 1
		getconfig(&FACE_DIR,     fp, "CHAT", "FACE_DIR");
#endif
		getconfig(&htmlheader,   fp, "HTML", "HEADER");
		getconfig(&htmlfooter,   fp, "HTML", "FOOTER");
		getconfig(&allfooter,    fp, "HTML", "ALLFOOTER");
		getconfig(&htmlautoreload,fp, "HTML","AUTORELOAD");
		getconfig(&htmlreload,   fp, "HTML", "RELOAD");
		if (strcmp(chatmode, "checked") != 0) /* ちょっと食うので */
			getconfig(&noframehtml,  fp, "HTML", "NOFRAMEHTML");
#if YUIBOT == 1
		for (i = 0; i < BOT_MEMBERS; i++) {
			s = malloc(MAX_TIME);
			sprintf(s, "BOT-%d", i + 1);
			if (getconfig(&botmember[i].BOTNAME1,  fp, s, "BOTNAME1") == NULL)
				break;
			if (getconfig(&botmember[i].BOTCOLOR,  fp, s, "BOTCOLOR") == NULL)
				break;
			if (getconfig(&botmember[i].BOTHOST,   fp, s, "BOTHOST") == NULL)
				break;
			botmembercount++;
			getconfig(&botmember[i].MSG_KUJI,  fp, s, "MSG_KUJI");
			getconfig(&botmember[i].OMIKUJISTR,fp, s, "OMIKUJISTR");
			getconfig(&botmember[i].KUJI_FILE, fp, s, "KUJI_FILE");
#if OMIKUJI_ONLY == 0
			getconfig(&botmember[i].BOTNAME2,    fp, s, "BOTNAME2");
			getconfig(&botmember[i].MSG_KIOKU,   fp, s, "MSG_KIOKU");
			getconfig(&botmember[i].MSG_SHORT,   fp, s, "MSG_SHORT");
			getconfig(&botmember[i].MSG_ERROR,   fp, s, "MSG_ERROR");
			getconfig(&botmember[i].MSG_OSHIETE, fp, s, "MSG_OSHIETE");
			getconfig(&botmember[i].MSG_BOKE,    fp, s, "MSG_BOKE");
			getconfig(&botmember[i].BOT_FILE,    fp, s, "BOT_FILE");
			getconfig(&botmember[i].AGO_FILE,    fp, s, "AGO_FILE");
			getconfig(&botmember[i].BOKE_FILE,   fp, s, "BOKE_FILE");
#endif
#if YUI_CUTE == 1
			getconfig(&botmember[i].FACE_DIR,     fp, s, "FACE_DIR");
			getconfig(&botmember[i].FACE_FILE,    fp, s, "FACE_FILE");
#endif
#if MONO_TALK == 1
			getconfig(&botmember[i].MONO_FILE,    fp, s, "MONO_FILE");
#endif
			if (getini(botmessage, fp, s, "MEMBERS_TALK") != NULL) {
				int		argc = 0, k1 = 0, j;
				char	*argv[MAX_ARRAY];
				for (j = 0; j < MAX_ARRAY; j++)
					argv[j] = NULL;
				argc = parse(botmessage, ",", argv);
				for (j = 0; j < MAX_ARRAY && argv[j] != NULL; j++) {
					k1 = argv[j] != NULL ? atoi(argv[j]) : k1;
					botmember[i].MEMBERS_TALK[j] = k1;
				}
			}
		}
#endif
		fclose(fp);
	} else {
		error(FILE_ERROR, message3);
		exit(1);
	}
}
