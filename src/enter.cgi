#! /usr/bin/perl
#
#	�䂢�������Pro 1.0
#	Japanese	version--sjis�\����p
#	Copyright(c)	1997	yui@cup.com
#
#	Maintained by Yui Suzuki <yui@cup.com>
#	http://www.cup.com/yui/
#
#enter.cgi�́A�������̏����Ȃǂ��s���܂��B
require './jcodeLE.pl';
require './pref.cgi';
$| = 1;

&init;
&getcookie;
&htmlenter if($ENV{'QUERY_STRING'} eq 'enter');

&decode;   &jikan;
$autoclear = $FORM{'autoclear'};
&setcookie;
&hide;	&html;   &readlog;
$brauza = $ENV{'HTTP_USER_AGENT'};
$brauza =~s/<//g;
$value = "<FONT COLOR=\"#0000ff\"><B>�Ǘ��l</B></FONT> &gt; <B><FONT COLOR=\"$color\" SIZE=+2>$name</FONT></B><FONT COLOR=\"red\"><B>����A�����ł₷���B</B> $brauza</FONT><FONT COLOR=\"#888888\" SIZE=-1>($date $host)$histry</FONT><HR>\n";

&writelog;
&sanka;   &ended();   exit;

sub getcookie{	#�������[�𒸂��܂��B
	$cooks = $ENV{'HTTP_COOKIE'};
	$cooks = '' unless($cooks =~s/.*yuidata=(.*)yuiend.*/$1/);
	($name,$reload,$email,$mode,$color,$window,$lastdate,$count,$autoclear) = split(/\t/, $cooks);
	$histry = "$count���:LAST LOGIN:$lastdate" if($lastdate);
	if(!$count){	#�������[���̖����ꍇ�̃f�t�H���g�l
	$reload = 30;   $mode = 'checked';$color = 'red';   $window = 30;
	$count = 1;$autoclear = 'checked'; }
}#getcookie END

