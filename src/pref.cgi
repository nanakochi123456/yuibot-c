#! /usr/local/bin/perl
#
#�䂢�������Pro1.0(pref.cgi)
#
#pref.cgi�́Achat.cgi��enter.cgi�ŁA���ʂ��Ďg����T�u���[�`���W�ł��B

sub init{	#�����ݒ�Ȃ�
$rank_file='./rank.dat';#���������L���O�L�^
$sanka_file = './sanka.dat'; #�Q���L�^�t�@�C��
$chat_file = './chat.dat'; #�`���b�g���O(�t�@�C�����͂Ȃ�ׂ��ύX���Ă������ƁB)
$method = 'GET';#����ς�AGET��˂��`
#�ގ���̃y�[�W�́A���̂g�o�ɂ��ς����܂��B
$endpage= 'http://www.big.or.jp/~vampire/test/chat.cgi?window=50&reload=150&mode=checked';
$title = '�䂢�������Pro1.0';
$body='<BODY BGCOLOR="#A1FE9F" TEXT="#000000" LINK="#ff0000" VLINK="#ff0000" ALINK="#FF0000">';
$max = 50;#���O�̍ő�s��
$host = &gethost;
$metacode = '<META HTTP-EQUIV="Content-type" CONTENT="text/html; charset=x-sjis">';#SJIS�F��������
####�A�N�Z�X�������g�p����Ȃ�A#�������B
#$kill_file = './kill.dat';#�A�N�Z�X�����p
#open(DB,"$kill_file") || die "Cannot Open Log File $kill_file: $!";
#	@lines2 = <DB>;	close(DB);
#foreach $line (@lines2) {
#	next if(length($line)<4);	chop $line;
#	if( $host=~/$line/ ){	print "Status: 204\n\n";	exit;	}
#}
####�A�N�Z�X���������܂�
#���b�N�t�@�C�����g�p����Ȃ�A#�������Block �Ƃ����f�B���N�g����p�ӂ���i777�j
#$lockfile = './lock/lock-file.lock';
#$retry = 5;
#while (!symlink(".", $lockfile)) {
#if (--$retry <= 0) { print "Status: 204\n\n"; unlink($lockfile); exit; }
#sleep(1);
#}
#���b�N�t�@�C�������܂�
}#init END

sub ended{	#unlink�ƃG���[�g���b�v��exit���s�����߂����̂��́B
#	unlink($lockfile);#���b�N�t�@�C�����g�p����Ȃ�A#�������B
print "Content-type: text/plain\n\n $_[0] \n"if($_[0]);#���̍s�͍폜�ł��B
exit;
}#ended END

sub decode{	#��ʓI�ȃf�R�[�h���ϐ��ւ̑��
$buffer = $ENV{'QUERY_STRING'};
@pairs = split(/&/,$buffer);
	foreach $pair (@pairs) {
		($name, $value) = split(/=/, $pair);
		$value =~ tr/+/ /;
		$value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
		&jcode'convert(*value,'sjis');
		$value =~ s/\t/ /g;$value =~ s/</\t/g;$value =~ s/\n//g;
		$FORM{$name} = $value;
	}
$name = $FORM{'name'};	$name =~ s/ //g; $name =~ s/�@//g;
$name =~ s/\t/&lt;/g;	$name = $host if (!$name);
$color = $FORM{'color'};	$color='red' if(!$color);
$mode = $FORM{'mode'};	$reload = $FORM{'reload'};
$email = $FORM{'email'};	$window =$FORM{'window'};
}#decode END

sub jikan{#$date�Ɏ����������܂��B
	$times = time;#18���Ԏ���������Ȃ�A$times = time+18*60*60�Ƃ���B
	($sec,$min,$hour,$mday,$month,$year,$wday,$yday,$isdst) = localtime($times);
	$min = "0$min" if ($min < 10);	$month++;
	$youbi = ('��','��','��','��','��','��','�y') [$wday];
	$date = "$month/$mday($youbi)$hour:$min";
}#jikan END

