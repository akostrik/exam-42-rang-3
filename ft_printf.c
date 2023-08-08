#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

int	put_digit(long n, int base)
{
	char	*hex = "0123456789abcdef";

	if (n < 0)
		return (write(1, "-", 1) + put_digit(-n, base));
	if (n < base)
		return (write(1, &hex[n], 1));
	return (put_digit(n / base, base) + write(1, &hex[n % base], 1));
}

int	put_str(char *str)
{
	int len = 0;

	if (!str)
		str = "(null)";
	while (*str)
		len += write(1, str++, 1);
	return (len);
}

int	ft_printf(const char *f, ...)
{
	int len = 0;
	va_list	args;

	va_start(args, f);
	while (*f)
	{
		if (*f == '%' && *(f + 1) == 'd')
			len += put_digit((long)va_arg(args, int), 10);
		else if (*f == '%' && *(f + 1) == 'x')
			len += put_digit((long)va_arg(args, unsigned int), 16);
		else if (*f == '%' && *(f + 1) == 's')
			len += put_str(va_arg(args, char *));
		else
			len += write(1, f, 1);
		if (*f == '%' && (*(f + 1) == 'd' || *(f + 1) == 'x' || *(f + 1) == 's'))
			f++;
		f++;
	}
	va_end(args);
	return (len);
}

int main()
{
	int n2 =    printf("%d %d %d %d %d %d %d %d %d", -0, 1, 99, 100, -42, INT_MAX, INT_MIN, INT_MAX+1, INT_MIN-1); printf(" (return %d)\n", n2);
	int n1 = ft_printf("%d %d %d %d %d %d %d %d %d", -0, 1, 99, 100, -42, INT_MAX, INT_MIN, INT_MAX+1, INT_MIN-1); printf(" (return %d)\n", n1);
	printf("\n");
	int n3 =    printf("%x %x %x %x %x %x %x %x %x", -0, 1, 255, 256, -42, INT_MAX, INT_MIN, INT_MAX+1, INT_MIN-1); printf(" (return %d)\n", n3);
	int n4 = ft_printf("%x %x %x %x %x %x %x %x %x", -0, 1, 255, 256, -42, INT_MAX, INT_MIN, INT_MAX+1, INT_MIN-1); printf(" (return %d)\n", n4);
	printf("\n");
	int n5 =    printf("%x %x %x %x %x", 42, LONG_MAX, LONG_MAX + 1, LONG_MIN - 1, LONG_MIN); printf(" (return %d)\n", n5);
	int n6 = ft_printf("%x %x %x %x %x", 42, LONG_MAX, LONG_MAX + 1, LONG_MIN - 1, LONG_MIN); printf(" (return %d)\n", n6);
	printf("\n");
	int n7 =    printf("%s %s %s", NULL, "", "toto"); printf(" (return %d)\n", n7);
	int n8 = ft_printf("%s %s %s", NULL, "", "toto"); printf(" (return %d)\n", n8);
}