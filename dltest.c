#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <dlfcn.h>

void usage(int argc, const char *argv[]) {
	printf("Usage: %s library [-flag...] [--] [symbol...]\n", argc > 0 ? argv[0] : "dltest");
}

int parse_args(int argc, const char *argv[], const char **lib, int *flags, int *symind) {
	if (argc < 2) {
		fprintf(stderr, "*** error: missing library argument\n");
		return 0;
	}
	*lib = argv[1];

	int i = 2;
	int f = 0;
	int hasflag = 0;
	for (; i < argc; ++ i) {
		const char *arg = argv[i];
		if (arg[0] != '-') break;

		if (strcmp(arg,"--") == 0) {
			++ i;
			break;
		}
		else if (strcasecmp(arg,"-RTLD_LAZY") == 0 || strcasecmp(arg,"-LAZY") == 0 ) {
			f |= RTLD_LAZY;
			hasflag = 1;
		}
		else if (strcasecmp(arg,"-RTLD_NOW") == 0 || strcasecmp(arg,"-NOW") == 0 ) {
			f |= RTLD_NOW;
			hasflag = 1;
		}
		else if (strcasecmp(arg,"-RTLD_GLOBAL") == 0 || strcasecmp(arg,"-GLOBAL") == 0 ) {
			f |= RTLD_GLOBAL;
			hasflag = 1;
		}
		else if (strcasecmp(arg,"-RTLD_LOCAL") == 0 || strcasecmp(arg,"-LOCAL") == 0 ) {
			f |= RTLD_LOCAL;
			hasflag = 1;
		}
#ifdef RTLD_NODELETE
		else if (strcasecmp(arg,"-RTLD_NODELETE") == 0 || strcasecmp(arg,"-NODELETE") == 0 ) {
			f |= RTLD_NODELETE;
			hasflag = 1;
		}
#endif
#ifdef RTLD_NOLOAD
		else if (strcasecmp(arg,"-RTLD_NOLOAD") == 0 || strcasecmp(arg,"-NOLOAD") == 0 ) {
			f |= RTLD_NOLOAD;
			hasflag = 1;
		}
#endif
#ifdef RTLD_DEEPBIND
		else if (strcasecmp(arg,"-RTLD_DEEPBIND") == 0 || strcasecmp(arg,"-DEEPBIND") == 0 ) {
			f |= RTLD_DEEPBIND;
			hasflag = 1;
		}
#endif
		else {
			fprintf(stderr, "*** error: unknown option '%s'\n", arg);
			return 0;
		}
	}

	if (hasflag) {
		*flags = f;
	}
	else {
		*flags = RTLD_LAZY;
	}
	
	*symind = i;

	return 1;
}

int main(int argc, const char *argv[]) {
	int flags = 0;
	int symind = 0;
	const char *lib = NULL;
	
	if (!parse_args(argc, argv, &lib, &flags, &symind)) {
		usage(argc, argv);
		return 1;
	}

	void *handle = dlopen(lib, flags);

	if (!handle) {
		fprintf(stderr, "*** error: %s: %s\n", lib, dlerror());
		return 1;
	}

	// clear errors
	dlerror();
	const char *msg = NULL;

	for (; symind < argc; ++ symind) {
		const char *name = argv[symind];
		void *sym = dlsym(handle, name);

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "*** error: %s: %s\n", name, msg);
		}
		else {
			printf("%s: 0x%zx\n", name, (size_t)sym);
		}
	}

	dlclose(handle);

	return 0;
}
