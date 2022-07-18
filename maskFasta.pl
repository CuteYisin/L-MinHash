#!/usr/bin/perl

use strict;
use warnings;


my $ifn = shift | die "USAGE: maskFasta.pl UNMASKED_IN.fasta MASKED_OUT.fasta";
my $ofn = shift | die "USAGE: maskFasta.pl UNMASKED_IN.fasta MASKED_OUT.fasta";
my $maxlen = 9;


sub mask {
    my ($seq) = @_;
    my $seqlen = length($seq);

    for (my $i = 0; $i < int($seqlen / $maxlen / 5); $i++) {
        my $masklen = int(rand() * ($maxlen + 1));
        next unless $masklen;

        my $idx = int(rand() * ($seqlen - $masklen - $maxlen * 2)) + $maxlen;
        for (my $valid = 0; $valid == 0; $idx = int(rand() * ($seqlen - $masklen - $maxlen * 2)) + $maxlen) {
            $valid = 1;
            for (my $j = $idx - $maxlen; $j < $idx + $masklen + $maxlen; $j++) {
                next if $j < 0;
                last if $j >= $seqlen;

                if (substr($seq, $j, 1) eq "_") {
                    $valid = 0;
                    last;
                }
            }
        }
        substr($seq, $idx, $masklen) = "_" x $masklen;
    }

    return $seq;
}


my ($header, $buffer) = (undef, "");
open(INPUT, $ifn);
open(OUTPUT, ">$ofn");
while (<INPUT>) {
    if (/^>/) {
        print(OUTPUT $header, mask($buffer), "\n") if $header;
        ($header, $buffer) = ($_, "");
    } else {
        chomp($buffer .= $_);
    }
}
print(OUTPUT $header, mask($buffer), "\n") if $header;
close(OUTPUT);
close(INPUT);

