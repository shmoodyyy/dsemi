#pragma once

#include "Dsemi/Core/Exceptions/DsemiException.h"

#include <sstream>

#ifdef _DEBUG
#define ASSERT(x, msg) if(!(x)) { std::ostringstream oss; oss << "[Info] " << msg << std::endl << std::endl \
                                                              << "[File] " << __FILE__ << std::endl \
                                                              << "[Line] " << __LINE__; \
                                    display_except(oss.str(), "Assert Failure!"); __debugbreak(); }
#else
#define ASSERT(x, msg)
#endif