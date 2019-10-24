#ifndef __ROUTE_INPUT_ADAPTER_H__
#define __ROUTE_INPUT_ADAPTER_H__

#include "../route.h"
#include <string>

class RouteInputAdapterImpl : public Route
{
	std::string filename;

public:
	RouteInputAdapterImpl(const char* filename);
	void compute(CallbackFunc);
};

#endif
