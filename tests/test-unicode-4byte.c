#include <yaml.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#define BUFFER_SIZE 65536

typedef struct {
    char *title;
    char *test;
    char *expected_output_unicode0;  /* expected output with unicode=0 */
    char *expected_output_unicode1;  /* expected output with unicode=1 */
} test_case;

test_case unicode_cases[] = {
    /* ASCII - never escapes in either case */
    {"ascii", "Hello", "Hello\n", "Hello\n"},
    /* 2-byte UTF-8 - escapes without unicode, not with unicode */
    {"2-byte utf-8", "\xc2\xa9", "\"\\xA9\"\n", "\xc2\xa9\n"},    /* © */
    /* 3-byte UTF-8 - escapes without unicode, not with unicode */
    {"3-byte utf-8", "\xe2\x98\x83", "\"\\u2603\"\n", "\xe2\x98\x83\n"},  /* ☃ */
    /* 4-byte UTF-8 - CURRENTLY FAILS */
    {"4-byte start", "\xf0\x90\x80\x80", "\"\\U00010000\"\n", "\xf0\x90\x80\x80\n"},  /* U+10000 */
    {"4-byte emoji", "\xf0\x9f\x90\x9b", "\"\\U0001F41B\"\n", "\xf0\x9f\x90\x9b\n"},   /* U+1F41B 🐛 */
    {"4-byte end", "\xf4\x8f\xbf\xbf", "\"\\U0010FFFF\"\n", "\xf4\x8f\xbf\xbf\n"},     /* U+10FFFF */
    /* Control characters - ALWAYS escape (even with unicode=1) */
    {"bell", "\x07", "\"\\a\"\n", "\"\\a\"\n"},
    {"backspace", "\x08", "\"\\b\"\n", "\"\\b\"\n"},
    {"form feed", "\x0C", "\"\\f\"\n", "\"\\f\"\n"},
    /* Invalid UTF-8 - scalar event initialization fails (cannot be used in scalars) */
    {"invalid-5byte", "\xf5\x80\x80\x80\x80", NULL, NULL},
    {"invalid-ff", "\xff\x80\x80\x80", NULL, NULL},
    {"truncated-4byte", "\xf0\x90", NULL, NULL},
    {"truncated-4byte-mid", "\xf0\x90\x80", NULL, NULL},
    {"invalid-cont1", "\xf0\x20\x80\x80", NULL, NULL},
    {"invalid-cont2", "\xf0\x90\x20\x80", NULL, NULL},
    {"invalid-cont3", "\xf0\x90\x80\x20", NULL, NULL},
    {"overlong-null", "\xf0\x80\x80\x80", NULL, NULL},
    {"overlong-u82", "\xf0\x80\x81\x82", NULL, NULL},
    /* Non-printable characters in YAML - escape as Unicode escapes */
    {"c0-control-u80", "\xc2\x80", "\"\\x80\"\n", "\"\\x80\"\n"},  /* U+0080 escapes as single byte */
    {"c0-control-u9f", "\xc2\x9f", "\"\\x9F\"\n", "\"\\x9F\"\n"},  /* U+009F escapes as single byte */
    {"surrogate-high", "\xed\xa0\x80", "\"\\uD800\"\n", "\"\\uD800\"\n"},  /* U+D800 surrogate */
    {"surrogate-low", "\xed\xbf\xbf", "\"\\uDFFF\"\n", "\"\\uDFFF\"\n"},  /* U+DFFF surrogate */
    {"non-char-fffe", "\xef\xbf\xbe", "\"\\uFFFE\"\n", "\"\\uFFFE\"\n"},  /* U+FFFE non-character */
    {"non-char-ffff", "\xef\xbf\xbf", "\"\\uFFFF\"\n", "\"\\uFFFF\"\n"},  /* U+FFFF non-character */
    /* Context-sensitive cases */
    {"bom-middle", "Hello\xef\xbb\xbfWorld", "\"Hello\\uFEFFWorld\"\n", "\"Hello\\uFEFFWorld\"\n"},  /* BOM escaped as Unicode in middle */
    {"bom-alone", "\xef\xbb\xbf", "\"\\uFEFF\"\n", "\"\\uFEFF\"\n"},  /* BOM escaped as Unicode if alone */
    {NULL, NULL, NULL, NULL}
};

static int test_with_unicode(unsigned char *title, char *data, char *expected, int unicode_enabled) {
    unsigned char buffer[BUFFER_SIZE];
    yaml_emitter_t emitter;
    yaml_event_t event;
    size_t written;

    printf("\t%s, unicode=%d: ", title, unicode_enabled);

    /* If expected is NULL, we expect scalar event initialization to fail */
    if (expected == NULL) {
        memset(&emitter, 0, sizeof(emitter));
        yaml_emitter_initialize(&emitter);
        if (!yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)data, strlen(data), 1, 1, YAML_PLAIN_SCALAR_STYLE)) {
            printf("\tOk\n");
            yaml_emitter_delete(&emitter);
            return 0;
        } else {
            printf("\tFAILED\n");
            printf("\t\tUNEXPECTED SUCCESS: Scalar event initialization succeeded\n");
            yaml_emitter_delete(&emitter);
            return 1;
        }
    }

    memset(&emitter, 0, sizeof(emitter));
    assert(yaml_emitter_initialize(&emitter));
    yaml_emitter_set_output_string(&emitter, buffer, BUFFER_SIZE, &written);
    yaml_emitter_set_unicode(&emitter, unicode_enabled);

    assert(yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING));
    assert(yaml_emitter_emit(&emitter, &event));
    assert(yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1));
    assert(yaml_emitter_emit(&emitter, &event));
    if (!yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)data, strlen(data), 1, 1, YAML_PLAIN_SCALAR_STYLE)) {
        printf("\tFAILED\n");
        printf("\t\tSCALAR EVENT INITIALIZATION FAILED\n");
        yaml_emitter_delete(&emitter);
        return 1;
    }
    assert(yaml_emitter_emit(&emitter, &event));
    assert(yaml_document_end_event_initialize(&event, 1));
    assert(yaml_emitter_emit(&emitter, &event));
    assert(yaml_stream_end_event_initialize(&event));
    assert(yaml_emitter_emit(&emitter, &event));
    buffer[written] = '\0';
    yaml_emitter_delete(&emitter);

    int match = (strcmp(buffer, expected) == 0);
    if (match) {
        printf("\tOk\n");
        return 0;
    } else {
        printf("\tFAILED\n");
        printf("\t\texpected: %s\n", expected);
        printf("\t\tactual: %s\n", buffer);
        return 1;
    }
}

int check_unicode_printable(void)
{
    int failed = 0;
    int k;

    printf("checking unicode printable behavior...\n");

    for (k = 0; unicode_cases[k].test; k++) {
        char *title = unicode_cases[k].title;
        char *test_data = unicode_cases[k].test;
        char *expected_u0 = unicode_cases[k].expected_output_unicode0;
        char *expected_u1 = unicode_cases[k].expected_output_unicode1;

        failed += test_with_unicode(title, test_data, expected_u0, 0);
        failed += test_with_unicode(title, test_data, expected_u1, 1);
    }

    printf("checking unicode printable behavior: %d fail(s)\n", failed);
    return failed;
}

int
main(void)
{
    return check_unicode_printable();
}
