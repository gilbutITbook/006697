#!/usr/bin/perl
#use encoding "utf-8";
open(FILE, @ARGV[0]);
@lines = <FILE>;

foreach (@lines) {
    @sl = split;
    dumpArr(@sl);
}

close(FILE);

sub dumpArr {
    for($i = 0; $i <= $#_; $i++) {
        print $_[$i];
        print "\n";
    }
}
