#! /usr/bin/perl
#
#	ゆいちゃっとPro 1.0
#	Japanese	version--sjis表示専用
#	Copyright(c)	1997	yui@cup.com
#
#	Maintained by Yui Suzuki <yui@cup.com>
#	http://www.cup.com/yui/
#
#enter.cgiは、入室時の処理などを行います。
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
$value = "<FONT COLOR=\"#0000ff\"><B>管理人</B></FONT> &gt; <B><FONT COLOR=\"$color\" SIZE=+2>$name</FONT></B><FONT COLOR=\"red\"><B>さん、おいでやすぅ。</B> $brauza</FONT><FONT COLOR=\"#888888\" SIZE=-1>($date $host)$histry</FONT><HR>\n";

&writelog;
&sanka;   &ended();   exit;

sub getcookie{	#くっきーを頂きます。
	$cooks = $ENV{'HTTP_COOKIE'};
	$cooks = '' unless($cooks =~s/.*yuidata=(.*)yuiend.*/$1/);
	($name,$reload,$email,$mode,$color,$window,$lastdate,$count,$autoclear) = split(/\t/, $cooks);
	$histry = "$count回目:LAST LOGIN:$lastdate" if($lastdate);
	if(!$count){	#くっきー情報の無い場合のデフォルト値
	$reload = 30;   $mode = 'checked';$color = 'red';   $window = 30;
	$count = 1;$autoclear = 'checked'; }
}#getcookie END

sub setcookie{	#くっきーをブラウザにプレゼントします。
	local($dmy,$mdc,$monc,$yrc,$wdayc,$mc,$yc);
	($dmy,$dmy,$dmy,$mdc,$monc,$yrc,$wdayc,$dmy,$dmy) = localtime($times + 7776000);
	$yc = ('Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday') [$wdayc];
	$mc = ('Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec') [$monc];
	$yrc = $yrc+1900;	$mdc = "0$mdc" if ($mdc < 10);
	$count++;
	$data = "$name\t$reload\t$email\t$mode\t$color\t$window\t$date\t$count\t$autoclear\tyuiend";
	$data = 'yuiend' if($FORM{'cook'} ne 'ok');
	print "Set-Cookie: yuidata=$data; expires=$yc, $mdc-$mc-$yrc 00:00:00 GMT\n";
}#このクッキーはだいたい3ヶ月間有効です。
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
<TR><TD colspan=2 >[<A HREF="http://www.big.or.jp/~vampire/support/list.cgi"TARGET="_blank">サポート掲示板</A>][<A HREF="./tool.cgi?rank"TARGET="down">発言ランキング</A>]</TD></TR>
$hidden
<INPUT TYPE=hidden NAME="window" VALUE="$window">
<INPUT TYPE=hidden NAME="mode" VALUE="checked">
<TR><TH><INPUT TYPE=submit VALUE="更新"></TH></FORM>
<FORM METHOD="$method" ACTION="chat.cgi" TARGET="down" NAME="send" ONSUBMIT="setTimeout(&quot;autoclear()&quot;,10)">
<TD nowrap align="left"><INPUT TYPE=submit VALUE="$title de発言"><INPUT TYPE=reset VALUE="消す">
</B>おなまえ:<B>$name</B>$reload秒更新</TD></TR>
<INPUT TYPE=hidden NAME="mode" VALUE="checked">
$hidden
<TR><TD nowrap colspan=2>発言:<INPUT TYPE=text  SIZE="70" NAME="chat"></TD></TR><TR><TD colspan=2>
$kao
</TD></TR>
<TR><TD colspan=2 align="left">
<TABLE border=0 cellpadding=0 cellspacing=0><TR><TH nowrap colspan=2 align="right" >
ログ行数:<br>$logw</TH></FORM><th>
<FORM METHOD="$method" ACTION="chat.cgi" TARGET="_top">
$hidden
<INPUT TYPE=hidden NAME="chat" VALUE="退室">
<TH><INPUT TYPE=submit VALUE="退室する"></TH></FORM>
</TR></TABLE></TD></TR></TABLE>
<UL><LI><TABLE border=0 cellpadding=0 cellspacing=0><TR><FORM NAME="cmode"><TD><font color=blue>自動消去(Mac可)</font>:<INPUT TYPE=CHECKBOX NAME="autoclear" $autoclear>自動消去にはJavaScriptを利用しています。</TD></FORM></TR></TABLE>
<LI>上下を分けている境界線は自由に移動できますよ。
<LI>半角で<FONT COLOR=blue><B>cut</B></FONT>と発言すると画像がはずせます。
<LI>半角で<FONT COLOR=blue><B>clear</B></FONT>と発言するとお掃除できます。
<LI>各種設定を変更したい場合は<A HREF="./enter.cgi?enter" TARGET="top_w">こちらへ。</A>
<LI><FORM METHOD="$method" ACTION="chat.cgi" TARGET="_parent">
<INPUT TYPE=hidden NAME="window" VALUE="$window">
$hidden
<INPUT TYPE=submit VALUE="フレーム解除"></FORM>
</UL><DIV align=right><FONT SIZE=2><A HREF="./tool.cgi?rom" TARGET="down">Π</A></FONT></DIV></BODY></HTML>
_HTML_
 }else{

print <<"_HTML_";
<FORM METHOD="$method" ACTION="chat.cgi" TARGET="_parent">
<FONT SIZE=+2><B>$title</B></FONT><BR>
おなまえ:$name　<INPUT TYPE=submit VALUE="入室">$hidden
<INPUT TYPE=hidden NAME="window" VALUE="$window">
</FORM></BODY></HTML>
_HTML_
}
}#html END

