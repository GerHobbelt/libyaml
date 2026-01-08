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
    {NULL, NULL, NULL, NULL}
};

int check_unicode_printable(void)
{
    yaml_emitter_t emitter;
    yaml_document_t document;
    yaml_event_t event;
    unsigned char buffer[BUFFER_SIZE];
    size_t written;
    int failed = 0;
    int k;
    
    printf("checking unicode printable behavior...\n");
    
    for (k = 0; unicode_cases[k].test; k++) {
        char *title = unicode_cases[k].title;
        char *test_data = unicode_cases[k].test;
        char *expected_u0 = unicode_cases[k].expected_output_unicode0;
        char *expected_u1 = unicode_cases[k].expected_output_unicode1;
        char *actual_u0, *actual_u1;
        
        printf("\t%s:\n", title);
        
        /* Test with unicode=0 */
        memset(&emitter, 0, sizeof(emitter));
        assert(yaml_emitter_initialize(&emitter));
        yaml_emitter_set_output_string(&emitter, buffer, BUFFER_SIZE, &written);
        yaml_emitter_set_unicode(&emitter, 0);
        
        yaml_event_t event;
        assert(yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING));
        assert(yaml_emitter_emit(&emitter, &event));
        assert(yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1));
        assert(yaml_emitter_emit(&emitter, &event));
        assert(yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)test_data, strlen(test_data), 1, 1, YAML_PLAIN_SCALAR_STYLE));
        assert(yaml_emitter_emit(&emitter, &event));
        assert(yaml_document_end_event_initialize(&event, 1));
        assert(yaml_emitter_emit(&emitter, &event));
        assert(yaml_stream_end_event_initialize(&event));
        assert(yaml_emitter_emit(&emitter, &event));
        buffer[written] = '\0';
        actual_u0 = strdup((char*)buffer);
        
        yaml_emitter_delete(&emitter);
        
        /* Test with unicode=1 */
        memset(&emitter, 0, sizeof(emitter));
        assert(yaml_emitter_initialize(&emitter));
        yaml_emitter_set_output_string(&emitter, buffer, BUFFER_SIZE, &written);
        yaml_emitter_set_unicode(&emitter, 1);
        
        assert(yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING));
        assert(yaml_emitter_emit(&emitter, &event));
        assert(yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 1));
        assert(yaml_emitter_emit(&emitter, &event));
        assert(yaml_scalar_event_initialize(&event, NULL, NULL, (yaml_char_t*)test_data, strlen(test_data), 1, 1, YAML_PLAIN_SCALAR_STYLE));
        assert(yaml_emitter_emit(&emitter, &event));
        assert(yaml_document_end_event_initialize(&event, 1));
        assert(yaml_emitter_emit(&emitter, &event));
        assert(yaml_stream_end_event_initialize(&event));
        assert(yaml_emitter_emit(&emitter, &event));
        buffer[written] = '\0';
        actual_u1 = strdup((char*)buffer);
        
        yaml_emitter_delete(&emitter);
        
        /* Validate both outputs */
        int u0_match = (strcmp(actual_u0, expected_u0) == 0);
        int u1_match = (strcmp(actual_u1, expected_u1) == 0);
        
        if (!u0_match || !u1_match) {
            printf("\t\t- MISMATCH DETECTED:\n");
            printf("\t\t    unicode=0:\n");
            printf("\t\t      expected (hex): ");
            for (int i = 0; i < strlen(expected_u0); i++) {
                printf("%02X ", (unsigned char)expected_u0[i]);
            }
            printf("\n");
            printf("\t\t      actual (hex):   ");
            for (int i = 0; i < strlen(actual_u0); i++) {
                printf("%02X ", (unsigned char)actual_u0[i]);
            }
            printf("\n");
            printf("\t\t    unicode=1:\n");
            printf("\t\t      expected (hex): ");
            for (int i = 0; i < strlen(expected_u1); i++) {
                printf("%02X ", (unsigned char)expected_u1[i]);
            }
            printf("\n");
            printf("\t\t      actual (hex):   ");
            for (int i = 0; i < strlen(actual_u1); i++) {
                printf("%02X ", (unsigned char)actual_u1[i]);
            }
            printf("\n");
            
            failed++;
        }
        else {
            printf("\t\t+ ");
        }
        
        free(actual_u0);
        free(actual_u1);
    }
    
    printf("checking unicode printable behavior: %d fail(s)\n", failed);
    return failed;
}

int
main(void)
{
    return check_unicode_printable();
}
