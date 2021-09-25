#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hs.h"

using namespace std;
int matched = 0;

static int eventHandler(unsigned int id, unsigned long long from,
                        unsigned long long to, unsigned int flags, void *ctx) {
    printf("Match for pattern \"%s\" at offset %llu\n", (char *)ctx,to);
	matched++;
	return 0;
}

int regMatch(const char * pattern,int plen,const char * target, int tlen){	
	matched = 0;
	hs_database_t *database = NULL;
	hs_compile_error_t *compile_err = NULL;
	char * contex = (char *)malloc(plen);

	/*build data reg base*/
	memcpy(contex,pattern,plen);
    if (hs_compile(pattern, HS_FLAG_DOTALL, HS_MODE_BLOCK, NULL, &database,
                   &compile_err) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to compile pattern \"%s\": %s\n",
                pattern, compile_err->message);
        hs_free_compile_error(compile_err);
        return -1;
    }

	/*alloc scratch*/
	hs_scratch_t *scratch = NULL;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        hs_free_database(database);
        return -1;
    }

	/* scan */
	if (hs_scan(database, target, tlen, 0, scratch, eventHandler,
                contex) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
        hs_free_scratch(scratch);
        hs_free_database(database);
        return -1;
    }

    /* Scanning is complete, any matches have been handled, so now we just
     * clean up and exit.
     */
    hs_free_scratch(scratch);
    hs_free_database(database);
	free(contex);
    return matched;
}

/*
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern> <target>\n", argv[0]);
        return -1;
    }

    char *pattern = argv[1];
    char *target = argv[2];
	isMatch(pattern,strlen(pattern),target,strlen(target));

	return 0;
}*/

