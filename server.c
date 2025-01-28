#include "minitalk.h"
#include "ft_printf/ft_printf.h"

//Gelen sinyalleri dinler ve bunları anlamlı bir karakter dizisine (string) dönüştürür.
//Mesaj tamamlandığında bunu ekrana yazdırır.

//handler fonksiyonu olacak, bu fonksiyon sinyalleri dinleyecek ve 
//sinyalleri char'a çevirerek yazdıracak.

void    ft_handler(int signal)
{

}

int main(void)
{
    int server_pid;

    server_pid = getpid();
    ft_printf("Server PID:%d\n", server_pid);
    
}