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







[DEBUG] line: (pwd && wd) << ls
=================================================================
==40775==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000003840 at pc 0x56668473b9ca bp 0x7fff80ede990 sp 0x7fff80ede980
READ of size 8 at 0x602000003840 thread T0
    #0 0x56668473b9c9 in print_nodes parsing/parser.c:44
    #1 0x56668473be30 in parser parsing/parser.c:98
    #2 0x56668473abd1 in main /home/gouzi/Desktop/code 💻/1337/minishell/main.c:87
    #3 0x710ff0629d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #4 0x710ff0629e3f in __libc_start_main_impl ../csu/libc-start.c:392
    #5 0x56668473a3c4 in _start (/home/gouzi/Desktop/code 💻/1337/minishell/minishell+0x23c4)

0x602000003840 is located 0 bytes to the right of 16-byte region [0x602000003830,0x602000003840)
allocated by thread T0 here:
    #0 0x710ff0ab4887 in __interceptor_malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:145
    #1 0x56668473acfb in gc_malloc /home/gouzi/Desktop/code 💻/1337/minishell/memo.c:7
    #2 0x56668474051c in smart_split parsing/split.c:159
    #3 0x566684740090 in smart_split parsing/split.c:120
    #4 0x566684740418 in smart_split parsing/split.c:147
    #5 0x56668473bdb6 in parser parsing/parser.c:92
    #6 0x56668473abd1 in main /home/gouzi/Desktop/code 💻/1337/minishell/main.c:87
    #7 0x710ff0629d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

