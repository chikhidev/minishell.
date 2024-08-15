| Command                                            | Given output                                                                |
|----------------------------------------------------|----------------------------------------------------------------------|
| `ls`                                               | `error.c   libft   Makefile  minishell  string.c`<br>`includes  main.c  memo.c    parsing    tests.txt` |
| `ls -la`                                           | `ls -la: command not found`                                          |
| `ls $HOME`                                         | `1337     Documents  minishell  Pictures  snap       Videos`<br>`Desktop  Downloads  Music      Public    Templates` |
| `unset PATH`<br>`ls $HOME`                         | `bash: ls: No such file or directory`                                |
| `ls *`                                             | `error.c  main.c  Makefile  memo.c  minishell  string.c  tests.txt`<br><br>`includes:`<br>`main.h  memo.h  parsing.h`<br><br>`libft:`<br>`ft_atoi.c     ft_itoa.c                ft_memcpy.o      ft_strchr.o    ft_strncmp.o`<br>`...`<br>`parsing:`<br>`expanding.c  op.c         paranths.c  quoting.c        split.c` |
| `ls ">salah"`                                      | `ls: cannot access '>salah': No such file or directory`              |
| `ls > *`                                           | `bash: *: ambiguous redirect`                                        |
| `ping google.com`                                  | `PING google.com (142.250.200.110) 56(84) bytes of data.`<br>`64 bytes from mad41s13-in-f14.1e100.net (142.250.200.110): icmp_seq=1 ttl=63 time=24.8 ms`<br>`64 bytes from mad41s13-in-f14.1e100.net (142.250.200.110): icmp_seq=2 ttl=63 time=33.8 ms`<br>`...`<br>`^C`<br>`--- google.com ping statistics ---`<br>`6 packets transmitted, 6 received, 0% packet loss, time 5040ms`<br>`rtt min/avg/max/mdev = 24.765/36.908/88.463/23.289 ms` |
| `cmd1 && (cmd2 || (cmd3 | testfile)) > (cmd2 || (cmd3 | testfile)) > (cmd2 || (cmd3 | testfile))` | `...`                                                                |
| `ls && cat << DLIMITER cm0 cmd1 cmd2 << DELIMITER2 cm0 cm1 || wc -l` | `...`                                                                |
| `pwd`                                             | `/current/directory/path`                                            |
| `unset PWD`<br>`env | grep PWD`<br>`cd $PWD`<br>`pwd` | `...`                                                                |
| `echo '''"$Hecho '''"$HOME"'"`                     | `"/nfs/homes/sgouzi"`                                                |
| `echo $\"HOLA\"`                                   | `$HOLA`                                                              |
| `echo $NOT_HING`                                   | `Segmentation fault (core dumped)`                                   |
| `valgrind --track-fds=yes ./minishell`             | `==1407185== Memcheck, a memory error detector`<br>`...`<br>`==1407238== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)` |
| `echo '$\"USER'`                                   | `$USER`                                                              |
| `ls '$\"USER'`                                     | `ls: cannot access '$"USER': No such file or directory`              |
| `cat '$\"USER'`                                    | `cat: '$"USER': No such file or directory`                           |
