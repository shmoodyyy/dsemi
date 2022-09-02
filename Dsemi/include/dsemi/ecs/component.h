#ifndef ECS_COMPONENT_HEADER
#define ECS_COMPONENT_HEADER

#include "Dsemi/ecs/ecs_defines.h"

#define ECS_NUM_COMPONENTS dsemi::ECS::BaseComponent::CountComponentTypes()

namespace dsemi {
	namespace ECS {

		struct BaseComponent {
			size_t EntityID = ECS_INVALID_ENTITY;

			static const size_t CountComponentTypes() noexcept {
				return BaseComponent::s_nextComponentID;
			}

		protected:
			static const size_t SetNextID();

		private:
			static size_t s_nextComponentID;
		};


		template<typename T>
		struct IComponent : public BaseComponent {
			using BaseComponent::BaseComponent;
			//virtual inline const size_t GetID() const noexcept override { return ID; }
			static const size_t ID;
			static const size_t size;
		};

		template<typename T>
		const size_t IComponent<T>::ID = BaseComponent::SetNextID();

		template<typename T>
		const size_t IComponent<T>::size = sizeof(T);
	}
}

#endif