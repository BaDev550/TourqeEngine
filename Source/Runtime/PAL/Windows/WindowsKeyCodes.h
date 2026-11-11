#pragma once

#include <Runtime/PAL/KeyCodes.h>
#include <Windows.h>

namespace Runtime {
    KeyCode TranslateWin32Key(WPARAM wParam, LPARAM lParam) {
        bool isExtended = (lParam & (1 << 24)) != 0;

        switch (wParam) {
        case '0': return KeyCode::Num0;
        case '1': return KeyCode::Num1;
        case '2': return KeyCode::Num2;
        case '3': return KeyCode::Num3;
        case '4': return KeyCode::Num4;
        case '5': return KeyCode::Num5;
        case '6': return KeyCode::Num6;
        case '7': return KeyCode::Num7;
        case '8': return KeyCode::Num8;
        case '9': return KeyCode::Num9;

        case 'A': return KeyCode::A;
        case 'B': return KeyCode::B;
        case 'C': return KeyCode::C;
        case 'D': return KeyCode::D;
        case 'E': return KeyCode::E;
        case 'F': return KeyCode::F;
        case 'G': return KeyCode::G;
        case 'H': return KeyCode::H;
        case 'I': return KeyCode::I;
        case 'J': return KeyCode::J;
        case 'K': return KeyCode::K;
        case 'L': return KeyCode::L;
        case 'M': return KeyCode::M;
        case 'N': return KeyCode::N;
        case 'O': return KeyCode::O;
        case 'P': return KeyCode::P;
        case 'Q': return KeyCode::Q;
        case 'R': return KeyCode::R;
        case 'S': return KeyCode::S;
        case 'T': return KeyCode::T;
        case 'U': return KeyCode::U;
        case 'V': return KeyCode::V;
        case 'W': return KeyCode::W;
        case 'X': return KeyCode::X;
        case 'Y': return KeyCode::Y;
        case 'Z': return KeyCode::Z;

        case VK_SPACE:      return KeyCode::Space;
        case VK_OEM_7:      return KeyCode::Apostrophe;
        case VK_OEM_COMMA:  return KeyCode::Comma;
        case VK_OEM_MINUS:  return KeyCode::Minus;
        case VK_OEM_PERIOD: return KeyCode::Period;
        case VK_OEM_2:      return KeyCode::Slash;
        case VK_OEM_1:      return KeyCode::Semicolon;
        case VK_OEM_PLUS:   return KeyCode::Equal;
        case VK_OEM_4:      return KeyCode::LeftBracket;
        case VK_OEM_5:      return KeyCode::Backslash;
        case VK_OEM_3:      return KeyCode::GraveAccent;

        case VK_ESCAPE:     return KeyCode::Escape;
        case VK_RETURN:     return isExtended ? KeyCode::KP_Enter : KeyCode::Enter;
        case VK_TAB:        return KeyCode::Tab;
        case VK_BACK:       return KeyCode::Backspace;
        case VK_INSERT:     return KeyCode::Insert;
        case VK_DELETE:     return KeyCode::Delete;
        case VK_RIGHT:      return KeyCode::Right;
        case VK_LEFT:       return KeyCode::Left;
        case VK_DOWN:       return KeyCode::Down;
        case VK_UP:         return KeyCode::Up;
        case VK_PRIOR:      return KeyCode::PageUp;
        case VK_NEXT:       return KeyCode::PageDown;
        case VK_HOME:       return KeyCode::Home;
        case VK_END:        return KeyCode::End;
        case VK_CAPITAL:    return KeyCode::CapsLock;
        case VK_SCROLL:     return KeyCode::ScrollLock;
        case VK_NUMLOCK:    return KeyCode::NumLock;
        case VK_SNAPSHOT:   return KeyCode::PrintScreen;
        case VK_PAUSE:      return KeyCode::Pause;

        case VK_F1:  return KeyCode::F1;
        case VK_F2:  return KeyCode::F2;
        case VK_F3:  return KeyCode::F3;
        case VK_F4:  return KeyCode::F4;
        case VK_F5:  return KeyCode::F5;
        case VK_F6:  return KeyCode::F6;
        case VK_F7:  return KeyCode::F7;
        case VK_F8:  return KeyCode::F8;
        case VK_F9:  return KeyCode::F9;
        case VK_F10: return KeyCode::F10;
        case VK_F11: return KeyCode::F11;
        case VK_F12: return KeyCode::F12;
        case VK_F13: return KeyCode::F13;
        case VK_F14: return KeyCode::F14;
        case VK_F15: return KeyCode::F15;
        case VK_F16: return KeyCode::F16;
        case VK_F17: return KeyCode::F17;
        case VK_F18: return KeyCode::F18;
        case VK_F19: return KeyCode::F19;
        case VK_F20: return KeyCode::F20;
        case VK_F21: return KeyCode::F21;
        case VK_F22: return KeyCode::F22;
        case VK_F23: return KeyCode::F23;
        case VK_F24: return KeyCode::F24;

        case VK_NUMPAD0:  return KeyCode::KP_0;
        case VK_NUMPAD1:  return KeyCode::KP_1;
        case VK_NUMPAD2:  return KeyCode::KP_2;
        case VK_NUMPAD3:  return KeyCode::KP_3;
        case VK_NUMPAD4:  return KeyCode::KP_4;
        case VK_NUMPAD5:  return KeyCode::KP_5;
        case VK_NUMPAD6:  return KeyCode::KP_6;
        case VK_NUMPAD7:  return KeyCode::KP_7;
        case VK_NUMPAD8:  return KeyCode::KP_8;
        case VK_NUMPAD9:  return KeyCode::KP_9;
        case VK_DECIMAL:  return KeyCode::KP_Decimal;
        case VK_DIVIDE:   return KeyCode::KP_Divide;
        case VK_MULTIPLY: return KeyCode::KP_Multiply;
        case VK_SUBTRACT: return KeyCode::KP_Subtract;
        case VK_ADD:      return KeyCode::KP_Add;

        case VK_SHIFT: {
            UINT scancode = static_cast<UINT>((lParam & 0x00FF0000) >> 16);
            return (MapVirtualKeyW(scancode, MAPVK_VSC_TO_VK_EX) == VK_RSHIFT)
                ? KeyCode::RightShift
                : KeyCode::LeftShift;
        }
        case VK_LSHIFT:   return KeyCode::LeftShift;
        case VK_RSHIFT:   return KeyCode::RightShift;

        case VK_CONTROL:  return isExtended ? KeyCode::RightControl : KeyCode::LeftControl;
        case VK_LCONTROL: return KeyCode::LeftControl;
        case VK_RCONTROL: return KeyCode::RightControl;

        case VK_MENU:     return isExtended ? KeyCode::RightAlt : KeyCode::LeftAlt;
        case VK_LMENU:    return KeyCode::LeftAlt;
        case VK_RMENU:    return KeyCode::RightAlt;

        case VK_LWIN:     return KeyCode::LeftSuper;
        case VK_RWIN:     return KeyCode::RightSuper;
        case VK_APPS:     return KeyCode::Menu;

        default:          return KeyCode::Unknown;
        }
    }
}