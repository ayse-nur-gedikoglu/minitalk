#include "../inc/minitalk.h"
#include "../libft/inc/libft.h"

void	ft_handler(int sig, siginfo_t *info, void *context)
{
	static int i = 0;
	static int c = 0;

	(void)context;
	if (sig == SIGUSR2)
		c = c << 1;
	else if (sig == SIGUSR1)
		c = (c << 1) | 1;

	i++;
	if (i == 7)
	{
		ft_printf("%c", c);
		i = 0;
		c = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction sa;
	pid_t server_pid;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sa.sa_sigaction = ft_handler;
	if (sigaction(SIGUSR1, &sa, NULL) == -1  // 0 
		|| sigaction(SIGUSR2, &sa, NULL) == -1) // 1
	{
		ft_printf("Error calling sigaction.\n");
		return (1);
	}
	server_pid = getpid();
	ft_printf("Server PID: %d\n", server_pid);
	while (1)
		pause();
	return (0);
}
