/*************************************************************
 *	�䂢������Ƃb����� 1.0y1
 *	Japanese	version--sjis�\����p
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

/*
 *
 * ��{�ݒ�
 *
 */

/* �S�^�O���g���Ȃ�����ɂ́A1�A�g����悤�ɂ���ɂ� 0 */
#define NO_TAG			1

/* ���O�Ƀ^�O���g�p�ł���悤�ɂ���ɂ� 0 */
#define	NO_NAMETAG		1

/* ���������N���g�p����ɂ́A1�A�g�p���Ȃ��悤�ɂ���ɂ� 0 */
#define	TAG_AUTOLINK	1

/* �����̐ݒ�i�b�P�ʁj */
#define JISA			0

/* POST���\�b�h�݂̂̏������݋��́A1 */
#define	POST_ONLY		0

/* �t�@�C�����b�N
�@ 0 �t�@�C�����b�N���g�p���Ȃ�
�@ 1 Linux, FreeBSD�p
�@ 2 ���̑�
*/
#define	FILE_LOCK		1

/* �z�X�g���̎擾���@
�@ 0 IP�̂�
�@ 1 �z�X�g��
�@ 2 �z�X�g���o�͂Ȃ�(IP)
�@ 3 IP(proxy�̃A�N�Z�X��������΂���IP)
�@ 4 �z�X�g��(proxy�̃A�N�Z�X��������΂���IP)
�@ 5 �z�X�g���o�͂Ȃ�(IP+proxy)
*/

#define	HOST_LOOKUP		0

/* �Q�d�Ń��O���Ƃ�ꍇ�A1 */
#define	DOUBLELOG		1

/* �A�N�Z�X�������g�p����ꍇ�A1 */
#define	KILL			1

/* REFERER�`�F�b�N
�@ 0 �g�p���Ȃ�
�@ 1 �g�p����(NULL�͒ʂ�) - SegaSaturn�u���E�U�΍� ���܂肨�����߂��܂���
�@ 2 �g�p����(NULL�͒ʂ��Ȃ�)
*/
#define	REFERERCHECK	0

/* kill.dat �Ƀ��C���h�J�[�h .* ���g����悤�ɂ���ꍇ�A1 */
#define	KILL_REUSE		1

/*
 *
 * �Q���ҁA�����L���O�̐ݒ�
 *
 */

/* �Q���ҕ\����L���ɂ���ɂ͂P */
#define	ALLOW_SANKA		1

/* chat.cgi �ŎQ���ҕ\�������Ȃ��i�L�^�����Ƃ�j �̏ꍇ�P */
#define	NOTPRINT_SANKA	1

/* mode=sanka �ŎQ���ҕ\��������ꍇ 1 */
#define	MODE_SANKA		1

/* �����L���O��L���ɂ���ɂ͂P */
#define	ALLOW_RANK		1

/* �\�z�����ő�Q���Ґ��i�����L���O�l���j */
#define	MAX_MEMBER		3000

/* �����L���O�ۑ����� (60��)*/
#define	RANK_TIME		60*24*60*60


/*
 *
 * �䂢������Ƃ̊e�탂�W���[���̐ݒ�
 *
 */

/* ��摜�A�C�R�����g�p�ł���悤�ɂ���ɂ� 1 */
#define	YUI_CUTE		0

/* ��摜�A�C�R���g�p���̃Z�L�����e�B�[�ݒ� */
/* �ʏ�͂��̂܂܂�OK�A���T�[�o�[����̎Q�Ƃ��\�ɂ���ɂ� 0 */
#define	YUI_CUTE_SEC	1

/* ��摜�ɂ���g���q */
#define YUI_CUTE_EXT	""
/* #define	YUI_CUTE_EXT	".gif" */

/*
 *
 * �ァ�ڂ��Ƃ̐ݒ�
 *
 */

/* �ァ�ڂ��Ƃ��g�p����ɂ� 1 */
#define	YUIBOT			1

/* ���݂����̂ݎg�p����ꍇ 1 */
#define	OMIKUJI_ONLY	0

/* bot�������Ŕ�������f�t�H���g�l��(�ebot�Őݒ��) */
#define	BOT_TALK		10

/* �ЂƂ育�Ƃ�����ꍇ 1 */
#define	MONO_TALK		1

/* �ЂƂ育�Ƃ̔����f�t�H���g���ԁi�b���j */
#define	MONO_TALK_TIME	18000000

/* bot�̍ő�l���i���F���݂�����1�l�ɐ����邱�Ɓj */
#define	BOT_MEMBERS		10

/* �L�[���[�h�����ɂ����āA�J�i�A�S�p�A���t�@�x�b�g�̕ϊ� */
#define	YUIBOT_KEYCONV	1

/* �w�K������ꍇ�A1 */
#define	BOT_KIOKU		0

/* �w�K�̕����� */
#define	LEARNSTR		"==="

/* �w�K�ł���Œᕶ���� */
#define	KEYLENGTH		4

/* �N���H */
#define	TOYUI			"��"

/* �����݂̊��� (�ő�100�j */
#define	BOKE_RATIO		5

/* �������ă��[�h�̊����i�ő�10�j */
#define	OSHIETE_RATIO	5

/* �ァ�ڂ��Ǝ����̋�؂蕶�� */
#define	YUIBOT_DIC	":#"

/* ALL���̕��� */
#define	ALL1			"ALL"
#define	ALL2			"all"
#define	ALL3			"All"
#define	ALL4			"�`�k�k"
#define	ALL5			"������"
#define	ALL6			"�`����"
#define	ALL7			"������"
#define	ALL8			"�I�[��"
#define	ALL9			"������"
#define	ALL10			"�݂��"

/* CGI�̃p�����[�^���� */
#define	WINDOW_STR			"window"
#define	RELOAD_STR			"reload"
#define	NAME_STR			"name"
#define	CHAT_STR			"chat"
#define	COLOR_STR			"color"
#define	EMOJI_STR			"emoji"
#define	EMAIL_STR			"email"
#define	MODE_STR			"mode"
#define	CHARA_STR			"chara"
#define	ROOM_STR			"log"

#define	CONFIGEXTENTION		"cgi"

#define	BOTKEY_NAME			"NAME"
#define	BOTKEY_HOST			"HOST"
#define	BOTKEY_FACE			"face#"
