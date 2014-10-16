#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"chat.h"

void            ReadMembers(char *buff);
char           *AllocAndCopy(char *str);
int             parse(char *buff, char *seps, char *argv[], char rem);
void            GetValue();
void            Sanka();
void            Html();
void            Write();

int decode(char *str,int henkanmode);
int error(int code);

struct membersstr {
	char           *name;
	char           *site;
	long            time;
}               members[36];/*予測される最大参加人数*/

struct formpairs {
	char           *name;
	char           *value;
}               forms[MAX_FORM - 1];/*FORM(GET)の値*/

int             number_of_members = 0;/*アクセス人数*/
int             found = 0;/*参加者記録ファイルに自分が居るか*/
int             count = 0;/*参加表示人数*/
long            current_time;

char           *host;
char            name[64], message[MAX_MESSAGE], message2[MAX_MESSAGE], emoji[64], email[64];
char           *name_p, *message_p, *emoji_p, *email_p;
char           *env, *window, *reload, *color, *chatmode;
int             windows;
int             i = 0;
int             j = 0;
char            buff[LINE_LENGTH];
char           *logs[MAX_LINE];
char            dmy[] = "";
char            dmywindow[] = "10";
char            dmyreload[] = "30";
char            dmycolor[] = "red";
char            d_time[26];
struct tm      *tim;
FILE           *fp_dat;
int             ret;
static char    *youbi[] = {
	"日", "月", "火", "水", "木", "金", "土"
		};

static char    *taglist[] = {
	"<script","</script","<body","</body",
	"<html","</html","<head","</head",
	"<title","</title", "onload", "onclick", "<xmp", "<plain",
	"<!--#",
	""			/* EOF */
};


int main()
{
	GetValue();      /*GETメソッドでのデコード処理＆SJIS変換*/
	Sanka();          /*参加者記録処理*/
	if ((strcmp(message, "退室") == 0)) {       /*退室時は、書き込み後Locationヘッダ*/
		Write();
		fprintf(stdout, "Location: %s\n\n", END_PAGE);
		return 1;
	}
	
	Html();           /*HTML文を出力する。書き込みがあればWriteする。*/
	return 1;
}

void 
		GetValue()
{
	char           *decode_tmp;       /*デコード用TMP*/
	current_time = time(NULL) + JISA;        /*現在時刻が秒で入る*/
	tim = localtime(&current_time);
	sprintf(d_time, "%d/%d(%s) %d:%02d",
			tim->tm_mon + 1,
			tim->tm_mday,
			youbi[tim->tm_wday],
			tim->tm_hour,
			tim->tm_min);               /*時刻表示のフォーマット*/
	host = getenv("REMOTE_HOST");
	env = getenv("QUERY_STRING");
	if ((forms[0].name = strtok(env, "&")) == NULL) {
		puts("Content-type: text/html\n\naccess format error 1\n\n");
	}
           /*とりあえず、&記号で区切る。*/
	for (i = 1; i < MAX_FORM; i++) {	/*MAX_FORMは formの要素数より大きく。 */
		if ((forms[i].name = strtok(NULL, "&")) == NULL)
				break;	/* 最後はここでbreak */
	}
	j = i;		/* jはフォームの要素数になる */
	
	for (i = 0; i < j; i++) {               /* 次に=記号で区切る。*/
        decode_tmp=forms[i].name;
		if ((forms[i].name = strtok(decode_tmp, "=")) == NULL)
				forms[i].name = dmy;
		if ((forms[i].value = strtok(NULL, "&")) == NULL)
				forms[i].value = dmy;
	}
	
	for (i = 0; i < j; i++) {       /*name&valueのペアから、各変数に代入する。*/
		/* printf("[%s= %s]<BR>\n",forms[i].name,forms[i].value); */
		if (strcmp(forms[i].name, "window") == 0)
				window = forms[i].value;
		else if (strcmp(forms[i].name, "reload") == 0)
				reload = forms[i].value;
		else if (strcmp(forms[i].name, "name") == 0)
				name_p = forms[i].value;
		else if (strcmp(forms[i].name, "chat") == 0)
				message_p = forms[i].value;
		else if (strcmp(forms[i].name, "color") == 0)
				color = forms[i].value;
		else if (strcmp(forms[i].name, "emoji") == 0)
				emoji_p = forms[i].value;
		else if (strcmp(forms[i].name, "email") == 0)
				email_p = forms[i].value;
		else if (strcmp(forms[i].name, "mode") == 0)
				chatmode = forms[i].value;
	}       /*上記の処理によって、フォームの順番を気にしなくて済む。*/
	if ((host) == NULL)
			host = dmy;       /*FORM入力で値が入らなかったら、ダミー値を入れる。*/
	if ((window) == NULL)
			window = dmywindow;
	if ((name_p) == NULL)
			name_p = host;
	if ((reload) == NULL)
			reload = dmyreload;
	if ((message_p) == NULL)
			message_p = dmy;
	if ((color) == NULL)
			color = dmycolor;
	if ((emoji_p) == NULL)
			emoji_p = dmy;
	if ((email_p) == NULL)
			email_p = dmy;
	if ((chatmode) == NULL)
			chatmode = dmy;

	/*memset(name, '\0', sizeof(name));
	memset(message, '\0', sizeof(message));
	memset(emoji, '\0', sizeof(emoji));
	memset(email, '\0', sizeof(email));変数の初期化は不要かしら？*/
	
	strncpy(email, email_p, 63);
	strncpy(name, name_p, 63);
	strncpy(emoji, emoji_p, 63);	
	strncpy(message, message_p, MAX_MESSAGE-1);	/*デコードが必要なものはコピー*/
	
	if (strlen(name) != 0) {
		*(name + 63) = '\0';       /*万一に備えて\0を入れておく。おまぬけ？*/
		ret = decode(name, SJIS);      /*デコードしたのち、SJIS変換*/
		if (ret != NORMAL)
				error(ret);
	}
	if (strlen(message) != 0) {
		*(message + MAX_MESSAGE-1) = '\0';
		ret = decode(message, SJIS);
		if (ret != NORMAL)
				error(ret);
	}
	if (strlen(emoji) != 0) {
		*(emoji + 63) = '\0';
		ret = decode(emoji, SJIS);
		if (ret != NORMAL)
				error(ret);
	}
	if (strlen(email) != 0) {
		*(email + 63) = '\0';
		ret = decode(email, SJIS);
		if (ret != NORMAL)
				error(ret);
	}

	if (strcmp(emoji, "(=＾＾=)") == 0) {/*顔文字を加工する。*/
		sprintf(emoji, "(<font color=red>=</font>＾＾<font color=red>=</font>)\0");
	}

}


