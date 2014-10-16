/*************************************************************
 *	�䂢������Ƃb����� 1.0y1
 *	Japanese	version--sjis�\����p
 *	Copyright(c)	1998	yui@cup.com
 *	Modified		2000	yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *	http://www.yuibot.com/
 *************************************************************/

#ifndef	CHAT_H
#define	CHAT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "config.h"

/* �ݒ�́Aconfig.h �Ɉړ����܂����� */

#define	WILD_CARD			".*"

/* KANJI CODE */
#define JIS 				1
#define	EUC					2
#define	SJIS				3
#define NOT_CNVT			0
#define DEFAULT_KANJI		SJIS

/* return from function */
#define ERR					-1
#define NORMAL				0
#define	TRUE				1
#define	FALSE				0

/* internal error code */
#define FILE_ERROR			1
#define TAG_ERROR			2
#define TIME_OUT			3
#define	TOOLONG_STRING		4
#define	ACCESS_FORMAT		5
#define	FILE_WRITE_ERROR	6
#define	INI_FILE_ERROR		7
#define	FORBIDDEN			8

/* string length */
#define MAX_MESSAGE 16384		/* �����̍ő啶���� */
#define MAX_NAME 	64*3		/* ���O�AMail�Aemoji�̍ő啶���� */
#define MAX_FORM 	30			/* form�̍ő�� */
#define MAX_LINE 	50			/* �ő�L�����O�� */
#define LINE_LENGTH 16384		/* �o�b�t�@�T�C�Y �iNoFrame�ׂ̈ɂ��j*/
#define	MAX_TIME 	30			/* ���t������ */
#define	MAX_BOTFACE	10			/* �ァ�ڂ��Ƃ̍ő�摜�� */
#define MAX_ARRAY	10			/* �e��z��̍ő�l */

/* �ڂ��Ƃ̊i�[�ϐ� */

#if	YUIBOT == 1
typedef struct {
	char	*BOTCOLOR;		/* bot�̖��O�̐F */
	char	*BOTHOST;		/* bot�̃z�X�g�� */
	char	*MSG_KUJI;		/* ���݂����̏o�� */
	char	*OMIKUJISTR;	/* ���݂������������� */
	char	*KUJI_FILE;		/* ���݂������� */
	char	*BOTNAME1;		/* bot�̖��O�i��{�j */
#if YUI_CUTE == 1
	char	*FACE_DIR;		/* bot�̊�摜�i�[�f�B���N�g�� */
	char	*FACE_FILE;		/* bot�̊�摜�t�@�C���ꗗ */
#endif
#if OMIKUJI_ONLY == 0
	char	*BOTNAME2;		/* bot�̖��O�i�����p�j */
	char	*MSG_KIOKU;		/* ���� */
	char	*MSG_SHORT;		/* �Z������G���[ */
	char	*MSG_ERROR;		/* �L�[���[�h�G���[ */
	char	*MSG_BOKE;		/* �ڂ��̏o�� */
	char	*MSG_OSHIETE;	/* �����ă��[�h */
	char	*BOT_FILE;		/* ��{���� */
	char	*AGO_FILE;		/* �����o�^�Ȃ��̏ꍇ */
	char	*BOKE_FILE;		/* �����ݎ��� */
#if MONO_TALK == 1
	char	*MONO_FILE;		/* �ЂƂ育�Ǝ��� */
#endif
	int		MEMBERS_TALK[11];/*�l���ʂ̂����Ɨ� */
#endif
} BOT;

extern	char	botname[MAX_NAME], botmessage[MAX_MESSAGE], botwrite[MAX_MESSAGE], botcolor[MAX_NAME], bothost[MAX_NAME];
extern	BOT		botmember[BOT_MEMBERS];
extern	int		botmembercount;
#endif
extern	char	*CONFIGNAME;
extern	char	*CHAT_LOG;
#if	DOUBLELOG == 1
extern	char	*CHAT_LOG2;
#endif
extern	char	*CHAT_S;
extern	char	*CHAT_R;
extern	char	*CGI_URL;
extern	char	*EMAIL;
extern	char	*END_PAGE;
extern	char	*BODY;
extern	char	*MESSAGE;
extern	char	*MESSAGE_MAIL;
extern	char	*MESSAGE_EXIT;
#if YUI_CUTE == 1
extern	char	*MESSAGE_CUTE;
extern	char	*MESSAGE_MAIL_CUTE;
#endif
#if	DOUBLELOG == 1
extern	char	*MESSAGE_DOUBLELOG;
#endif
extern	char	*SANKA_ONLY;
extern	char	*SANKA_ALL;
extern	char	*SANKA_MEMBER;
extern	char	*CHAT_TITLE;
extern	char	*CLEAR_STR;
extern	char	*CLEAR_CMD;
extern	char	*CUT_CMD;
extern	char	*ADMIN_CMD;
extern	char	*REFERER;
extern	char	*KILLFILE;
extern	char	*METHOD;
extern	char	*DEFAULTCOLOR, *DEFAULTWINDOW, *DEFAULTRELOAD;
extern	char	*WARNING, *TAGWARNING;
#if YUI_CUTE == 1
extern	char	*FACE_DIR;
#endif
#if TAG_AUTOLINK == 1
extern	char	*AUTOLINK;
#endif

