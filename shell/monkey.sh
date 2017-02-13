 #!/bin/sh                                                                                                                                                                                   
 while [ 1 ]                                                                     
 do                                                                              
 monkey --throttle 600 -v -v -v --ignore-crashes --ignore-timeouts --ignore-security-exceptions --ignore-native-crashes --kill-process-after-error 999999999
 sleep 120                                                                       
 input keyevent 26                                                               
 done     
