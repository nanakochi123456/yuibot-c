package jcode;
;######################################################################
;#
;# jcode.pl: Perl library for Japanese character code conversion
;#
;# Copyright (c) 1995,1996 Kazumasa Utashiro <utashiro@iij.ad.jp>
;# Internet Initiative Japan Inc.
;# 1-4 Sanban-cho, Chiyoda-ku, Tokyo 102, Japan
;#
;# Copyright (c) 1992,1993,1994 Kazumasa Utashiro
;# Software Research Associates, Inc.
;# Original by srekcah@sra.co.jp, Feb 1992
;#
;# Redistribution for any purpose, without significant modification,
;# is granted as long as all copyright notices are retained.  THIS
;# SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
;# IMPLIED WARRANTIES ARE DISCLAIMED.
;#
;; $rcsid = q$Id: jcode.pl,v 2.0 1996/10/02 16:02:38 utashiro Rel $;
;#
;######################################################################
;#注意！！
;#このjcodeLE.plは、チャットスクリプト　ゆいちゃっとPro専用に利用するために、
;#余分な機能を削除した、機能限定版(Limited Edition)です。
;#不要なサブルーチンの削除と、それにともなう一部のルーチンの修正だけしています。
;#オリジナルのjcode.plは、多機能かつ汎用性に優れているものです。
;#
;#私はあまりプログラム等に詳しくないので..正確なことはわからないのですが、
;#一般にCGIのコード変換用には、
;#		&jcode'convert(*value,'sjis');
;#の一行だけでjcode.plを利用しているみたいです。
;#ただでさえ、サーバに負荷をかけやすいPerlのCGIに、jcode.plの未使用の機能を持たせたまま
;#require './jcode.pl';で取り込んで使うよりはメモリー等への負担が少ないのでは.........と考えての提案です。
;#そんな考え方はまったく意味なし！！って、思う専門的な知識をお持ちの方はぜひ、御教示くださいませ。
;#E-mail:yui@cup.com
;#
;#ちなみに、この限定jcodeLE.plは、
;#&jcode'convert(*value,'sjis');#通常のsjisへの変換
;#のみ可能です。euc変換機能はありません。
;#他のCGIスクリプトで上記の機能しか使わないのなら、jcodeLE.plでも大丈夫でしょう。
;#でも...jcodeLE.plを御利用のさいには、かならずオリジナルのjcode.pl-2.0も入手しておいてください。
;#ftp://ftp.iij.ad.jp/pub/published/oreilly/nutshell/ujip/perl/です。
;#1997年4月5日
;# 改変責任者　Suzuki Yui <E-mail:yui@cup.com>
;#####################################################################
&init unless defined $version;
sub init {
    $version = $rcsid =~ /,v ([\d.]+)/ ? $1 : 'unkown';
    $re_bin  = '[\000-\006\177\377]';
    $re_jp   = '\e\$[\@B]';
    $re_asc  = '\e\([BJ]';
    $re_kana = '\e\(I';
    ($esc_jp, $esc_asc, $esc_kana) = ("\e\$B", "\e(B", "\e(I");
    $re_sjis_c = '[\201-\237\340-\374][\100-\176\200-\374]';
    $re_sjis_kana = '[\241-\337]';
    $re_euc_c = '[\241-\376][\241-\376]';
    $re_euc_kana = '\216[\241-\337]';
    $re_euc_s = "($re_euc_c)+";
    $re_sjis_s = "($re_sjis_c)+";
	$convf{'jis', 'sjis'} = *jis2sjis;
	$convf{'sjis', 'sjis'} = *sjis2sjis;
	$convf{'euc', 'sjis'} = *euc2sjis;
}
sub getcode {
    local(*_) = @_;
    return undef unless /[\e\200-\377]/;
    return 'jis' if /$re_jp|$re_asc|$re_kana/o;
    return 'binary' if /$re_bin/o;
    local($sjis, $euc);
    $sjis += length($&) while /($re_sjis_c)+/go;
    $euc  += length($&) while /($re_euc_c)+/go;
    (&max($sjis, $euc), ('euc', undef, 'sjis')[($sjis<=>$euc) + $[ + 1]);
}
sub max { $_[ $[ + ($_[$[] < $_[$[+1]) ]; }
sub convert {
    local(*_, $ocode, $icode) = @_;
    return (undef, undef) unless $icode = $icode || &getcode(*_);
    return (undef, $icode) if $icode eq 'binary';
    local(*convf) = $convf{$icode, $ocode};
    do convf(*_);
    (*convf, $icode);
}
sub jis2sjis {
    local(*_, $n) = @_;
    s/($re_jp|$re_asc|$re_kana)([^\e]*)/&_jis2sjis($1,$2)/geo;
    $n;
}
sub _jis2sjis {
    local($esc, $_) = @_;
    if ($esc !~ /$re_asc/o) {
	$n += tr/\041-\176/\241-\376/;
	s/$re_euc_c/$e2s{$&}||&e2s($&)/geo if $esc =~ /$re_jp/o;
    }
    $_;
}
sub euc2sjis {
    local(*_, $n) = @_;
    $n = s/$re_euc_c|$re_euc_kana/$e2s{$&}||&e2s($&)/geo;
}
sub e2s {
    local($c1, $c2, $code);
    ($c1, $c2) = unpack('CC', $code = shift);
    if ($c1 == 0x8e) {
	return substr($code, 1, 1);
    } elsif ($c1 % 2) {
	$c1 = ($c1>>1) + ($c1 < 0xdf ? 0x31 : 0x71);
	$c2 -= 0x60 + ($c2 < 0xe0);
    } else {
	$c1 = ($c1>>1) + ($c1 < 0xdf ? 0x30 : 0x70);
	$c2 -= 2;
    }
	$e2s{$code} = pack('CC', $c1, $c2);
}
sub sjis2sjis {
    local(*_) = @_;
}
1;

