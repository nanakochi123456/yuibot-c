[CHAT]

# チャットログファイル （必須）
CHAT_LOG=./chatlog.dat

# ２重ログ取得ファイル ($YYYYMMDD を入れると、日付ごとにロテートできます）
CHAT_LOG2=./abzlog/chat1/$YYYYMMDD.dat

# 参加者ファイル（空にすると作成しません）
CHAT_S=./sanka.dat

# ランキングファイル（空にすると作成しません）
CHAT_R=./rank.dat1

# CGIのURL
CGI_URL=./chat.cgi

# 終了ページ
END_PAGE=http://chat.daiba.cx/keiji/chat1.htm

# チャットの名前
CHAT_TITLE=おかしな人の集うチャット

# CGIのURL (REFERERチェック用
#REFERER=http://chat.daiba.cx

# アクセス制限ファイル
KILLFILE=./kill.dat

# BODYタグ
BODY=<BODY BGCOLOR="#FFFFE0" TEXT="#000000" LINK="#ff00ff" VLINK="#0000ff" ALINK="#0000ff">\n

# 名前の色
DEFAULTCOLOR=#ff00ff

# ウィンドウ行数
DEFAULTWINDOW=20

# リロード時間
DEFAULTRELOAD=30

# ログ表示
#MESSAGE=<font color=$COLOR><b>$NAME </b></font>&gt; <b>$CHAT $EMOJI</b><font size=2 color=#888888>($DTIME $HOST)</font><hr>\n

MESSAGE=<table border="0"><tr><td><FONT COLOR="$COLOR"><B>$NAME > </b></font><b>$CHAT </b></font> <FONT COLOR="darkgreen" SIZE=2>($DTIME <FONT SIZE=1>$HOST</FONT>)</FONT></table><HR>\n

# ログ表示（メール）
#MESSAGE_MAIL=<font color=$COLOR><b>$NAME </b></font><a href=\"mailto:$EMAIL\">|&gt; </a><b>$CHAT $EMOJI</b><font size=2 color=#888888>($DTIME $HOST)</font><hr>\n
MESSAGE_MAIL=<table border="0"><tr><td><b><A HREF="mailto:$EMAIL"><font color="$COLOR">$NAME > </font></a></b><b>$CHAT </b>　<FONT COLOR="darkgreen" SIZE=2>($DTIME <FONT SIZE=1>$HOST</FONT>)</FONT></table><HR>

# 退室ログ
#MESSAGE_EXIT=a

# ２重ログ出力用
DOUBLELOG=$NAME\t$COLOR\t$EMAIL\t$CHAT\t$EMOJI\t$DTIME\t$HOST\n

# 参加者表示のみ
SANKA_ONLY=<font size=2><font color=blue>◆</font><a target="_blank" href="$END_PAGE"><font color=#ff00ff>$TITLE</font></a></font><font size=2>参加者($MEMBERS人)：$SANKA_MEMBER</font>

# 参加者表示（全体）
SANKA_ALL=<font size=2>参加者($MEMBERS人)：$SANKA_MEMBER見学：$ROMMEMBERS人</font><hr>

# 参加者表示（参加者ごと=$SANKA_MEMBER）
SANKA_MEMBER=<font color=$MEMBER_COLOR>$MEMBER_NAME</font>☆

# clearしたときに出力する文字
CLEAR_STR=♪～,(^o^) /~~~ ,～♪

# clearのコマンド
CLEAR_CMD=clear,kuria

# cutのコマンド
CUT_CMD=cut

# 管理者コマンド
ADMIN_CMD=admin

# 入室禁止メッセージ（なしの場合、File not found)
WARNING=<HTML><HEAD><TITLE>入室できません</TITLE></HEAD><BODY BGCOLOR="#FFFFE0" TEXT="#000000" LINK="#ff00ff" VLINK="#0000ff" ALINK="#0000ff"><FONT SIZE=4>入室できません<BR>ＩＰアドレス $REMOTE_ADDR</BODY></HTML>

# 不正タグを使用したときの警告メッセージ
TAGWARNING=<HTML><HEAD><TITLE>入室できません</TITLE></HEAD><BODY BGCOLOR="#FFFFE0" TEXT="#000000" LINK="#ff00ff" VLINK="#0000ff" ALINK="#0000ff"><FONT SIZE=4>入室できません<BR>ＩＰアドレス $REMOTE_ADDR</BODY></HTML>

# 自動リンク文字列
AUTOLINK=<a target="_blank" href="$URL">$URL</a>(<a target="_blank" href="$URL">*</a>)

# 美保の設定
[BOT-1]

# botの名前の色
BOTCOLOR=#ff00ff

# botの名前
BOTNAME1=美保

