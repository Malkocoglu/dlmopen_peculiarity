/*
gcc -O1 -fPIC -rdynamic -shared -o libdll0.so dll0.c
*/

#include "util.h"

#include <unistd.h>

static int dll_oky = -9;
// ...
static void	 *dll_obj = NULL;
static int	(*dll_get)() = NULL;
static int	(*dll_set)(int pi_val) = NULL;

static int init_dll1()
{
	if(dll_oky > 0){return dll_oky;}

	char cwd[1024] = "";
	char nam[1280] = "";

	const char *dir = getcwd(cwd, lenof(cwd));
	// ...
	if(dir == NULL){dll_oky = -5; return dll_oky;}

	snprintf(nam, lenof(nam), "%s/libdll1.so", cwd);

	dll_obj = dlopen(nam, RTLD_NOW);
	// ...
	if(dll_obj == NULL){dll_oky = -1; return dll_oky;}

	GET_SYM(dll_get, "get", dll_obj, dll_oky, -2);
	GET_SYM(dll_set, "set", dll_obj, dll_oky, -3);

	dll_oky = 1; return dll_oky;
}

int job(int pi_job, int pi_value)
{
	const int rv = init_dll1();
	// ...
	if(rv < 1){return rv;}

	if(pi_job < 1)
	{
		return dll_get();
	}else{
		return dll_set(pi_value);
	}
}

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
