
#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

int yaml_example_deconstructor_alt_main(int argc, const char** argv);
int yaml_example_deconstructor_main(int argc, const char** argv);
int yaml_example_example_reformatter_alt_main(int argc, const char** argv);
int yaml_example_reformatter_main(int argc, const char** argv);
int yaml_run_comment_roundtrip_tests_main(int argc, const char** argv);
int yaml_run_dumper_main(int argc, const char** argv);
int yaml_run_emitter_test_suite_main(int argc, const char** argv);
int yaml_run_emitter_main(int argc, const char** argv);
int yaml_run_loader_main(int argc, const char** argv);
int yaml_run_parser_test_suite_main(int argc, const char** argv);
int yaml_run_parser_main(int argc, const char** argv);
int yaml_run_scanner_main(int argc, const char** argv);
int yaml_test_bom_main(void);
int yaml_test_CVE_2024_35325_main(void);
int yaml_test_CVE_2024_35326_main(void);
int yaml_test_CVE_2024_35328_main(void);
int yaml_test_CVE_2024_35329_main(void);
int yaml_test_CVE_tm_emit_main(void);
int yaml_test_CVE_tm_poc_main(void);
int yaml_test_directive_inheritance_main(void);
int yaml_test_emitter_main(void);
int yaml_test_load_error_main(void);
int yaml_test_nesting_main(void);
int yaml_test_reader_main(void);
int yaml_test_realloc_main(void);
int yaml_test_tag_uri_main(void);
int yaml_test_unicode_4byte_main(void);
int yaml_test_version_main(void);

#ifdef __cplusplus
}
#endif

#endif
