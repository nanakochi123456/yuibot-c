#! /usr/bin/perl
#
#ゆいちゃっとPro1.0(tool.cgi)
#
#オプションCGI(ロムチェック＆発言ランキング）
require './pref.cgi';
&init;
&jikan;
print "Content-type: text/html\n\n";
if($ENV{'QUERY_STRING'} eq 'rom'){
	&romcheck;
}elsif($ENV{'QUERY_STRING'} eq 'rank'){
	&rank;
}
print "えら～？？\n";
&ended;

sub romcheck{
print "<HTML><HEAD><TITLE>アクセス情報</TITLE>\n";
print "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"60;URL=./tool.cgi?rom\"></HEAD>\n";
print "$body\n";

print "$titleの参加者および、ロムの一覧です。(60秒リロード）<HR>";
open(DBS,"$sanka_file") || &ended;
	@lines = <DBS>;
close(DBS);
foreach $line (@lines) {
	($timerec, $cname, $host,$dmy) = split(/\t/, $line);
	if( $times-60 < $timerec){
		$time2=$times-$timerec;
		print "<a href=\"mailto:$host\">$cname</a>($time2 秒前）<BR>";
	}
}
print "<HR>\n";
print "とりあえず、話題に入れない初心者と、忙しい常連さんのロムは容認してます...ゆいは..。<BR>\n";

print "名前のところには、mailtoでホスト名が入ってます。<BR>\n";
print "ここをクリックしても意味ありません。<BR>\n";
print "</BODY></HTML>\n";
&ended;
}#romcheck END

sub rank{
$mini=3;#記録とする発言回数の下限

open(DBS,"$rank_file") || &ended;
	@lines = <DBS>;
close(DBS);
foreach $line (@lines) {
	($name2, $count,$time2,$host2) =split(/\t/, $line);
	if( $times-10*24*60*60 > $time2){	$line='';	next;}

	$counts{$name2}=$count;
	($sec,$min,$hour,$mday,$month,$year,$wday,$yday,$isdst) = localtime($time2);
	$min = "0$min" if ($min < 10);
	$month++;
	$youbi = ('日','月','火','水','木','金','土') [$wday];
	$date2 = "$month/$mday($youbi)$hour:$min";
	$dates{$name2}=$date2;
	$hosts{$name2}=$host2;
}
     
open(DB,">$rank_file") || &ended;
print DB @lines;
close(DB);

print "<HTML><HEAD><TITLE>発言らんきんぐ</TITLE></HEAD>\n";
print "$body";
print "$titleでの発言らんきんぐ～です。\n";
print "<BR><HR>\n";
print "<TABLE>\n";
print "<TR><TH nowrap>おなまえ</TH><TH nowrap>発言回数</TH><TH nowrap>最終発言時刻</TH><TH nowrap>ホスト情報</TH></TR>\n";

foreach (sort { $counts{$b} <=> $counts{$a} } keys %counts) {
	last if ($counts{$_} < $mini);
	print "<TR><TD>$_</TD><TD>$counts{$_}</TD><TD>$dates{$_}</TD><TD>$hosts{$_}</TD></TR>\n";
}

print "</TABLE><HR>\n";
print "</BODY></HTML>\n";
&ended;
}#rank END
__END__
