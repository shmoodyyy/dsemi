#pragma once
#include <vector>
#include "dsemi/core/layer.h"

namespace dsemi {
	class layer_stack
	{
	public:
		layer_stack();
		~layer_stack();

		void push_layer(layer* Layer);
		void push_overlay(layer* overlay);
		void pop_layer(layer* Layer);
		void pop_overlay(layer* overlay);

		std::vector<layer*>::iterator begin() { return _layers.begin(); }
		std::vector<layer*>::iterator end() { return _layers.end(); }

	private:
		std::vector<layer*> _layers;
		std::vector<layer*>::iterator _layer_iterator;
	};
}