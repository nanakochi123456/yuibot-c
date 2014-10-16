/*************************************************************
 *	ゆいちゃっとＣ言語版 1.0y1
 *	Japanese	version--sjis表示専用
 *	Copyright(c)	1998	yui@cup.com
 *	Modified		2000	yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *	http://www.yuibot.com/
 *************************************************************/

#include"chat.h"

/* 参加者、ランキング共用 */
membersstr	members[MAX_MEMBER];

struct formpairs {
	char		   *name;
	char		   *value;
}				forms[MAX_FORM];

int 			number_of_members = 0;/*アクセス人数*/
int				number_of_rommembers = 0; /* ROM人数 */
int 			found = 0;/*参加者記録ファイルに自分が居るか*/
int 			count = 0;/*参加表示人数*/
long			current_time;

char			*query_string, *request_method;
char			*host;
char			name[MAX_NAME], message[MAX_MESSAGE], message2[MAX_MESSAGE], message3[MAX_MESSAGE];
char			emoji[MAX_NAME], email[MAX_NAME];
char			color[MAX_NAME];
#if YUI_CUTE == 1
char			chara[MAX_NAME];
#endif
char			htmlbuf[LINE_LENGTH];
char			*htmlheader, *htmlfooter, *htmlautoreload, *htmlreload, *noframehtml;

char			*name_p, *message_p, *emoji_p, *email_p;
char			*env, *window, *reload, *chatmode;
char			*room;
#if YUI_CUTE == 1
char			*chara_p = NULL;
#endif
int 			windows;
int 			i = 0;
int 			j = 0;
char			buff[LINE_LENGTH];
char			outbuff[LINE_LENGTH];
char		   *logs[MAX_LINE];
char			dmy[] = "";
char			d_time[MAX_TIME];
char		   *code = dmy;
struct tm	   *tim;
FILE		   *fp_dat;
int 			ret;
int				content = 0;
FILE			*chat_out;

static char    *youbi[] = {
	"日", "月", "火", "水", "木", "金", "土"
		};

int main(void) {
    // 乱数初期化
    srandom(time(0));
	GetValue(); 	 /*GET&POSTメソッドでのデコード処理＆SJIS変換*/
	configread(room);
#if KILL == 1
	hostkill();
#endif
#if REFERERCHECK != 0
	referercheck();
#endif
	SetValue();
#if MODE_SANKA == 0
	Sanka();		  /*参加者記録処理*/
#else
	Sanka(strcmp(chatmode, "sanka") == 0 ? 0 : 1);
#endif
	if ((strcmp(message, "退室") == 0)) {		/*退室時は、書き込み後Locationヘッダ*/
		Write();
		fprintf(stdout, "Location: %s\n\n", END_PAGE);
		return 1;
	}
	
	Html(); 		  /*HTML文を出力する。書き込みがあればWriteする。*/
	return 0;
}

#if KILL == 1
void	hostkill(void) {
	char	*s;

	if (*KILLFILE == '\0')
		return;

	if ((fp_dat = fopen(KILLFILE, "r")) == NULL)
		return;

	while (fgets(buff, LINE_LENGTH-1, fp_dat) != NULL) {
		if ((s = strchr(buff, '\n')) != NULL)
			*s = '\0';
#if KILL_REUSE == 0
		if (strstr(host, buff) != NULL)
#else
		if (re(buff, host, 1) != NULL)
#endif
			error(FORBIDDEN, host);
#if HOST_LOOKUP != 0 && HOST_LOOKUP != 4
		s = getenv("REMOTE_HOST");
		if (s != NULL) {
#if KILL_REUSE == 0
			if (strstr(s, buff) != NULL)
#else
			if (re(buff, s, 1) != NULL)
#endif
				error(FORBIDDEN, host);
		}
#endif
#if HOST_LOOKUP != 2 && HOST_LOOKUP != 5
		s = getenv("REMOTE_ADDR");
		if (s != NULL) {
#if KILL_REUSE == 0
			if (strstr(s, buff) != NULL)
#else
			if (re(buff, s, 1) != NULL)
#endif
				error(FORBIDDEN, host);
		}
#endif
	}
	fclose(fp_dat);
}
#endif

