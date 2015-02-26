#include <windows.h>

#include "Input/Keyboard.h"

namespace fsi
{

bool Keyboard::isKeyPressed(Key key)
{
    int virtualKey = 0;
    switch (key)
    {
    case Keyboard::Escape:        virtualKey = VK_ESCAPE;     break;
    case Keyboard::F1:            virtualKey = VK_F1;         break;
    case Keyboard::F2:            virtualKey = VK_F2;         break;
    case Keyboard::F3:            virtualKey = VK_F3;         break;
    case Keyboard::F4:            virtualKey = VK_F4;         break;
    case Keyboard::F5:            virtualKey = VK_F5;         break;
    case Keyboard::F6:            virtualKey = VK_F6;         break;
    case Keyboard::F7:            virtualKey = VK_F7;         break;
    case Keyboard::F8:            virtualKey = VK_F8;         break;
    case Keyboard::F9:            virtualKey = VK_F9;         break;
    case Keyboard::F10:           virtualKey = VK_F10;        break;
    case Keyboard::F11:           virtualKey = VK_F11;        break;
    case Keyboard::F12:           virtualKey = VK_F12;        break;
    case Keyboard::PrintScreen:   virtualKey = VK_PRINT;      break;
    case Keyboard::ScrollLock:    virtualKey = VK_SCROLL;     break;
    case Keyboard::Pause:         virtualKey = VK_PAUSE;      break;
    case Keyboard::Tilde:         virtualKey = VK_OEM_3;      break;
    case Keyboard::Num1:          virtualKey = '1';           break;
    case Keyboard::Num2:          virtualKey = '2';           break;
    case Keyboard::Num3:          virtualKey = '3';           break;
    case Keyboard::Num4:          virtualKey = '4';           break;
    case Keyboard::Num5:          virtualKey = '5';           break;
    case Keyboard::Num6:          virtualKey = '6';           break;
    case Keyboard::Num7:          virtualKey = '7';           break;
    case Keyboard::Num8:          virtualKey = '8';           break;
    case Keyboard::Num9:          virtualKey = '9';           break;
    case Keyboard::Num0:          virtualKey = '0';           break;
    case Keyboard::UnderScore:    virtualKey = VK_OEM_MINUS;  break;
    case Keyboard::Equals:        virtualKey = VK_OEM_PLUS;   break;
    case Keyboard::BackSpace:     virtualKey = VK_BACK;       break;
    case Keyboard::Tab:           virtualKey = VK_TAB;        break;
    case Keyboard::Q:             virtualKey = 'Q';           break;
    case Keyboard::W:             virtualKey = 'W';           break;
    case Keyboard::E:             virtualKey = 'E';           break;
    case Keyboard::R:             virtualKey = 'R';           break;
    case Keyboard::T:             virtualKey = 'T';           break;
    case Keyboard::Y:             virtualKey = 'Y';           break;
    case Keyboard::U:             virtualKey = 'U';           break;
    case Keyboard::I:             virtualKey = 'I';           break;
    case Keyboard::O:             virtualKey = 'O';           break;
    case Keyboard::P:             virtualKey = 'P';           break;
    case Keyboard::LeftBracket:   virtualKey = VK_OEM_4;      break;
    case Keyboard::RightBracket:  virtualKey = VK_OEM_6;      break;
    case Keyboard::BackSlash:     virtualKey = VK_OEM_5;      break;
    case Keyboard::CapsLock:      virtualKey = VK_CAPITAL;    break;
    case Keyboard::A:             virtualKey = 'A';           break;
    case Keyboard::S:             virtualKey = 'S';           break;
    case Keyboard::D:             virtualKey = 'D';           break;
    case Keyboard::F:             virtualKey = 'F';           break;
    case Keyboard::G:             virtualKey = 'G';           break;
    case Keyboard::H:             virtualKey = 'H';           break;
    case Keyboard::J:             virtualKey = 'J';           break;
    case Keyboard::K:             virtualKey = 'K';           break;
    case Keyboard::L:             virtualKey = 'L';           break;
    case Keyboard::SemiColon:     virtualKey = VK_OEM_1;      break;
    case Keyboard::Quote:         virtualKey = VK_OEM_7;      break;
    case Keyboard::Return:        virtualKey = VK_RETURN;     break;
    case Keyboard::LeftShift:     virtualKey = VK_LSHIFT;     break;
    case Keyboard::Z:             virtualKey = 'Z';           break;
    case Keyboard::X:             virtualKey = 'X';           break;
    case Keyboard::C:             virtualKey = 'C';           break;
    case Keyboard::V:             virtualKey = 'V';           break;
    case Keyboard::B:             virtualKey = 'B';           break;
    case Keyboard::N:             virtualKey = 'N';           break;
    case Keyboard::M:             virtualKey = 'M';           break;
    case Keyboard::Comma:         virtualKey = VK_OEM_COMMA;  break;
    case Keyboard::Period:        virtualKey = VK_OEM_PERIOD; break;
    case Keyboard::FrontSlash:    virtualKey = VK_OEM_2;      break;
    case Keyboard::RightShift:    virtualKey = VK_RSHIFT;     break;
    case Keyboard::LeftControl:   virtualKey = VK_LCONTROL;   break;
    case Keyboard::LeftSystem:    virtualKey = VK_LWIN;       break;
    case Keyboard::LeftAlt:       virtualKey = VK_LMENU;      break;
    case Keyboard::Space:         virtualKey = VK_SPACE;      break;
    case Keyboard::RightAlt:      virtualKey = VK_RMENU;      break;
    case Keyboard::RightSystem:   virtualKey = VK_RWIN;       break;
    case Keyboard::Menu:          virtualKey = VK_APPS;       break;
    case Keyboard::RightControl:  virtualKey = VK_RCONTROL;   break;
    case Keyboard::Insert:        virtualKey = VK_INSERT;     break;
    case Keyboard::Home:          virtualKey = VK_HOME;       break;
    case Keyboard::PageUp:        virtualKey = VK_PRIOR;      break;
    case Keyboard::Delete:        virtualKey = VK_DELETE;     break;
    case Keyboard::End:           virtualKey = VK_END;        break;
    case Keyboard::PageDown:      virtualKey = VK_NEXT;       break;
    case Keyboard::Up:            virtualKey = VK_UP;         break;
    case Keyboard::Left:          virtualKey = VK_LEFT;       break;
    case Keyboard::Down:          virtualKey = VK_DOWN;       break;
    case Keyboard::Right:         virtualKey = VK_RIGHT;      break;
    case Keyboard::NumLock:       virtualKey = VK_NUMLOCK;    break;
    case Keyboard::Divide:        virtualKey = VK_DIVIDE;     break;
    case Keyboard::Multiply:      virtualKey = VK_MULTIPLY;   break;
    case Keyboard::Subtract:      virtualKey = VK_SUBTRACT;   break;
    case Keyboard::NumPad7:       virtualKey = VK_NUMPAD7;    break;
    case Keyboard::NumPad8:       virtualKey = VK_NUMPAD8;    break;
    case Keyboard::NumPad9:       virtualKey = VK_NUMPAD9;    break;
    case Keyboard::Add:           virtualKey = VK_ADD;        break;
    case Keyboard::NumPad4:       virtualKey = VK_NUMPAD4;    break;
    case Keyboard::NumPad5:       virtualKey = VK_NUMPAD5;    break;
    case Keyboard::NumPad6:       virtualKey = VK_NUMPAD6;    break;
    case Keyboard::NumPad1:       virtualKey = VK_NUMPAD1;    break;
    case Keyboard::NumPad2:       virtualKey = VK_NUMPAD2;    break;
    case Keyboard::NumPad3:       virtualKey = VK_NUMPAD3;    break;
    case Keyboard::NumPad0:       virtualKey = VK_NUMPAD0;    break;
    case Keyboard::NumPadDecimal: virtualKey = VK_DECIMAL;    break;
    default:                      virtualKey = 0;             break;
    }

    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

}