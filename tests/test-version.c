#include <yaml.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "monolithic_examples.h"



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      yaml_test_version_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
    int major = -1;
    int minor = -1;
    int patch = -1;
    char buf[64];

    yaml_get_version(&major, &minor, &patch);
    sprintf(buf, "%d.%d.%d", major, minor, patch);
    assert(strcmp(buf, yaml_get_version_string()) == 0);

    /* Print structure sizes. */
    printf("sizeof(token) = %ld\n", (long)sizeof(yaml_token_t));
    printf("sizeof(event) = %ld\n", (long)sizeof(yaml_event_t));
    printf("sizeof(parser) = %ld\n", (long)sizeof(yaml_parser_t));

    return 0;
}
