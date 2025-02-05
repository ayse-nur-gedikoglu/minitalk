#include "../inc/minitalk_bonus.h"
#include "../libft/inc/libft.h"

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
        pause();
        i++;
    }
}

int main(int ac, char* av[])
{
    pid_t server_pid;
    char* str;

    if (ac == 3)
    {
        server_pid = ft_atoi(av[1]);
        if (kill(server_pid, 0) == -1)
        {
            ft_printf("Invalid PID\n");
            return (1);
        }
        str = av[2];
        ft_chrtobit(server_pid, str);
    }
    else 
        ft_printf("You have provided the argument input incorrectly.\n1. arg: ./program name 2. arg: PID, 3. arg: ""message""\n");
    return (0);
}