#if REFERERCHECK != 0
void	referercheck() {
	char	*s;

	s = getenv("HTTP_REFERER");
	if (s == NULL)
#if REFERERCHECK == 1
		return;
#else
		error(FORBIDDEN, dmy);
#endif
	strcpy(buff, s);
	if (decode(buff, SJIS) != NORMAL)
		error(FORBIDDEN, dmy);

	if (strncmp(buff, REFERER, strlen(REFERER)) != 0)
		error(FORBIDDEN, dmy);
}

#endif

void	getquery(void) {
	char	*buf;
	int	clength, loop;
	if (request_method = getenv("REQUEST_METHOD")) {
		if (strcmp(request_method, "GET") == 0) {
			query_string = getenv("QUERY_STRING");
			if (query_string == NULL) query_string = "\0";
			if (strlen(query_string) > LINE_LENGTH)
				error(TOOLONG_STRING, dmy);
			env = query_string;
		} else if (strcmp(request_method, "POST") == 0) {
			if (buf = getenv("CONTENT_LENGTH")) {
				clength = atoi(buf);
				if (clength > LINE_LENGTH)
					error(TOOLONG_STRING, dmy);
				env = malloc(clength + 10);
				for (loop = 0; clength && (!feof(stdin)); loop++
) {
					env[loop] = fgetc(stdin);
					if (env[loop] < ' ' || env[loop] == (char)0xff)
						break;
				}
				env[loop] = '\0';
			} else env = '\0';
		} else env = '\0';
	}
}

void SetValue(void) {
	if ((window) == dmy)	window = DEFAULTWINDOW;
	if ((reload) == dmy)	reload = DEFAULTRELOAD;
	if ((*color) == '\0')	strcpy(color, DEFAULTCOLOR);

#if NO_NAMETAG == 0
	NameTag();
#else
	strreplace(name, "<", "&lt;", 0);
#endif
	strreplace(emoji, "<", "&lt;", 0);
	strreplace(email, "<", "&lt;", 0);
#if YUI_CUTE == 1
	strreplace(chara, "<", "&lt;", 0);
#endif
}

