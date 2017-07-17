#!/bin/perl

###############################################################################
# parse_log.pl
# $Author: Jinhao Wu
# $DateTime: 2016/03/26
###############################################################################
use Cwd;
use File::Copy;
use Getopt::Long;

use constant PARSE_LOG 	  => "CLDMA_MD-10";
use constant PARSE_LOG_1  => "alarm";
use constant PARSE_LOG_2  => "EINT_212";

@parse_log_str_list = (
	#PARSE_LOG,
	#PARSE_LOG_1,
	#PARSE_LOG_2,
);

my %g_log_hash;
my %Hash;
sub addLogToHash
{
    my ($func_name, $log_result) = @_;
    
    ## It is log file name too. 
    my $key = $func_name;
     
    if($g_log_hash{$key} eq "")
    {
        ## Add new Key and Array to log_hash.        
        my $log_buf = "";
        $g_log_hash{$key} = \$log_buf;                 
    }
    
    ## Get ref buffer from Hash.
    my $ref_log_buf = $g_log_hash{$key};
    ## Add log result to hash ref buffer.
    $$ref_log_buf .= $log_result;    
}

#no use
sub getSubString
{
    my ($raw_str, $pre_str, $post_str) = @_;
    my $begin_pos = index($raw_str, $pre_str)+length($pre_str);
	my $end_pos = index($raw_str, $post_str, $begin_pos);
	if($end_pos < 0)
	{
	   $end_pos = length($raw_str); # string EOF
	}
	$resutlt_str = substr($raw_str, $begin_pos, $end_pos-$begin_pos);
    
    #print "DEBUG - raw_line:$raw_str ";
    #print "DEBUG - prestr:$pre_str ($begin_pos) \n";
    #print "DEBUG - poststr:$post_str ($end_pos) \n";
    #print "DEBUG - subStr: $resutlt_str \n";
    
    return $resutlt_str;
}

sub parse_log
{
    my ($line,$senstr) = @_;
    
	#change str pattern
	#if($line =~ /(\d)(\d)-(\d)(\d) (\d+):(\d+):(\d+)------->/){
	#	$log_result = "$1$2-$3$4 $5:$6:$7\n";
	#}	
	#03-25 17:15:22.759770  9787  9849 D Posix   : [Posix_connect Debug]Process com.android.music :80
	#modify by jinhao 
	#$log_result = $line;
	if($line =~ /.*\[Posix_connect Debug\]Process\s([\w+.:]+)\s.*/){
		$log_result = "$1\n";
		print "$log_result";
		
		if(defined $Hash{$1})
		{
			$Hash{$1}++;
		}else{
			$Hash{$1}=1;
		}
	}
	
	
	#if(index($line, "alarm") >= 0){
	#	my $hash_name = "alarm";
	#	addLogToHash($hash_name, $log_result);
	#} 
	
	#modify by jinhao 
	my $hash_name = "debug";
	addLogToHash($hash_name, $log_result);

	
}

sub checkLineContent
{
    my ($line) = @_;
	#print "$line\n";
	foreach $senstr (@parse_log_str_list)
    {
		if(/^#/){
		
		}
		else{
			#print"$senstr\n";
		    if(index($line, $senstr) >= 0)
			{
				parse_log($line,$senstr);
				goto end;
			}			
		}
    }  
end:
;	
}

sub parseLogFromFile
{
    my ($log_dir, $log_file) = @_;
    
    print "parseLogFromFile :$log_file\n";
    my $filePath;
    
    if($log_dir eq "")
    {
        $filePath = $log_file;
    }else
    {
		$filePath = $log_dir."\\".$log_file;
		print "parseLogFromFile :$filePath\n";
    }
    
    print "parseLogFromFile - LogFile:$filePath\n";    
    open (SVC_FILE, $filePath) || die("ERROR - Cannot open file:$log_file");    
   	while (my $line = <SVC_FILE>)
	{
	    checkLineContent($line);
	}
    close (SVC_FILE);
}   

$package_count = 0;
####################################################################
# Begin OF MAIN
####################################################################
GetOptions (
            'input_file=s'        => \ my $input_log_file,
			'output_file=s'        => \ my $output_log_file,
            'input_dir=s'        => \ my $input_log_dir,
			'output_dir=s'        => \ my $output_log_dir,
			'parse_keywork=s'        => \ my $keywork,
			'h'          => \ my $help,			
			);

	my $home_dir = $ENV{PWD};

    print "input file - $input_log_dir\\$input_log_file\n";
	print "output file - $output_log_dir\\$output_log_file\n";

	print "$keywork\n";
	@parse_log_str_list = split(',',$keywork);
	print "@parse_log_str_list\n";
	
	parseLogFromFile($input_log_dir, $input_log_file);

	$new_file = "$output_log_dir\\". "$output_log_file";
	open MYFILE, ">$new_file";
	
	#foreach $key (keys %g_log_hash)
	#{
		#print "$key\n";
	#	my $ref_buf = $g_log_hash{$key};
	#	print MYFILE $$ref_buf;	
	#}	
    
	
	foreach $element (sort{$Hash{$b}<=>$Hash{$a}} keys %Hash){
		print MYFILE "$element $Hash{$element}\n";
		$package_count = $package_count + $Hash{$element};
	}
	print MYFILE "$element $package_count\n";
	
	close (MYFILE);
	
	#foreach $element(sort keys %Hash){
	#	print "$element $Hash{$element}\n";
	#}
	
	
    #print "INFO - Finish \n";
    exit(0);
    	
####################################################################
# END OF MAIN
####################################################################
