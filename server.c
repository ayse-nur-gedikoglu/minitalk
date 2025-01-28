#include "minitalk.h"
#include "Libft/libft.h"

void    ft_handler(int sa)
{
    static  int i;
    static  int c;

    i = 0;
    c = 0;
	if (sa == SIGUSR2)
		c = c << 1;
	else if (sa == SIGUSR1)
		c = (c << 1) | 0b00000001;
    i++;
    if (i == 8)
    {
        ft_printf("%c", c);
        i = 0;
        c = 0;
    }
}

int main(void)
{
    struct  sigaction    sa;
    pid_t server_pid;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = ft_handler;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
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
