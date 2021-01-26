> [Wiki](Home) ▸ **Special Ruby Builds**

## AWS Ruby Packages

#### Windows 32-bit
(Using Git Bash)

Download and install [Ruby 2.0-p645](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-2.0.0-p645.exe)
```bash
cd /C/Ruby200/
rm -rf doc share unins000.* lib/libmsvcrt-ruby200-static.a
cd bin
gem install bundler --no-rdoc --no-ri
cd ..
rm -rf lib/ruby/gems/2.0.0/cache/*
mkdir ruby-2.0.0-aws-win32
cp -r bin include lib ruby-2.0.0-aws-win32/
```
Right click --> Send to --> Compressed (zipped) folder `ruby-2.0.0-p645-aws-win32.zip`

#### Windows 64-bit
(Using Git Bash)

Download and install [Ruby 2.0-p645 x64](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-2.0.0-p645-x64.exe)
```bash
cd /C/Ruby200-x64/
rm -rf doc share unins000.* lib/libx64-msvcrt-ruby200-static.a
cd bin
gem install bundler --no-rdoc --no-ri
cd ..
rm -rf lib/ruby/gems/2.0.0/cache/*
mkdir ruby-2.0.0-aws-win64
cp -r bin include lib ruby-2.0.0-aws-win64/
```
Right click --> Send to --> Compressed (zipped) folder `ruby-2.0.0-p645-aws-win64.zip`

#### OS X Mavericks (Yosemite is untested)
Build OpenSSL and Ruby
```bash
curl -LO http://openssl.org/source/openssl-1.0.1m.tar.gz
tar -xzf openssl-1.0.1m.tar.gz
rm openssl-1.0.1m.tar.gz
cd openssl-1.0.1m
./configure darwin64-x86_64-cc --prefix=$HOME/openssl
make -j8
make install
cd ..
rm -rf openssl-1.0.1m
```

```bash
curl -LO http://cache.ruby-lang.org/pub/ruby/2.0/ruby-2.0.0-p645.tar.gz
tar -xzf ruby-2.0.0-p645.tar.gz
rm ruby-2.0.0-p645.tar.gz
cd ruby-2.0.0-p645
./configure --enable-load-relative --disable-install-doc --prefix=$HOME/ruby-2.0.0-aws-Darwin --with-openssl-dir=$HOME/openssl
make -j8
make install
cd ..
rm -rf ruby-2.0.0-p645
cd $HOME/ruby-2.0.0-aws-Darwin/
rm -rf share
rm lib/libruby-static.a
bin/gem install bundler --no-rdoc --no-ri
rm lib/ruby/gems/2.0.0/cache/*
cd ..
tar -jcvf ruby-2.0.0-p645-aws-Darwin.tar.bz2 ruby-2.0.0-aws-Darwin
```

#### Ubuntu
```bash
wget http://cache.ruby-lang.org/pub/ruby/2.0/ruby-2.0.0-p645.tar.gz
tar -xzf ruby-2.0.0-p645.tar.gz
rm ruby-2.0.0-p645.tar.gz
cd ruby-2.0.0-p645
./configure --enable-load-relative --disable-install-doc --prefix=$HOME/ruby-2.0.0-aws-Linux-x86_64
make -j8
make install
cd ..
rm -rf ruby-2.0.0-p645
cd $HOME/ruby-2.0.0-aws-Linux-x86_64/
rm -rf share
bin/gem install bundler --no-rdoc --no-ri
rm lib/libruby-static.a
rm lib/ruby/gems/2.0.0/cache/*
cd ..
tar -jcvf ruby-2.0.0-p645-aws-Linux-x86_64.tar.bz2 ruby-2.0.0-aws-Linux-x86_64
```

## PAT 2.0 Ruby Packages

#### Windows
(Using Git Bash)

Download and install [Ruby 2.0-p648](http://dl.bintray.com/oneclick/rubyinstaller/rubyinstaller-2.0.0-p648.exe), adding the `bin` directory to your path

```bash
gem update --system
gem install bundler --no-rdoc --no-ri
rm -rf doc share unins000.* lib/libmsvcrt-ruby200-static.a lib/libmsvcrt-ruby200.dll.a
cd bin
cd ..
rm -rf lib/ruby/gems/2.0.0/cache/* lib/ruby/gems/2.0.0/doc/*
mkdir ruby-2.0.0-p648-windows
cp -r bin include lib ruby-2.0.0-p648-windows/
```

Right click --> Send to --> Compressed (zipped) folder `ruby-2.0.0p648-windows.zip`


#### OS X El Capitan
Build OpenSSL and Ruby
```bash
curl -LO http://openssl.org/source/openssl-1.0.2g.tar.gz
tar -xzf openssl-1.0.2g.tar.gz
rm openssl-1.0.2g.tar.gz
cd openssl-1.0.2g
./configure darwin64-x86_64-cc --prefix=$HOME/openssl
make -j8
make install
cd ..
rm -rf openssl-1.0.2g
```

```bash
curl -LO http://cache.ruby-lang.org/pub/ruby/2.0/ruby-2.0.0-p648.tar.gz
tar -xzf ruby-2.0.0-p648.tar.gz
rm ruby-2.0.0-p648.tar.gz
cd ruby-2.0.0-p648
./configure --enable-load-relative --enable-shared --disable-install-doc --prefix=$HOME/ruby-2.0.0-darwin --with-openssl-dir=$HOME/openssl
make -j8
make install
cd ..
rm -rf ruby-2.0.0-p648
cd $HOME/ruby-2.0.0-darwin/
bin/gem update --system --no-document
bin/gem install bundler --no-rdoc --no-ri
rm -rf share
# rm lib/libruby-static.a
rm lib/ruby/gems/2.0.0/cache/*
rm -rf lib/ruby/gems/2.0.0/doc/*
zip -r ruby-2.0.0-p648-darwin.zip .
```