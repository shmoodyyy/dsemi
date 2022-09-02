#include "dspch.h"
#include "Dsemi/Core/DsemiWindows.h"
#include "Dsemi/Core/Input.h"

namespace dsemi {

	bool Input::s_bRawInput = true;
	vector2i Input::s_mousePos;
	vector2i Input::s_mouseDelta;

	Input* Input::m_Instance = new Input();

	Input::Input()
	{

	}

	Input* Input::Instance()
	{
		return m_Instance;
	}

	bool Input::Init()
	{
		// TODO: set up initialization
		// wtf am i gonna initialize in this whore? oh the mapping for each different platform ig
		return true;
	}

	bool Input::GetKeyDown(unsigned int keycode)
	{
		// TODO: Implement custom Dsemi KeyCodes and then translate the DSemi Keycode to Win32
		auto findIt = m_mapPressedKeys.find(keycode);
		return findIt != m_mapPressedKeys.end();
	}

	bool Input::GetMouseButton(unsigned int mousebutton)
	{
		// TODO: Implement custom Dsemi MouseCodes and then translate the DSemi Keycode to Win32
		return GetKeyDown(mousebutton);
	}

	bool Input::GetButton(const std::string& button) const
	{
		if (m_mapButtons.find(button) == m_mapButtons.end())
			return false;
		return GetKeyState(m_mapButtons.at(button)) & 0x8000;
	}

	void Input::MapButton(const std::string& button, int key)
	{
		m_mapButtons[button] = key;
	}

	void Input::OnKeyDown(unsigned int keycode)
	{
		m_mapPressedKeys[keycode] = true;
	}

	void Input::OnKeyUp(unsigned int keycode)
	{
		auto findIt = m_mapPressedKeys.find(keycode);
		if (findIt != m_mapPressedKeys.end())
			m_mapPressedKeys.erase(findIt);
	}

	void Input::ClearKeys()
	{
		m_mapPressedKeys.clear();
	}

	void Input::ResetMouseDelta()
	{
		s_mouseDelta = vector2i();
	}
}