#include "common.hpp"
#include "nativeui.hpp"
#include "natives.hpp"
#include "memory/all.hpp"
#include "gta/enums.hpp"
#include "gta/vector.hpp"

void NativeUI::Draw::Text(const char* text, RGBAF rgbaf, VECTOR2 position, VECTOR2_2 size, bool center)
{
	UI::SET_TEXT_CENTRE(center);
	UI::SET_TEXT_COLOUR(rgbaf.r, rgbaf.g, rgbaf.b, rgbaf.a);
	UI::SET_TEXT_FONT(rgbaf.f);
	UI::SET_TEXT_SCALE(size.w, size.h);
	UI::SET_TEXT_DROPSHADOW(1, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(1, 0, 0, 0, 0);
	UI::SET_TEXT_OUTLINE();
	UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
	UI::END_TEXT_COMMAND_DISPLAY_TEXT(position.x, position.y);
}

void NativeUI::Draw::Sprite(std::string Streamedtexture, std::string textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
{
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)Streamedtexture.c_str()))
	{
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT((char*)Streamedtexture.c_str(), false);
	}
	else
	{
		GRAPHICS::DRAW_SPRITE((char*)Streamedtexture.c_str(), (char*)textureName.c_str(), x, y, width, height, rotation, r, g, b, a);
	}
}

void NativeUI::Draw::Rect(RGBA rgba, VECTOR2 position, VECTOR2_2 size)
{
	GRAPHICS::DRAW_RECT(position.x, position.y, size.w, size.h, rgba.r, rgba.g, rgba.b, rgba.a);
}
float gGlareDir;
float conv360(float base, float min, float max)
{
	float fVar0;
	if (min == max) return min;
	fVar0 = max - min;
	base -= SYSTEM::ROUND(base - min / fVar0) * fVar0;
	if (base < min) base += fVar0;
	return base;
}
void NativeUI::Draw::DrawGlare(const float x, const float y, const float sx, const float sy, const int r, const int g, const int b)
{
	int gGlareHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE(const_cast<char*>("MP_MENU_GLARE"));
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	float dir = conv360(rot.z, 0, 360);
	if ((gGlareDir == 0 || gGlareDir - dir > 0.5) || gGlareDir - dir < -0.5)
	{
		gGlareDir = dir;
		GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(gGlareHandle, const_cast <char*>("SET_DATA_SLOT"));
		GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_FLOAT(gGlareDir);
		GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	}
	GRAPHICS::DRAW_SCALEFORM_MOVIE(gGlareHandle, x, y, sx, sy, r, g, b, 20, 0);

}

float NativeUI::Settings::menuX = 0.2f;
float NativeUI::Settings::menuWidth = 0.23f;
bool NativeUI::Settings::selectPressed = false;
bool NativeUI::Settings::leftPressed = false;
bool NativeUI::Settings::rightPressed = false;
bool firstopen = true;
bool NativeUI::Settings::EnableGlare = true;
int NativeUI::Settings::maxVisOptions = 16;
int NativeUI::Settings::currentOption = 0;
int NativeUI::Settings::optionCount = 0;
SubMenus NativeUI::Settings::currentMenu;
int NativeUI::Settings::menuLevel = 0;
int NativeUI::Settings::optionsArray[1000];
SubMenus NativeUI::Settings::menusArray[1000];

RGBAF NativeUI::Settings::count{ 255, 255, 255, 255, 6 };
RGBAF NativeUI::Settings::titleText{ 255, 255, 255, 255, 7 };
RGBA NativeUI::Settings::titleRect{ 52, 73, 94, 255 };
RGBAF NativeUI::Settings::optionText{ 255, 255, 255, 255, 6 };
RGBAF NativeUI::Settings::breakText{ 255, 255, 255, 255, 1 };
RGBAF NativeUI::Settings::arrow{ 255, 255, 255, 255, 3 };                          // Mod Menu Colors 
RGBA NativeUI::Settings::optionRect{ 2, 41, 58, 255 };
RGBA NativeUI::Settings::scroller{ 121, 135, 136, 150 };
RGBAF NativeUI::Settings::integre{ 255, 255, 255, 255, 2 };
RGBA NativeUI::Settings::line{ 255, 255, 255, 255 };
RGBA NativeUI::Settings::primary{ 255, 0, 0 };
RGBA NativeUI::Settings::secondary{ 0, 255, 0 };