void 
		Sanka()
{
	/* 更新する参加ファイルの読み込み */
	
	if ((fp_dat = fopen(CHAT_S, "r")) == NULL) {
		puts("Content-type: text/html\n\nsanka_file open error\n\n");
		return;
	}
	while (fgets(buff, LINE_LENGTH-1, fp_dat) != NULL)
			ReadMembers(buff);
	fclose(fp_dat);
	
	/* 更新した参加ファイルの書き込み */
	if ((fp_dat = fopen(CHAT_S, "w")) == NULL) {
		puts("Content-type: text/html\n\nsanka_file write error");
		return;
	}
	for (i = 0; i < number_of_members; i++) {       /*60秒以内のアクセスのみ書き込む*/
		if ((members[i].time + 60) > current_time)
				fprintf(fp_dat, "%d\t%s\t%s\td\n", members[i].time, members[i].name, members[i].site);
	}
	
	if (!found)
			fprintf(fp_dat, "%d\t%s\t%s\td\n", current_time, name, host);
	fclose(fp_dat);
}


void 
		Html()
{
	printf("Content-type: text/html\n\n");    /*おきまりのヘッダ～*/
	printf("<HTML><HEAD><TITLE>ゆいちゃっと</TITLE>");
	
	
	if (strcmp(reload, "0") != 0 && strcmp(chatmode, "checked") == 0)
			printf("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"%s;URL=%s?window=%s&reload=%s&name=%s&mode=%s\">\n", reload, CGI_URL, window, reload, name, chatmode);
	printf("%s\n", BODY);
	
	if (strcmp(chatmode, "checked") != 0){/*ノンフレーム発言欄*/
		
		printf("<FORM METHOD=\"GET\" ACTION=\"chat.cgi\">");
		printf("<FONT SIZE=+2 COLOR=\"hotpink\"><B>ゆいちゃっと</B></FONT>おなまえ:<B>%s</B>\n",name);
		
		printf("<INPUT TYPE=hidden NAME=\"email\" VALUE=\"%s\">"
		"<INPUT TYPE=hidden NAME=\"reload\" VALUE=\"0\">"
		"<INPUT TYPE=hidden NAME=\"color\" VALUE=\"%s\">"
		"<INPUT TYPE=hidden NAME=\"name\" VALUE=\"%s\">",email,color,name);
		
		printf("ログ行数:<SELECT NAME=\"window\" SIZE=\"1\">"
		"<OPTION selected>%s<OPTION>40<OPTION>30"
		"<OPTION>20<OPTION>10</SELECT>",window);
		
		printf("今のきもち:<SELECT NAME=\"emoji\" SIZE=\"1\">"
		"<OPTION VALUE=\"\" selected>なし<OPTION VALUE=\"(＾＾)\">にこにこ"
		"<OPTION VALUE=\"(＾＾；)\">冷や汗もん<OPTION VALUE=\"(=＾＾=)\">はずかしいよぉ"
		"<OPTION VALUE=\"(；；)\">悲しい<OPTION VALUE=\"(＠＠；)\">驚き"
		"<OPTION>(-_-)zzz<OPTION>＼(~o~)／"
		"<OPTION> m(__)m<OPTION>(^_-)-☆<OPTION>φ(.. )　めもめも"
		"<OPTION>(;_;)/~~~<OPTION>(^^)/~~~<OPTION>(+_+)"
		"<OPTION>((((((^_^;)<OPTION>(^-^;<OPTION>(?_?)え?</SELECT>");
		
		printf("<BR>発言:<INPUT TYPE=text NAME=\"chat\" SIZE=\"80\">"
		"<TABLE border=0><TR>"
		"<TH><INPUT TYPE=submit VALUE=\"発言/リロード\"></TH>"
		"<TH><INPUT TYPE=reset VALUE=\"リセット\"></TH></FORM>"
		"<FORM METHOD=\"GET\" ACTION=\"chat.cgi\">\n");
		
		printf("<INPUT TYPE=hidden NAME=\"email\" VALUE=\"%s\">"
		"<INPUT TYPE=hidden NAME=\"reload\" VALUE=\"0\">"
		"<INPUT TYPE=hidden NAME=\"color\" VALUE=\"%s\">"
		"<INPUT TYPE=hidden NAME=\"name\" VALUE=\"%s\">",email,color,name);
		
		printf("<INPUT TYPE=hidden NAME=\"chat\" VALUE=\"退室\">"
		"<TH><INPUT TYPE=submit VALUE=\"退室する\"></TH></FORM>"
		"<TH>(<A HREF=\"./index.html\">変更</A>)</TH></TR></TABLE>\n");

	}/*ノンフレーム発言欄ここまで*/
	printf("<A HREF=\"%s?window=%s&reload=%s&name=%s&mode=%s\">[リロード]</A>", CGI_URL, window, reload, name, chatmode);
	/* 参加者表示 */
	
	printf("<FONT SIZE=2>参加者(%d)：", count);
	
	for (i = 0; i < number_of_members; i++) {
		
		if (strcmp(members[i].name, members[i].site) != 0) {
			if ((members[i].time + 60) > current_time)
					printf("%s☆", members[i].name);
		}
	}
	printf("</FONT><HR>");

	if (strlen(message) != 0) {      /*書き込みがある場合*/
		Write();
		
		if ((strcmp(message, "退室") == 0)) {      /*フォーマット、順番に注意*/
			fprintf(stdout, MESSAGE_EXIT, color, name, d_time, host);
		} else if (strlen(email) == 0) {
			fprintf(stdout, MESSAGE, color, name, message, emoji, d_time, host);
		} else {
			fprintf(stdout,MESSAGE_MAIL, color, name, email, message, emoji, d_time, host);
		}
		
		windows = atoi(window);
		for (i = 0; i < windows; i++) {
			if (logs[i] == NULL)
					break;
			printf("%s", logs[i]);
		}
		
	} else {
		
		if ((fp_dat = fopen(CHAT_LOG, "r")) == NULL) {
			puts("chat_file open error");
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
			printf("%s", buff);
			
		}
		fclose(fp_dat);
	}			/* if else end */
	
	
	printf("<H5 align=right><A HREF=\"http://www.cup.com/yui/\">ゆいちゃっと(C言語版)</A></H5></BODY>\n");
	
}
void 
		Write()
{
	if (strlen(name) == 0)
sprintf(name, "初心者\0");      /*名前未入力時の名前*/

	/* 更新するログファイルの読み込み */
	if ((fp_dat = fopen(CHAT_LOG, "r")) == NULL) {
		puts("Content-type: text/html\n\nchat_file open error");
		return;
	}
	for (i = 0; i < MAX_LINE; i++) {
		if (fgets(buff, LINE_LENGTH-1, fp_dat) == NULL) {
			if (ferror(fp_dat))
					break;
			if (feof(fp_dat))
					break;
		}
		logs[i] = AllocAndCopy(buff);
		
	}
	fclose(fp_dat);
	/* コマンド機能 */
	if (strcmp(message, "clear") == 0) {
		for (i = 0; i < MAX_LINE - 1; i++) {
			if ((logs[i]) == NULL)
					break;
			if ((strstr(logs[i], host) != NULL))
					logs[i] = dmy;/*clearの場合、ホスト一致で削除*/
		}
		sprintf(message, "♪～\0");
	} else if (strcmp(message, "cut") == 0) {
		/* cutは、<imgがあれば削除*/
		for (i = 0; i < MAX_LINE - 1; i++) {
			if ((logs[i]) == NULL)
					break;
			/**/
			strncpy(message2, logs[i], MAX_MESSAGE-1);
			{
				char           *p = message2;
				while (*p = tolower(*p))
						p++;
			}
			if ((strstr(message2, "<img") != NULL))
					logs[i] = dmy;
			/**/
			
		}
		sprintf(message, "♪♪～\0");
	}
	/* コマンド機能ここまで */
	/* 禁止タグ消し */
	strncpy(message2, message, MAX_MESSAGE-1);
	{
		int             count = 0;
		int             i, len, c = 0;
		char           *p = message2;
		while (*p = tolower(*p))
				p++;
		   /*禁止タグが含まれていたら、タグをエスケープする。*/
		while (*taglist[count] != '\0') {
			if ((strstr(message2, taglist[count]) != NULL)) {
				strncpy(message2, message, MAX_MESSAGE-1);
				len = strlen(message2);
				for (i = 0; i < len; i++) {
					if (message2[i] == '<') {
						sprintf(&message[c], "&lt;");
						c += 4;
						continue;
					} else if (message2[i] == '>') {
						sprintf(&message[c], "&gt;");
						c += 4;
						continue;
					} else {
						message[c] = message2[i];
						c++;
					}
				}
				message[c] = '\0';
				break;
			}
			count++;
		}
	}	
	/* 禁止タグ消しここまで */
	
	if ((fp_dat = fopen(CHAT_LOG, "w")) == NULL) {
		puts("Content-type: text/html\n\nchat_file write error");
		return;
	}
	if ((strcmp(message, "退室") == 0)) {   /*フォーマット、順番に注意*/
		fprintf(fp_dat, MESSAGE_EXIT, color, name, d_time, host);
	} else if (strlen(email) == 0) {
		fprintf(fp_dat,MESSAGE, color, name, message, emoji, d_time, host);
	} else {
		fprintf(fp_dat, MESSAGE_MAIL, color, name, email, message, emoji, d_time, host);
	}
	
	
	for (i = 0; i < MAX_LINE - 1; i++) {  /*残りのログを書き込む*/
		if ((logs[i]) == NULL)
				break;
		fprintf(fp_dat, "%s", logs[i]);
		
	}
	fclose(fp_dat);
}
char           *
		AllocAndCopy(char *str)
{
	char           *p;
	
	p = (char *) calloc((strlen(str) + 1), sizeof(char));
	sprintf(p, "%s\0", str);
	
	return p;
}


int
		parse(char *buff, char *seps, char *argv[], char rem)
{
	char           *p;
	int             argc = 0;
	if (rem && (p = strchr(buff, rem)) != NULL)
			*p = '\0';
	p = strtok(buff, seps);
	while (p) {
		argv[argc++] = p;
		p = strtok(NULL, seps);
	}
	return argc;
}

void 
		ReadMembers(char *buff)
{
	
	int             argc = 0;
	char           *argv[10];
	
	argc = parse(buff, "\t", argv, NULL);
	
	if (argc == 4) {/*4でない場合、分割に失敗したと思われる*/
		
		members[number_of_members].name = AllocAndCopy(argv[1]);
		members[number_of_members].site = AllocAndCopy(argv[2]);
		members[number_of_members].time = atol(argv[0]);
		
		if (strcmp(members[number_of_members].name, members[number_of_members].site) != 0) {
			
			if ((members[number_of_members].time + 60) > current_time)
					count++;
			/* host一致でタイム更新 */
			
			if (strcmp(host, members[number_of_members].site) == 0) {
				
				members[number_of_members].time = current_time;
				members[number_of_members].name = name;
				found = 1;
			}
			number_of_members++;
		}
	}
}