void GetValue(void) {
	char		   *decode_tmp; 	  /*デコード用TMP*/
	current_time = time(NULL) + JISA;		 /*現在時刻が秒で入る*/
	tim = localtime(&current_time);
//	sprintf(d_time, "%d/%d(%s) %d:%02d",
//			tim->tm_mon + 1,
//			tim->tm_mday,
//			youbi[tim->tm_wday],
//			tim->tm_hour,
//			tim->tm_min);				/*時刻表示のフォーマット*/
	sprintf(d_time, "%d/%d/%d %d:%02d:%02d",
			tim->tm_year + 1900,
			tim->tm_mon + 1,
			tim->tm_mday,
			tim->tm_hour,
			tim->tm_min,
			tim->tm_sec);
			
	host = gethost();
	getquery();
	if (env == NULL || (forms[0].name = strtok(env, "&")) == NULL) {
//		error(ACCESS_FORMAT, dmy);
	} else {
		   /*とりあえず、&記号で区切る。*/
		for (i = 1; i < MAX_FORM; i++) {	/*MAX_FORMは formの要素数より大きく。 */
			if ((forms[i].name = strtok(NULL, "&")) == NULL)
					break;	/* 最後はここでbreak */
		}
		j = i;		/* jはフォームの要素数になる */
		
		for (i = 0; i < j; i++) {				/* 次に=記号で区切る。*/
			decode_tmp=forms[i].name;
			if ((forms[i].name = strtok(decode_tmp, "=")) == NULL)
					forms[i].name = dmy;
			if ((forms[i].value = strtok(NULL, "&")) == NULL)
					forms[i].value = dmy;
		}

		for (i = 0; i < j; i++) {		/*name&valueのペアから、各変数に代入する。*/
//			 printf("[%s= %s]<BR>\n",forms[i].name,forms[i].value); 
			if (strcmp(forms[i].name, WINDOW_STR) == 0)
					window = forms[i].value;
			else if (strcmp(forms[i].name, RELOAD_STR) == 0)
					reload = forms[i].value;
			else if (strcmp(forms[i].name, NAME_STR) == 0)
					name_p = forms[i].value;
			else if (strcmp(forms[i].name, CHAT_STR) == 0)
					message_p = forms[i].value;
			else if (strcmp(forms[i].name, COLOR_STR) == 0)
					strcpy(color, forms[i].value);
			else if (strcmp(forms[i].name, EMOJI_STR) == 0)
					emoji_p = forms[i].value;
			else if (strcmp(forms[i].name, EMAIL_STR) == 0)
					email_p = forms[i].value;
			else if (strcmp(forms[i].name, MODE_STR) == 0)
					chatmode = forms[i].value;
			else if (strcmp(forms[i].name, ROOM_STR) == 0)
					room = forms[i].value;
// code=noip 処理
else if (strcmp(forms[i].name, "code") == 0)	//noip
	code = forms[i].value;						//noip
#if YUI_CUTE == 1
			else if (strcmp(forms[i].name, CHARA_STR) == 0)
					chara_p = forms[i].value;
#endif
		}		/*上記の処理によって、フォームの順番を気にしなくて済む。*/
	}
	/* FORM入力で値が入らなかったら、ダミー値を入れる。*/
	if ((host) == NULL)			host = dmy;
	if ((name_p) == NULL)		name_p = host;
	if ((window) == NULL)		window = dmy;
	if ((reload) == NULL)		reload = dmy;
	if ((message_p) == NULL)	message_p = dmy;
	if ((emoji_p) == NULL)		emoji_p = dmy;
	if ((email_p) == NULL)		email_p = dmy;
	if ((chatmode) == NULL)		chatmode = dmy;
	strncpy(email, email_p, MAX_NAME-1);
	strncpy(name, name_p, MAX_NAME-1);
	strncpy(emoji, emoji_p, MAX_NAME-1);	
	strncpy(message, message_p, MAX_MESSAGE-1);	/*デコードが必要なものはコピー*/
#if YUI_CUTE == 1
	if (chara_p == NULL)		chara_p = dmy;
	strncpy(chara, chara_p, MAX_NAME-1);	
#endif
	if (strlen(name) != 0) {
		*(name + MAX_NAME-1) = '\0';		 /*万一に備えて\0を入れておく。おまぬけ？*/
		ret = decode(name, SJIS);	   /*デコードしたのち、SJIS変換*/
		if (ret != NORMAL)
			error(ret, dmy);
	}

	if (strlen(message) != 0) {
		*(message + MAX_MESSAGE-1) = '\0';
		ret = decode(message, SJIS);
		if (ret != NORMAL)
			error(ret, dmy);
	}

	if (strlen(emoji) != 0) {
		*(emoji + MAX_NAME-1) = '\0';
		ret = decode(emoji, SJIS);
		if (ret != NORMAL)
			error(ret, dmy);
	}

	if (strlen(email) != 0) {
		*(email + MAX_NAME-1) = '\0';
		ret = decode(email, SJIS);
		if (ret != NORMAL)
			error(ret, dmy);
	}

	if (strlen(color) != 0) {
		*(color + MAX_NAME-1) = '\0';
		ret = decode(color, SJIS);
		if (ret != NORMAL)
			error(ret, dmy);
	}

#if YUI_CUTE == 1
	if (*chara != '\0') {
		ret = decode(chara, SJIS);
		if (ret != NORMAL)
			error(ret, dmy);
#if YUI_CUTE_SEC == 1
		if (dircheck(chara))
			error(ret, dmy);
#endif
#ifdef	YUI_CUTE_EXT
		strcat(chara, YUI_CUTE_EXT);
#endif
	}
#endif

	if (strcmp(emoji, "(=＾＾=)") == 0) {/*顔文字を加工する。*/
		strcpy(emoji, "(<font color=red>=</font>＾＾<font color=red>=</font>)");
	}
}

