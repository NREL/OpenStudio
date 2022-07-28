# Binary (in)compatibility of Conan package

The GLIBC version that is used to build the conan packages matter, and that is NOT part of the conan `package_id`.

* Ubuntu 20.04 has GLIBC 2.31
* Ubuntu 18.04 has GLIBC 2.27
* Centos7 has GLIBC 2.17

The conan docker images that are used:
* gcc7 for eg is based on Artful (17.10), GLIBC 2.26
* gcc8 is Bionic (18.04),
* gcc9 is Eoan (19.10), GLIC 2.30
* gcc10 and gcc11 are Focal (20.04)

cf for eg: https://github.com/conan-io/conan-docker-tools/blob/3f59cb379c0e43097a7685c74b44ae7a3676c0f7/legacy/gcc_7/Dockerfile#L1


On Centos7, you'll always get issues, every binary package downloaded will be binary incompatible.

On Ubuntu 18.04, if you use the system compiler which is gcc-7, it's all good because it'll download a binary package that was made with a 17.10 machine, so an older GLIBC.

Now on that Ubuntu 18.04 machine you upgrade to gcc-10 (because you want C++20 for example) -> It gets a binary package that was built on 20.04 => **Problem**
So now you can't link against boost/1.79.0 for eg, because that boost is looking for a GLIBC provided symbol like `fnctl64` or `statx` and that doesn't exist in GLIBC 2.27.

For example: `fnctl` was purely redefined to `fnctl64` in GLIBC 2.28.

## The solution: rebuild all dependencies

The only safe solution in that case is to just rebuild all of the dependencies on the target machine to **guarantee** binary compatibility.

Our `ConanInstall.cmake` allows getting a `-DCONAN_FIRST_TIME_BUILD_ALL:BOOL=ON` to achieve that. That boolean is automatically switched to OFF after the first pass.

```shell
/bin/rm -Rf ~/.conan/data
cmake -DCONAN_FIRST_TIME_BUILD_ALL:BOOL=ON ..
```
