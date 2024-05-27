#include <unistd.h>
#include <libft.h>
int main(int ac, char **av, char **env)
{
	execve("/bin/ls", ft_split("ls *.c", ' '), env);
	return 0;
}
