call "c:\Python26\Scripts\scons.bat" --boost-compiler=vc90 --boost-version=gd-1_40 mongoclient.lib
move mongoclient.lib mongoclientd.lib
call "c:\Python26\Scripts\scons.bat" --release --boost-compiler=vc90 --boost-version=1_40 all 
call "c:\Python26\Scripts\scons.bat" --release --boost-compiler=vc90 --boost-version=1_40 mongoclient.lib