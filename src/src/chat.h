/*
#define NO_TAG
�㉺�̃R�����g�}�[�N�s���폜����Ƃ��ׂẴ^�O���g���Ȃ��Ȃ�܂��B*/

#define JISA		0  /* �����͕b�P�ʂŏC���\ */
#define CHAT_LOG		"./chat.dat"		/* ���O�ۑ��t�@�C�� */
#define CHAT_S		"./sanka.dat"		/* �Q����       */

#define CGI_URL	"./chat.cgi"			/* �\��CGI          */

/* mail address */
#define EMAIL		"yui@cup.com"

/* �ޏo��̍s����*/
#define END_PAGE	"http://www.cup.com/yui/finder.html"

/* BODY�^�O */
#define BODY	"<body BGCOLOR=\"#A1FE9F\" TEXT=\"#000000\" LINK=\"#ff0000\" VLINK=\"#ff0000\" ALINK=\"#FF0000\">\n"

/* 
** ���b�Z�[�W�̃t�H�[�}�b�g (�Ō�ɂ͕K��\n�����邱��) 
** %s�̏��ԁA�����͂��������ŕύX���邱�Ƃ͂ł��܂���B
*/
#define MESSAGE		 "<font color=%s><b>%s </b></font>&gt; <b>%s %s</b><font size=2 color=#888888>(%s %s)</font><hr>\n"

/* 
** ���b�Z�[�W�̃t�H�[�}�b�g (�Ō�ɂ͕K��\n�����邱��) 
** --- Mail ADDRESS �����͂���Ă���ꍇ  ---
** %s�̏��ԁA�����͂��������ŕύX���邱�Ƃ͂ł��܂���B
*/
#define MESSAGE_MAIL	 "<font color=%s><b>%s </b></font><a href=\"mailto:%s\">|&gt; </a><b>%s %s</b><font size=2 color=#888888>(%s %s)</font><hr>\n"

#define MESSAGE_EXIT	 "<font color=red><b>�Ǘ��l</b></font>&gt; <b><font size=5 color=%s>%s</font>���񂪑ގ�����܂����B</b><font size=2 color=#888888>(%s %s)</font><hr>\n"

/*�ȉ��̐ݒ�͕ς��Ȃ��悤��...*/

#define MAX_MESSAGE 1024
#define MAX_FORM 10
#define MAX_LINE 50
#define LINE_LENGTH 1024

/* KANJI CODE */
#define JIS  	1
#define	EUC	2
#define	SJIS	3
#define NOT_CNVT 0
#define DEFAULT_KANJI SJIS

/* return from function */
#define ERR -1
#define NORMAL 0

/* internal error code */
#define FILE_ERROR 1
#define TAG_ERROR 2
#define TIME_OUT 3

