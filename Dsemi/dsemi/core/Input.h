#pragma once
#include <unordered_map>
#include <string>
#include "Dsemi/Math/vector2.h"

namespace dsemi {

	class Input
	{
		friend class Window;
		friend class application;
	public:
		static Input* Instance();

		bool GetKeyDown(unsigned int keycode);
		bool GetMouseButton(unsigned int mousebutton);
		bool GetButton(const std::string& button) const;

		void MapButton(const std::string& button, int key);

		static Vector2i GetMousePos() { return s_mousePos; }
		// Replace with buffered shit
		static Vector2i GetMouseDelta() { return s_mouseDelta;	}

		static bool IsMouseRawInput() { return s_bRawInput; }
		static bool SetMouseRawInput(bool bEnable) { s_bRawInput = bEnable; }

	private:
		static Input* m_Instance;
		std::unordered_map<std::string, unsigned int> m_mapButtons;
		std::unordered_map<unsigned int, bool> m_mapPressedKeys;

		void OnKeyDown(unsigned int keycode);
		void OnKeyUp(unsigned int keycode);
		void ClearKeys();
		static void ResetMouseDelta();

		// Singleton code safety
        // -- 03.08.2024: lmao paradigms moment
		Input();
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
	protected:
		bool Init();

		static bool s_bRawInput;
		static Vector2i s_mousePos, s_mouseDelta;
	};
}
