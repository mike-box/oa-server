#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "regmatch.h"
#include "hs.h"

using namespace std;

int g_matched = 0;

static int eventHandler(unsigned int id, unsigned long long from,
                        unsigned long long to, unsigned int flags, void *ctx) {
    printf("Match for pattern \"%s\" at offset %llu\n", (char *)ctx,to);
	g_matched++;
	return 0;
}

int regMatch(const string & pattern,const string & target){	
	hs_database_t *database = NULL;
	hs_compile_error_t *compile_err = NULL;
	char * contex = (char *)malloc(pattern.size());

	/*build data reg base*/
	memcpy(contex,pattern.c_str(),pattern.size());
    if (hs_compile(pattern.c_str(), HS_FLAG_DOTALL, HS_MODE_BLOCK, NULL, &database,
                   &compile_err) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to compile pattern \"%s\": %s\n",
                pattern.c_str(), compile_err->message);
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
	g_matched = 0;
	if (hs_scan(database, target.c_str(), target.size(), 0, scratch, eventHandler,
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
    return g_matched;
}
