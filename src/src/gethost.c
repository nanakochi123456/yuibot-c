/*************************************************************
 *	ゆいちゃっとＣ言語版 1.0y1
 *	Japanese	version--sjis表示専用
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

#include"chat.h"

#if HOST_LOOKUP == 3 || HOST_LOOKUP == 4 || HOST_LOOKUP == 5

/* ミスるかもしれない、IPV4 IP文字列抽出 */

char	ipv4strbuf[16];

char	*parseipv4(char *str) {
	register	char	*s, *s2, c, *buf;
	int			digit_count, dot_count;

	if (str == NULL || *str == '\0') return NULL;
	s2 = buf = ipv4strbuf;

	digit_count = dot_count = 0;
	for (s = str; *s != '\0'; s++) {
		c = *s;
		if (isdigit(c)) {	/* 数字文字ならカウント */
			*s2++ = c;
			digit_count++;
		} else if (c == '.') { /* dotでかつその前が数字なら１つめOK */
			if (digit_count >= 1 && digit_count <= 3) {
				*s2++ = c;
				dot_count++;
				digit_count = 0;
			} else {	/* ダメなら、検索再開 */
				digit_count = dot_count = 0;
				s2 = buf;
			}
		} else {
			if (dot_count == 3) {	/* めでたくIPと認識されてたら */
				*s2 = '\0';		 	/* 他の文字が存在しても終了 */
				return buf;
			} else {				/* IPと認識されてなければリセット */
				digit_count = dot_count = 0;
				s2 = buf;
			}
		}
	}
	if (dot_count == 3) {
		*s2 = '\0';
		return buf;
	}
	return NULL;
}
#endif

char	*gethost(void) {
	char	*host;
	char	*remote_addr = getenv("REMOTE_ADDR");
#if HOST_LOOKUP	== 1 || HOST_LOOKUP == 4
	char	*remote_host = getenv("REMOTE_HOST");
#endif
	/* proxyのIPを抜く場合 */
#if HOST_LOOKUP == 3 || HOST_LOOKUP == 4 || HOST_LOOKUP == 5
	/* 環境変数をそれぞれ抜いてみる */
	char	*http_via = getenv("HTTP_VIA");
	char	*http_x_forwarded_for = getenv("HTTP_X_FORWARDED_FOR");
	char	*http_forwarded_for = getenv("HTTP_FORWARDED_FOR");
	char	*http_forwarded = getenv("HTTP_FORWARDED");

	char	*s = NULL;


	/* HTTP_VIA の検査 */
	if (http_via != NULL)
		s = parseipv4(http_via);
	/* HTTP_X_FORWARDED_FOR の検査 */
	if (s == NULL && http_x_forwarded_for != NULL)
		s = parseipv4(http_x_forwarded_for);
	/* HTTP_FORWARDED_FOR の検査 */
	if (s == NULL && http_forwarded_for != NULL)
		s = parseipv4(http_forwarded_for);
	/* HTTP_FORWARDED の検査 */
	if (s == NULL && http_forwarded != NULL)
		s = parseipv4(http_forwarded);
	/* proxyIPがめでたく抜けていれば、remote_addrを置き換える */
	if (s != NULL)
		remote_addr = s;
#endif	/* プロクシIP抜き処理終了 */

	/* ホスト名をapacheから取得する */
#if HOST_LOOKUP	== 1 && HOST_LOOKUP == 4
	if (remote_host != NULL) {
		if (*remote_host != '\0')
			host = remote_host;
	} else
		host = remote_addr;
	/* proxyのIPが抜けていればそちら優先 */
#if HOST_LOOKUP == 4
	if (s != NULL)
		host = s;
#endif
#else
	host = remote_addr;
#endif

	return host;
}
