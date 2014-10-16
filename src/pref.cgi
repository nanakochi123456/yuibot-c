#! /usr/local/bin/perl
#
#ゆいちゃっとPro1.0(pref.cgi)
#
#pref.cgiは、chat.cgiとenter.cgiで、共通して使われるサブルーチン集です。

sub init{	#初期設定など
$rank_file='./rank.dat';#発言ランキング記録
$sanka_file = './sanka.dat'; #参加記録ファイル
$chat_file = './chat.dat'; #チャットログ(ファイル名はなるべく変更しておくこと。)
$method = 'GET';#やっぱり、GETよねぇ～
#退室後のページは、他のＨＰにも変えられます。
$endpage= 'http://www.big.or.jp/~vampire/test/chat.cgi?window=50&reload=150&mode=checked';
$title = 'ゆいちゃっとPro1.0';
$body='<BODY BGCOLOR="#A1FE9F" TEXT="#000000" LINK="#ff0000" VLINK="#ff0000" ALINK="#FF0000">';
$max = 50;#ログの最大行数
$host = &gethost;
$metacode = '<META HTTP-EQUIV="Content-type" CONTENT="text/html; charset=x-sjis">';#SJIS認識させる
####アクセス制限を使用するなら、#を消す。
#$kill_file = './kill.dat';#アクセス制限用
#open(DB,"$kill_file") || die "Cannot Open Log File $kill_file: $!";
#	@lines2 = <DB>;	close(DB);
#foreach $line (@lines2) {
#	next if(length($line)<4);	chop $line;
#	if( $host=~/$line/ ){	print "Status: 204\n\n";	exit;	}
#}
####アクセス制限ここまで
#ロックファイルを使用するなら、#を消す。lock というディレクトリを用意する（777）
#$lockfile = './lock/lock-file.lock';
#$retry = 5;
#while (!symlink(".", $lockfile)) {
#if (--$retry <= 0) { print "Status: 204\n\n"; unlink($lockfile); exit; }
#sleep(1);
#}
#ロックファイルここまで
}#init END

sub ended{	#unlinkとエラートラップとexitを行うためだけのもの。
#	unlink($lockfile);#ロックファイルを使用するなら、#を消す。
print "Content-type: text/plain\n\n $_[0] \n"if($_[0]);#この行は削除可です。
exit;
}#ended END

sub decode{	#一般的なデコード＆変数への代入
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
$name = $FORM{'name'};	$name =~ s/ //g; $name =~ s/　//g;
$name =~ s/\t/&lt;/g;	$name = $host if (!$name);
$color = $FORM{'color'};	$color='red' if(!$color);
$mode = $FORM{'mode'};	$reload = $FORM{'reload'};
$email = $FORM{'email'};	$window =$FORM{'window'};
}#decode END

sub jikan{#$dateに時刻を代入します。
	$times = time;#18時間時差があるなら、$times = time+18*60*60とする。
	($sec,$min,$hour,$mday,$month,$year,$wday,$yday,$isdst) = localtime($times);
	$min = "0$min" if ($min < 10);	$month++;
	$youbi = ('日','月','火','水','木','金','土') [$wday];
	$date = "$month/$mday($youbi)$hour:$min";
}#jikan END

sub sanka{#ここで、アクセス記録をとる。
open(LOG,"$sanka_file") || &ended('$sanka_file open error');
	seek(LOG,0,0);	@sanka=<LOG>;
close(LOG);
$flag=1;
foreach $line (@sanka) {
	($timerec, $handle, $host2,$dmy) = split(/\t/, $line);
	if( $times-60 > $timerec){$line = '';	next;}
		if(($host2 eq $host) && $flag){#ホスト名が同一の場合、時間と名前を最新のものにする。
			$line = "$times\t$name\t$host\td\n";
			$flag =0;$handle = $name;	$handle =~ s/☆/★/g;
		}
	push (@sanka3, "$handle☆") if($handle ne $host2); 
	}#foreach

	if($flag){	#新しい参加者は、加える
		push(@sanka,"$times\t$name\t$host\td\n");
		$name =~ s/☆/★/g;
		push (@sanka3, "$name☆") if($name ne $host); 
	}

undef(@sanka) if($chat eq 'sanka');
open(LOG,">$sanka_file")  || &ended('$sanka_file write error');
	eval 'flock(LOG,2);';
	seek(LOG,0,0);	print LOG @sanka;
	eval 'flock(LOG,8);';
close(LOG);
}#sanka END

sub readlog{	#配列@linesにログを読み込む
	open(DB,"$chat_file") || &ended('$chat_file open error');
		seek(DB,0,0);  @lines = <DB>;   close(DB);
}#readlog END

sub writelog{	#$valueを@linesに加えたのち、書き込む
	(@lines < $max-1) || (@lines = @lines[0 .. $max - 2]);
	unshift( @lines,$value);
	open(DB,">$chat_file") || &ended('$chat_file write error');
		eval 'flock(DB,2);';
		seek(DB,0,0);	print DB @lines;
		eval 'flock(DB,8);';
	close(DB);
}#writelog END
sub hide{	#HTML生成時に共有される部分を一つにまとめる。
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
今のきもち:<SELECT NAME="emoji" SIZE="1">
<OPTION VALUE="" selected>なし<OPTION VALUE="(＾＾)">にこにこ
<OPTION VALUE="(＾＾；)">冷や汗もん<OPTION VALUE="(=＾＾=)">はずかしいよぉ
<OPTION VALUE="(；；)">悲しい<OPTION VALUE="(＠＠；)">驚き
<OPTION>(-_-)zzz<OPTION>＼(~o~)／
<OPTION> m(__)m<OPTION>(^_-)-☆<OPTION>φ(.. )　めもめも
<OPTION>(;_;)/~~~<OPTION>(^^)/~~~<OPTION>(+_+)
<OPTION>((((((^_^;)<OPTION>(^-^;<OPTION>(?_?)え?</SELECT>
_HIDE_
#顔文字は、最初の6個は残してね。
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