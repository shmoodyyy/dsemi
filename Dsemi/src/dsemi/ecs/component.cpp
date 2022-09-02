#include "dspch.h"
#include "Dsemi/ecs/Component.h"

namespace dsemi {
	namespace ECS {
		size_t BaseComponent::s_nextComponentID = 0;

		const size_t BaseComponent::SetNextID() {
			return BaseComponent::s_nextComponentID++;
		}
	}
}