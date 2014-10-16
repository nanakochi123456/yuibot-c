/*************************************************************
 *	ゆいちゃっとＣ言語版 1.0y1
 *	Japanese	version--sjis表示専用
 *	Copyright(c)	1998	yui@cup.com
 *  Modified        2000    yuichat@daiba.cx
 *	Maintained by Yui Suzuki <yui@cup.com>
 *	http://www.cup.com/yui/
 *  http://www.yuibot.com/
 *************************************************************/

/*
 *
 * 基本設定
 *
 */

/* 全タグを使えなくするには、1、使えるようにするには 0 */
#define NO_TAG			1

/* 名前にタグを使用できるようにするには 0 */
#define	NO_NAMETAG		1

/* 自動リンクを使用するには、1、使用しないようにするには 0 */
#define	TAG_AUTOLINK	1

/* 時差の設定（秒単位） */
#define JISA			0

/* POSTメソッドのみの書き込み許可は、1 */
#define	POST_ONLY		0

/* ファイルロック
　 0 ファイルロックを使用しない
　 1 Linux, FreeBSD用
　 2 その他
*/
#define	FILE_LOCK		1

/* ホスト名の取得方法
　 0 IPのみ
　 1 ホスト名
　 2 ホスト名出力なし(IP)
　 3 IP(proxyのアクセス元があればそのIP)
　 4 ホスト名(proxyのアクセス元があればそのIP)
　 5 ホスト名出力なし(IP+proxy)
*/

#define	HOST_LOOKUP		0

/* ２重でログをとる場合、1 */
#define	DOUBLELOG		1

/* アクセス制限を使用する場合、1 */
#define	KILL			1

/* REFERERチェック
　 0 使用しない
　 1 使用する(NULLは通す) - SegaSaturnブラウザ対策 あまりおすすめしません
　 2 使用する(NULLは通さない)
*/
#define	REFERERCHECK	0

/* kill.dat にワイルドカード .* が使えるようにする場合、1 */
#define	KILL_REUSE		1

/*
 *
 * 参加者、ランキングの設定
 *
 */

/* 参加者表示を有効にするには１ */
#define	ALLOW_SANKA		1

/* chat.cgi で参加者表示をしない（記録だけとる） の場合１ */
#define	NOTPRINT_SANKA	1

/* mode=sanka で参加者表示をする場合 1 */
#define	MODE_SANKA		1

/* ランキングを有効にするには１ */
#define	ALLOW_RANK		1

/* 予想される最大参加者数（ランキング考慮） */
#define	MAX_MEMBER		3000

/* ランキング保存期間 (60日)*/
#define	RANK_TIME		60*24*60*60


/*
 *
 * ゆいちゃっとの各種モジュールの設定
 *
 */

/* 顔画像アイコンを使用できるようにするには 1 */
#define	YUI_CUTE		0

/* 顔画像アイコン使用時のセキュリティー設定 */
/* 通常はこのままでOK、他サーバーからの参照も可能にするには 0 */
#define	YUI_CUTE_SEC	1

/* 顔画像につける拡張子 */
#define YUI_CUTE_EXT	""
/* #define	YUI_CUTE_EXT	".gif" */

/*
 *
 * ゅぃぼっとの設定
 *
 */

/* ゅぃぼっとを使用するには 1 */
#define	YUIBOT			1

/* おみくじのみ使用する場合 1 */
#define	OMIKUJI_ONLY	0

/* botが自動で反応するデフォルト人数(各botで設定可) */
#define	BOT_TALK		10

/* ひとりごとをする場合 1 */
#define	MONO_TALK		1

/* ひとりごとの反応デフォルト時間（秒数） */
#define	MONO_TALK_TIME	18000000

/* botの最大人数（注：おみくじも1人に数えること） */
#define	BOT_MEMBERS		10

/* キーワード検索において、カナ、全角アルファベットの変換 */
#define	YUIBOT_KEYCONV	1

/* 学習させる場合、1 */
#define	BOT_KIOKU		0

/* 学習の文字列 */
#define	LEARNSTR		"==="

/* 学習できる最低文字数 */
#define	KEYLENGTH		4

/* 誰宛？ */
#define	TOYUI			"＞"

/* つっこみの割合 (最大100） */
#define	BOKE_RATIO		5

/* おしえてモードの割合（最大10） */
#define	OSHIETE_RATIO	5

/* ゅぃぼっと辞書の区切り文字 */
#define	YUIBOT_DIC	":#"

/* ALL宛の文字 */
#define	ALL1			"ALL"
#define	ALL2			"all"
#define	ALL3			"All"
#define	ALL4			"ＡＬＬ"
#define	ALL5			"ａｌｌ"
#define	ALL6			"Ａｌｌ"
#define	ALL7			"おおる"
#define	ALL8			"オール"
#define	ALL9			"おうる"
#define	ALL10			"みんな"

/* CGIのパラメータ名称 */
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
