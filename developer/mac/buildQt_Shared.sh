#~/bin/bash

cd ~
curl -O http://releases.qt-project.org/qt4/source/qt-everywhere-opensource-src-4.8.2.tar.gz
tar -xzf qt-everywhere-opensource-src-4.8.2.tar.gz
rm qt-everywhere-opensource-src-4.8.2.tar.gz
cd qt-everywhere-opensource-src-4.8.2

xcode=$(xcode-select -print-path)
if [ $xcode == "/Developer" ]; then
    #Xcode 4.2.1 or earlier
	./configure -sdk /Developer/SDKs/MacOSX10.6.sdk -debug-and-release -opensource -arch x86 -arch x86_64 -qt-sql-sqlite -plugin-sql-sqlite -nomake examples -nomake demos -nomake docs -no-qt3support -confirm-license
elif [ $xcode == "/Applications/Xcode.app/Contents/Developer" ]; then
    #Xcode 4.3 or later
	./configure -sdk /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk -debug-and-release -opensource -arch x86 -arch x86_64 -qt-sql-sqlite -plugin-sql-sqlite -nomake examples -nomake demos -nomake docs -no-qt3support -confirm-license
else
    echo "Unrecognized Xcode location"
fi

make -j2
sudo make install
cd ..
rm -rf qt-everywhere-opensource-src-4.8.2


#sudo nano /etc/paths
##Add "/usr/local/Trolltech/Qt-4.8.2/bin"
