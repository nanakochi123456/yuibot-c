/*************************************************************
 *	Ç‰Ç¢ÇøÇ·Ç¡Ç∆ÇbåæåÍî≈ 1.0y1
 *	Japanese	version--sjisï\é¶êÍóp
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

#include "chat.h"

int		in_host(char *buf, char *parm);
int		in_name(char *buf, char *parm);
int		in_enc(char *buf, char *parm);
int		in_color(char *buf, char *parm);
int		in_email(char *buf, char *parm);
int		in_chat(char *buf, char *parm);
int		in_emoji(char *buf, char *parm);
#if YUI_CUTE == 1
int		in_chara(char *buf, char *parm);
#if YUIBOT == 1
int		in_botchara(char *buf, char *parm);
#endif
#endif
int		in_endpage(char *buf, char *parm);
int		in_mode(char *buf, char *parm);
int		in_utime(char *buf, char *parm);
int		in_dtime(char *buf, char *parm);
int		in_genv(char *buf, char *parm);
int		in_sanka(char *buf, char *parm);
int		in_cgiurl(char *buf, char *parm);
int		in_reload(char *buf, char *parm);
int		in_window(char *buf, char *parm);
int		in_title(char *buf, char *parm);
int		in_config(char *buf, char *parm);
int		in_method(char *buf, char *parm);
#if DOUBLELOG == 1
int	in_yyyymmdd(char *buf, char *parm);
#endif
#if TAG_AUTOLINK == 1
int in_url(char *buf, char *parm);
#endif

JMPTBL	chatline[] = {
	"HOST",				in_host,
	"NAME",				in_name,
	"COLOR",			in_color,
	"EMAIL",			in_email,
	"CHAT",				in_chat,
	"EMOJI",			in_emoji,
	"MODE",				in_mode,
	"UTIME",			in_utime,
	"DTIME",			in_dtime,
	"CGI_URL",			in_cgiurl,
	"RELOAD",			in_reload,
	"WINDOW",			in_window,
	"TITLE",			in_title,
	"REMOTE_HOST",		in_genv,
	"REMOTE_ADDR",		in_genv,
	"HTTP_REFERER",		in_genv,
	"HTTP_USER_AGENT",	in_genv,
	"SANKA_MEMBER",		in_sanka,
	"MEMBERS",			in_sanka,
	"ROMMEMBERS",		in_sanka,
	"ROOM",				in_config,
	"METHOD",			in_method,
	"END_PAGE",			in_endpage,
	"ENC_NAME",			in_enc,
	"ENC_COLOR",		in_enc,
#if DOUBLELOG == 1
	"YYYYMMDD",			in_yyyymmdd,
#endif
#if YUI_CUTE == 1
	"CHARA",			in_chara,
#endif
#if TAG_AUTOLINK == 1
	"URL",				in_url,
#endif
	NULL,			NULL
};

JMPTBL	sankaline[] = {
	"MEMBER_COLOR",		in_sanka,
	"MEMBER_NAME",		in_sanka,
};

#if TAG_AUTOLINK == 1
int	in_url(char *buf, char *parm) {
	strcpy(buf, message2);
	return 0;
}
#endif

#if DOUBLELOG == 1
int	in_yyyymmdd(char *buf, char *parm) {
	strftime(buf, 256, "%Y%m%d", tim);
	return 0;
}
#endif

int	in_config(char *buf, char *parm) {
	sprintf(buf, "%s", CONFIGNAME);
	return 0;
}

int	in_endpage(char *buf, char *parm) {
	sprintf(buf, "%s", END_PAGE);
	return 0;
}

int	in_method(char *buf, char *parm) {
	strcpy(buf, METHOD);
	return 0;
}

int	in_utime(char *buf, char *parm) {
	sprintf(buf, "%d", current_time);
	return 0;
}

int	in_cgiurl(char *buf, char *parm) {
	strcpy(buf, CGI_URL);
	return 0;
}

int	in_reload(char *buf, char *parm) {
	strcpy(buf, reload);
	return 0;
}

int	in_window(char *buf, char *parm) {
	strcpy(buf, window);
	return 0;
}

int	in_title(char *buf, char *parm) {
	strcpy(buf, CHAT_TITLE);
	return 0;
}

int	in_dtime(char *buf, char *parm) {
	strcpy(buf, d_time);
	return 0;
}

int	in_host(char *buf, char *parm) {
#if HOST_LOOKUP == 2 || HOST_LOOKUP == 5
	sprintf(buf, "<!--host:%s-->", host);
#else
	strcpy(buf, host);
#endif
	return 0;
}

int	in_name(char *buf, char *parm) {
	strcpy(buf, name);
	return 0;
}

int	in_color(char *buf, char *parm) {
	strcpy(buf, color);
	return 0;
}

int	in_email(char *buf, char *parm) {
	strcpy(buf, email);
	return 0;
}

int	in_enc(char *buf, char *parm) {
	encode(buf,
		  strcmp(parm, "ENC_COLOR") == 0 ? color
		: strcmp(parm, "ENC_NAME") == 0 ? name
		: "");
	return 0;
}

int	in_chat(char *buf, char *parm) {
	strcpy(buf, message);
	return 0;
}

int	in_emoji(char *buf, char *parm) {
	strcpy(buf, emoji);
	return 0;
}

int	in_mode(char *buf, char *parm) {
	strcpy(buf, chatmode);
	return 0;
}

#if YUI_CUTE == 1
int	in_chara(char *buf, char *parm) {
	sprintf(buf, "%s%s", FACE_DIR, chara);
	return 0;
}
#if YUIBOT == 1
int	in_botchara(char *buf, char *parm) {
	sprintf(buf, "%s%s", botfacedir, botfacefile);
	return 0;
}
#endif
#endif

int	in_genv(char *buf, char *parm) {
	char	*s;
	if ((s = getenv(parm)) != NULL)
		strncpy(buf, s, MAX_MESSAGE);
	else
		*buf = '\0';
	return 0;
}

membersstr	sankamembers;

int	in_sanka(char *buf, char *parm) {
	char	member_buf[MAX_NAME];

	if (strcmp(parm, "MEMBER_COLOR") == 0) {
		strcpy(buf, sankamembers.color);
	} else if (strcmp(parm, "MEMBER_NAME") == 0) {
		strcpy(buf, sankamembers.name);
	} else if (strcmp(parm, "SANKA_MEMBER") == 0) {
		int		i;
		*buf = '\0';
		for (i = 0; i < count; i++) {
			sankamembers = members[i];
			if (strcmp(members[i].name, members[i].site) != 0) {
				strcpy(member_buf, SANKA_MEMBER);
				insertparm(member_buf, sankaline);
				strcat(buf, member_buf);
			}
		}
	} else if (strcmp(parm, "MEMBERS") == 0) {
		sprintf(buf, "%d", count+2);
	} else if (strcmp(parm, "ROMMEMBERS") == 0) {
		sprintf(buf, "%d", number_of_rommembers);
	}
}

#if	YUIBOT == 1

int	in_bot(char *buf, char *parm) {
	if (strcmp(parm, "BOTNAME") == 0)
		strcpy(buf, botname);
	else if (strcmp(parm, "BOTCOLOR") == 0)
		strcpy(buf, botcolor);
	else if (strcmp(parm, "BOTKEY") == 0)
		strcpy(buf, message);
	else if (strcmp(parm, "BOTMESSAGE") == 0)
		strcpy(buf, botmessage);
	return 0;
}

JMPTBL	yuibot_value[] = {
	"BOTNAME",			in_bot,
	"BOTCOLOR",			in_bot,
	"BOTKEY",			in_bot,
	"BOTMESSAGE",		in_bot,
	"UTIME",			in_utime,
#if YUI_CUTE == 1
	"CHARA",			in_chara,
	"BOTCHARA",			in_botchara,
#endif
	"DTIME",			in_dtime,
	"HOST",				in_host,
	"NAME",				in_name,
	"COLOR",			in_color,
	"EMAIL",			in_email,
	"CHAT",				in_chat,
	"EMOJI",			in_emoji,
	"MODE",				in_mode,
	"REMOTE_HOST",		in_genv,
	"REMOTE_ADDR",		in_genv,
	"HTTP_REFERER",		in_genv,
	"HTTP_USER_AGENT",	in_genv,
	NULL,			NULL
};

#endif