void	comment(char *s) {
	char	*c1, *c2;

	while((c1 = strstr(s, "<!")) != NULL) {
		if ((c2 = strchr(c1, '>')) == NULL)
			*c1 = '\0';
		else
			strcpy(c1, c2 + 1);
	}
}

void	Content() {
	FILE	*fp;
	char	*enc;

	enc = getenv("HTTP_ACCEPT_ENCODING");

	printf("Content-type: text/html\n");	  /*おきまりのヘッダ〜*/
	content=1;
	if (enc != NULL) {
		if (strstr(enc, "x-gzip")) {
			if ((fp = popen("gzip -c -3", "w")) != NULL) {
				chat_out=fp;
				content=2;
				printf("Content-Encoding: x-gzip\n");
			}
		}
		if (strstr(enc, "gzip")) {
			if ((fp = popen("gzip -c -3", "w")) != NULL) {
				chat_out=fp;
				content=2;
				printf("Content-Encoding: gzip\n");
			}
		}
		else chat_out = fopen("/dev/stdout", "w");
	} else
		chat_out = fopen("/dev/stdout", "w");

	printf("\n");
}

void 	Html() {
	Content();

	/* ヘッダー */
	strcpy(htmlbuf, htmlheader);
	insertparm(htmlbuf, chatline);
	fprintf(chat_out, "%s\n", htmlbuf);

#if MODE_SANKA == 1
	if (strcmp(chatmode, "sanka") != 0) {
#endif
		/* 自動リロード */
		if (strcmp(reload, "0") != 0 && strcmp(chatmode, "checked") == 0) {
			strcpy(htmlbuf, htmlautoreload);
			insertparm(htmlbuf, chatline);
			fprintf(chat_out, "%s\n", htmlbuf);
		}
#if MODE_SANKA == 1
	}
#endif
	fprintf(chat_out, "%s\n", BODY);
#if MODE_SANKA == 1
	if (strcmp(chatmode, "sanka") != 0) {
#endif

		/* ノンフレーム発言欄 */
		if (strcmp(chatmode, "checked") != 0) {
			strcpy(htmlbuf, noframehtml);
			insertparm(htmlbuf, chatline);
			fprintf(chat_out, "%s\n", htmlbuf);
		}

		/* リロードリンク */
		strcpy(htmlbuf, htmlreload);
		insertparm(htmlbuf, chatline);
		fprintf(chat_out, "%s\n", htmlbuf);
#if MODE_SANKA == 1
	}
#endif

	/* 参加者表示 */
#if YUIBOT == 1 && OMIKUJI_ONLY == 0
//	count = botsanka(count);
#endif
#if MODE_SANKA == 1
	if (strcmp(chatmode, "sanka") == 0) {
		strcpy(htmlbuf, SANKA_ONLY);
		insertparm(htmlbuf, chatline);
		fprintf(chat_out, "%s\n", htmlbuf);
		strcpy(htmlbuf, allfooter);
		fprintf(chat_out, "%s\n", htmlbuf);
		return;
	}
#endif

#if NOTPRINT_SANKA != 1
	strcpy(htmlbuf, SANKA_ALL);
	insertparm(htmlbuf, chatline);
	fprintf(chat_out, "%s\n", htmlbuf);
#endif

	if (strlen(message) != 0) { 	 /*書き込みがある場合*/
#if FILE_LOCK != 0
		lock();
#endif
		Write();
		windows = atoi(window);
		for (i = 0; i < windows; i++) {
			if (logs[i] == NULL)
					break;
			comment(logs[i]);
			fprintf(chat_out, "%s", logs[i]);
		}
#if	YUIBOT == 1						/* ゆいぼっとの書き込み */
		bot_write();
#endif
#if FILE_LOCK != 0
		unlock();
#endif

#if MONO_TALK == 1 && YUIBOT == 1
	} else {
		if (bot4check()) {
#if FILE_LOCK != 0
			lock();
#endif
			bot4();
			windows = atoi(window);
			for (i = 0; i < windows; i++) {
			if (logs[i] == NULL)
						break;
				comment(logs[i]);
				fprintf(chat_out, "%s", logs[i]);
			}
#if FILE_LOCK != 0
			unlock();
#endif
		} else {
#endif
		if ((fp_dat = fopen(CHAT_LOG, "r")) == NULL) {
			error(FILE_ERROR, "chat_file");
			return;
		}
		windows = atoi(window);
		for (i = 0; i < windows; i++) {
			if (fgets(buff, LINE_LENGTH-1, fp_dat) == NULL) {
				if (ferror(fp_dat))
						break;
				if (feof(fp_dat))
						break;
			}
			fprintf(chat_out, "%s", buff);
			
		}
		fclose(fp_dat);
#if MONO_TALK == 1 && YUIBOT == 1
		}
#endif
	}			/* if else end */
	strcpy(htmlbuf, htmlfooter);
	insertparm(htmlbuf, chatline);
	fprintf(chat_out, "%s", htmlbuf);
	strcpy(htmlbuf, allfooter);
	insertparm(htmlbuf, chatline);
	fprintf(chat_out, "%s", htmlbuf);
}

