#decompress
sudo mkdir /opt/
sudo tar -xvf opt.tar
sudo chmod 777 -Rf opt/
sudo mv  opt/*  /opt/.

#python install 2.7.6
cd /opt
cd Python-2.7.6 
sudo ./configure 
sudo make install 

#install gcc4.8 fixed build error for libstdc++.so.6.0.20
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update; sudo apt-get install gcc-4.8 g++-4.8
sudo update-alternatives --remove-all gcc
sudo update-alternatives --remove-all g+
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20

sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
sudo update-alternatives --config gcc
  
sudo update-alternatives --remove-all gcc
sudo update-alternatives --remove-all g++
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.6 30
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.6 30
sudo update-alternatives --config gcc
sudo update-alternatives --config g++

#fix build error for install gcc4.8
 
sudo apt-get install libbz2-dev 

