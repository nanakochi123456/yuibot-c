#! /usr/bin/perl
#
#�䂢�������Pro1.0(tool.cgi)
#
#�I�v�V����CGI(�����`�F�b�N�����������L���O�j
require './pref.cgi';
&init;
&jikan;
print "Content-type: text/html\n\n";
if($ENV{'QUERY_STRING'} eq 'rom'){
	&romcheck;
}elsif($ENV{'QUERY_STRING'} eq 'rank'){
	&rank;
}
print "����`�H�H\n";
&ended;

sub romcheck{
print "<HTML><HEAD><TITLE>�A�N�Z�X���</TITLE>\n";
print "<META HTTP-EQUIV=\"Refresh\" CONTENT=\"60;URL=./tool.cgi?rom\"></HEAD>\n";
print "$body\n";

print "$title�̎Q���҂���сA�����̈ꗗ�ł��B(60�b�����[�h�j<HR>";
open(DBS,"$sanka_file") || &ended;
	@lines = <DBS>;
close(DBS);
foreach $line (@lines) {
	($timerec, $cname, $host,$dmy) = split(/\t/, $line);
	if( $times-60 < $timerec){
		$time2=$times-$timerec;
		print "<a href=\"mailto:$host\">$cname</a>($time2 �b�O�j<BR>";
	}
}
print "<HR>\n";
print "�Ƃ肠�����A�b��ɓ���Ȃ����S�҂ƁA�Z������A����̃����͗e�F���Ă܂�...�䂢��..�B<BR>\n";

print "���O�̂Ƃ���ɂ́Amailto�Ńz�X�g���������Ă܂��B<BR>\n";
print "�������N���b�N���Ă��Ӗ�����܂���B<BR>\n";
print "</BODY></HTML>\n";
&ended;
}#romcheck END

sub rank{
$mini=3;#�L�^�Ƃ��锭���񐔂̉���

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
	$youbi = ('��','��','��','��','��','��','�y') [$wday];
	$date2 = "$month/$mday($youbi)$hour:$min";
	$dates{$name2}=$date2;
	$hosts{$name2}=$host2;
}
     
open(DB,">$rank_file") || &ended;
print DB @lines;
close(DB);

print "<HTML><HEAD><TITLE>������񂫂�</TITLE></HEAD>\n";
print "$body";
print "$title�ł̔�����񂫂񂮁`�ł��B\n";
print "<BR><HR>\n";
print "<TABLE>\n";
print "<TR><TH nowrap>���Ȃ܂�</TH><TH nowrap>������</TH><TH nowrap>�ŏI��������</TH><TH nowrap>�z�X�g���</TH></TR>\n";

foreach (sort { $counts{$b} <=> $counts{$a} } keys %counts) {
	last if ($counts{$_} < $mini);
	print "<TR><TD>$_</TD><TD>$counts{$_}</TD><TD>$dates{$_}</TD><TD>$hosts{$_}</TD></TR>\n";
}

print "</TABLE><HR>\n";
print "</BODY></HTML>\n";
&ended;
}#rank END
__END__
