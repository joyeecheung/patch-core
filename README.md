# patch-core

A simple tool for patching a coredump with a specified OS/ABI byte.

## Why?

Coredumps generated by some virtualized environment do not have their OS/ABI byte set properly.
Some debuggers would try to guest the OS by looking into the ELF notes, but still, these
virtualized environment may not put anything recognizable in the notes either. 

See the comments in [the source code of LLDB](https://github.com/llvm-mirror/lldb/blob/82db09b4ecc2870cf5a226d3415be472eda5f831/source/Plugins/ObjectFile/ELF/ObjectFileELF.cpp#L1554-L1560), a coredump without proper OS/ABI byte
and ELF notes would cause a segmentation fault when you try to load it with LLDB.

```
elf-core::CreateRegisterContextForFrame:: Architecture(27) or OS(0) not supported
Segmentation fault (core dumped)
```

To help debuggers recognize the OS generating the coredump, this tool allows you to patch
the OS/ABI byte manually.

## Usage

Clone this repo, compile it:

```console
$ git clone git@github.com:joyeecheung/patch-core.git
$ cd patch-core
$ make

# See the current OS/ABI type of a coredump (/tmp/core.20184)
$ patch-core /tmp/core.20184
# [Info] Original OSABI = 0x0 (ELFOSABI_SYSV)

# Patch the coredump with the GNU Linux OS/ABI type
$ patch-core /tmp/core.20184 LINUX
# [Info] Original OSABI = 0x0 (ELFOSABI_SYSV)
# [Info] Putting 0x3 in position 7
# [Info] Done.
# [Info] The OS/ABI byte of /tmp/core.20184 is now 0x3 (ELFOSABI_LINUX)
```

```
Usage: patch-core <core> [OS/ABI type]
Available OS/ABI types:
  SYSV = 0x0
  NONE = 0x0
  HPUX = 0x1
  NETBSD = 0x2
  LINUX = 0x3
  GNU = 0x3
  HURD = 0x4
  SOLARIS = 0x6
  AIX = 0x7
  IRIX = 0x8
  FREEBSD = 0x9
  TRU64 = 0xa
  MODESTO = 0xb
  OPENBSD = 0xc
  OPENVMS = 0xd
  NSK = 0xe
  AROS = 0xf
  FENIXOS = 0x10
  ARM_AEABI = 0x40
  ARM = 0x61
```

## License

MIT

