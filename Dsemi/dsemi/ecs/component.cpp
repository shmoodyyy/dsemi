#include "Dsemi/ecs/Component.h"

namespace dsemi {
	namespace ecs {
		size_t icomponent_base::s_nextComponentID = 0;

		const size_t icomponent_base::SetNextID() {
			return icomponent_base::s_nextComponentID++;
		}
	}
}
