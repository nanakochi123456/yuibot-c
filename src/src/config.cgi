[CHAT]

# �`���b�g���O�t�@�C�� �i�K�{�j
CHAT_LOG=./chatlog.dat

# �Q�d���O�擾�t�@�C�� ($YYYYMMDD ������ƁA���t���ƂɃ��e�[�g�ł��܂��j
CHAT_LOG2=./abzlog/chat1/$YYYYMMDD.dat

# �Q���҃t�@�C���i��ɂ���ƍ쐬���܂���j
CHAT_S=./sanka.dat

# �����L���O�t�@�C���i��ɂ���ƍ쐬���܂���j
CHAT_R=./rank.dat1

# CGI��URL
CGI_URL=./chat.cgi

# �I���y�[�W
END_PAGE=http://chat.daiba.cx/keiji/chat1.htm

# �`���b�g�̖��O
CHAT_TITLE=�������Ȑl�̏W���`���b�g

# CGI��URL (REFERER�`�F�b�N�p
#REFERER=http://chat.daiba.cx

# �A�N�Z�X�����t�@�C��
KILLFILE=./kill.dat

# BODY�^�O
BODY=<BODY BGCOLOR="#FFFFE0" TEXT="#000000" LINK="#ff00ff" VLINK="#0000ff" ALINK="#0000ff">\n

# ���O�̐F
DEFAULTCOLOR=#ff00ff

# �E�B���h�E�s��
DEFAULTWINDOW=20

# �����[�h����
DEFAULTRELOAD=30

# ���O�\��
#MESSAGE=<font color=$COLOR><b>$NAME </b></font>&gt; <b>$CHAT $EMOJI</b><font size=2 color=#888888>($DTIME $HOST)</font><hr>\n

MESSAGE=<table border="0"><tr><td><FONT COLOR="$COLOR"><B>$NAME > </b></font><b>$CHAT </b></font> <FONT COLOR="darkgreen" SIZE=2>($DTIME <FONT SIZE=1>$HOST</FONT>)</FONT></table><HR>\n

# ���O�\���i���[���j
#MESSAGE_MAIL=<font color=$COLOR><b>$NAME </b></font><a href=\"mailto:$EMAIL\">|&gt; </a><b>$CHAT $EMOJI</b><font size=2 color=#888888>($DTIME $HOST)</font><hr>\n
MESSAGE_MAIL=<table border="0"><tr><td><b><A HREF="mailto:$EMAIL"><font color="$COLOR">$NAME > </font></a></b><b>$CHAT </b>�@<FONT COLOR="darkgreen" SIZE=2>($DTIME <FONT SIZE=1>$HOST</FONT>)</FONT></table><HR>

# �ގ����O
#MESSAGE_EXIT=a

# �Q�d���O�o�͗p
DOUBLELOG=$NAME\t$COLOR\t$EMAIL\t$CHAT\t$EMOJI\t$DTIME\t$HOST\n

# �Q���ҕ\���̂�
SANKA_ONLY=<font size=2><font color=blue>��</font><a target="_blank" href="$END_PAGE"><font color=#ff00ff>$TITLE</font></a></font><font size=2>�Q����($MEMBERS�l)�F$SANKA_MEMBER</font>

# �Q���ҕ\���i�S�́j
SANKA_ALL=<font size=2>�Q����($MEMBERS�l)�F$SANKA_MEMBER���w�F$ROMMEMBERS�l</font><hr>

# �Q���ҕ\���i�Q���҂���=$SANKA_MEMBER�j
SANKA_MEMBER=<font color=$MEMBER_COLOR>$MEMBER_NAME</font>��

# clear�����Ƃ��ɏo�͂��镶��
CLEAR_STR=��`,(^o^) /~~~ ,�`��

# clear�̃R�}���h
CLEAR_CMD=clear,kuria

# cut�̃R�}���h
CUT_CMD=cut

# �Ǘ��҃R�}���h
ADMIN_CMD=admin

# �����֎~���b�Z�[�W�i�Ȃ��̏ꍇ�AFile not found)
WARNING=<HTML><HEAD><TITLE>�����ł��܂���</TITLE></HEAD><BODY BGCOLOR="#FFFFE0" TEXT="#000000" LINK="#ff00ff" VLINK="#0000ff" ALINK="#0000ff"><FONT SIZE=4>�����ł��܂���<BR>�h�o�A�h���X $REMOTE_ADDR</BODY></HTML>

# �s���^�O���g�p�����Ƃ��̌x�����b�Z�[�W
TAGWARNING=<HTML><HEAD><TITLE>�����ł��܂���</TITLE></HEAD><BODY BGCOLOR="#FFFFE0" TEXT="#000000" LINK="#ff00ff" VLINK="#0000ff" ALINK="#0000ff"><FONT SIZE=4>�����ł��܂���<BR>�h�o�A�h���X $REMOTE_ADDR</BODY></HTML>

# ���������N������
AUTOLINK=<a target="_blank" href="$URL">$URL</a>(<a target="_blank" href="$URL">*</a>)

# ���ۂ̐ݒ�
[BOT-1]

# bot�̖��O�̐F
BOTCOLOR=#ff00ff

