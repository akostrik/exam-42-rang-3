#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
/*
int ft_strlen(const char *str) {
    int len = 0;
    while (str[len])
        len++;
    return len;
}

void ft_putstr(char *str) {
    write(1, str, ft_strlen(str));
}

void ft_putchar(char c) {
    write(1, &c, 1);
}

char *ft_itoa(int num) {
    int sign = 1;
    if (num < 0) {
        sign = -1;
        num = -num;
    }
    
    int temp = num;
    int length = 0;
    while (temp > 0) {
        temp /= 10;
        length++;
    }
    
    if (num == 0)
        length = 1;
    
    char *str = (char *)malloc((length + 1) * sizeof(char));
    if (!str)
        return NULL;
    
    str[length] = '\0';
    length--;
    
    while (num > 0) {
        str[length] = '0' + (num % 10);
        num /= 10;
        length--;
    }
    
    if (sign == -1)
        str[0] = '-';
    
    return str;
}

char *ft_itoa_hex(int num) {
    char *hex_chars = "0123456789abcdef";
    
    char *str = (char *)malloc(9 * sizeof(char)); // Assuming 32-bit int
    if (!str)
        return NULL;
    
    str[8] = '\0';
    
    for (int i = 7; i >= 0; i--) {
        int hex_digit = num & 0xF;
        str[i] = hex_chars[hex_digit];
        num >>= 4;
    }
    
    return str;
}

int ft_pr_gpt(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    int written_chars = 0;
    while (*format) {
        if (*format == '%') {
            format++;
            if (*format == 's') {
                char *str = va_arg(args, char *);
                if (str == NULL)
                    str = "(null)";
                ft_putstr(str);
                written_chars += ft_strlen(str);
            }
            else if (*format == 'd') {
                int num = va_arg(args, int);
                char *num_str = ft_itoa(num);
                ft_putstr(num_str);
                free(num_str);
            }
            else if (*format == 'x') {
                int num = va_arg(args, int);
                char *hex_str = ft_itoa_hex(num);
                ft_putstr(hex_str);
                free(hex_str);
            }
        }
        else {
            ft_putchar(*format);
            written_chars++;
        }
        
        format++;
    }
    va_end(args);
    return written_chars;
}
*/
int	put_number(long long n, int base)
{
	char	*hex = "0123456789abcdef";
	int 	len = 0;

	if (n < 0)
	{
		if (write(1, "-", 1) < 0)
			return (-1);
		len = put_number(-n, base);	
		if (len < 0)
			return (-1);
		return (1 + len);
	}
	if (n < base)
	{
		if (write(1, &hex[n], 1) < 0)
			return (-1);
		return (1);
	}
	len = put_number(n / base, base);
	if (len < 0)
		return (-1);
	if (write(1, &hex[n % base], 1) < 0)
		return (-1);
	return (len + 1);
}

int	put_str(char *str)
{
	int len = 0;

	if (!str)
		str = "(null)";
	while (*str)
	{
		if (write(1, str++, 1) < 0)
			return (-1);
		len ++;
	}
	return (len);
}

int	ft_printf(const char *f, ...)
{
	int len = 0;
	int len0 = 0;
	va_list	args;

	if (f == NULL)                              ////
		return (-1);
	va_start(args, f);
	while (*f)
	{
		// if (*f == '%' && *(f + 1) != 'd' && *(f + 1) != 'x' && *(f + 1) != 's')
		// {
		// 	va_end(args);
		//	return (-1);
		//}
		if (*f == '%' && *(f + 1) == 'd')
			len0 = put_number((long long)va_arg(args, int), 10);
		else if (*f == '%' && *(f + 1) == 'x')
			len0 = put_number((long long)va_arg(args, unsigned int), 16);
		else if (*f == '%' && *(f + 1) == 's')
			len0 = put_str(va_arg(args, char *));
		else
			len0 = write(1, f, 1);
		if (len0 < 0)
		{
			va_end(args);
			return (-1);
		}
		len += len0;
		if (*f == '%' && (*(f + 1) == 'd' || *(f + 1) == 'x' || *(f + 1) == 's'))
			f++;
		f++;
	}
	va_end(args);
	return (len);
}


