#~/bin/bash

#Xcode 4.2.1 and earlier
curl -O http://downloads.sourceforge.net/project/boost/boost/1.47.0/boost_1_47_0.tar.gz
tar -xzf boost_1_47_0.tar.gz
rm boost_1_47_0.tar.gz
cd boost_1_47_0
sh ./bootstrap.sh
./b2 variant=release address-model=32_64 architecture=x86 debug-symbols=on pch=off macosx-version=10.6 threading=multi install --prefix=build -j2
cd build
sudo cp -r * /usr/local/
cd ../..
rm -rf boost_1_47_0
