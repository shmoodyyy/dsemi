#ifndef GRAPHCIS_INDEX_BUFFER_H
#define GRAPHCIS_INDEX_BUFFER_H
#include "dsemi/graphics/api_include.h"
#include "dsemi/graphics/bindable.h"

namespace dsemi 
{
	namespace graphics 
	{
		class index_buffer : public Bindable
		{
			friend class Device;
		public:
			index_buffer() 
				: Bindable()
                , _dx_buffer(nullptr) 
			{}

			virtual void bind() override;

		private:
			ID3D11Buffer* _dx_buffer;
		};
	}
}

#endif