void	command(char *cmd, char *str, char *str2) {
	char	*p = message2;
	char	*clearkey1 = NULL, *clearkey2 = NULL;
	int		i;
	int		argc = 0;
	char	*argv[MAX_ARRAY];

	argc = parse(cmd, ",", argv);

	for (i = 0; i < argc; i++) {
		trim(message3, argv[i]);
		if (str != NULL) {
			if (strcmp(message, message3) == 0 && *message3 != '\0') {
				clearkey1 = str;
				clearkey2 = str2;
			}
		} else if (strncmp(message, message3, strlen(message3)) == 0 && *message3 != '\0') {
			clearkey1 = message + strlen(message3);
			clearkey2 = NULL;
		}
	}
	if (clearkey1 == NULL) return;

	argc = parse(CLEAR_STR, ",", argv);
	if (argc)
		trim(message, argv[dice(argc)]);
	else
		*message = '\0';

	for (i = 1; i < MAX_LINE - 1; i++) {
		if (logs[i] == NULL)
			break;
		if (strstr(logs[i], clearkey1) != NULL) {
			if (str2 == NULL)
				logs[i] = dmy;
			else if (strstr(logs[i], clearkey2) != NULL)
				logs[i] = dmy;
		}
		strcpy(message2, logs[i]);
		for (; (*p=tolower((int)(*p)))!= '\0'; p++);
			if (strstr(logs[i], clearkey1) != NULL) {
				if (str2 == NULL)
					logs[i] = dmy;
				else if (strstr(logs[i], clearkey2) != NULL)
					logs[i] = dmy;
		}
	}
}

void	logsread() {
	if ((fp_dat = fopen(CHAT_LOG, "r")) == NULL) {
		error(FILE_ERROR, "chat_file");
		return;
	}
	for (i = 1; i < MAX_LINE; i++) {
		if (fgets(buff, LINE_LENGTH-1, fp_dat) == NULL) {
			if (ferror(fp_dat))
					break;
			if (feof(fp_dat))
					break;
		}
		logs[i] = AllocAndCopy(buff);
		
	}
	fclose(fp_dat);
}

