#include "minitalk.h"
#include "libft/libft.h"
//Mesajı bitlere çevir ve bitleri sırayla sinyal olarak sunucuya gönder
void    ft_chrtobit(int pid, char *str)
{
    int i;
    int bit;

    while (str[i])
    {
        bit = 7;
        while (bit <= 0)
        {
            if ((SIGUSR1 >> bit) & 1)
               kill(pid, SIGUSR1);
            else
                kill(pid, SIGUSR2);
            bit--;
            sleep(100);
        }
        i++;
    }
}

int main(int ac, char* av[])
{
    pid_t server_pid;

    if (ac == 3)
    {
        server_pid = ft_atoi(av[1]);
        if (kill(server_pid, 0) == -1)
        ft_printf("Invalid PID\n");
        signal(SIGUSR1, ft_chrtobit); //sigaction kullanılmalı
    }
    else
    {    
        ft_printf("You have provided the argument input incorrectly.\n1. arg: ./program name 2. arg: PID, 3. arg: ""message""\n");
    return (0);
}
