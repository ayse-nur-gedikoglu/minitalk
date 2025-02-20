/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agedikog <agedikog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:54:35 by agedikog          #+#    #+#             */
/*   Updated: 2025/02/09 16:54:35 by agedikog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minitalk_bonus.h"
#include "../libft/inc/libft.h"

void	ft_handler(int sa, siginfo_t *info, void *context)
{
	static int	i = 0;
	static int	c = 0;

	(void)context;
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
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;
	pid_t				server_pid;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &ft_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_printf("Error calling sigaction.");
		return (1);
	}
	server_pid = getpid();
	ft_printf("Server PID: %d\n", server_pid);
	while (1)
		pause();
	return (0);
}