int		logswrite() {
#if ALLOW_RANK == 1
	int	rankflag = 1;
#endif
	if ((fp_dat = fopen(CHAT_LOG, "w")) == NULL) {
		error(FILE_WRITE_ERROR, "chat_file");
		return;
	}
// code=noip処理
if ((strcmp(code, "noip") == 0)) {	//noip
char	*host2;						//noip
host2 = malloc(1024);				//noip
sprintf(host2, "<!--%s-->", host);	//noip
host=host2;							//noip
}									//noip
	if ((strcmp(message, "退室") == 0)) {
		strcpy(outbuff, MESSAGE_EXIT);
		insertparm(outbuff, chatline);
		fprintf(fp_dat, "%s", outbuff);
#if ALLOW_RANK == 1
		rankflag = 0;
#endif
	} else {
#if YUI_CUTE == 1
		strcpy(outbuff,
			*chara == '\0' ?
				(*email == '\0' ? MESSAGE : MESSAGE_MAIL)
			  : (*email == '\0' ? MESSAGE_CUTE : MESSAGE_MAIL_CUTE));
#else
		strcpy(outbuff, *email == '\0' ? MESSAGE : MESSAGE_MAIL);
#endif
		insertparm(outbuff, chatline);
		fprintf(fp_dat, "%s", outbuff);
	}

	logs[0] = outbuff;

	for (i = 1; i < MAX_LINE - 1; i++) {  /*残りのログを書き込む*/
		if ((logs[i]) == NULL)
				break;
		fprintf(fp_dat, "%s", logs[i]);
		
	}
	fclose(fp_dat);

#if ALLOW_RANK == 1
	return rankflag;
#else
	return 0;
#endif
}

#if DOUBLELOG == 1
void	doublelog() {
	if (*CHAT_LOG2 == '\0')
		return;

	strcpy(message3, CHAT_LOG2);
	insertparm(message3, chatline);
	if ((fp_dat = fopen(message3, "a")) != NULL) {
		strcpy(message3, MESSAGE_DOUBLELOG);
		insertparm(message3, chatline);
		fprintf(fp_dat, "%s", message3);
		fclose(fp_dat);
	} else error(FILE_WRITE_ERROR, "doublelog_file");
}
#endif

void 	Write() {
	int	botf = 0;	/* ボットの反応フラグ */

	/* POSTで送信されたときのみ書き込み */
#if POST_ONLY == 1
	if (strcmp(request_method, "POST"))
		return;
#endif
	if (strlen(name) == 0)
		strcpy(name, host); 	 /*名前未入力時の名前*/

#if NO_TAG == 0
	Tag();
#endif

	/* ２重でログをとる処理 (bot発言はログとらない）*/
#if DOUBLELOG == 1
	doublelog();
#endif
	/* 更新するログファイルの読み込み */
	logsread();

	/* コマンド機能 */
	command(CLEAR_CMD, host, name);
	command(CUT_CMD, "<img", NULL);
	command(CUT_CMD, "<image", NULL);
	command(ADMIN_CMD, NULL, NULL);
	/* コマンド機能ここまで */

#if	YUIBOT == 1
	yuibot(number_of_members);
#endif

#if ALLOW_RANK == 1
	if (logswrite())
		Rank();
#else
	logswrite();
#endif
}