int main(){
	int n;
	
	n =    printf("%d %d %d %d %d %d %d %d", 0, 1, -1, 99, 100, 101, -42, 42+2); printf(" (return %d)\n", n);
	n = ft_printf("%d %d %d %d %d %d %d %d", 0, 1, -1, 99, 100, 101, -42, 42+2); printf(" (return %d)\n", n);

	n =    printf("%d %d %d %d %d %d", INT_MAX, INT_MAX+1, INT_MIN, INT_MIN-1, UINT_MAX, UINT_MAX + 1); printf(" (return %d)\n", n);
	n = ft_printf("%d %d %d %d %d %d", INT_MAX, INT_MAX+1, INT_MIN, INT_MIN-1, UINT_MAX, UINT_MAX + 1); printf(" (return %d)\n", n);

	n =   printf("%d %d %d %d %d %d", ULONG_MAX, ULONG_MAX + 1, LLONG_MAX, LLONG_MAX + 1, LLONG_MIN - 1, LLONG_MIN); printf(" (return %d)\n", n);
	n = ft_printf("%d %d %d %d %d %d", ULONG_MAX, ULONG_MAX + 1, LLONG_MAX, LLONG_MAX + 1, LLONG_MIN - 1, LLONG_MIN); printf(" (return %d)\n", n);

	n =    printf("%x %x %x %x %x %x %x %x", 0, 1, -1, 255, 256, 257, -42, 42+2); printf(" (return %d)\n", n);
	n = ft_printf("%x %x %x %x %x %x %x %x", 0, 1, -1, 255, 256, 257, -42, 42+2); printf(" (return %d)\n", n);

	n =    printf("%x %x %x %x %x %x %x %x %x %x", INT_MAX, INT_MAX+1, INT_MIN, INT_MIN-1, ULONG_MAX, ULONG_MAX + 1, LLONG_MAX, LLONG_MAX + 1, LLONG_MIN - 1, LLONG_MIN); printf(" (return %d)\n", n);
	n = ft_printf("%x %x %x %x %x %x %x %x %x %x", INT_MAX, INT_MAX+1, INT_MIN, INT_MIN-1, ULONG_MAX, ULONG_MAX + 1, LLONG_MAX, LLONG_MAX + 1, LLONG_MIN - 1, LONG_MIN); printf(" (return %d)\n", n);

	n =    printf("%s %s %s %s %s %s %s %s", NULL, "", "\0", "toto", " ", 0, "\n", "\t"); printf(" (return %d)\n", n);
	n = ft_printf("%s %s %s %s %s %s %s %s", NULL, "", "\0", "toto", " ", 0, "\n", "\t"); printf(" (return %d)\n", n);

//	n =    printf(NULL); printf(" (return %d)\n", n);
//	n = ft_printf(NULL); printf(" (return %d)\n", n);

//	n =    printf(NULL, 42); printf(" (return %d)\n", n);
//	n = ft_printf(NULL, 42); printf(" (return %d)\n", n6);

	n =    printf(""); printf(" (return %d)\n", n);
	n = ft_printf(""); printf(" (return %d)\n", n);

	n =    printf("", 42); printf(" (return %d)\n", n);
	n = ft_printf("", 42); printf(" (return %d)\n", n);

	n =    printf("\0"); printf(" (return %d)\n", n);
	n = ft_printf("\0"); printf(" (return %d)\n", n);

	n =    printf("%d %x %s"); printf(" (return %d)\n", n);
	n = ft_printf("%d %x %s"); printf(" (return %d)\n", n);

	n =    printf("%d %d %d", NULL, "toto", 99999999999999999999); printf(" (return %d)\n", n);
	n = ft_printf("%d %d %d", NULL, "toto", 99999999999999999999); printf(" (return %d)\n", n);

	n=    printf("%x %x %x", NULL, "toto", 99999999999999999999); printf(" (return %d)\n", n);
	n = ft_printf("%x %x %x", NULL, "toto", 99999999999999999999); printf(" (return %d)\n", n);

	n =    printf("test %j", 42); printf(" (return %d)\n", n);
	n = ft_printf("test %j", 42); printf(" (return %d)\n", n);

	n =    printf("test %z", 42); printf(" (return %d)\n", n);
	n = ft_printf("test %z", 42); printf(" (return %d)\n", n);

	n =    printf("test %", 42); printf(" (return %d)\n", n);
	n = ft_printf("test %", 42); printf(" (return %d)\n", n);
}
