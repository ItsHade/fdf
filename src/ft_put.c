#include "../include/fdf.h"

void ft_putchar(char c)
{
    write (1, &c, 1);
}

void ft_putstr(char *s)
{
	int i;
	
	i = 0;
	while (s[i])
	{
		write (1, &s[i], 1);
		i++;
	}
}

void ft_putnbr(int nb)
{
    int num;

    if (nb < 0)
    {
        num = -nb;
        ft_putchar('-');
    }
    else
        num = nb;
    if (num <= 9)
        ft_putchar(num + 48);
    else
    {
        ft_putnbr(num / 10);
        ft_putchar(num % 10 + 48);
    }
}