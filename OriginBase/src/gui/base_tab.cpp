#include "common.hpp"
#include "base_tab.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "nativeui.hpp"
#include "features.hpp"
namespace Origin
{
    int TestInt = 69;
    float TestFloat = 420;
    int NumberKeyboard() {
        GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(1, "", "", "", "", "", "", 10);
        while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) SYSTEM::WAIT(0);
        if (!GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT()) return 0;
        return atof(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
    }

    void base_tab::native_ui()
    {
        /*Draw NativeUI Here*/
        NativeUI::Checks::Controlls();

        switch (NativeUI::Settings::currentMenu) {

        case mainmenu:
        {
            NativeUI::Title(ORIGIN_NAME); /*To change this head over to -> common.hpp*/
            NativeUI::Option("Test Option");
            NativeUI::MenuOption("Test Menu", testmenu); /*To add a SubMenu head over to -> nativeui.hpp*/
            NativeUI::Int("Test Int", TestInt, 0, 69);
            NativeUI::Float("Test Float", TestFloat, 0, 69, 420);
            NativeUI::Bool("Test Loop Bool", features::testbool); /*To add a Looped Bool head over to -> features.hpp & features.cpp and look at how to make one*/
            NativeUI::MenuOption("Menu Settings", menusettings);
        }
        break;
        case testmenu:
        {
            NativeUI::Title(ORIGIN_NAME); /*To change this head over to -> common.hpp*/
            NativeUI::Option("Test Option");
        }
        break;
        case menusettings:
        {
            NativeUI::Title("Settings");
            NativeUI::MenuOption("Colors", settingstheme);
            NativeUI::MenuOption("Credits", Credits);
            NativeUI::Bool("Globe on Title", NativeUI::Settings::EnableGlare);
            NativeUI::Option("~HUD_COLOUR_GOLD~Social Club Account:");
            NativeUI::Option(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()));
            if (NativeUI::Option("Unload")) {  g_running = false; };
            if (NativeUI::Option("Close GTA")) { exit(0); };
        }
        break;
        case Credits:
        {
            NativeUI::Title("Credits");
            NativeUI::Option("~g~JanDenondt");
            NativeUI::Option("Base : ~b~Pocakking");
            NativeUI::Option("Thanks to:");
            NativeUI::Option("~b~DireDan");
            NativeUI::Option("~g~gir489");
            NativeUI::Option("V. 0.0");
        }
        break;
        case settingstheme:
        {
            NativeUI::Title("Colors");
            NativeUI::MenuOption("Theme Loader", themeloader);
            NativeUI::MenuOption("Title Background", settingstitlerect);
            NativeUI::MenuOption("Selection Box", settingsscroller);
            NativeUI::MenuOption("Option Text", settingsoptiontext);
            if (NativeUI::Option("MenuX plus")) {
                if (NativeUI::Settings::menuX < 0.81f) NativeUI::Settings::menuX += 0.01f;
            }
            if (NativeUI::Option("MenuX minus")) {
                if (NativeUI::Settings::menuX > 0.17f) NativeUI::Settings::menuX -= 0.01f;
            }
        }
        break;
        case themeloader:
        {
            NativeUI::Title("Theme Colors");
            if (NativeUI::Option("Red Theme")) {
                NativeUI::Settings::titleRect = { 100, 0, 0, 255 };
                NativeUI::Settings::optionRect = { 50, 50, 50, 100 };
                NativeUI::Settings::scroller = { 100, 0, 0, 255 };
            }
            if (NativeUI::Option("Blue Theme")) {
                NativeUI::Settings::titleRect = { 0, 0, 200, 255 };
                NativeUI::Settings::optionRect = { 50, 50, 50, 100 };
                NativeUI::Settings::scroller = { 0, 0, 200, 255 };
            }
            if (NativeUI::Option("Green Theme")) {
                NativeUI::Settings::titleRect = { 0, 180, 0, 255 };
                NativeUI::Settings::optionRect = { 50, 50, 50, 100 };
                NativeUI::Settings::scroller = { 0, 0, 180, 255 };
            }
            if (NativeUI::Option("Load Default Theme")) {
                NativeUI::Settings::titleRect = { 52, 73, 94, 255 };
                NativeUI::Settings::scroller = { 121, 135, 136, 150 };
                NativeUI::Settings::optionRect = { 2, 41, 58, 255 };
            }
        }
        break;
        case settingstitlerect:
        {
            NativeUI::Title("Title Rect");
            if (NativeUI::Int("Red", NativeUI::Settings::titleRect.r, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::titleRect.r = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Green", NativeUI::Settings::titleRect.g, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::titleRect.g = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Blue", NativeUI::Settings::titleRect.b, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::titleRect.b = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Opacity", NativeUI::Settings::titleRect.a, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::titleRect.a = NumberKeyboard();
                }
            }
        }
        break;
        case settingsoptiontext:
        {
            NativeUI::Title("Option Text");
            if (NativeUI::Int("Red", NativeUI::Settings::optionText.r, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::optionText.r = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Green", NativeUI::Settings::optionText.g, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::optionText.g = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Blue", NativeUI::Settings::optionText.b, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::optionText.b = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Opacity", NativeUI::Settings::optionText.a, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::optionText.a = NumberKeyboard();
                }
            }
            NativeUI::MenuOption("Font", font);
        }
        break;
        case font:
        {
            NativeUI::Title("Font");
            if (NativeUI::Option("Standard Font")) { NativeUI::Settings::optionText.f = 6; }
            if (NativeUI::Option("Chalet London")) { NativeUI::Settings::optionText.f = 0; }
            if (NativeUI::Option("House Script")) { NativeUI::Settings::optionText.f = 1; }
            if (NativeUI::Option("Monospace")) { NativeUI::Settings::optionText.f = 2; }
            if (NativeUI::Option("Wing Dings")) { NativeUI::Settings::optionText.f = 3; }
            if (NativeUI::Option("Chalet Comprime Cologne")) { NativeUI::Settings::optionText.f = 4; }
            if (NativeUI::Option("Pricedown")) { NativeUI::Settings::optionText.f = 7; }
        }
        break;
        case settingsscroller:
        {
            NativeUI::Title("Scroller");
            if (NativeUI::Int("Red", NativeUI::Settings::scroller.r, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::scroller.r = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Green", NativeUI::Settings::scroller.g, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::scroller.g = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Blue", NativeUI::Settings::scroller.b, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::scroller.b = NumberKeyboard();
                }
            }
            if (NativeUI::Int("Opacity", NativeUI::Settings::scroller.a, 0, 255))
            {
                if (keypushed(VK_NUMPAD5) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept)) {
                    NativeUI::Settings::scroller.a = NumberKeyboard();
                }
            }
        }
        break;
        }
        NativeUI::End();
    }
}