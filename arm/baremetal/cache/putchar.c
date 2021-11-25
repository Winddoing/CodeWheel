extern void _uart_putchar(int c);

int putchar(int c)
{
	if (c == '\n') {
		_uart_putchar('\r');
	}
	_uart_putchar(c);
	return 0;
}

void show_msg(char *msg)
{
	char *p;

	p = msg;
	while (*p) {
		putchar(*p);
		p++;
	}
}

