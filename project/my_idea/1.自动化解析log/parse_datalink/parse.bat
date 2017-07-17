del .\datalink_apk.log
perl %~dp0\apk_datalink_summary.pl -input_file main_log -input_dir . -output_dir . -output_file datalink_apk.log -parse_keywork "Posix_connect Debug" 
pause