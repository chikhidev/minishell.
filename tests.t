cmds only (no pipes no redirections no heredoc)



$ ls
error.c  includes  libft  main.c  Makefile  memo.c  minishell  parsing  string.c
$ 


$ ls -la
total 236
drwxrwxr-x  7 salah salah   4096 Jun  6 22:03 .
drwxr-x--- 22 salah salah   4096 Jun  6 22:05 ..
-rw-rw-r--  1 salah salah    136 Jun  5 23:16 error.c
drwxrwxr-x  8 salah salah   4096 Jun  6 21:10 .git
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 includes
drwxrwxr-x  2 salah salah   4096 Jun  6 00:11 libft
-rw-rw-r--  1 salah salah   2697 Jun  5 23:28 main.c
-rw-rw-r--  1 salah salah    990 Jun  5 23:16 Makefile
-rw-rw-r--  1 salah salah   1589 Jun  5 23:16 memo.c
-rwxrwxr-x  1 salah salah 185224 Jun  6 00:11 minishell
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 parsing
-rw-rw-r--  1 salah salah   1314 Jun  5 23:16 string.c
-rw-rw-r--  1 salah salah    463 Jun  6 22:21 tests.txt
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 .vscode
$ 


$ ls  -la  # TAB instead of space
total 236
drwxrwxr-x  7 salah salah   4096 Jun  6 22:03 .
drwxr-x--- 22 salah salah   4096 Jun  6 22:05 ..
-rw-rw-r--  1 salah salah    136 Jun  5 23:16 error.c
drwxrwxr-x  8 salah salah   4096 Jun  6 21:10 .git
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 includes
drwxrwxr-x  2 salah salah   4096 Jun  6 00:11 libft
-rw-rw-r--  1 salah salah   2697 Jun  5 23:28 main.c
-rw-rw-r--  1 salah salah    990 Jun  5 23:16 Makefile
-rw-rw-r--  1 salah salah   1589 Jun  5 23:16 memo.c
-rwxrwxr-x  1 salah salah 185224 Jun  6 00:11 minishell
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 parsing
-rw-rw-r--  1 salah salah   1314 Jun  5 23:16 string.c
-rw-rw-r--  1 salah salah   2812 Jun  6 22:25 tests.txt
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 .vscode
$ 


$             ls    # whitespaces befor
error.c   libft   Makefile  minishell  string.c
includes  main.c  memo.c    parsing    tests.txt
$ ls                # whitespaces after
error.c   libft   Makefile  minishell  string.c
includes  main.c  memo.c    parsing    tests.txt
$ 


$ ls -l                     -a  # args seperated with many whitespaces
total 236
drwxrwxr-x  7 salah salah   4096 Jun  6 22:32 .
drwxr-x--- 22 salah salah   4096 Jun  6 22:05 ..
-rw-rw-r--  1 salah salah    136 Jun  5 23:16 error.c
drwxrwxr-x  8 salah salah   4096 Jun  6 22:32 .git
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 includes
drwxrwxr-x  2 salah salah   4096 Jun  6 22:31 libft
-rw-rw-r--  1 salah salah   2697 Jun  5 23:28 main.c
-rw-rw-r--  1 salah salah    990 Jun  5 23:16 Makefile
-rw-rw-r--  1 salah salah   1589 Jun  5 23:16 memo.c
-rwxrwxr-x  1 salah salah 185224 Jun  6 00:11 minishell
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 parsing
-rw-rw-r--  1 salah salah   1314 Jun  5 23:16 string.c
-rw-rw-r--  1 salah salah   3984 Jun  6 22:35 tests.txt
drwxrwxr-x  2 salah salah   4096 Jun  5 23:16 .vscode
$ 


$ ls "-a"  # quotes around args
.   error.c  includes  main.c    memo.c     parsing   tests.txt
..  .git     libft     Makefile  minishell  string.c  .vscode
$ 

$ lsa
Command '' not found
$ 


$ ""
Command '' not found
$ 


$ .
bash: .: filename argument required
$


$ "ls"
error.c   libft   Makefile  minishell  string.c
includes  main.c  memo.c    parsing    tests.txt
$ "ls -la"
ls -la: command not found
$ 


$ ls $HOME
1337     Documents  minishell  Pictures  snap       Videos
Desktop  Downloads  Music      Public    Templates
$ 


$ unset PATH
$ ls $HOME
bash: ls: No such file or directory
$ 


$ ls *
error.c  main.c  Makefile  memo.c  minishell  string.c  tests.txt