sub sanka{#�����ŁA�A�N�Z�X�L�^���Ƃ�B
open(LOG,"$sanka_file") || &ended('$sanka_file open error');
	seek(LOG,0,0);	@sanka=<LOG>;
close(LOG);
$flag=1;
foreach $line (@sanka) {
	($timerec, $handle, $host2,$dmy) = split(/\t/, $line);
	if( $times-60 > $timerec){$line = '';	next;}
		if(($host2 eq $host) && $flag){#�z�X�g��������̏ꍇ�A���ԂƖ��O���ŐV�̂��̂ɂ���B
			$line = "$times\t$name\t$host\td\n";
			$flag =0;$handle = $name;	$handle =~ s/��/��/g;
		}
	push (@sanka3, "$handle��") if($handle ne $host2); 
	}#foreach

	if($flag){	#�V�����Q���҂́A������
		push(@sanka,"$times\t$name\t$host\td\n");
		$name =~ s/��/��/g;
		push (@sanka3, "$name��") if($name ne $host); 
	}

undef(@sanka) if($chat eq 'sanka');
open(LOG,">$sanka_file")  || &ended('$sanka_file write error');
	eval 'flock(LOG,2);';
	seek(LOG,0,0);	print LOG @sanka;
	eval 'flock(LOG,8);';
close(LOG);
}#sanka END

sub readlog{	#�z��@lines�Ƀ��O��ǂݍ���
	open(DB,"$chat_file") || &ended('$chat_file open error');
		seek(DB,0,0);  @lines = <DB>;   close(DB);
}#readlog END

sub writelog{	#$value��@lines�ɉ������̂��A��������
	(@lines < $max-1) || (@lines = @lines[0 .. $max - 2]);
	unshift( @lines,$value);
	open(DB,">$chat_file") || &ended('$chat_file write error');
		eval 'flock(DB,2);';
		seek(DB,0,0);	print DB @lines;
		eval 'flock(DB,8);';
	close(DB);
}#writelog END
sub hide{	#HTML�������ɋ��L����镔������ɂ܂Ƃ߂�B
$hidden =<<"_HIDE_";
<INPUT TYPE=hidden NAME="email" VALUE="$email">
<INPUT TYPE=hidden NAME="reload" VALUE="$reload">
<INPUT TYPE=hidden NAME="color" VALUE="$color">
<INPUT TYPE=hidden NAME="name" VALUE="$name">
_HIDE_
$logw =<<"_HIDE_";
<SELECT NAME="window" SIZE="1">
<OPTION selected>$window
<OPTION>$max<OPTION>40<OPTION>30 
<OPTION>20<OPTION>10</SELECT>
_HIDE_
$kao =<<'_HIDE_';
���̂�����:<SELECT NAME="emoji" SIZE="1">
<OPTION VALUE="" selected>�Ȃ�<OPTION VALUE="(�O�O)">�ɂ��ɂ�
<OPTION VALUE="(�O�O�G)">��⊾����<OPTION VALUE="(=�O�O=)">�͂��������悧
<OPTION VALUE="(�G�G)">�߂���<OPTION VALUE="(�����G)">����
<OPTION>(-_-)zzz<OPTION>�_(~o~)�^
<OPTION> m(__)m<OPTION>(^_-)-��<OPTION>��(.. )�@�߂��߂�
<OPTION>(;_;)/~~~<OPTION>(^^)/~~~<OPTION>(+_+)
<OPTION>((((((^_^;)<OPTION>(^-^;<OPTION>(?_?)��?</SELECT>
_HIDE_
#�當���́A�ŏ���6�͎c���ĂˁB
}#hide END

sub gethost{
$host=$ENV{'REMOTE_HOST'};$host2='';
$host2=$ENV{'HTTP_VIA'} if( $ENV{'HTTP_VIA'}=~s/.*\s(\d+)\.(\d+)\.(\d+)\.(\d+)/$1.$2.$3.$4/ );
$host2=$ENV{'HTTP_X_FORWARDED_FOR'} if( 
$ENV{'HTTP_X_FORWARDED_FOR'}=~s/^(\d+)\.(\d+)\.(\d+)\.(\d+)(\D*).*/$1.$2.$3.$4/ );
$host2=$ENV{'HTTP_FORWARDED'} if( 
$ENV{'HTTP_FORWARDED'}=~s/.*\s(\d+)\.(\d+)\.(\d+)\.(\d+)/$1.$2.$3.$4/ );
$host .=$host2 if($host2);
return $host;
}#gethost END
1;  # RETURN TRUE
__END__