# もうひとつのbotの名前（不要なら空）
BOTNAME2=美保

# botのホスト名
BOTHOST=

# 教育メッセージ（顔画像あり）
#MSG_KIOKU=

# 短すぎるエラー
#MSG_SHORT=

# キーワード不正
#MSG_ERROR=

# つっこみメッセージ
MSG_BOKE=$BOTMESSAGE

# おしえてモード
MSG_OSHIETE=

# 基本辞書
BOT_FILE=./miho.dat

# 辞書登録なしの場合
AGO_FILE=./miho.ago.dat

# つっこみ辞書
BOKE_FILE=./miho.boke.dat

# ひとりごと辞書
MONO_FILE=./miho.ago.dat

# ＞誰をつけなかったときの会話率(1-100%）10人まで指定可
MEMBERS_TALK=100,80,70,60,50,40,30,25,20,10

# 迷亭の設定
[BOT-2]

# botの名前の色
BOTCOLOR=#ff00ff

# botの名前
BOTNAME1=迷亭

# もうひとつのbotの名前（不要なら空）
BOTNAME2=迷亭

# botのホスト名
BOTHOST=

# 教育メッセージ（顔画像あり）
#MSG_KIOKU=

# 短すぎるエラー
#MSG_SHORT=

# キーワード不正
#MSG_ERROR=

# つっこみメッセージ
MSG_BOKE=$BOTMESSAGE

# おしえてモード
MSG_OSHIETE=

# 基本辞書
BOT_FILE=./meitei.dic.dat

# 辞書登録なしの場合
AGO_FILE=./meitei.ago.dat

# つっこみ辞書
BOKE_FILE=./meitei.boke.dat

# ひとりごと辞書
MONO_FILE=./meitei.ago.dat

# ＞誰をつけなかったときの会話率(1-100%）10人まで指定可
MEMBERS_TALK=100,80,70,60,50,40,30,25,20,10

[HTML]

# 最初のHTML
HEADER=\
<HTML><HEAD><TITLE>$TITLE</TITLE>\n\
<META HTTP-EQUIV="Content-type" CONTENT="text/html; charset=Shift_JIS">\n

# 自動リロードのHTML
AUTORELOAD=\
<META HTTP-EQUIV="Refresh" CONTENT="$RELOAD;URL=$CGI_URL?window=$WINDOW&reload=$RELOAD&name=$ENC_NAME&mode=$MODE&log=$ROOM">\n

# 手動リロードのHTML
RELOAD=<br>

# フッタ（かえないでね♪）
FOOTER=Copyright <A HREF=http://www.cup.com/yui/ TARGET=top>ゆいチャット(C言語版)</A>　－　<A HREF="http://lineage.netgamers.jp/">Modefied By ゆったん (ななみって名乗ってます)</A><BR>Special thanks to  <A HREF=http://www.ksc.co.jp/ TARGET=_top>System Consultant Co.,Ltd.</A>

# 共通フッタ
ALLFOOTER=</BODY></HTML>

# フレームなしの出力HTML（注：重いのでフレームなしのときしか読み込みません）
NOFRAMEHTML=\
<FORM METHOD="$METHOD" ACTION="$CGI_URL">\n\
<FONT SIZE=+2 COLOR="hotpink"><B>$TITLE</B></FONT>おなまえ:<B>$NAME</B>\n\
<INPUT TYPE=hidden NAME="email" VALUE="$EMAIL">\n\
<INPUT TYPE=hidden NAME="reload" VALUE="0">\n\
<INPUT TYPE=hidden NAME="color" VALUE="$COLOR">\n\
<INPUT TYPE=hidden NAME="name" VALUE="$NAME">\n\
ログ行数:<SELECT NAME="window" SIZE="1">\n\
<BR>発言:<INPUT TYPE=text NAME="chat" SIZE="80">\n\
<TABLE border=0><TR>\n\
<TH><INPUT TYPE=submit VALUE="発言/リロード"></TH>\n\
<TH><INPUT TYPE=reset VALUE="リセット"></TH></FORM>\n\
<FORM METHOD="GET" ACTION="chat.cgi">\n\
<INPUT TYPE=hidden NAME="email" VALUE="$EMAIL">\n\
<INPUT TYPE=hidden NAME="reload" VALUE="0">\n\
<INPUT TYPE=hidden NAME="color" VALUE="$COLOR">\n\
<INPUT TYPE=hidden NAME="name" VALUE="$NAME">\n\
<INPUT TYPE=hidden NAME="chat" VALUE="退室">\n\
<TH><INPUT TYPE=submit VALUE="退室する"></TH></FORM>\n\
<TH>(<A HREF="./index.html">変更</A>)</TH></TR></TABLE>\n

