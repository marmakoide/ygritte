/*
 * Alexandre Devert
 * saucats library
 *
 * Tiny unit testing framework
 *
 * Heavily inspired from from www.jera.com 
 */

#ifndef SAUCATS_MINUNIT_H
#define SAUCATS_MINUNIT_H

#ifdef __cplusplus
extern "C" {
#endif



#define mu_assert(test, format_str, ...) \
do { \
  if (!(test)) { \
		fprintf(stderr, "[%s, function %s, line %d] ", __FILE__, __func__, __LINE__); \
		fprintf(stderr, format_str, ##__VA_ARGS__); \
		fprintf(stderr, "\n"); \
		return 1; \
	} \
} while (0)



#define mu_run_test(test) \
do { \
	int failed = test(); \
	mu_tests_run++; \
	if (failed) \
		return failed; \
} while (0)


static unsigned int mu_tests_run;



#ifdef __cplusplus
}
#endif

#endif // SAUCATS_MINUNIT_H