extern	char	color[MAX_NAME];
extern	char	name[MAX_NAME];
extern	char	email[MAX_NAME];
extern	char	message[MAX_MESSAGE];
extern	char	message2[MAX_MESSAGE];
extern	char	message3[MAX_MESSAGE];
extern	char	emoji[MAX_NAME];
extern	char	d_time[MAX_TIME];
#if YUI_CUTE == 1
extern	char	chara[MAX_NAME];
#if YUIBOT == 1
extern	char	botfacedir[MAX_NAME], botfacefile[MAX_NAME];
#endif

#endif
extern	char	*host;
extern	char	*env, *window, *reload, color[], *chatmode;
extern	long	current_time;
extern	int		count, number_of_members, number_of_rommembers;
extern	char	*htmlheader, *htmlfooter, *allfooter, *htmlautoreload, *htmlreload, *noframehtml;
extern	struct tm *tim;
extern	int		content;

typedef	struct {
	char		*name;
	char		*site;
	char		*color;
	long		time;
	long		count;
} membersstr;
extern	membersstr	members[MAX_MEMBER];

#define	MAX_STACK	256

typedef	struct {
	char	*stack[MAX_STACK];
	int		no;
	int		useflag;
} STACK;

typedef	struct {
	char	*name;
	int		(*compar)(char *, char *);
} JMPTBL;

extern	JMPTBL	chatline[];
#if YUIBOT == 1
extern	JMPTBL	yuibot_value[];
#endif

/* proto type */
void			lock();
void			unlock();
void			hostkill();
void			referercheck();
void			jc(char *str, int code);
void			jconv(char *buf, char *tmp, int mode);
char			*insertparm(char *s, JMPTBL jmptbl[]);
void			ReadMembers(char *buff);
char		   *AllocAndCopy(char *str);
int 			parse(char *buff, char *seps, char *argv[]);
void			GetValue();
void			SetValue(void);
#if MODE_SANKA == 1
void			Sanka(int);
#else
void			Sanka();
#endif
void			Html();
void			Write();
void			logsread();
int				logswrite();
void			Tag();
void			NameTag();

int				decode(char *str,int henkanmode);
char			*encode(char *dest, char *src);
int				error(int code, char *err);
void			Rank(void);
void			yuibot(int number_of_members);
int				bot4check(void);
void			bot4();
int				botsanka(int n);
int				dice(int n);
char			*gethost(void);
char			*insertparm(char *s, JMPTBL jmptbl[]);
char			*strreplace(char *s, char *old, char *newstr, int lower);
char			*re(char *ptn, char *s, int lower);

char			*stack_init(STACK **stack, int memsize);
char			*stack_push(STACK **stack, int memsize);
char			*stack_pop(STACK **stack);

int				dircheck(char *s);

int				ext_strncmp(register const char *s1,register const char *s2,register size_t  n,	int mode);
char			*ext_strstr(register const char *s,register const char *find,int mode);
char			*trim(register char *buf, register char *src);
void			stringup(register char *str);
char			*datafirst(char *buf, int len, char *word, FILE *rfp);
int				sectioncount(FILE *rfp);
void			getarg(char **left, char **right, char *buf);
char			*getini(char *buf, FILE *fp, char *section, char *name);
int				getinibool(FILE *fp, char *section, char *name, int def);
void			Content();
extern	FILE	*chat_out;

/* macros */
#define SWAP(X,Y) {int tmpval; tmpval = (Y); (Y) = (X); (X) = tmpval; }
#define MIN(X,Y) (((X) > (Y)) ? (Y) : (X))
#define MAX(X,Y) (((X) > (Y)) ? (X) : (Y))
#define iskanji1st(c) ((unsigned int) (c ^ 0x20) - 0xa1 < 0x3c)
#define	ext_tolower(c, mode)	(mode?tolower(c):c)

#endif
