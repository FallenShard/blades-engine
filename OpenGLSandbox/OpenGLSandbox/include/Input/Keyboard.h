#pragma once

namespace fsi
{

class Keyboard
{
public:

    enum Key
    {
        Unknown = -1,
        Escape = 0,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        PrintScreen,
        ScrollLock,
        Pause,
        Tilde,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,
        Num0,
        UnderScore,
        Equals,
        BackSpace,
        Tab,
        Q,
        W,
        E,
        R,
        T,
        Y,
        U,
        I,
        O,
        P,
        LeftBracket,
        RightBracket,
        BackSlash,
        CapsLock,
        A,
        S,
        D,
        F,
        G,
        H,
        J,
        K,
        L,
        SemiColon,
        Quote,
        Return,
        LeftShift,
        Z,
        X,
        C,
        V,
        B,
        N,
        M,
        Comma,
        Period,
        FrontSlash,
        RightShift,
        LeftControl,
        LeftSystem,
        LeftAlt,
        Space,
        RightAlt,
        RightSystem,
        Menu,
        RightControl,
        Insert,
        Home,
        PageUp,
        Delete,
        End,
        PageDown,
        Up,
        Left,
        Down,
        Right,
        NumLock,
        Divide,
        Multiply,
        Subtract,
        NumPad7,
        NumPad8,
        NumPad9,
        Add,
        NumPad4,
        NumPad5,
        NumPad6,
        NumPad1,
        NumPad2,
        NumPad3,
        NumPad0,
        NumPadDecimal,

        KeyCount      
    };

    static bool isKeyPressed(Key key);
};

}