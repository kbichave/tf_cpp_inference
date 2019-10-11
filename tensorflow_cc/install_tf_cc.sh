apt update
apt-get -y install build-essential 
apt-get -y install curl 
apt-get -y install git 
apt-get -y install cmake
apt-get -y install unzip 
apt-get -y install autoconf 
apt-get -y install autogen 
apt-get -y install automake 
apt-get -y install libtool 
apt-get -y install mlocate 
apt-get -y install zlib1g-dev 
apt-get -y install g++-7 
apt-get -y install python 
apt-get -y install python3-numpy 
apt-get -y install python3-dev 
apt-get -y install python3-pip 
apt-get -y install python3-wheel 
apt-get -y install wget
rm -rf build
mkdir build && cd build
cmake ..
make && make install

