#include <iostream>
using namespace std;

// A simple registry for caffe commands.
typedef int (*BrewFunction)();
typedef map<string, BrewFunction> BrewMap;
BrewMap g_brew_map;

#define RegisterBrewFunction(func) \
namespace { \
class __Registerer_##func { \
 public: /* NOLINT */ \
  __Registerer_##func() { \
    g_brew_map[#func] = &func; \
  } \
}; \
__Registerer_##func g_registerer_##func; \
}

int test_1()
{
	/*...*/
	return 0;
}
RegisterBrewFunction(test_1)

int test_2()
{
	/*...*/
	return 0;
}
RegisterBrewFunction(test_2)
