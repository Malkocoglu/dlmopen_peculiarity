/*
gcc -O1 -fPIC -rdynamic -shared -o libdll1.so dll1.c
*/

static int value = 0;

int get()
{
	return value;
}

int set(int pi_value)
{
	value = pi_value;
	// ...
	return value;
}

