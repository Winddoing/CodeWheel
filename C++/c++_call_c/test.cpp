#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

	void hello();

#ifdef __cplusplus
}
#endif

int main()
{
	hello();

	return 0;
}
