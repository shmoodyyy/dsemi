#pragma once
#include <unordered_map>
#include "Dsemi/Math/vector2.h"

namespace dsemi {

	class Input
	{
		friend class window;
		friend class application;
	public:
		static Input* Instance();

		bool GetKeyDown(unsigned int keycode);
		bool GetMouseButton(unsigned int mousebutton);
		bool GetButton(const std::string& button) const;

		void MapButton(const std::string& button, int key);

		static vector2i GetMousePos() noexcept { return s_mousePos; }
		// Replace with buffered shit
		static vector2i GetMouseDelta() noexcept { return s_mouseDelta;	}

		static bool IsMouseRawInput() noexcept { return s_bRawInput; }
		static bool SetMouseRawInput(bool bEnable) noexcept { s_bRawInput = bEnable; }

	private:
		static Input* m_Instance;
		std::unordered_map<std::string, unsigned int> m_mapButtons;
		std::unordered_map<unsigned int, bool> m_mapPressedKeys;

		void OnKeyDown(unsigned int keycode);
		void OnKeyUp(unsigned int keycode);
		void ClearKeys();
		static void ResetMouseDelta();

		// Singleton code safety
		Input();
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
	protected:
		bool Init();

		static bool s_bRawInput;
		static vector2i s_mousePos, s_mouseDelta;
	};
}