void NativeUI::Title(const char* title)
{
	Draw::Text(title, Settings::titleText, { Settings::menuX, 0.095f }, { 0.85f, 0.85f }, true);
	Draw::Rect(Settings::titleRect, { Settings::menuX, 0.1175f }, { Settings::menuWidth, 0.085f });

	if (Settings::EnableGlare) {
		Draw::DrawGlare(Settings::menuX + 0.36f, 0.482f, 1.088999f, 0.902000f, 255, 255, 255); //Globe
	}

	// Disable Controls
	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(0);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(10);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(6);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(7);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(9);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(8);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_NEXT_CAMERA, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_CHARACTER_WHEEL, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_LIGHT, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_HEAVY, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MULTIPLAYER_INFO, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK_ALTERNATE, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_CIN_CAM, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_MAP_POI, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_RADIO_WHEEL, true);
	CONTROLS::DISABLE_CONTROL_ACTION(2, INPUT_VEH_HEADLIGHT, true);
}
bool NativeUI::Option(const char* option)
{
	Settings::optionCount++;
	bool onThis = Settings::currentOption == Settings::optionCount ? true : false;
	if (Settings::currentOption <= 16 && Settings::optionCount <= 16)
	{
		Draw::Text(option, Settings::optionText, { Settings::menuX - 0.1f, (Settings::optionCount) * 0.035f + 0.125f }, { 0.45f, 0.45f }, false);
		Draw::Rect(Settings::optionRect, { Settings::menuX, (Settings::optionCount) * 0.035f + 0.1415f }, { Settings::menuWidth, 0.035f });
		onThis ? Draw::Rect(Settings::scroller, { Settings::menuX, (Settings::optionCount) * 0.035f + 0.1415f }, { Settings::menuWidth, 0.035f }) : NULL;
	}
	else if (Settings::optionCount > (Settings::currentOption - 16) && Settings::optionCount <= Settings::currentOption)
	{
		Draw::Text(option, Settings::optionText, { Settings::menuX - 0.1f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.125f }, { 0.45f, 0.45f }, false);
		Draw::Rect(Settings::optionRect, { Settings::menuX,  (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.1415f }, { Settings::menuWidth, 0.035f });
		onThis ? Draw::Rect(Settings::scroller, { Settings::menuX,  (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.1415f }, { Settings::menuWidth, 0.035f }) : NULL;
	}
	if (Settings::currentOption == Settings::optionCount)
	{
		if (Settings::selectPressed)
		{
			return true;
		}
	}
	return false;
}
bool NativeUI::Option(const char* option, std::function<void()> function)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	return false;
}
bool NativeUI::MenuOption(const char* option, SubMenus newSub)
{
	Option(option);

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(">>", Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(">>", Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		MenuLevelHandler::MoveMenu(newSub);
		return true;
	}
	return false;
}

bool NativeUI::MenuOption(const char* option, SubMenus newSub, std::function<void()> function)
{
	MenuOption(option, newSub);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	return false;
}

bool NativeUI::Bool(const char* option, bool& b00l)
{
	Option(option);
	if (b00l)
	{
		if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
			Draw::Sprite("commonmenu", b00l ? "shop_box_tick" : "shop_box_blank", Settings::menuX + 0.068f, (Settings::optionCount * 0.035f + 0.141f), 0.03f, 0.05f, 0, 255, 255, 255, 200);
		else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
			Draw::Sprite("commonmenu", b00l ? "shop_box_tick" : "shop_box_blank", Settings::menuX + 0.068f, ((Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.141f), 0.03f, 0.05f, 0, 255, 255, 255, 200);
	}
	else
	{
		if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
			Draw::Sprite("commonmenu", b00l ? "shop_box_tick" : "shop_box_blank", Settings::menuX + 0.068f, (Settings::optionCount * 0.035f + 0.141f), 0.03f, 0.05f, 0, 255, 255, 255, 200);
		else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
			Draw::Sprite("commonmenu", b00l ? "shop_box_tick" : "shop_box_blank", Settings::menuX + 0.068f, ((Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.141f), 0.03f, 0.05f, 0, 255, 255, 255, 200);
	}
	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		b00l ^= 1;
		return true;
	}
	return false;
}
bool NativeUI::Bool(const char* option, bool& b00l, std::function<void()> function)
{
	Bool(option, b00l);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	return false;
}
bool NativeUI::Int(const char* option, int& _int, int min, int max)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		if (Settings::leftPressed) {
			_int >= min ? _int-- : _int = max;
		}
		if (Settings::rightPressed) {
			_int < max ? _int++ : _int = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(Tools::StringToChar(std::to_string(_int)), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(Tools::StringToChar(std::to_string(_int)), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool NativeUI::Int(const char* option, int& _int, int min, int max, int step)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		if (Settings::leftPressed) {
			_int >= min ? _int -= step : _int = max;
		}
		if (Settings::rightPressed) {
			_int < max ? _int += step : _int = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(Tools::StringToChar(std::to_string(_int)), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(Tools::StringToChar(std::to_string(_int)), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool NativeUI::Int(const char* option, int& _int, int min, int max, std::function<void()> function)
{
	Int(option, _int, min, max);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) {
		function();
		return true;
	}
	return false;
}
bool NativeUI::Int(const char* option, int& _int, int min, int max, int step, std::function<void()> function)
{
	Int(option, _int, min, max, step);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) {
		function();
		return true;
	}
	return false;
}
#pragma warning(disable: 4244)
bool NativeUI::Float(const char* option, float& _float, int min, int max)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		if (Settings::leftPressed) {
			_float <= min ? _float = max : _float -= 0.1f;
		}
		if (Settings::rightPressed) {
			_float >= max ? _float = min : _float += 0.1f;
		}
		_float < min ? _float = max : _float > max ? _float = min : NULL;
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(Tools::StringToChar(std::to_string(_float)), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(Tools::StringToChar(std::to_string(_float)), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool NativeUI::Float(const char* option, float& _float, int min, int max, int step)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		if (Settings::leftPressed) {
			_float <= min ? _float = max : _float -= 0.1f;
		}
		if (Settings::rightPressed) {
			_float >= max ? _float = min : _float += 0.1f;
		}
		_float < min ? _float = max : _float > max ? _float = min : NULL;
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(Tools::StringToChar(std::to_string(_float)), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(Tools::StringToChar(std::to_string(_float)), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
#pragma warning(default: 4244)
bool NativeUI::Float(const char* option, float& _float, int min, int max, std::function<void()> function)
{
	Float(option, _float, min, max);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) {
		function();
		return true;
	}
	return false;
}
bool NativeUI::Float(const char* option, float& _float, int min, int max, int step, std::function<void()> function)
{
	Float(option, _float, min, max, step);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) {
		function();
		return true;
	}
	return false;
}
#pragma warning(disable: 4267)
bool NativeUI::IntVector(const char* option, std::vector<int> Vector, int& position)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		int max = Vector.size() - 1;
		int min = 0;
		if (Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(Tools::StringToChar(std::to_string(Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(Tools::StringToChar(std::to_string(Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool NativeUI::IntVector(const char* option, std::vector<int> Vector, int& position, std::function<void()> function)
{
	IntVector(option, Vector, position);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) {
		function();
		return true;
	}
	return false;
}
bool NativeUI::FloatVector(const char* option, std::vector<float> Vector, int& position)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		size_t max = Vector.size() - 1;
		int min = 0;
		if (Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(Tools::StringToChar(std::to_string(Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(Tools::StringToChar(std::to_string(Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool NativeUI::FloatVector(const char* option, std::vector<float> Vector, int& position, std::function<void()> function)
{
	FloatVector(option, Vector, position);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) {
		function();
		return true;
	}
	return false;
}
bool NativeUI::StringVector(const char* option, std::vector<std::string> Vector, int& position)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		size_t max = Vector.size() - 1;
		int min = 0;
		if (Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(Tools::StringToChar((Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(Tools::StringToChar((Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
bool NativeUI::StringVector(const char* option, std::vector<std::string> Vector, int& position, std::function<void()> function)
{
	StringVector(option, Vector, position);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) {
		function();
		return true;
	}
	return false;
}
bool NativeUI::StringVector(const char* option, std::vector<char*> Vector, int& position)
{
	Option(option);

	if (Settings::optionCount == Settings::currentOption) {
		size_t max = Vector.size() - 1;
		int min = 0;
		if (Settings::leftPressed) {
			position >= 1 ? position-- : position = max;
		}
		if (Settings::rightPressed) {
			position < max ? position++ : position = min;
		}
	}

	if (Settings::currentOption <= Settings::maxVisOptions && Settings::optionCount <= Settings::maxVisOptions)
		Draw::Text(((Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, Settings::optionCount * 0.035f + 0.125f }, { 0.5f, 0.5f }, true);
	else if (Settings::optionCount > Settings::currentOption - Settings::maxVisOptions && Settings::optionCount <= Settings::currentOption)
		Draw::Text(((Vector[position])), Settings::optionText, { Settings::menuX + 0.068f, (Settings::optionCount - (Settings::currentOption - 16)) * 0.035f + 0.12f }, { 0.5f, 0.5f }, true);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) return true;
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) return true;
	return false;
}
#pragma warning(default: 4267)
bool NativeUI::StringVector(const char* option, std::vector<char*> Vector, int& position, std::function<void()> function)
{
	StringVector(option, Vector, position);

	if (Settings::optionCount == Settings::currentOption && Settings::selectPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::leftPressed) {
		function();
		return true;
	}
	else if (Settings::optionCount == Settings::currentOption && Settings::rightPressed) {
		function();
		return true;
	}
	return false;
}
void NativeUI::End()
{
	if (Settings::optionCount > 16) {
		Draw::Text(Tools::StringToChar(std::to_string(Settings::currentOption) + "/" + std::to_string(Settings::optionCount)), Settings::count, { Settings::menuX - 0.1f, 17 * 0.035f + 0.125f }, { 0.5f, 0.5f }, false);
		Draw::Rect(Settings::titleRect, { Settings::menuX, 17 * 0.035f + 0.1415f }, { Settings::menuWidth, 0.035f });

		/*Switches the Arrow on optioncount*/
		if (Settings::currentOption == 1) {
			Draw::Sprite("commonmenu", "arrowright", Settings::menuX, ((16 + 1) * 0.035f + 0.145f), 0.010f, 0.0175f, 90, Settings::line.r, Settings::line.g, Settings::line.b, Settings::line.a);
		}
		else if (Settings::currentOption == Settings::optionCount) {
			Draw::Sprite("commonmenu", "arrowright", Settings::menuX, ((16 + 1) * 0.035f + 0.145f), 0.010f, 0.0175f, 270, Settings::line.r, Settings::line.g, Settings::line.b, Settings::line.a);
		}
		else {
			Draw::Sprite("commonmenu", "shop_arrows_upanddown", Settings::menuX, ((16 + 1) * 0.035f + 0.140f), 0.020f, 0.035f, 180, Settings::line.r, Settings::line.g, Settings::line.b, Settings::line.a);
		}
	}
	else if (Settings::currentOption > 0) {
		Draw::Text(Tools::StringToChar(std::to_string(Settings::currentOption) + "/" + std::to_string(Settings::optionCount)), Settings::count, { Settings::menuX - 0.1f, (Settings::optionCount + 1) * 0.035f + 0.125f }, { 0.5f, 0.5f }, false);
		Draw::Rect(Settings::titleRect, { Settings::menuX, (Settings::optionCount + 1) * 0.035f + 0.1415f }, { Settings::menuWidth, 0.035f });

		/*Switches the Arrow on optioncount*/
		if (Settings::currentOption == 1 && Settings::optionCount > 1) {
			Draw::Sprite("commonmenu", "arrowright", Settings::menuX, ((Settings::optionCount + 1) * 0.035f + 0.145f), 0.010f, 0.0175f, 90, Settings::line.r, Settings::line.g, Settings::line.b, Settings::line.a);
		}
		else if (Settings::currentOption == Settings::optionCount && Settings::optionCount > 1) {
			Draw::Sprite("commonmenu", "arrowright", Settings::menuX, ((Settings::optionCount + 1) * 0.035f + 0.145f), 0.010f, 0.0175f, 270, Settings::line.r, Settings::line.g, Settings::line.b, Settings::line.a);
		}
		else {
			Draw::Sprite("commonmenu", "shop_arrows_upanddown", Settings::menuX, ((Settings::optionCount + 1) * 0.035f + 0.140f), 0.020f, 0.035f, 180, Settings::line.r, Settings::line.g, Settings::line.b, Settings::line.a);
		}
	}
}
void PlaySoundFrontend_default(char* sound_name)
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name, "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}
void PlaySoundFrontend_default2(char* sound_name)
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, sound_name, "DLC_HEIST_BIOLAB_PREP_HACKING_SOUNDS", 0);
}

int NativeUI::Settings::keyPressDelay = 200;
int NativeUI::Settings::keyPressPreviousTick = GetTickCount();
int NativeUI::Settings::keyPressDelay2 = 100;
int NativeUI::Settings::keyPressPreviousTick2 = GetTickCount();
int NativeUI::Settings::keyPressDelay3 = 140;
int NativeUI::Settings::keyPressPreviousTick3 = GetTickCount();
int NativeUI::Settings::openKey = VK_INSERT;
int NativeUI::Settings::backKey = VK_NUMPAD0;
int NativeUI::Settings::upKey = VK_NUMPAD8;
int NativeUI::Settings::downKey = VK_NUMPAD2;
int NativeUI::Settings::leftKey = VK_NUMPAD4;
int NativeUI::Settings::rightKey = VK_NUMPAD6;
int NativeUI::Settings::selectKey = VK_NUMPAD5;
int NativeUI::Settings::arrowupKey = VK_UP;
int NativeUI::Settings::arrowdownKey = VK_DOWN;
int NativeUI::Settings::arrowleftKey = VK_LEFT;
int NativeUI::Settings::arrowrightKey = VK_RIGHT;
int NativeUI::Settings::enterKey = VK_RETURN;
int NativeUI::Settings::deleteKey = VK_BACK;

#pragma endregion

bool NativeUI::Settings::controllerinput = true;
void NativeUI::Checks::Controlls()
{
	Settings::selectPressed = false;
	Settings::leftPressed = false;
	Settings::rightPressed = false;
	if (GetTickCount() - Settings::keyPressPreviousTick > Settings::keyPressDelay) {
		if (GetTickCount() - Settings::keyPressPreviousTick2 > Settings::keyPressDelay2) {
			if (GetTickCount() - Settings::keyPressPreviousTick3 > Settings::keyPressDelay3) {
				if (keypushed(VK_INSERT) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlScriptRB) && CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneRight) && Settings::controllerinput) {
					Settings::menuLevel == 0 ? MenuLevelHandler::MoveMenu(SubMenus::mainmenu) : Settings::menuLevel == 1 ? MenuLevelHandler::BackMenu() : NULL;
					Settings::keyPressPreviousTick = GetTickCount();
				}
				else if (keypushed(VK_NUMPAD0) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendCancel) && Settings::controllerinput) {
					Settings::menuLevel > 0 ? MenuLevelHandler::BackMenu() : NULL;
					if (Settings::menuLevel > 0)
						PlaySoundFrontend_default("BACK");

					Settings::keyPressPreviousTick = GetTickCount();
				}
				else if (keypushed(VK_NUMPAD8) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendUp) && Settings::controllerinput) {
					Settings::currentOption > 1 ? Settings::currentOption-- : Settings::currentOption = Settings::optionCount;
					if (Settings::menuLevel > 0)
						PlaySoundFrontend_default("NAV_UP_DOWN");

					Settings::keyPressPreviousTick2 = GetTickCount();
				}
				else if (keypushed(VK_NUMPAD2) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendDown) && Settings::controllerinput) {
					Settings::currentOption < Settings::optionCount ? Settings::currentOption++ : Settings::currentOption = 1;
					if (Settings::menuLevel > 0)
						PlaySoundFrontend_default("NAV_UP_DOWN");

					Settings::keyPressPreviousTick2 = GetTickCount();
				}
				else if (keypushed(VK_NUMPAD6) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneRight) && Settings::controllerinput) {
					Settings::leftPressed = true;
					if (Settings::menuLevel > 0)
						PlaySoundFrontend_default("NAV_UP_DOWN");

					Settings::keyPressPreviousTick3 = GetTickCount();
				}
				else if (keypushed(VK_NUMPAD4) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlPhoneLeft) && Settings::controllerinput) {
					Settings::rightPressed = true;
					if (Settings::menuLevel > 0)
						PlaySoundFrontend_default("NAV_UP_DOWN");

					Settings::keyPressPreviousTick3 = GetTickCount();
				}
				else if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept) && Settings::controllerinput) {
					Settings::selectPressed = true;
					if (Settings::menuLevel > 0)
						PlaySoundFrontend_default("SELECT");

					Settings::keyPressPreviousTick = GetTickCount();
				}
			}
		}
	}
	Settings::optionCount = 0;
}
#pragma warning(default : 4018)
void NativeUI::MenuLevelHandler::MoveMenu(SubMenus menu)
{
	Settings::menusArray[Settings::menuLevel] = Settings::currentMenu;
	Settings::optionsArray[Settings::menuLevel] = Settings::currentOption;
	Settings::menuLevel++;
	Settings::currentMenu = menu;
	Settings::currentOption = 1;
}

void NativeUI::MenuLevelHandler::BackMenu()
{
	Settings::menuLevel--;
	Settings::currentMenu = Settings::menusArray[Settings::menuLevel];
	Settings::currentOption = Settings::optionsArray[Settings::menuLevel];
}

char* NativeUI::Tools::StringToChar(std::string string)
{
	return _strdup(string.c_str());
}

void NativeUI::AddSmallTitle(char* text)
{
	if (NativeUI::Settings::menuX < 0.78f)
	{
		Draw::Text(text, Settings::titleText, { Settings::menuX + 0.175f, 0.090f }, { 0.425f, 0.425f }, true);
		Draw::Sprite("CommonMenu", "", Settings::menuX + 0.175f, 0.1175f - 0.019f, 0.115f, 0.045f, 180, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	}
	else
	{
		Draw::Text(text, Settings::titleText, { Settings::menuX - 0.175f, 0.090f }, { 0.425f, 0.425f }, true);
		Draw::Sprite("CommonMenu", "", Settings::menuX - 0.175f, 0.1175f - 0.019f, 0.115f, 0.045f, 180, Settings::titleRect.r, Settings::titleRect.g, Settings::titleRect.b, Settings::titleRect.a);
	}
}
void NativeUI::AddSmallInfo(char* text, short line)
{
	if (NativeUI::Settings::menuX < 0.78f)
	{
		if (line == 1) {
			Draw::Rect(Settings::optionRect, { Settings::menuX + 0.175f, ((16 * 0.035f) / 2.0f) + 0.159f - 0.135f }, { 0.115f, 16 * 0.035f + -0.193f });
		}
		Draw::Text(text, Settings::count, { Settings::menuX + 0.120f, (line * 0.020f) + 0.123f }, { 0.375f, 0.375f }, false);
	}
	else
	{
		if (line == 1) {
			Draw::Rect(Settings::optionRect, { Settings::menuX - 0.175f, ((16 * 0.035f) / 2.0f) + 0.159f - 0.135f }, { 0.115f, 16 * 0.035f + -0.193f });
		}
		Draw::Text(text, Settings::count, { Settings::menuX - 0.228f, (line * 0.020f) + 0.123f }, { 0.375f, 0.375f }, false);
	}
}