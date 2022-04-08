prog simple:
	char ch;
	set ch = 1 || 2 && 3 == 4 + 5 * 6 || (7 || 8 && 9 == 10 + 11 * 12);

	int i;
	set i = 0;
	while (i < 10):
		set ch = i + 'A';
		set i = i + 1;
	done
done :)