sub htmlenter{#入室前の画面
print "Content-type: text/html\n\n";
print <<"_HTML_";
<HTML><HEAD><TITLE>$title</TITLE>$metacode</HEAD>$body
初めての方は、名前だけ記入して参加ボタンを押してくださいね。
<TABLE  border=2 cellpadding=0 cellspacing=0><TR><TD>
<FORM METHOD="$method" ACTION="enter.cgi">
[<A HREF="http://www.cup.com/yui/"TARGET="_parent\">ゆいのホームページ</A>][<A HREF="./wwwboard.html"TARGET="_parent">サポート掲示板</A>]$histry</TD></TR>
<TR><TD><FONT SIZE=+2 COLOR="hotpink"><B>$title</B></FONT>
<INPUT TYPE=submit VALUE="チャットに参加する"><INPUT TYPE=reset VALUE="リセット"></TD></TR>
<TR><TD bgcolor="pink">おなまえ:<INPUT TYPE=text NAME="name" SIZE="20"VALUE="$name" maxlength=24>記入してね！</TD></TR>
<TR><TD>メールアドレス:<INPUT TYPE=text NAME="email" SIZE="30"VALUE="$email"></TD></TR>
<TR><TD>名前の色:<INPUT TYPE=text NAME="color" SIZE="10" VALUE="$color">
<A HREF="http://www.cup.com/yui/color.html"TARGET="down">(色見本)</A></TD></TR>
<TR><TD>画面モード(フレーム):<INPUT TYPE=checkbox NAME="mode" VALUE="checked" $mode>利用する。</TD></TR>
<TR><TD>ログ行数:<SELECT NAME="window" SIZE="1">
<OPTION SELECTED>$window
<OPTION>10<OPTION>20<OPTION>30
<OPTION>40<OPTION>$max</SELECT></TD></TR>
<TR><TD>リロード時間:<SELECT NAME="reload" SIZE="1">
<OPTION SELECTED>$reload
<OPTION>30<OPTION>45<OPTION>60
<OPTION>120<OPTION>200<OPTION >0
</SELECT>0秒なら、手動です。</TD></TR>
<TR><TD><INPUT TYPE=checkbox NAME="autoclear" VALUE="checked" $autoclear>発言時自動消去(JavaScript使用)</TD></TR>
<TR><TD><INPUT TYPE=checkbox NAME="cook" VALUE="ok" checked>入力データをブラウザに記憶させる</TD></TR></TABLE>
（この機能\はNetscape1.1以降やInternetExplore3.0以降などのいくつかのブラウザに有効です。）
</FORM>
<UL><LI>上下を分けている境界線は自由に移動できますよ。</UL>
<DIV align=right><FONT SIZE=2><A HREF="./tool.cgi?rom" TARGET="down">Π</A></FONT></DIV></BODY></HTML>
_HTML_
&ended();
}#htmlenter END
__END__
