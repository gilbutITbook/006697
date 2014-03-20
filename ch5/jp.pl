#!/usr/bin/perl

#일본어 인코딩
use encoding "utf-8" ;
#use encoding "sjis" ;
#use encoding "euc-jp" ;

#n-gram의 길이
$N = 3 ;

#1문자 단위로 다시 나열함
while (<>) {
    chop;
    push(@chars, split(//, $_));
}
# n-gram을 만듦

for ($i = 0; $i <= $#chars - $N + 1; ++$i) {
   $ngram = "";
   for ($j = 0; $j < $N; ++$j) {
      $ngram = join("", ($ngram, $chars[$i+$j]));
   }
   push(@ngrams, $ngram);
}

#출현 횟수 집계
for (@ngrams) {
    $count{$_}++;
}
for (sort keys %count) {
    printf "%d\t%s\n", $count{$_}, $_;
}


