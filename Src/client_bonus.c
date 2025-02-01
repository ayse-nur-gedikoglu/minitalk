#include "minitalk_bonus.h"
#include "libft/inc/libft.h"

void ft_client_handler(int sa)
{
    static int i = 0;
    static int c = 0;

    if (sa == SIGUSR2)
        c = c << 1;
    else if (sa == SIGUSR1)
        c = (c << 1) | 1;

    i++;
    if (i == 8)
    {
        if (c == '\0')
            ft_printf("Acknowledgment received from server!\n");
        else
            ft_printf("%c\n", c);
        i = 0;
        c = 0;
    }
}

void    ft_chrtobit(int pid, char *str)
{
    int i;
    int bit;

    i = 0;
    while (str[i])
    {
        bit = 7;
        while (bit >= 0)
        {
            if ((str[i] >> bit) & 1)
            	kill(pid, SIGUSR1);
            else
            	kill(pid, SIGUSR2);
            bit--;
            usleep(142);
        }
        i++;
    }
    bit = 7;
        bit = 7;
    while (bit >= 0)
    {
        kill(pid, SIGUSR2);
        usleep(142);
        bit--;
    }
}

int main(int ac, char* av[])
{
    pid_t server_pid;
    char* str;
    struct sigaction sa;
    
    if (ac == 3)
    {
        server_pid = ft_atoi(av[1]);
        if (kill(server_pid, 0) == -1)
        {
            ft_printf("Invalid PID\n");
            return (1);
        }
        str = av[2];

        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sa.sa_handler = ft_client_handler;
        sigaction(SIGUSR1, &sa, NULL);
        sigaction(SIGUSR2, &sa, NULL);

        ft_chrtobit(server_pid, str);

        while (1)
            pause();
    }
    else 
        ft_printf("You have provided the argument input incorrectly.\n1. arg: ./program name 2. arg: PID, 3. arg: ""message""\n");
    return (0);
}
