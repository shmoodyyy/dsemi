#include "dspch.h"

#include "Dsemi/Core/LayerStack.h"



namespace dsemi {
	layer_stack::layer_stack()
	{
		_layer_iterator = _layers.begin();
	}

	layer_stack::~layer_stack()
	{
		for (layer* layer : _layers)
		{
			layer->on_detach();
			delete layer;
		}
	}

	void layer_stack::push_layer(layer* layer)
	{
		_layer_iterator = _layers.emplace(_layer_iterator, layer);
		layer->on_attach();
	}

	void layer_stack::push_overlay(layer* overlay)
	{
		_layers.emplace_back(overlay);
		overlay->on_attach();
	}

	void layer_stack::pop_layer(layer* layer)
	{
		auto it = std::find(_layers.begin(), _layers.end(), layer);
		if (it != _layers.end())
		{
			(*it)->on_detach();
			_layers.erase(it);
			_layer_iterator--;
		}
	}

	void layer_stack::pop_overlay(layer* overlay)
	{
		auto it = std::find(_layers.begin(), _layers.end(), overlay);
		if (it != _layers.end())
		{
			(*it)->on_detach();
			_layers.erase(it);
		}
	}
}