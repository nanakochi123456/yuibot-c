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
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/time.h>

#define	LOCKDIR	"."

#define	INSERT_BUFSIZ	16384
#define	ESCAPE_CHAR	'$'

#if FILE_LOCK == 1
#define	LOCKMODE1	// Linux, FreeBSD OK
#endif
#if FILE_LOCK == 2
#define	LOCKMODE2	// いんちき版
#endif

#if	FILE_LOCK != 0
int lockFd = 0;

#ifdef	LOCKMODE2
char	lockname[256];
#define	LOCK_READ	'r';
#define	LOCK_WRITE	'w';
#define	TIMEOUT		10
#endif

void	u_sleep(int secs, int usecs) {
	struct timeval timeout;
	timeout.tv_sec = secs;
	timeout.tv_usec = usecs;
	select(1, NULL, NULL, NULL, &timeout);
}

// ロック
void lock() {
#ifdef	LOCKMODE1
	int i;

	for (i = 0; i < 20; i++) {
		lockFd = open(LOCKDIR, O_RDWR);
		if(flock(lockFd, LOCK_EX)) 
			return;
		u_sleep(0, 100);
	}
	exit(0);
#endif
#ifdef	LOCKMODE2
	FILE	*lockfp;		/* ロックファイルのファイルハンドル */
	int	i;
	strcpy(lockname, LOCKDIR);
	strcat(lockname, ".lock");

	for (i = 0; i < 10; i++) {
		if ((lockfp = fopen(lockname, "r")) != NULL) {
			usleep(1);
		} else break;
	}

	unlink(lockname); // ちょっち強制？
	if ((lockfp = fopen(lockname, "w")) == NULL) {
		exit(1);
	} else {
		fclose(lockfp);
	}
#endif
}

// アンロック
void unlock() {
#ifdef	LOCKMODE1
    flock(lockFd, LOCK_UN);
    close(lockFd);
#endif
#ifdef	LOCKMODE2
	strcpy(lockname, LOCKDIR);
	strcat(lockname, ".lock");
	unlink(lockname);		/* locknameの削除 */
#endif
}
#endif

// 乱数
int	dice(int n) {
	return (random()  + getpid() ) % n;
}

char	*stack_init(STACK **stack, int memsize) {
	char	*s;
	*stack = (STACK *)malloc(sizeof(STACK) * (MAX_STACK + 1));
	s = (char *)malloc(memsize);
	(*stack)->stack[0] = s;
	(*stack)->no = 0;
	(*stack)->useflag = 1;
	return(s);
}

char	*stack_push(STACK **stack, int memsize) {
	char	*s;
	if (*stack == NULL)
		return stack_init(stack, memsize);

	if ((*stack)->no == MAX_STACK - 1)
		return(NULL);

	s = (char *)malloc(memsize);
	(*stack)->stack[++(*stack)->no] = s;
	return(s);
}

char	*stack_pop(STACK **stack) {
	if (*stack == NULL)
		return NULL;

	if ((*stack)->no == 0)
		return((*stack)->stack[0]);

	free((*stack)->stack[(*stack)->no--]);
	return((*stack)->stack[(*stack)->no]);
}

time_t	ltime;
STACK	*insstack = NULL;

