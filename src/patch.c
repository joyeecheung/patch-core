#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 8

#define EI_OSABI 7                  /* OS/ABI identification */
#define ELFOSABI_NONE 0             /* UNIX System V ABI */
#define ELFOSABI_SYSV 0             /* Alias.  */
#define ELFOSABI_HPUX 1             /* HP-UX */
#define ELFOSABI_NETBSD 2           /* NetBSD.  */
#define ELFOSABI_GNU 3              /* Object uses GNU ELF extensions.  */
#define ELFOSABI_LINUX ELFOSABI_GNU /* Compatibility alias.  */
#define ELFOSABI_HURD 4             /* 	GNU Hurd  */
#define ELFOSABI_SOLARIS 6          /* Sun Solaris.  */
#define ELFOSABI_AIX 7              /* IBM AIX.  */
#define ELFOSABI_IRIX 8             /* SGI Irix.  */
#define ELFOSABI_FREEBSD 9          /* FreeBSD.  */
#define ELFOSABI_TRU64 10           /* Compaq TRU64 UNIX.  */
#define ELFOSABI_MODESTO 11         /* Novell Modesto.  */
#define ELFOSABI_OPENBSD 12         /* OpenBSD.  */
#define ELFOSABI_OPENVMS 13         /* 	OpenVMS.  */
#define ELFOSABI_NSK 14             /* 	NonStop Kernel.  */
#define ELFOSABI_AROS 15            /* 	AROS.  */
#define ELFOSABI_FENIXOS 16         /* 	Fenix OS.  */
#define ELFOSABI_ARM_AEABI 64       /* ARM EABI */
#define ELFOSABI_ARM 97             /* ARM */

#define OS_ABI_LIST(V)                                                         \
  V(SYSV)                                                                      \
  V(NONE)                                                                      \
  V(HPUX)                                                                      \
  V(NETBSD)                                                                    \
  V(LINUX)                                                                     \
  V(GNU)                                                                       \
  V(HURD)                                                                      \
  V(SOLARIS)                                                                   \
  V(AIX)                                                                       \
  V(IRIX)                                                                      \
  V(FREEBSD)                                                                   \
  V(TRU64)                                                                     \
  V(MODESTO)                                                                   \
  V(OPENBSD)                                                                   \
  V(OPENVMS)                                                                   \
  V(NSK)                                                                       \
  V(AROS)                                                                      \
  V(FENIXOS)                                                                   \
  V(ARM_AEABI)                                                                 \
  V(ARM)

static const char *os_abi_to_string(char osabi_byte) {
#define _MAKE_OSABI_CASE(x)                                                    \
  if (osabi_byte == ELFOSABI_##x) {                                            \
    return #x;                                                                 \
  }

  OS_ABI_LIST(_MAKE_OSABI_CASE)

  return "UNKNOWN";
#undef _MAKE_OSABI_CASE
}

static int string_to_osabi_byte(const char *str) {
#define _MAKE_OSABI_CASE(x)                                                    \
  if (strcmp(str, #x) == 0) {                                                  \
    return ELFOSABI_##x;                                                       \
  }

  OS_ABI_LIST(_MAKE_OSABI_CASE)

  return -1;
#undef _MAKE_OSABI_CASE
}

static void print_available_osabi() {
#define _MAKE_OSABI_CASE(x) fprintf(stderr, "  %s = 0x%x\n", #x, ELFOSABI_##x);

  OS_ABI_LIST(_MAKE_OSABI_CASE)

#undef _MAKE_OSABI_CASE
}

int is_elf_header(const char *buf) {
  // Check the magic number
  if (buf[0] == 0x7f && buf[1] == 'E' && buf[2] == 'L' && buf[3] == 'F') {
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: patch-core <core> [OS/ABI type]\n");
    fprintf(stderr, "Available OS/ABI types:\n");
    print_available_osabi();
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  FILE *fp = fopen(filename, "r+b");
  if (fp == NULL) {
    fprintf(stderr, "[Error] Unable to open file: %s\n", filename);
    fclose(fp);
    exit(EXIT_FAILURE);
  }

  char header[HEADER_SIZE];
  int bytes_read = fread(header, 1, HEADER_SIZE, fp);
  if (bytes_read != HEADER_SIZE) {
    fprintf(stderr, "[Error] File size is too small. "
                    "Are you sure it is a coredump?\n");
    fclose(fp);
    exit(EXIT_FAILURE);
  }

  if (!is_elf_header(header)) {
    fprintf(stderr, "[Error] The file doesn't seem to be in ELF format. "
                    "Are you sure it is a coredump?\n");
    fclose(fp);
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "[Info] Original OSABI = 0x%x (ELFOSABI_%s)\n", header[7],
          os_abi_to_string(header[7]));

  if (argc < 3) {
    return 0;
  }

  char *osabi_name = argv[2];
  int osabi_value = string_to_osabi_byte(osabi_name);
  if (osabi_value == -1) {
    fprintf(stderr, "[Error] %s is not recognized as a OS/ABI type\n",
            osabi_name);
    fprintf(stderr, "Available OS/ABI types:\n");
    print_available_osabi();
    fclose(fp);
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "[Info] Putting 0x%x in position %d\n", osabi_value,
          EI_OSABI);
  fseek(fp, EI_OSABI, SEEK_SET);
  fputc(osabi_value, fp);
  fprintf(stdout, "[Info] Done.\n");
  fprintf(stdout, "[Info] The OS/ABI byte of %s is now 0x%x (ELFOSABI_%s)\n",
          filename, osabi_value, osabi_name);
  fclose(fp);
  return 0;
  
  
  
  Add 1
}