includes:
main.h  memo.h  parsing.h

libft:
ft_atoi.c     ft_itoa.c                ft_memcpy.o      ft_strchr.o    ft_strncmp.o
ft_atoi.o     ft_itoa.o                ft_memmove.c     ft_strdup.c    ft_strnstr.c
ft_bzero.c    ft_lstadd_back_bonus.c   ft_memmove.o     ft_strdup.o    ft_strnstr.o
ft_bzero.o    ft_lstadd_front_bonus.c  ft_memset.c      ft_striteri.c  ft_strrchr.c
ft_calloc.c   ft_lstclear_bonus.c      ft_memset.o      ft_striteri.o  ft_strrchr.o
ft_calloc.o   ft_lstdelone_bonus.c     ft_putchar_fd.c  ft_strjoin.c   ft_strtrim.c
ft_isalnum.c  ft_lstiter_bonus.c       ft_putchar_fd.o  ft_strjoin.o   ft_strtrim.o
ft_isalnum.o  ft_lstlast_bonus.c       ft_putendl_fd.c  ft_strlcat.c   ft_substr.c
ft_isalpha.c  ft_lstmap_bonus.c        ft_putendl_fd.o  ft_strlcat.o   ft_substr.o
ft_isalpha.o  ft_lstnew_bonus.c        ft_putnbr_fd.c   ft_strlcpy.c   ft_tolower.c
ft_isascii.c  ft_lstsize_bonus.c       ft_putnbr_fd.o   ft_strlcpy.o   ft_tolower.o
ft_isascii.o  ft_memchr.c              ft_putstr_fd.c   ft_strlen.c    ft_toupper.c
ft_isdigit.c  ft_memchr.o              ft_putstr_fd.o   ft_strlen.o    ft_toupper.o
ft_isdigit.o  ft_memcmp.c              ft_split.c       ft_strmapi.c   libft.a
ft_isprint.c  ft_memcmp.o              ft_split.o       ft_strmapi.o   libft.h
ft_isprint.o  ft_memcpy.c              ft_strchr.c      ft_strncmp.c   Makefile

parsing:
expanding.c  op.c         paranths.c  quoting.c        split.c
nodes.c      paranths2.c  parser.c    quoting_utils.c
$ 


$ "ls *"
ls *: command not found
$


$ ls >salah  # redirects to salah
$ ls ">salah" # runs ls on file ">salah"
'>salah'
$ 


$ ls > *
bash: *: ambiguous redirect
$ 


$ ping google.com  #cmd doesnt stop until ^C
PING google.com (142.250.200.110) 56(84) bytes of data.
64 bytes from mad41s13-in-f14.1e100.net (142.250.200.110): icmp_seq=1 ttl=63 time=24.8 ms
64 bytes from mad41s13-in-f14.1e100.net (142.250.200.110): icmp_seq=2 ttl=63 time=33.8 ms
64 bytes from mad41s13-in-f14.1e100.net (142.250.200.110): icmp_seq=3 ttl=63 time=88.5 ms
64 bytes from mad41s13-in-f14.1e100.net (142.250.200.110): icmp_seq=4 ttl=63 time=24.8 ms
64 bytes from mad41s13-in-f14.1e100.net (142.250.200.110): icmp_seq=5 ttl=63 time=24.8 ms
64 bytes from mad41s13-in-f14.1e100.net (142.250.200.110): icmp_seq=6 ttl=63 time=24.8 ms
^C
--- google.com ping statistics ---
6 packets transmitted, 6 received, 0% packet loss, time 5040ms
rtt min/avg/max/mdev = 24.765/36.908/88.463/23.289 ms
$ 


cmd1 && (cmd2 || (cmd3 | testfile)) > (cmd2 || (cmd3 | testfile)) > (cmd2 || (cmd3 | testfile)) 



ls && cat << DLIMITER cm0 cmd1 cmd2 << DELIMITER2 cm0 cm1 || wc -l










salah@salah-VirtualBox:~/minishell$ ./minishell 
Lowa@salah~ ls

[DEBUG] line: ls
CMD_NODE: [ls] 
Lowa@salah~ << ls

[DEBUG] line: << ls
=================================================================
==22263==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x502000002e48 at pc 0x630632453972 bp 0x7ffc3e171e00 sp 0x7ffc3e171df0
READ of size 8 at 0x502000002e48 thread T0
    #0 0x630632453971 in print_nodes parsing/parser.c:44
    #1 0x630632453e1b in parser parsing/parser.c:98
    #2 0x630632452bd1 in main /home/salah/minishell/main.c:87
    #3 0x75262d22a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #4 0x75262d22a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #5 0x6306324523c4 in _start (/home/salah/minishell/minishell+0x23c4) (BuildId: 2190aa2c11d7bec856acd8220ea903b1f2f17c30)

