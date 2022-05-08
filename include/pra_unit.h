#pragma once

#include <stdio.h>
#include <stdbool.h>

#define PRA_STRINGIZE_DETAIL(x) #x
#define PRA_STRINGIZE(x) PRA_STRINGIZE_DETAIL(x)
#define PRA_ASSERT(test) do { if (!(test)) { printf("assert: %s:%d: %s: Assertion '%s' failed.\n", __FILE__, __LINE__, __func__, #test); return false; } } while (0)
#define PRA_ASSERT_MESSAGE(message, test) do { if (!(test)) { printf("%s\n", (message)); return false; } } while (0)
#define PRA_RUN_TEST(test) do { bool success = test(); printf("%s:%s\n", #test, success ? "PASS" : "FAIL"); if (!success) return false; } while (0)
