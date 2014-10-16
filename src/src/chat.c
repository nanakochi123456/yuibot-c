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
}               members[36];/*�\�������ő�Q���l��*/

struct formpairs {
	char           *name;
	char           *value;
}               forms[MAX_FORM - 1];/*FORM(GET)�̒l*/

int             number_of_members = 0;/*�A�N�Z�X�l��*/
int             found = 0;/*�Q���ҋL�^�t�@�C���Ɏ��������邩*/
int             count = 0;/*�Q���\���l��*/
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
	"��", "��", "��", "��", "��", "��", "�y"
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
	GetValue();      /*GET���\�b�h�ł̃f�R�[�h������SJIS�ϊ�*/
	Sanka();          /*�Q���ҋL�^����*/
	if ((strcmp(message, "�ގ�") == 0)) {       /*�ގ����́A�������݌�Location�w�b�_*/
		Write();
		fprintf(stdout, "Location: %s\n\n", END_PAGE);
		return 1;
	}
	
	Html();           /*HTML�����o�͂���B�������݂������Write����B*/
	return 1;
}

void 
		GetValue()
{
	char           *decode_tmp;       /*�f�R�[�h�pTMP*/
	current_time = time(NULL) + JISA;        /*���ݎ������b�œ���*/
	tim = localtime(&current_time);
	sprintf(d_time, "%d/%d(%s) %d:%02d",
			tim->tm_mon + 1,
			tim->tm_mday,
			youbi[tim->tm_wday],
			tim->tm_hour,
			tim->tm_min);               /*�����\���̃t�H�[�}�b�g*/
	host = getenv("REMOTE_HOST");
	env = getenv("QUERY_STRING");
	if ((forms[0].name = strtok(env, "&")) == NULL) {
		puts("Content-type: text/html\n\naccess format error 1\n\n");
	}
           /*�Ƃ肠�����A&�L���ŋ�؂�B*/
	for (i = 1; i < MAX_FORM; i++) {	/*MAX_FORM�� form�̗v�f�����傫���B */
		if ((forms[i].name = strtok(NULL, "&")) == NULL)
				break;	/* �Ō�͂�����break */
	}
	j = i;		/* j�̓t�H�[���̗v�f���ɂȂ� */
	
	for (i = 0; i < j; i++) {               /* ����=�L���ŋ�؂�B*/
        decode_tmp=forms[i].name;
		if ((forms[i].name = strtok(decode_tmp, "=")) == NULL)
				forms[i].name = dmy;
		if ((forms[i].value = strtok(NULL, "&")) == NULL)
				forms[i].value = dmy;
	}
	
	for (i = 0; i < j; i++) {       /*name&value�̃y�A����A�e�ϐ��ɑ������B*/
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
	}       /*��L�̏����ɂ���āA�t�H�[���̏��Ԃ��C�ɂ��Ȃ��čςށB*/
	if ((host) == NULL)
			host = dmy;       /*FORM���͂Œl������Ȃ�������A�_�~�[�l������B*/
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
	memset(email, '\0', sizeof(email));�ϐ��̏������͕s�v������H*/
	
	strncpy(email, email_p, 63);
	strncpy(name, name_p, 63);
	strncpy(emoji, emoji_p, 63);	
	strncpy(message, message_p, MAX_MESSAGE-1);	/*�f�R�[�h���K�v�Ȃ��̂̓R�s�[*/
	
	if (strlen(name) != 0) {
		*(name + 63) = '\0';       /*����ɔ�����\0�����Ă����B���܂ʂ��H*/
		ret = decode(name, SJIS);      /*�f�R�[�h�����̂��ASJIS�ϊ�*/
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

	if (strcmp(emoji, "(=�O�O=)") == 0) {/*�當�������H����B*/
		sprintf(emoji, "(<font color=red>=</font>�O�O<font color=red>=</font>)\0");
	}

}


void 
		Sanka()
{
	/* �X�V����Q���t�@�C���̓ǂݍ��� */
	
	if ((fp_dat = fopen(CHAT_S, "r")) == NULL) {
		puts("Content-type: text/html\n\nsanka_file open error\n\n");
		return;
	}
	while (fgets(buff, LINE_LENGTH-1, fp_dat) != NULL)
			ReadMembers(buff);
	fclose(fp_dat);
	
	/* �X�V�����Q���t�@�C���̏������� */
	if ((fp_dat = fopen(CHAT_S, "w")) == NULL) {
		puts("Content-type: text/html\n\nsanka_file write error");
		return;
	}
	for (i = 0; i < number_of_members; i++) {       /*60�b�ȓ��̃A�N�Z�X�̂ݏ�������*/
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
	printf("Content-type: text/html\n\n");    /*�����܂�̃w�b�_�`*/
	printf("<HTML><HEAD><TITLE>�䂢�������</TITLE>");
	
	
	if (strcmp(reload, "0") != 0 && strcmp(chatmode, "checked") == 0)
			printf("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"%s;URL=%s?window=%s&reload=%s&name=%s&mode=%s\">\n", reload, CGI_URL, window, reload, name, chatmode);
	printf("%s\n", BODY);
	
	if (strcmp(chatmode, "checked") != 0){/*�m���t���[��������*/
		
		printf("<FORM METHOD=\"GET\" ACTION=\"chat.cgi\">");
		printf("<FONT SIZE=+2 COLOR=\"hotpink\"><B>�䂢�������</B></FONT>���Ȃ܂�:<B>%s</B>\n",name);
		
		printf("<INPUT TYPE=hidden NAME=\"email\" VALUE=\"%s\">"
		"<INPUT TYPE=hidden NAME=\"reload\" VALUE=\"0\">"
		"<INPUT TYPE=hidden NAME=\"color\" VALUE=\"%s\">"
		"<INPUT TYPE=hidden NAME=\"name\" VALUE=\"%s\">",email,color,name);
		
		printf("���O�s��:<SELECT NAME=\"window\" SIZE=\"1\">"
		"<OPTION selected>%s<OPTION>40<OPTION>30"
		"<OPTION>20<OPTION>10</SELECT>",window);
		
		printf("���̂�����:<SELECT NAME=\"emoji\" SIZE=\"1\">"
		"<OPTION VALUE=\"\" selected>�Ȃ�<OPTION VALUE=\"(�O�O)\">�ɂ��ɂ�"
		"<OPTION VALUE=\"(�O�O�G)\">��⊾����<OPTION VALUE=\"(=�O�O=)\">�͂��������悧"
		"<OPTION VALUE=\"(�G�G)\">�߂���<OPTION VALUE=\"(�����G)\">����"
		"<OPTION>(-_-)zzz<OPTION>�_(~o~)�^"
		"<OPTION> m(__)m<OPTION>(^_-)-��<OPTION>��(.. )�@�߂��߂�"
		"<OPTION>(;_;)/~~~<OPTION>(^^)/~~~<OPTION>(+_+)"
		"<OPTION>((((((^_^;)<OPTION>(^-^;<OPTION>(?_?)��?</SELECT>");
		
		printf("<BR>����:<INPUT TYPE=text NAME=\"chat\" SIZE=\"80\">"
		"<TABLE border=0><TR>"
		"<TH><INPUT TYPE=submit VALUE=\"����/�����[�h\"></TH>"
		"<TH><INPUT TYPE=reset VALUE=\"���Z�b�g\"></TH></FORM>"
		"<FORM METHOD=\"GET\" ACTION=\"chat.cgi\">\n");
		
		printf("<INPUT TYPE=hidden NAME=\"email\" VALUE=\"%s\">"
		"<INPUT TYPE=hidden NAME=\"reload\" VALUE=\"0\">"
		"<INPUT TYPE=hidden NAME=\"color\" VALUE=\"%s\">"
		"<INPUT TYPE=hidden NAME=\"name\" VALUE=\"%s\">",email,color,name);
		
		printf("<INPUT TYPE=hidden NAME=\"chat\" VALUE=\"�ގ�\">"
		"<TH><INPUT TYPE=submit VALUE=\"�ގ�����\"></TH></FORM>"
		"<TH>(<A HREF=\"./index.html\">�ύX</A>)</TH></TR></TABLE>\n");

	}/*�m���t���[�������������܂�*/
	printf("<A HREF=\"%s?window=%s&reload=%s&name=%s&mode=%s\">[�����[�h]</A>", CGI_URL, window, reload, name, chatmode);
	/* �Q���ҕ\�� */
	
	printf("<FONT SIZE=2>�Q����(%d)�F", count);
	
	for (i = 0; i < number_of_members; i++) {
		
		if (strcmp(members[i].name, members[i].site) != 0) {
			if ((members[i].time + 60) > current_time)
					printf("%s��", members[i].name);
		}
	}
	printf("</FONT><HR>");

	if (strlen(message) != 0) {      /*�������݂�����ꍇ*/
		Write();
		
		if ((strcmp(message, "�ގ�") == 0)) {      /*�t�H�[�}�b�g�A���Ԃɒ���*/
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
	
	
	printf("<H5 align=right><A HREF=\"http://www.cup.com/yui/\">�䂢�������(C�����)</A></H5></BODY>\n");
	
}
void 
		Write()
{
	if (strlen(name) == 0)
sprintf(name, "���S��\0");      /*���O�����͎��̖��O*/

	/* �X�V���郍�O�t�@�C���̓ǂݍ��� */
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
	/* �R�}���h�@�\ */
	if (strcmp(message, "clear") == 0) {
		for (i = 0; i < MAX_LINE - 1; i++) {
			if ((logs[i]) == NULL)
					break;
			if ((strstr(logs[i], host) != NULL))
					logs[i] = dmy;/*clear�̏ꍇ�A�z�X�g��v�ō폜*/
		}
		sprintf(message, "��`\0");
	} else if (strcmp(message, "cut") == 0) {
		/* cut�́A<img������΍폜*/
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
		sprintf(message, "���`\0");
	}
	/* �R�}���h�@�\�����܂� */
	/* �֎~�^�O���� */
	strncpy(message2, message, MAX_MESSAGE-1);
	{
		int             count = 0;
		int             i, len, c = 0;
		char           *p = message2;
		while (*p = tolower(*p))
				p++;
		   /*�֎~�^�O���܂܂�Ă�����A�^�O���G�X�P�[�v����B*/
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
	/* �֎~�^�O���������܂� */
	
	if ((fp_dat = fopen(CHAT_LOG, "w")) == NULL) {
		puts("Content-type: text/html\n\nchat_file write error");
		return;
	}
	if ((strcmp(message, "�ގ�") == 0)) {   /*�t�H�[�}�b�g�A���Ԃɒ���*/
		fprintf(fp_dat, MESSAGE_EXIT, color, name, d_time, host);
	} else if (strlen(email) == 0) {
		fprintf(fp_dat,MESSAGE, color, name, message, emoji, d_time, host);
	} else {
		fprintf(fp_dat, MESSAGE_MAIL, color, name, email, message, emoji, d_time, host);
	}
	
	
	for (i = 0; i < MAX_LINE - 1; i++) {  /*�c��̃��O����������*/
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
	
	if (argc == 4) {/*4�łȂ��ꍇ�A�����Ɏ��s�����Ǝv����*/
		
		members[number_of_members].name = AllocAndCopy(argv[1]);
		members[number_of_members].site = AllocAndCopy(argv[2]);
		members[number_of_members].time = atol(argv[0]);
		
		if (strcmp(members[number_of_members].name, members[number_of_members].site) != 0) {
			
			if ((members[number_of_members].time + 60) > current_time)
					count++;
			/* host��v�Ń^�C���X�V */
			
			if (strcmp(host, members[number_of_members].site) == 0) {
				
				members[number_of_members].time = current_time;
				members[number_of_members].name = name;
				found = 1;
			}
			number_of_members++;
		}
	}
}

