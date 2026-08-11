use strict;
use warnings;

my ($input, $output) = @ARGV;
die "Usage: generate_app_icon.pl <icon.h> <output.svg>\n" unless defined $input && defined $output;

open my $in, '<', $input or die "Cannot open $input: $!\n";
local $/;
my $source = <$in>;
close $in;

$source =~ /uint8_t\s+icon32\[\]\s*=\s*\{(.*?)\};/s or die "icon32 array not found in $input\n";
my @bytes = map { hex($_) } ($1 =~ /0x([0-9a-fA-F]{2})/g);
die "icon32 must contain 4096 RGBA bytes\n" unless @bytes == 4096;

open my $out, '>', $output or die "Cannot create $output: $!\n";
print {$out} qq{<svg xmlns="http://www.w3.org/2000/svg" version="1.1" baseProfile="tiny" width="32" height="32" viewBox="0 0 32 32">\n};

for my $y (0 .. 31) {
    my $x = 0;
    while ($x < 32) {
        my $index = ($y * 32 + $x) * 4;
        my @rgba = @bytes[$index .. $index + 3];
        my $run = 1;
        while ($x + $run < 32) {
            my $next = ($y * 32 + $x + $run) * 4;
            last unless $bytes[$next] == $rgba[0]
                && $bytes[$next + 1] == $rgba[1]
                && $bytes[$next + 2] == $rgba[2]
                && $bytes[$next + 3] == $rgba[3];
            ++$run;
        }

        if ($rgba[3] != 0) {
            my $fill = sprintf('#%02x%02x%02x', @rgba[0 .. 2]);
            my $opacity = '';
            if ($rgba[3] != 255) {
                my $alpha = sprintf('%.6f', $rgba[3] / 255);
                $alpha =~ s/0+$//;
                $alpha =~ s/\.$//;
                $opacity = qq{ fill-opacity="$alpha"};
            }
            print {$out} qq{<rect x="$x" y="$y" width="$run" height="1" fill="$fill"$opacity/>\n};
        }

        $x += $run;
    }
}

print {$out} "</svg>\n";
close $out;
