#include "../inc/minitalk_bonus.h"
#include "../libft/inc/libft.h"

void    ft_handler(int sa)
{
    static  int i = 0;
    static  int c = 0;

	if (sa == SIGUSR2)
		c = c << 1;
	else if (sa == SIGUSR1)
		c = (c << 1) | 1;
    i++;
    if (i == 8)
    {
        ft_printf("%c", c);
        i = 0;
        c = 0;
    }
}

int main(int ac, char* av[])
{
    struct  sigaction    sa;
    pid_t server_pid;
    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = ft_handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1 
        || sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        ft_printf("Error calling sigaction.");
        return (1);
    }
    server_pid = getpid();
    ft_printf("Server PID:%d\n", server_pid);
    while (1)
        pause();
    return (0);
}