char	*insertparm(char *s, JMPTBL jmptbl[]) {
	register	char	*ps, *pss;
	register	int		i;
	char	*ss;
	char	*cmpbuf, *execbuf;
	int		flg;

	if (strchr(s, ESCAPE_CHAR) == NULL)
		return(s);

	if (insstack == NULL)
		stack_init(&insstack, INSERT_BUFSIZ);

	ss = stack_push(&insstack, INSERT_BUFSIZ);
	ps = s;
	pss = ss;

	cmpbuf = stack_push(&insstack, INSERT_BUFSIZ);
	execbuf = stack_push(&insstack, INSERT_BUFSIZ);
	while(*ps != '\0') {
		flg = 0;
		if (*ps == ESCAPE_CHAR) {
//			if (!(iskanji1st((unsigned char)(*(ps - 1))))) {
				strncpy(cmpbuf, ps + 1, INSERT_BUFSIZ);
				for (i = 0; jmptbl[i].name != NULL; i++) {
					if (ext_strncmp(cmpbuf, jmptbl[i].name, strlen(jmptbl[i].name), 0) == 0) {
						/* 文字列一致時 */
						ps += strlen(jmptbl[i].name) + 1;
						(*jmptbl[i].compar)(execbuf, jmptbl[i].name);
						if (*execbuf) {
							strcpy(pss, execbuf);
							while(*(++pss) != '\0');
						}
						flg = 1;
					}
//				}
			}
		}
		if (flg == 0)
			*pss++ = *ps++;
//		flg = 0;
	}
	*pss = '\0';
	strcpy(s, ss);

	stack_pop(&insstack);
	stack_pop(&insstack);
	stack_pop(&insstack);

	return(s);
}

char	*strreplace(char *s, char *old, char *newstr, int lower) {
	register	char	*ps, *pss;
	register	int		i;
	char	*ss;
	char	*cmpbuf, *execbuf;
	int		flg;

	if (ext_strstr(s, old, lower) == NULL)
		return(s);

	if (insstack == NULL)
		stack_init(&insstack, INSERT_BUFSIZ);

	ss = stack_push(&insstack, INSERT_BUFSIZ);
	ps = s;
	pss = ss;

	cmpbuf = stack_push(&insstack, INSERT_BUFSIZ);
	execbuf = stack_push(&insstack, INSERT_BUFSIZ);

	while(*ps != '\0') {
		flg = 0;
		if (*ps == *old) {
			if (ext_strncmp(ps, old, strlen(old), lower) == 0) {
				ps += strlen(old);// + 1;
				strcpy(pss, newstr);
				while(*(++pss) != '\0');
				flg = 1;
			}
		}
		if (flg == 0)
			*pss++ = *ps++;
	}
	*pss = '\0';
	strcpy(s, ss);

	stack_pop(&insstack);
	stack_pop(&insstack);
	stack_pop(&insstack);

	return(s);
}

STACK	*restack = NULL;

char	*re(char *ptn, char *s, int lower) {
	register	char	*s2, *b2, *b3, *s3, *s4 = NULL, *ret;

	if (restack == NULL)
		stack_init(&restack, INSERT_BUFSIZ);

	b2 = stack_push(&restack, INSERT_BUFSIZ);
	s2 = stack_push(&restack, INSERT_BUFSIZ);
	strcpy(s2, s);
	strcpy(b2, ptn);
	do {
		/* ワイルドカードがなければ、もしくは最後であれば */
		if ((ret = ext_strstr(s2, b2, lower)) != NULL) {
			stack_pop(&restack);
			stack_pop(&restack);
			return s4 == NULL ? (ret - s2 + s) : s4;
		}
		/* 後ろにワイルドカードが存在すれば */
		if ((b3 = ext_strstr(b2, WILD_CARD, lower)) != NULL) {
			*b3 = '\0';
			/* かつ一致すれば */
			if ((s3 = ext_strstr(s2, b2, lower)) != NULL) {
				s4 = s4 == NULL ? (s3 - s2 + s): s4;
				s3++;
				b2 = b3 + strlen(WILD_CARD);
			} else {
				break;
			}
		} else {
			break;
		}
	} while(*b2 != '\0');
	stack_pop(&restack);
	stack_pop(&restack);
	return NULL;
}

int	ext_strncmp(s1, s2, n, mode)
        register const char *s1, *s2;
        register size_t n;
	int	mode;
{

        if (n == 0)
                return (0);
        do {
                if (ext_tolower(*s1, mode) != ext_tolower(*s2++, mode))
                        return (*(const unsigned char *)s1 -
                                *(const unsigned char *)(s2 - 1));
                if (*s1++ == 0)
                        break;
        } while (--n != 0);
        return (0);
}

