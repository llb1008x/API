
insmod s3c2410-gpg.o
ifconfig eth0 172.20.223.112 netmask 255.255.255.0
ifconfig eth0 up

insmod s3c2410-gpg.o
insmod s3c2410-adc.o

#boa
mount /root/boa_server /etc
/root/boa

echo start boa server success! 
