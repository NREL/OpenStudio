# Conan configuration

Shared Conan 2 configuration for OpenStudio, installed into your Conan home with:

```shell
conan config install developer/conan/config
```

* `settings_user.yml`: declares an optional `os.libc` / `os.libc.version` sub-setting on Linux
* `profiles/glibc`: includes your `default` profile, sets `os.libc` / `os.libc.version` from the detected glibc, and sets `CMAKE_POLICY_VERSION_MINIMUM=3.5` for packages built from source
* `global.conf`: non-interactive mode, parallel downloads, and a source download cache next to the Conan home

## Why: CentOS 9 / AlmaLinux 9

Conan doesn't include the glibc version in the `package_id`, so a binary built against a newer glibc can get downloaded on a machine with an older one, and then fail to link.

This is a problem on CentOS 9 (AlmaLinux 9), which has glibc 2.34: the gcc 14 binaries on `nrel-v2` are built on Ubuntu 24.04 against glibc 2.39 (conancenter has no gcc 14 Linux binaries), and you get errors like `undefined reference to '__isoc23_sscanf'`. (It could be a problem if we were building for an older Ubuntu release too)
See [binary_incompatility_glibc.md](../binary_incompatility_glibc.md) and https://github.com/NatLabRockies/openstudio-cmake-tools/issues/6

## How it works

Installing this config does **not** change anything by itself: `os.libc` defaults to `null`, so package IDs stay the same, and Ubuntu, macOS and Windows keep using the existing conancenter / `nrel-v2` binaries.

It only kicks in when you pass the `glibc` profile (`-pr:a glibc`), i.e. on CentOS 9. Since `os.libc` is a sub-setting of `os`, every recipe that declares `os` in its `settings` (nearly all of them) then gets a **different `package_id`**. No existing conancenter or `nrel-v2` binary matches, so `--build=missing` builds them from source against the local glibc. Header-only packages are still downloaded, since their `package_id` ignores settings.

## Usage (CentOS 9)

```shell
conan install . --build=missing -pr:a glibc -s compiler.cppstd=20 -s build_type=Release
conan upload "*" -r nrel-v2 -c
```

Once uploaded to `nrel-v2`, subsequent CentOS builds download those binaries instead of rebuilding them. CI (`full-build.yml`) passes `-pr:a glibc` for the CentOS 9 build only.
