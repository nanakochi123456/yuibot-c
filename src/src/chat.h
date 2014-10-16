/*
#define NO_TAG
上下のコメントマーク行を削除するとすべてのタグが使えなくなります。*/

#define JISA		0  /* 時差は秒単位で修正可能 */
#define CHAT_LOG		"./chat.dat"		/* ログ保存ファイル */
#define CHAT_S		"./sanka.dat"		/* 参加者       */

#define CGI_URL	"./chat.cgi"			/* 表示CGI          */

/* mail address */
#define EMAIL		"yui@cup.com"

/* 退出後の行き先*/
#define END_PAGE	"http://www.cup.com/yui/finder.html"

/* BODYタグ */
#define BODY	"<body BGCOLOR=\"#A1FE9F\" TEXT=\"#000000\" LINK=\"#ff0000\" VLINK=\"#ff0000\" ALINK=\"#FF0000\">\n"

/* 
** メッセージのフォーマット (最後には必ず\nを入れること) 
** %sの順番、総数はここだけで変更することはできません。
*/
#define MESSAGE		 "<font color=%s><b>%s </b></font>&gt; <b>%s %s</b><font size=2 color=#888888>(%s %s)</font><hr>\n"

/* 
** メッセージのフォーマット (最後には必ず\nを入れること) 
** --- Mail ADDRESS が入力されている場合  ---
** %sの順番、総数はここだけで変更することはできません。
*/
#define MESSAGE_MAIL	 "<font color=%s><b>%s </b></font><a href=\"mailto:%s\">|&gt; </a><b>%s %s</b><font size=2 color=#888888>(%s %s)</font><hr>\n"

#define MESSAGE_EXIT	 "<font color=red><b>管理人</b></font>&gt; <b><font size=5 color=%s>%s</font>さんが退室されました。</b><font size=2 color=#888888>(%s %s)</font><hr>\n"

/*以下の設定は変えないように...*/

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