#if ALLOW_RANK == 1
/* ランキング （ロック必須）*/
void	Rank(void) {
	FILE	*rank_fp;
	int	 writeflag = 0;
	int	 rankcnt = 0;

	if (*CHAT_R == '\0')
		return;

#if NO_NAMETAG == 0 && NO_TAG == 0
	if (strstr(name, "<!") != NULL)
		return;
#endif

	if ((rank_fp = fopen(CHAT_R, "r")) == NULL)
		error(FILE_ERROR, "rank_file");

	for (;fgets(buff, LINE_LENGTH-1, rank_fp) != NULL; ++rankcnt) {
		int 			argc = 0;
		char		   *argv[10];
		char			*s;
		if ((s = strchr(buff, '\n')) != NULL)
			*s = '\0';
		if ((argc = parse(buff, "\t", argv)) == 4) {
			members[rankcnt].name = AllocAndCopy(argv[0]);
			members[rankcnt].count = atoi(argv[1]);
			members[rankcnt].time = atoi(argv[2]);
			members[rankcnt].site = AllocAndCopy(argv[3]);
			/* rankと参加者一致で＋１ */
			if (strcmp(members[rankcnt].name, name) == 0) {
				members[rankcnt].count ++;
				writeflag = 1;
				members[rankcnt].time = current_time;
				members[rankcnt].site = host;
			}
		}
	}
	if (writeflag == 0) {
		members[rankcnt].name = name;
		members[rankcnt].count = 1;
		members[rankcnt].time = current_time;
		members[rankcnt].site = host;
		rankcnt++;
	}
	fclose(rank_fp);

	if ((rank_fp = fopen(CHAT_R, "w")) == NULL)
		error(FILE_WRITE_ERROR, "rank_file");

	for (i = 0; i < rankcnt; i++) {
	/* time が指定より上なら書き込み */
		if (members[i].time > current_time - RANK_TIME) {
			fprintf(rank_fp, "%s\t%d\t%d\t%s\n"
				, members[i].name, members[i].count, members[i].time, members[i].site);
		}
	}
	fclose(rank_fp);
}
#endif

/* 更新する参加ファイルの読み込み */
#if MODE_SANKA == 1
void 	Sanka(int sankamode) {
#else
void	Sanka() {
#endif
	int 			argc = 0;
	char		   *argv[10];

	if (*CHAT_S == '\0')
		return;

	if ((fp_dat = fopen(CHAT_S, "r")) == NULL)
		error(FILE_ERROR, "sanka_file");

	while (fgets(buff, LINE_LENGTH-1, fp_dat) != NULL) {
		argc = parse(buff, "\t", argv);
		if (argc == 5) {/*5でない場合、分割に失敗したと思われる*/
			members[number_of_members].name = AllocAndCopy(argv[1]);
			members[number_of_members].site = AllocAndCopy(argv[2]);
			members[number_of_members].color = AllocAndCopy(argv[3]);
			members[number_of_members].time = atol(argv[0]);

			if (strcmp(members[number_of_members].name, members[number_of_members].site) != 0) {
				if ((members[number_of_members].time + 60) > current_time)
						count++;
				/* host一致でタイム更新 */
				if (strcmp(host, members[number_of_members].site) == 0 && sankamode == 1) {
					members[number_of_members].time = current_time;
					members[number_of_members].name = name;
					members[number_of_members].color = color;
					found = 1;
				}
				number_of_members++;
			} else number_of_rommembers++;
		}
	}

	fclose(fp_dat);

#if NO_NAMETAG == 0 && NO_TAG == 0
	if (strstr(name, "<!") == NULL) {
#endif
#if MODE_SANKA == 1
		if (strcmp(chatmode, "sanka") != 0) {
#endif
	/* 更新した参加ファイルの書き込み */
		if ((fp_dat = fopen(CHAT_S, "w")) == NULL)
			error(FILE_WRITE_ERROR, "sanka_file");

		for (i = 0; i < number_of_members; i++) {		/*60秒以内のアクセスのみ書き込む*/
			if ((members[i].time + 60) > current_time)
				fprintf(fp_dat, "%d\t%s\t%s\t%s\td\n", (int)members[i].time, members[i].name, members[i].site, members[i].color);
		}

		if (!found)
				fprintf(fp_dat, "%d\t%s\t%s\t%s\td\n", (int)current_time, name, host, color);
		fclose(fp_dat);
#if MODE_SANKA == 1
		}
#endif
#if NO_NAMETAG == 0 && NO_TAG == 0
	}
#endif
}
