#include "Utility.h"
#include <stdexcept>

using namespace std;

void CheckHR(HRESULT hr)
{
	if (FAILED(hr))
		throw runtime_error("HRESULT is failed value.");
}
