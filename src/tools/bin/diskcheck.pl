#!/usr/bin/perl
#
# Original: DiskCheck 3.1.1 (11/18/98)
#
# This program was originally written by Kirk Bauer (kirk@kaybee.org).
# Some additions/changes have been made by Benjamin Cabell (q98@besiex.org).
# 
# Kirk's disclaimer:
#   First of all, I am *NOT* responsible for anything bad that might happen
#   because of this program.  It doesn't do anything bad on my system, but
#   it is not my fault if it does something bad on your system.
# 
# Benjamin's Disclaimer:
#   (see Kirk's).
#

require '/root/tools/conf/diskcheck.conf' || die "Config File Not Found";

$hostname = `hostname`;
chomp($hostname);

@list=`/bin/df $ignore`;

($null, $blocksize, $null, $null, $null, $null, $null) = split (/\s+/, shift(@list));
$blocksize =~ s/-blocks$//;

# this function is only used if $tempfile can't be written, usually a full partition
sub MailError{
	my $msg = shift ;
	open (MAIL , "|sendmail -s \"$!\" $mailto");
	print MAIL $msg;
	close MAIL;
	exit;
}

open(MFILE, ">$tempfile.$$") || MailError($!);

print MFILE "To: $mailto\n";
print MFILE "From: $mailfrom\n";
print MFILE "Subject: WARNING: Disk usage high on $hostname!!!\n";
print MFILE "\n";
$high = 0;

print MFILE "Disk usage for $hostname:\n\n";

for (@list) {
   ($volume, $total, $used, $available, $pct, $mountPoint) = split (/\s+/);
   chop($pct);
   chomp($mountPoint);
   if ( $exclude !~ m=$volume= ) {
      if ($Cutoff{$volume} ) {
         if ($pct >= $Cutoff{$volume}) {
            $high = 1;
            $available = int($available / 1024) ;
            print MFILE "$volume ($mountPoint) is $pct% full -- $available(MB) remain\n";
         }
      }
      elsif ($Cutoff{$mountPoint} ) {
         if ($pct >= $Cutoff{$mountPoint}) {
            $high = 1;
            $available = int($available / 1024) ;
            print MFILE "$volume ($mountPoint) is $pct% full -- $available(MB) remain\n";
         }
      }
      elsif ($pct >= $DefaultCutoff) {
        $high = 1;
        $available = int($available / 1024) ;
        print MFILE "$volume ($mountPoint) is $pct% full -- $available(MB) remain\n";
      }
   }
}

close (MFILE);


if ($high != 0) {
  #mail the message
  open(MAIL,"|$mailprog -t");

  open(MFILE, "$tempfile.$$") || die "Can't open $tempfile.$$!";

  while(<MFILE>) {
    print MAIL $_;
  }

  close (MFILE);
}

unlink ("$tempfile.$$");

exit(0);

