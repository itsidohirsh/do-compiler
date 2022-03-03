prog my_prog:
	int num;
	set num = 0;
	
	while (num < 10):
		set num = num + 1;
	done

	int result;
	char i;
	set result = 0;
	set i = '0'

	if (result == 0 && i == 'A'):
		set result = 1 - result;
		set i = '1';
	done
	else:
		set result = 1 - result;
		set i = '0'
	done
done :)
