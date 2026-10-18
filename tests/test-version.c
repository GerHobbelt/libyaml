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
    int exp_major = major, exp_minor = minor, exp_patch = patch;

    /* Verify NULL safety in yaml_get_version */
    major = -1; minor = -1; patch = -1;
    yaml_get_version(&major, NULL, NULL);
    assert(major == exp_major);
    yaml_get_version(NULL, &minor, NULL);
    assert(minor == exp_minor);
    yaml_get_version(NULL, NULL, &patch);
    assert(patch == exp_patch);
    yaml_get_version(NULL, NULL, NULL);

    /* Verify defensive NULL handling in core API functions */
    assert(yaml_parser_initialize(NULL) == 0);
    yaml_parser_delete(NULL);
    yaml_parser_set_input_string(NULL, NULL, 0);
    yaml_parser_set_input_file(NULL, NULL);
    yaml_parser_set_input(NULL, NULL, NULL);
    yaml_parser_set_encoding(NULL, YAML_UTF8_ENCODING);
    assert(yaml_emitter_initialize(NULL) == 0);
    yaml_emitter_delete(NULL);
    yaml_emitter_set_output_string(NULL, NULL, 0, NULL);
    yaml_emitter_set_output_file(NULL, NULL);
    yaml_emitter_set_output(NULL, NULL, NULL);
    yaml_emitter_set_encoding(NULL, YAML_UTF8_ENCODING);
    yaml_emitter_set_canonical(NULL, 1);
    yaml_emitter_set_indent(NULL, 2);
    yaml_emitter_set_width(NULL, 80);
    yaml_emitter_set_unicode(NULL, 1);
    yaml_emitter_set_break(NULL, YAML_LN_BREAK);
    yaml_token_delete(NULL);
    assert(yaml_stream_start_event_initialize(NULL, YAML_UTF8_ENCODING) == 0);
    assert(yaml_stream_end_event_initialize(NULL) == 0);
    assert(yaml_document_start_event_initialize(NULL, NULL, NULL, NULL, 1) == 0);
    assert(yaml_document_end_event_initialize(NULL, 1) == 0);
    assert(yaml_alias_event_initialize(NULL, NULL) == 0);
    assert(yaml_scalar_event_initialize(NULL, NULL, NULL, NULL, 0, 1, 1, YAML_PLAIN_SCALAR_STYLE) == 0);
    assert(yaml_sequence_start_event_initialize(NULL, NULL, NULL, 1, YAML_BLOCK_SEQUENCE_STYLE) == 0);
    assert(yaml_sequence_end_event_initialize(NULL) == 0);
    assert(yaml_mapping_start_event_initialize(NULL, NULL, NULL, 1, YAML_BLOCK_MAPPING_STYLE) == 0);
    assert(yaml_mapping_end_event_initialize(NULL) == 0);
    yaml_event_delete(NULL);
    assert(yaml_document_initialize(NULL, NULL, NULL, NULL, 1, 1) == 0);
    yaml_document_delete(NULL);
    assert(yaml_document_get_node(NULL, 0) == NULL);
    assert(yaml_document_get_root_node(NULL) == NULL);
    assert(yaml_document_add_scalar(NULL, NULL, (const yaml_char_t *)"a", -1, YAML_PLAIN_SCALAR_STYLE) == 0);
    assert(yaml_document_add_sequence(NULL, NULL, YAML_BLOCK_SEQUENCE_STYLE) == 0);
    assert(yaml_document_add_mapping(NULL, NULL, YAML_BLOCK_MAPPING_STYLE) == 0);
    assert(yaml_document_append_sequence_item(NULL, 1, 1) == 0);
    assert(yaml_document_append_mapping_pair(NULL, 1, 1, 1) == 0);
    assert(yaml_parser_parse(NULL, NULL) == 0);
    assert(yaml_parser_scan(NULL, NULL) == 0);
    assert(yaml_emitter_emit(NULL, NULL) == 0);
    assert(yaml_emitter_open(NULL) == 0);
    assert(yaml_emitter_close(NULL) == 0);
    assert(yaml_emitter_dump(NULL, NULL) == 0);
    assert(yaml_parser_load(NULL, NULL) == 0);

    /* Print structure sizes. */
    printf("sizeof(token) = %ld\n", (long)sizeof(yaml_token_t));
    printf("sizeof(event) = %ld\n", (long)sizeof(yaml_event_t));
    printf("sizeof(parser) = %ld\n", (long)sizeof(yaml_parser_t));

    return 0;
}