# bot�̖��O
BOTNAME1=����

# �����ЂƂ�bot�̖��O�i�s�v�Ȃ��j
BOTNAME2=����

# bot�̃z�X�g��
BOTHOST=

# ���烁�b�Z�[�W�i��摜����j
#MSG_KIOKU=

# �Z������G���[
#MSG_SHORT=

# �L�[���[�h�s��
#MSG_ERROR=

# �����݃��b�Z�[�W
MSG_BOKE=$BOTMESSAGE

# �������ă��[�h
MSG_OSHIETE=

# ��{����
BOT_FILE=./miho.dat

# �����o�^�Ȃ��̏ꍇ
AGO_FILE=./miho.ago.dat

# �����ݎ���
BOKE_FILE=./miho.boke.dat

# �ЂƂ育�Ǝ���
MONO_FILE=./miho.ago.dat

# ���N�����Ȃ������Ƃ��̉�b��(1-100%�j10�l�܂Ŏw���
MEMBERS_TALK=100,80,70,60,50,40,30,25,20,10

# �����̐ݒ�
[BOT-2]

# bot�̖��O�̐F
BOTCOLOR=#ff00ff

# bot�̖��O
BOTNAME1=����

# �����ЂƂ�bot�̖��O�i�s�v�Ȃ��j
BOTNAME2=����

# bot�̃z�X�g��
BOTHOST=

# ���烁�b�Z�[�W�i��摜����j
#MSG_KIOKU=

# �Z������G���[
#MSG_SHORT=

# �L�[���[�h�s��
#MSG_ERROR=

# �����݃��b�Z�[�W
MSG_BOKE=$BOTMESSAGE

# �������ă��[�h
MSG_OSHIETE=

# ��{����
BOT_FILE=./meitei.dic.dat

# �����o�^�Ȃ��̏ꍇ
AGO_FILE=./meitei.ago.dat

# �����ݎ���
BOKE_FILE=./meitei.boke.dat

# �ЂƂ育�Ǝ���
MONO_FILE=./meitei.ago.dat

# ���N�����Ȃ������Ƃ��̉�b��(1-100%�j10�l�܂Ŏw���
MEMBERS_TALK=100,80,70,60,50,40,30,25,20,10

[HTML]

# �ŏ���HTML
HEADER=\
<HTML><HEAD><TITLE>$TITLE</TITLE>\n\
<META HTTP-EQUIV="Content-type" CONTENT="text/html; charset=Shift_JIS">\n

# ���������[�h��HTML
AUTORELOAD=\
<META HTTP-EQUIV="Refresh" CONTENT="$RELOAD;URL=$CGI_URL?window=$WINDOW&reload=$RELOAD&name=$ENC_NAME&mode=$MODE&log=$ROOM">\n

# �蓮�����[�h��HTML
RELOAD=<br>

# �t�b�^�i�����Ȃ��łˁ�j
FOOTER=Copyright <A HREF=http://www.cup.com/yui/ TARGET=top>�䂢�`���b�g(C�����)</A>�@�|�@<A HREF="http://lineage.netgamers.jp/">Modefied By ������� (�ȂȂ݂��Ė�����Ă܂�)</A><BR>Special thanks to  <A HREF=http://www.ksc.co.jp/ TARGET=_top>System Consultant Co.,Ltd.</A>

# ���ʃt�b�^
ALLFOOTER=</BODY></HTML>

# �t���[���Ȃ��̏o��HTML�i���F�d���̂Ńt���[���Ȃ��̂Ƃ������ǂݍ��݂܂���j
NOFRAMEHTML=\
<FORM METHOD="$METHOD" ACTION="$CGI_URL">\n\
<FONT SIZE=+2 COLOR="hotpink"><B>$TITLE</B></FONT>���Ȃ܂�:<B>$NAME</B>\n\
<INPUT TYPE=hidden NAME="email" VALUE="$EMAIL">\n\
<INPUT TYPE=hidden NAME="reload" VALUE="0">\n\
<INPUT TYPE=hidden NAME="color" VALUE="$COLOR">\n\
<INPUT TYPE=hidden NAME="name" VALUE="$NAME">\n\
���O�s��:<SELECT NAME="window" SIZE="1">\n\
<BR>����:<INPUT TYPE=text NAME="chat" SIZE="80">\n\
<TABLE border=0><TR>\n\
<TH><INPUT TYPE=submit VALUE="����/�����[�h"></TH>\n\
<TH><INPUT TYPE=reset VALUE="���Z�b�g"></TH></FORM>\n\
<FORM METHOD="GET" ACTION="chat.cgi">\n\
<INPUT TYPE=hidden NAME="email" VALUE="$EMAIL">\n\
<INPUT TYPE=hidden NAME="reload" VALUE="0">\n\
<INPUT TYPE=hidden NAME="color" VALUE="$COLOR">\n\
<INPUT TYPE=hidden NAME="name" VALUE="$NAME">\n\
<INPUT TYPE=hidden NAME="chat" VALUE="�ގ�">\n\
<TH><INPUT TYPE=submit VALUE="�ގ�����"></TH></FORM>\n\
<TH>(<A HREF="./index.html">�ύX</A>)</TH></TR></TABLE>\n

