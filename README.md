## ft_printf
Write a function named ft_printf that will mimic the real printf but it will manage only the following conversions:
```
s (string)
d (decimal) 
x (lowercase hexademical)
```
Allowed functions : malloc, free, write, va_start, va_arg, va_copy and va_end

Prototype : int ft_printf(const char *, ... )

Examples
```
call: ft_printf("%s\n", "toto")
out: toto$

call: ft_printf("Magic %s is %d", "number", 42)
out: Magic number is 42%

call: ft_printf("Hexadecimal for %d is %x\n", 42, 42)
out: Hexadecimal for 42 is 2a$
```

## get_next_line
Allowed Functions : read, free, malloc

Prototype : char *get_next_line(int fd);

Return :
1) returns a line that has been read. "Line that has been read" = a succesion of 0 to n characters that end with '\n' or EOF.
2) The line should be returned including the '\n' in case there is one at the end of the line read.
3) When you've reached the EOF, you must store the current buffer in a char * and return it.
4) If the buffer is empty you must return NULL.
5) In case of error return NULL.
6) In case of not returning NULL, the pointer should be free-able.
7) When you've reached the EOF, your function should keep 0 memory allocated with malloc, except the line that has been returned.

Your program will be compiled with the flag -D BUFFER_SIZE=xx, which has to be used as the buffer size for the read calls in your functions.

Your function must be memory leak free.

Calling your function get_next_line in a loop will therefore allow you to read the text avaiable on a file descriptor one line at a time until the end of the text, no matter the size or either the text or one of its lines.

Make sure that your function behaves well when it reads from a file, from the standard output, from a redirection, etc...

No call to another function will be done on the file descriptor between 2 calls of get_next_line.

We consider that get_next_line has an undefined behaviour when reading from a binary file.