# Binary (in)compatibility of Conan package

The GLIBC version that is used to build the conan packages matters, and that is NOT part of the conan `package_id`.

glibc is backward compatible but not forward compatible: a binary built against an older glibc runs on a newer one, but a binary built against a newer glibc may reference symbols that don't exist on an older one.

| Distribution           | GLIBC |
|------------------------|-------|
| CentOS 7               | 2.17  |
| Ubuntu 18.04           | 2.27  |
| Ubuntu 20.04           | 2.31  |
| CentOS 9 (AlmaLinux 9) | 2.34  |
| Ubuntu 22.04           | 2.35  |
| Ubuntu 24.04           | 2.39  |

So if a remote (conancenter or `nrel-v2`) has binaries built on a newer distro than yours, the `package_id` still matches (same os / arch / compiler / build_type), Conan downloads it, and you get a link error.

For example, on CentOS 9 with gcc 14 (gcc-toolset-14), the boost binary was built against glibc >= 2.38 [^1], where `sscanf` is redirected to the ISO C23 `__isoc23_sscanf`, which doesn't exist in glibc 2.34:

```
libboost_filesystem.a(operations.o): in function `boost::filesystem::detail::(anonymous namespace)::syscall_initializer::syscall_initializer()':
operations.cpp:(.text.startup+0x5f): undefined reference to `__isoc23_sscanf'
```

See https://github.com/NatLabRockies/openstudio-cmake-tools/issues/6

[^1]: conan-center-index only provides Linux binaries for gcc 11 and gcc 13, so there is no gcc 14 boost on conancenter. The gcc 14 binaries on `nrel-v2` are ones we build ourselves on `ubuntu-24.04` (glibc 2.39), see [conan-recipes/.github/workflows/conan_v2.yml](https://github.com/jmarrec/conan-recipes/blob/3b0898bc298335efce8396b49d4d69f593c8a490/.github/workflows/conan_v2.yml#L51).

## Rebuilding all dependencies

The brute-force solution is to rebuild all of the dependencies on the target machine (`conan install . --build="*"`) to **guarantee** binary compatibility. And this is what you should do if developing locally.

But nothing stops Conan from downloading an incompatible binary again next time (e.g. after a cache eviction in CI), and the rebuilt binaries can't be shared since they have the same `package_id` as the incompatible ones on the remotes.

## The solution: make glibc part of the `package_id`

[config/](config/README.md) declares an optional `os.libc` / `os.libc.version` sub-setting (defaulting to `null`, so nothing changes unless you pass it), and a `glibc` profile that sets it from the detected glibc:

```shell
conan config install developer/conan/config
conan install . --build=missing -pr:a glibc -s compiler.cppstd=20 -s build_type=Release
conan upload "*" -r nrel-v2 -c -p "os.libc.version=2.34
```

Since `os.libc` is a sub-setting of `os`, every recipe that declares `os` in its `settings` gets a different `package_id` when you pass the `glibc` profile. No existing conancenter or `nrel-v2` binary matches, so `--build=missing` builds them from source against the local glibc, and they can be uploaded to `nrel-v2` and reused by other machines with the same glibc.

We use this for CentOS 9 (AlmaLinux 9, glibc 2.34) only.