0x502000002e48 is located 8 bytes befor 16-byte region [0x502000002e50,0x502000002e60)
allocated by thread T0 here:
    #0 0x75262d6fbb37 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x630632452e82 in gc_malloc /home/salah/minishell/memo.c:22
    #2 0x6306324574d9 in split_line parsing/split.c:60
    #3 0x630632457bf2 in smart_split parsing/split.c:112
    #4 0x630632453da1 in parser parsing/parser.c:92
    #5 0x630632452bd1 in main /home/salah/minishell/main.c:87
    #6 0x75262d22a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #7 0x75262d22a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #8 0x6306324523c4 in _start (/home/salah/minishell/minishell+0x23c4) (BuildId: 2190aa2c11d7bec856acd8220ea903b1f2f17c30)

SUMMARY: AddressSanitizer: heap-buffer-overflow parsing/parser.c:44 in print_nodes
Shadow bytes around the buggy address:
  0x502000002b80: fa fa fd fd fa fa fd fa fa fa fd fd fa fa fd fa
  0x502000002c00: fa fa 00 fa fa fa 00 04 fa fa fd fa fa fa 00 04
  0x502000002c80: fa fa 06 fa fa fa 06 fa fa fa 00 00 fa fa 00 00
  0x502000002d00: fa fa fd fd fa fa fd fd fa fa 00 00 fa fa 00 00
  0x502000002d80: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
=>0x502000002e00: fa fa 00 00 fa fa 01 fa fa[fa]00 00 fa fa 04 fa
  0x502000002e80: fa fa 00 00 fa fa 00 00 fa fa fd fd fa fa fd fd
  0x502000002f00: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 03 fa
  0x502000002f80: fa fa 00 00 fa fa fa fa fa fa fa fa fa fa fa fa
  0x502000003000: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x502000003080: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==22263==ABORTING
salah@salah-VirtualBox:~/minishell$ 







Lowa@abdoo~     << ps
=================================================================
==11019==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000002b8f at pc 0x5a6f12daad47 bp 0x7ffed92cddf0 sp 0x7ffed92cdde0
READ of size 1 at 0x602000002b8f thread T0
    #0 0x5a6f12daad46 in is_operator_backward parsing/paranths.c:70
    #1 0x5a6f12dafd28 in good_place_for_op parsing/op.c:80
    #2 0x5a6f12db07fd in track_operators parsing/op.c:169
    #3 0x5a6f12da9d8d in parser parsing/parser.c:90
    #4 0x5a6f12da8bf1 in main /home/abdoo/prj/minishell/main.c:87
    #5 0x7ef3cba2814f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #6 0x7ef3cba28208 in __libc_start_main_impl ../csu/libc-start.c:360
    #7 0x5a6f12da83e4 in _start (/home/abdoo/prj/minishell/minishell+0x23e4) (BuildId: 2d8db960efd590d0f1aa0dbf9267efeae4589c06)

0x602000002b8f is located 1 bytes befor 10-byte region [0x602000002b90,0x602000002b9a)
allocated by thread T0 here:
    #0 0x7ef3cbedefdf in __interceptor_malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x5a6f12da8d01 in gc_malloc /home/abdoo/prj/minishell/memo.c:7
    #2 0x5a6f12da8ac8 in main /home/abdoo/prj/minishell/main.c:80
    #3 0x7ef3cba2814f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

SUMMARY: AddressSanitizer: heap-buffer-overflow parsing/paranths.c:70 in is_operator_backward
Shadow bytes around the buggy address:
  0x602000002900: fa fa fd fd fa fa fd fd fa fa fd fd fa fa fd fd
  0x602000002980: fa fa fd fd fa fa fd fd fa fa fd fd fa fa fd fd
  0x602000002a00: fa fa fd fd fa fa fd fd fa fa 00 00 fa fa fd fa
  0x602000002a80: fa fa fd fa fa fa fd fd fa fa fd fa fa fa 00 fa
  0x602000002b00: fa fa 00 04 fa fa fd fd fa fa 00 04 fa fa 00 02
=>0x602000002b80: fa[fa]00 02 fa fa 00 00 fa fa 00 00 fa fa fa fa