char *ext_strstr(s, find, mode)
	register const char *s, *find;
	int	mode;
{
	register char c, sc;
	register size_t len;

	if ((c = ext_tolower(*find++, mode)) != 0) {
		len = strlen(find);
		do {
			do {
				if ((sc = ext_tolower(*s++, mode)) == 0)
					return (NULL);
			} while (sc != c);
		} while (ext_strncmp(s, find, len, mode) != 0);
		s--;
	}
	return ((char *)s);
}

char	*trim(register char *buf, register char *src) {
	char	*destorg;

	destorg = buf;

	while(*src == '\t')
		src++;
	while(*src == ' ')
		src++;
	while(*src == '\t')
		src++;

	while(*src != '\0')
		*buf++ = *src++;
	*buf = '\0';

	while(*(buf-1) == '\t')
		*(--buf) = '\0';
	while(*(buf-1) == ' ')
		*(--buf) = '\0';
	while(*(buf-1) == '\t')
		*(--buf) = '\0';

	return(destorg);
}

char	readbuf[LINE_LENGTH];

char	*getini_comment(char *buf2) {
	char	*s;
	if (s = strchr(buf2, '\r'))
		*s = '\0';
	if (s = strchr(buf2, '\n'))
		*s = '\0';
	if (*buf2 == ';')
		*buf2 = '\0';
	if (*buf2 == '#')
		*buf2 = '\0';

	strreplace(buf2, "\\n", "\n", 0);
	strreplace(buf2, "\\t", "\t", 0);
	return buf2;
}

char	*iniget(char *buf, int len, FILE *rfp) {
	char	*buf2, *buf3;
	int		nowadd = 0;

	*buf = '\0';
	while(!feof(rfp)) {
		fgets(buf2 = readbuf, LINE_LENGTH, rfp);
		getini_comment(buf2);
		if (*buf2 == '\0' || *buf2 == '\r' || *buf2 == '\n' || *buf2 == '#' || *buf2 == ';')
			continue;
		trim(buf, buf2);

		if (*buf == '\0')
			continue;

		while(buf[strlen(buf)-1] == '\\') {
			buf[strlen(buf)-1] = '\0';
			fgets(buf2 = readbuf, LINE_LENGTH, rfp);
			getini_comment(buf2);
			if (*buf == '\0' || *buf == '\r' || *buf == '\n' || *buf == '#' || *buf == ';')
				continue;
			if (strlen(buf) + strlen(buf2) >= nowadd + LINE_LENGTH) {
				nowadd += LINE_LENGTH;
				buf3 = malloc(nowadd + LINE_LENGTH);
				strcpy(buf3, buf);
				free(buf);
				buf = buf3;
			}
			buf3 = malloc(LINE_LENGTH);
			trim(buf3, buf2);
			strcat(buf, buf3);
			free(buf3);
		}
		return(buf);
	}
	return(NULL);
}

char	*sectionnext(char *buf, int len, FILE *rfp) {
	char	*temp;

	while(1) {
		if (iniget(buf, len, rfp) == NULL)
			return(NULL);

		if (buf[0] == '[' && buf[strlen(buf) - 1] == ']') {
			temp = buf;
			while(*temp != ']') {
				*temp = *(temp + 1);
				temp++;
			}
			*(--temp) = '\0';
			return(buf);
		}
	}
}

char	*sectionfirst(char *buf, int len, FILE *rfp) {

	fseek(rfp, 0L, SEEK_SET);

	return(sectionnext(buf, len, rfp));
}

char	*datanext(char *buf, int len, FILE *rfp) {
	long	pnt;

	while(1) {
		pnt = ftell(rfp);
		if (iniget(buf, len, rfp) == NULL)
			return(NULL);

		if (buf[0] != '[') {
			return(buf);
		} else {
			fseek(rfp, pnt, SEEK_SET);
			return(NULL);
		}
	}
}

