#!/bin/sh

#
# Download Sun JDK 6 from:
# http://www.oracle.com/technetwork/java/javase/downloads/index.html
# Java SE Development Kit 6 Update 43
#
# You can download it by browser
# http://download.oracle.com/otn-pub/java/jdk/6u43-b01/jdk-6u43-linux-x64.bin
# wget doesn't work
# 

chmod +x jdk-6u43-linux-x64.bin
./jdk-6u43-linux-x64.bin

mkdir -p /usr/lib/jvm
mv jdk1.6.0_43 /usr/lib/jvm

# Update symbolic links for Java
update-alternatives --install "/usr/bin/java" "java" "/usr/lib/jvm/jdk1.6.0_43/bin/java" 1
update-alternatives --install "/usr/bin/javac" "javac" "/usr/lib/jvm/jdk1.6.0_43/bin/javac" 1
update-alternatives --install "/usr/bin/javadoc" "javadoc" "/usr/lib/jvm/jdk1.6.0_43/bin/javadoc" 1
update-alternatives --install "/usr/lib/mozilla/plugins/libjavaplugin.so" "mozilla-javaplugin.so" "/usr/lib/jvm/jdk1.6.0_43/jre/lib/amd64/libnpjp2.so" 1
update-alternatives --install "/usr/bin/javaws" "javaws" "/usr/lib/jvm/jdk1.6.0_43/bin/javaws" 1
update-alternatives --install "/usr/bin/jar" "jar" "/usr/lib/jvm/jdk1.6.0_43/bin/jar" 1

# Choose the java you installed as default 
update-alternatives --config java
update-alternatives --config javac
update-alternatives --config javadoc
update-alternatives --config mozilla-javaplugin.so
update-alternatives --config javaws
update-alternatives --config jar

# Verify installation
java -version
javac -version


# See more -
# https://help.ubuntu.com/community/Java

