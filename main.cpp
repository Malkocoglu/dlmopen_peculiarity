#include <string.h>
#include <unistd.h>

/*
gcc -O1 -fPIC -rdynamic -shared -o libdll0.so dll0.c
gcc -O1 -fPIC -rdynamic -shared -o libdll1.so dll1.c
gcc -O1 -o SO_test main.cpp
*/

#include "util.h"

typedef struct _S_dll{

	void	 *obj;
	int		(*get)();
	int		(*set)(int pi_val);
	int		(*job)(int pi_job, int pi_val);
	// ...
	int		val0; // for DLL0
	int		val1; // for DLL1
	// ...
	int		idx;
	int		oky;

}S_dll;

static S_dll G_dll[4] = {{.idx = 0}, {.idx = 1}, {.idx = 2}, {.idx = 3}};

static int load_syms(S_dll &px_dll)
{
	if(px_dll.obj == NULL)
	{
		px_dll.oky = -1; return px_dll.oky;
	}

	GET_SYM(px_dll.get, "get", px_dll.obj, px_dll.oky, -2);
	GET_SYM(px_dll.set, "set", px_dll.obj, px_dll.oky, -3);
	GET_SYM(px_dll.job, "job", px_dll.obj, px_dll.oky, -4);

	Print("load_syms OK :: handle=<%p> get=<%p> set=<%p> job=<%p>", px_dll.obj, (void*)px_dll.get, (void*)px_dll.set, (void*)px_dll.job);
	// ...
	px_dll.oky = +1; return px_dll.oky;
}

int main(int argc, char *argv[])
{
	char method = 'N'; // NEWLM

	char CW_dir[1024] = "";

	G_dll[0].val0 = 13; G_dll[0].val1 = 23;
	G_dll[1].val0 = 15; G_dll[1].val1 = 25;
	G_dll[2].val0 = 17; G_dll[2].val1 = 27;
	G_dll[3].val0 = 19; G_dll[3].val1 = 29;

	const char *dir = getcwd(CW_dir, lenof(CW_dir));
	// ...
	if(dir == NULL)
	{
		Print("getcwd() returned NULL , <%s>", CW_dir);
		// ...
		return 0;
	}

	if(argc > 1)
	{
		if(strcmp(argv[1], "copy") == 0)
		{
			method = 'C'; // COPY
		}
	}

	Print("%s", "");

	if(method == 'N')
	{
		char nam[1280] = "";
		// ...
		snprintf(nam, lenof(nam), "%s/libdll0.so", CW_dir);

		Print("Using the dlmopen function (%s) with LM_ID_NEWLM\r\n", nam);

		for(size_t i = 0; i < cntof(G_dll); i++)
		{
			S_dll &dll = G_dll[i];

			dll.obj = dlmopen((Lmid_t)LM_ID_NEWLM, nam, RTLD_NOW);
			// ...
			if(dll.obj == NULL)
			{
				Print("Error dlmopen(<%s>)", nam);
			}
			else
			{
				const int r0 = load_syms(dll);
				// ...
				if(r0 < 1)
				{
					Print("load_syms returned error (%d) for <%s>", r0, nam);
				}
			}
		}
	}
	else
	{
		Print("Using the regular dlopen function (%s/libdll0_X.so)\r\n", CW_dir);

		for(size_t i = 0; i < cntof(G_dll); i++)
		{
			S_dll &dll = G_dll[i];

			char nam[1280] = "";

			snprintf(nam, lenof(nam), "%s/libdll0_%zu.so", CW_dir, i);

			dll.obj = dlopen(nam, RTLD_NOW);
			// ...
			if(dll.obj == NULL)
			{
				Print("Error dlopen(<%s>)", nam);
			}
			else
			{
				const int r0 = load_syms(dll);
				// ...
				if(r0 < 1)
				{
					Print("load_syms returned error (%d) for <%s>", r0, nam);
				}
			}
		}
	}

	Print("%s", "\r\nCall these DLL0 functions in sequence :: get, set, get");

	for(size_t i = 0; i < cntof(G_dll); i++)
	{
		S_dll &dll = G_dll[i];

		if(dll.oky > 0)
		{
			const int r0 = dll.get();
			const int r1 = dll.set(dll.val0);
			const int r2 = dll.get();

			Print("DLL0 :: get_r0(%2d) , set_r1(%2d) , get_r2(%2d)", r0, r1, r2);
		}
	}

	Print("%s", "\r\nCheck the DLL0 expected/returned values...");

	for(size_t i = 0; i < cntof(G_dll); i++)
	{
		S_dll &dll = G_dll[i];

		if(dll.oky > 0)
		{
			const int r0 = dll.get();

			Print("DLL0 :: val0(%2d) get_r0(%2d) okey(%d)", dll.val0, r0, (dll.val0 == r0));
		}
	}

	Print("%s", "\r\n!!! HERE COMES THE EXCITING PART !!!");

	Print("%s", "\r\nCall these DLL0 functions that in turn calls DLL1 functions in sequence :: job->get, job->set, job->get");

	for(size_t i = 0; i < cntof(G_dll); i++)
	{
		S_dll &dll = G_dll[i];

		if(dll.oky > 0)
		{
			const int r0 = dll.job(0, -1);
			const int r1 = dll.job(1, dll.val1);
			const int r2 = dll.job(0, -1);

			Print("DLL1 :: get_r0(%2d) , set_r1(%2d) , get_r2(%2d)", r0, r1, r2);
		}
	}

	Print("%s", "\r\nCheck the DLL1 expected/returned values...");

	for(size_t i = 0; i < cntof(G_dll); i++)
	{
		S_dll &dll = G_dll[i];

		if(dll.oky > 0)
		{
			const int r0 = dll.job(0, -1);

			Print("DLL1 :: val1(%2d) get_r0(0, -1, %2d) okey(%d)", dll.val1, r0, (dll.val1 == r0));
		}
	}

	const int pid = getpid();
	// ...
	Print("%s", "\r\nDemonstration finished, you can exit now...");
	Print("\r\nYou can also observe /proc/%d/map_files OR /proc/%d/task/%d/maps on another shell", pid, pid, pid);

	while(true)
	{
		sleep(1);
	}

	return 0;
}