void	stringup(register char *str) {
	while (*str = toupper(*str)) str++;
}

char	*datafirst(char *buf, int len, char *word, FILE *rfp) {
	char	*temp;

	fseek(rfp, 0L, SEEK_SET);

	while(1) {
		temp = sectionnext(buf, len, rfp);
		if (temp == NULL)
			return(NULL);

		if (strcmp(buf, word) == 0)
			break;
	}
	return(datanext(buf, len, rfp));
}

int		sectioncount(FILE *rfp) {
	char	*temp;
	long	pnt;
	int		count;

	temp = malloc(LINE_LENGTH);
	pnt = ftell(rfp);
	if (sectionfirst(temp, LINE_LENGTH, rfp) == NULL) {
		fseek(rfp, pnt, SEEK_SET);
		free(temp);
		return(0);
	}
	count = 0;

	while(sectionnext(temp, LINE_LENGTH, rfp) != NULL)
		count++;

	fseek(rfp, pnt, SEEK_SET);
	free(temp);
	return(count);
}

void	getarg(char **left, char **right, char *buf) {
	char	*temp;

	temp = malloc(LINE_LENGTH);

	if (strchr(buf, '=') == NULL)
		error(INI_FILE_ERROR, "");

	*right = strchr(buf, '=');
	(*right)++;
	*strchr(buf, '=') = '\0';
	*left = buf;

	trim(temp, *left);	strcpy(*left, temp);
	trim(temp, *right);	strcpy(*right, temp);

	free(temp);
}

char	*getini(char *buf, FILE *fp, char *section, char *name) {
	long	pnt;
	char	*temp;
	char	*ret;
	char	*t_sec;
	char	*t_nam;
	char	**left, **right;
	int		flg;

	left = (char **) malloc(sizeof(char **));
	right = (char **) malloc(sizeof(char **));

	temp = malloc(LINE_LENGTH);
	t_sec = malloc(LINE_LENGTH);
	t_nam = malloc(LINE_LENGTH);

	flg = 0;
	strcpy(t_sec, section);	stringup(t_sec);
	strcpy(t_nam, name); 		stringup(t_nam);
 
	pnt = ftell(fp);

	if (datafirst(temp, LINE_LENGTH, t_sec, fp) != NULL) {
		do {
			getarg(left, right, temp);
			stringup(*left);
			if (strcmp(*left, t_nam) == 0) {
				flg++;
				break;
			}
		} while(datanext(temp, LINE_LENGTH, fp) != NULL);
	}
	if (flg) {
		strcpy(ret = buf, *right);
	} else
		ret = NULL;

	fseek(fp, pnt, SEEK_SET);
	free(right);
	free(left);
	free(t_nam);
	free(t_sec);
	free(temp);
	return(ret);
}

int		getinibool(FILE *fp, char *section, char *name, int def) {
	int		i;
	char	*buf;

	buf = malloc(LINE_LENGTH);
	i = def;
	if (getini(buf, fp, section, name) != NULL) {
		stringup(buf);
		if (strcmp(buf, "TRUE") == 0)
			i = TRUE;
		if (strcmp(buf, "FALSE") == 0)
			i = FALSE;
	}
	free(buf);
	return i;
}

char           *AllocAndCopy(char *str) {
	char           *p;
	
	p = (char *) calloc((strlen(str) + 1), sizeof(char));
	strcpy(p, str);
	return p;
}

int	dircheck(char *s) {
	if (strstr(s, "..") != NULL || strchr(s, '/') != NULL
	 || strchr(s, '\\') != NULL)
		return 1;
	return 0;
}

int		parse(char *buff, char *seps, char *argv[]) {
	char		   *p;
	int 			argc = 0;
	p = strtok(buff, seps);
	while (p) {
		argv[argc++] = p;
		p = strtok(NULL, seps);
	}
	return argc;
}
