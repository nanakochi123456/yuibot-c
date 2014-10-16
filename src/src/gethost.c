/*************************************************************
 *	�䂢������Ƃb����� 1.0y1
 *	Japanese	version--sjis�\����p
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

#include"chat.h"

#if HOST_LOOKUP == 3 || HOST_LOOKUP == 4 || HOST_LOOKUP == 5

/* �~�X�邩������Ȃ��AIPV4 IP�����񒊏o */

char	ipv4strbuf[16];

char	*parseipv4(char *str) {
	register	char	*s, *s2, c, *buf;
	int			digit_count, dot_count;

	if (str == NULL || *str == '\0') return NULL;
	s2 = buf = ipv4strbuf;

	digit_count = dot_count = 0;
	for (s = str; *s != '\0'; s++) {
		c = *s;
		if (isdigit(c)) {	/* ���������Ȃ�J�E���g */
			*s2++ = c;
			digit_count++;
		} else if (c == '.') { /* dot�ł����̑O�������Ȃ�P��OK */
			if (digit_count >= 1 && digit_count <= 3) {
				*s2++ = c;
				dot_count++;
				digit_count = 0;
			} else {	/* �_���Ȃ�A�����ĊJ */
				digit_count = dot_count = 0;
				s2 = buf;
			}
		} else {
			if (dot_count == 3) {	/* �߂ł���IP�ƔF������Ă��� */
				*s2 = '\0';		 	/* ���̕��������݂��Ă��I�� */
				return buf;
			} else {				/* IP�ƔF������ĂȂ���΃��Z�b�g */
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
	/* proxy��IP�𔲂��ꍇ */
#if HOST_LOOKUP == 3 || HOST_LOOKUP == 4 || HOST_LOOKUP == 5
	/* ���ϐ������ꂼ�ꔲ���Ă݂� */
	char	*http_via = getenv("HTTP_VIA");
	char	*http_x_forwarded_for = getenv("HTTP_X_FORWARDED_FOR");
	char	*http_forwarded_for = getenv("HTTP_FORWARDED_FOR");
	char	*http_forwarded = getenv("HTTP_FORWARDED");

	char	*s = NULL;


	/* HTTP_VIA �̌��� */
	if (http_via != NULL)
		s = parseipv4(http_via);
	/* HTTP_X_FORWARDED_FOR �̌��� */
	if (s == NULL && http_x_forwarded_for != NULL)
		s = parseipv4(http_x_forwarded_for);
	/* HTTP_FORWARDED_FOR �̌��� */
	if (s == NULL && http_forwarded_for != NULL)
		s = parseipv4(http_forwarded_for);
	/* HTTP_FORWARDED �̌��� */
	if (s == NULL && http_forwarded != NULL)
		s = parseipv4(http_forwarded);
	/* proxyIP���߂ł��������Ă���΁Aremote_addr��u�������� */
	if (s != NULL)
		remote_addr = s;
#endif	/* �v���N�VIP���������I�� */

	/* �z�X�g����apache����擾���� */
#if HOST_LOOKUP	== 1 && HOST_LOOKUP == 4
	if (remote_host != NULL) {
		if (*remote_host != '\0')
			host = remote_host;
	} else
		host = remote_addr;
	/* proxy��IP�������Ă���΂�����D�� */
#if HOST_LOOKUP == 4
	if (s != NULL)
		host = s;
#endif
#else
	host = remote_addr;
#endif

	return host;
}
