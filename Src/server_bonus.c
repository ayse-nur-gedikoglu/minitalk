#include "minitalk_bonus.h"
#include "libft/inc/libft.h"

void    send_message_to_client(pid_t client_pid, char* message)
{
    int i;
    int bit;

    i = 0;
    while (message[i])
    {
        bit = 7;
        while (bit >= 0)
        {
            if ((message[i] >> bit) & 1)
                kill(client_pid, SIGUSR1);
            else
                kill(client_pid, SIGUSR2);
            usleep(142);
            bit--;
        }
        i++;
    } 
}

void    ft_handler(int sa, siginfo_t* info, void* context)
{
    static  int i = 0;
    static  int c = 0;
    static pid_t client_pid = 0;

    (void)context;

    if (client_pid == 0)
        client_pid = info->si_pid;
    
	if (sa == SIGUSR2)
		c = c << 1;
	else if (sa == SIGUSR1)
		c = (c << 1) | 1;
    i++;
    if (i == 8)
    {
        if (c == '\0')
        {
            ft_printf("Message received. Sending acknowledgment to client...\n");
            send_message_to_client(client_pid, "Message received!\n");
            client_pid = 0;
        }
        else
        {
            ft_printf("%c\n", c);
            i = 0;
            c = 0;
        }
    }
}

int main(void)
{
    struct  sigaction    sa;
    pid_t server_pid;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = ft_handler;
    sa.sa_sigaction = ft_handler;
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
