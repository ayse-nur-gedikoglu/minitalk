#include "../inc/minitalk.h"
#include "../libft/inc/libft.h"

volatile sig_atomic_t g_signal_received = 0;

void signal_handler(int signum)
{
    g_signal_received = 1;
    if (signum == SIGUSR1)
        return;
}

void ft_chrtobit(int pid, char *str)
{
    int i;
    int bit;

    i = 0;
    while (str[i])
    {
        bit = 6;
        while (bit >= 0)
        {
            g_signal_received = 0;
            if ((str[i] >> bit) & 1)
                kill(pid, SIGUSR1);
            else
                kill(pid, SIGUSR2);

            while (!g_signal_received)
                pause();
            bit--;
        }
        i++;
    } 
}

int main(int ac, char *av[])
{
    pid_t server_pid;
    char *str;

    if (ac == 3)
    {
        server_pid = ft_atoi(av[1]);
        if (kill(server_pid, 0) == -1 || server_pid > 4194304)
        {
            ft_printf("Invalid PID\n");
            return (1);
        }
        str = av[2];
        
        signal(SIGUSR1, signal_handler);
        signal(SIGUSR2, signal_handler);

        ft_chrtobit(server_pid, str);
        ft_chrtobit(server_pid, "\n");
    }
    else 
        ft_printf("1. arg: ./program name 2. arg: PID, 3. arg: ""message""\n");

    return (0);
}
