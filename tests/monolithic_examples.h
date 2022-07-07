
#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

int yaml_example_deconstructor_alt_main(int argc, const char** argv);
int yaml_example_deconstructor_main(int argc, const char** argv);
int yaml_example_example_reformatter_alt_main(int argc, const char** argv);
int yaml_example_reformatter_main(int argc, const char** argv);
int yaml_run_dumper_main(int argc, const char** argv);
int yaml_run_emitter_test_suite_main(int argc, const char** argv);
int yaml_run_emitter_main(int argc, const char** argv);
int yaml_run_loader_main(int argc, const char** argv);
int yaml_run_parser_test_suite_main(int argc, const char** argv);
int yaml_run_parser_main(int argc, const char** argv);
int yaml_run_scanner_main(int argc, const char** argv);
int yaml_test_reader_main(int argc, const char** argv);
int yaml_test_version_main(int argc, const char** argv);

#ifdef __cplusplus
}
#endif

#endif