sub setcookie{	#�������[���u���E�U�Ƀv���[���g���܂��B
	local($dmy,$mdc,$monc,$yrc,$wdayc,$mc,$yc);
	($dmy,$dmy,$dmy,$mdc,$monc,$yrc,$wdayc,$dmy,$dmy) = localtime($times + 7776000);
	$yc = ('Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday') [$wdayc];
	$mc = ('Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec') [$monc];
	$yrc = $yrc+1900;	$mdc = "0$mdc" if ($mdc < 10);
	$count++;
	$data = "$name\t$reload\t$email\t$mode\t$color\t$window\t$date\t$count\t$autoclear\tyuiend";
	$data = 'yuiend' if($FORM{'cook'} ne 'ok');
	print "Set-Cookie: yuidata=$data; expires=$yc, $mdc-$mc-$yrc 00:00:00 GMT\n";
}#���̃N�b�L�[�͂�������3�����ԗL���ł��B
sub html{
print "Content-type: text/html\n\n";
print <<"_HTML_";
<HTML><HEAD><TITLE>$title</TITLE>$metacode
<SCRIPT LANGUAGE="JavaScript">
<!--
function autoclear() {
  if (self.document.send) {
    if (self.document.cmode && self.document.cmode.autoclear) {
      if (self.document.cmode.autoclear.checked) {
        if (self.document.send.emoji) {
          self.document.send.emoji.options[0].selected = true;
        }
        if (self.document.send.chat) {
          self.document.send.chat.value = "";
          self.document.send.chat.focus();
        }
      }
    }
  }
}
// -->
</SCRIPT></HEAD>
$body
_HTML_
if($mode eq 'checked'){
	print <<"_HTML_";
<BASE TARGET="down">
<TABLE border=0 cellpadding=0 cellspacing=0>
<FORM METHOD="$method" ACTION="chat.cgi" TARGET="down">
<TR><TD colspan=2 >[<A HREF="http://www.big.or.jp/~vampire/support/list.cgi"TARGET="_blank">�T�|�[�g�f����</A>][<A HREF="./tool.cgi?rank"TARGET="down">���������L���O</A>]</TD></TR>
$hidden
<INPUT TYPE=hidden NAME="window" VALUE="$window">
<INPUT TYPE=hidden NAME="mode" VALUE="checked">
<TR><TH><INPUT TYPE=submit VALUE="�X�V"></TH></FORM>
<FORM METHOD="$method" ACTION="chat.cgi" TARGET="down" NAME="send" ONSUBMIT="setTimeout(&quot;autoclear()&quot;,10)">
<TD nowrap align="left"><INPUT TYPE=submit VALUE="$title de����"><INPUT TYPE=reset VALUE="����">
</B>���Ȃ܂�:<B>$name</B>$reload�b�X�V</TD></TR>
<INPUT TYPE=hidden NAME="mode" VALUE="checked">
$hidden
<TR><TD nowrap colspan=2>����:<INPUT TYPE=text  SIZE="70" NAME="chat"></TD></TR><TR><TD colspan=2>
$kao
</TD></TR>
<TR><TD colspan=2 align="left">
<TABLE border=0 cellpadding=0 cellspacing=0><TR><TH nowrap colspan=2 align="right" >
���O�s��:<br>$logw</TH></FORM><th>
<FORM METHOD="$method" ACTION="chat.cgi" TARGET="_top">
$hidden
<INPUT TYPE=hidden NAME="chat" VALUE="�ގ�">
<TH><INPUT TYPE=submit VALUE="�ގ�����"></TH></FORM>
</TR></TABLE></TD></TR></TABLE>
<UL><LI><TABLE border=0 cellpadding=0 cellspacing=0><TR><FORM NAME="cmode"><TD><font color=blue>��������(Mac��)</font>:<INPUT TYPE=CHECKBOX NAME="autoclear" $autoclear>���������ɂ�JavaScript�𗘗p���Ă��܂��B</TD></FORM></TR></TABLE>
<LI>�㉺�𕪂��Ă��鋫�E���͎��R�Ɉړ��ł��܂���B
<LI>���p��<FONT COLOR=blue><B>cut</B></FONT>�Ɣ�������Ɖ摜���͂����܂��B
<LI>���p��<FONT COLOR=blue><B>clear</B></FONT>�Ɣ�������Ƃ��|���ł��܂��B
<LI>�e��ݒ��ύX�������ꍇ��<A HREF="./enter.cgi?enter" TARGET="top_w">������ցB</A>
<LI><FORM METHOD="$method" ACTION="chat.cgi" TARGET="_parent">
<INPUT TYPE=hidden NAME="window" VALUE="$window">
$hidden
<INPUT TYPE=submit VALUE="�t���[������"></FORM>
</UL><DIV align=right><FONT SIZE=2><A HREF="./tool.cgi?rom" TARGET="down">��</A></FONT></DIV></BODY></HTML>
_HTML_
 }else{

print <<"_HTML_";
<FORM METHOD="$method" ACTION="chat.cgi" TARGET="_parent">
<FONT SIZE=+2><B>$title</B></FONT><BR>
���Ȃ܂�:$name�@<INPUT TYPE=submit VALUE="����">$hidden
<INPUT TYPE=hidden NAME="window" VALUE="$window">
</FORM></BODY></HTML>
_HTML_
}
}#html END

sub htmlenter{#�����O�̉��
print "Content-type: text/html\n\n";
print <<"_HTML_";
<HTML><HEAD><TITLE>$title</TITLE>$metacode</HEAD>$body
���߂Ă̕��́A���O�����L�����ĎQ���{�^���������Ă��������ˁB
<TABLE  border=2 cellpadding=0 cellspacing=0><TR><TD>
<FORM METHOD="$method" ACTION="enter.cgi">
[<A HREF="http://www.cup.com/yui/"TARGET="_parent\">�䂢�̃z�[���y�[�W</A>][<A HREF="./wwwboard.html"TARGET="_parent">�T�|�[�g�f����</A>]$histry</TD></TR>
<TR><TD><FONT SIZE=+2 COLOR="hotpink"><B>$title</B></FONT>
<INPUT TYPE=submit VALUE="�`���b�g�ɎQ������"><INPUT TYPE=reset VALUE="���Z�b�g"></TD></TR>
<TR><TD bgcolor="pink">���Ȃ܂�:<INPUT TYPE=text NAME="name" SIZE="20"VALUE="$name" maxlength=24>�L�����ĂˁI</TD></TR>
<TR><TD>���[���A�h���X:<INPUT TYPE=text NAME="email" SIZE="30"VALUE="$email"></TD></TR>
<TR><TD>���O�̐F:<INPUT TYPE=text NAME="color" SIZE="10" VALUE="$color">
<A HREF="http://www.cup.com/yui/color.html"TARGET="down">(�F���{)</A></TD></TR>
<TR><TD>��ʃ��[�h(�t���[��):<INPUT TYPE=checkbox NAME="mode" VALUE="checked" $mode>���p����B</TD></TR>
<TR><TD>���O�s��:<SELECT NAME="window" SIZE="1">
<OPTION SELECTED>$window
<OPTION>10<OPTION>20<OPTION>30
<OPTION>40<OPTION>$max</SELECT></TD></TR>
<TR><TD>�����[�h����:<SELECT NAME="reload" SIZE="1">
<OPTION SELECTED>$reload
<OPTION>30<OPTION>45<OPTION>60
<OPTION>120<OPTION>200<OPTION >0
</SELECT>0�b�Ȃ�A�蓮�ł��B</TD></TR>
<TR><TD><INPUT TYPE=checkbox NAME="autoclear" VALUE="checked" $autoclear>��������������(JavaScript�g�p)</TD></TR>
<TR><TD><INPUT TYPE=checkbox NAME="cook" VALUE="ok" checked>���̓f�[�^���u���E�U�ɋL��������</TD></TR></TABLE>
�i���̋@�\\��Netscape1.1�ȍ~��InternetExplore3.0�ȍ~�Ȃǂ̂������̃u���E�U�ɗL���ł��B�j
</FORM>
<UL><LI>�㉺�𕪂��Ă��鋫�E���͎��R�Ɉړ��ł��܂���B</UL>
<DIV align=right><FONT SIZE=2><A HREF="./tool.cgi?rom" TARGET="down">��</A></FONT></DIV></BODY></HTML>
_HTML_
&ended();
}#htmlenter END
__END__
