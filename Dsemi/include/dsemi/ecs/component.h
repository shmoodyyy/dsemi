#ifndef ECS_COMPONENT_HEADER
#define ECS_COMPONENT_HEADER

#include "dsemi/ecs/ecsdefines.h"

#define ECS_NUM_COMPONENTS dsemi::ecs::icomponent_base::CountComponentTypes()

namespace dsemi {
	namespace ecs {

		struct icomponent_base {
			size_t EntityID = ECS_INVALID_ENTITY;

			static const size_t CountComponentTypes() noexcept {
				return icomponent_base::s_nextComponentID;
			}

		protected:
			static const size_t SetNextID();

		private:
			static size_t s_nextComponentID;
		};


		template<typename T>
		struct icomponent : public icomponent_base {
			using icomponent_base::icomponent_base;
			//virtual inline const size_t GetID() const noexcept override { return ID; }
			static const size_t ID;
			static const size_t size;
		};

		template<typename T>
		const size_t icomponent<T>::ID = icomponent_base::SetNextID();

		template<typename T>
		const size_t icomponent<T>::size = sizeof(T);
	}
}

#endif