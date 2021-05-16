using System.Runtime.CompilerServices;

namespace Sand
{

	public enum Mousecode
	{
        Left, Right, Middle
	}
    public enum Keycode
    {
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,

        D0 = 48,
        D1 = 49,
        D2 = 50,
        D3 = 51,
        D4 = 52,
        D5 = 53,
        D6 = 54,
        D7 = 55,
        D8 = 56,
        D9 = 57,

        Semicolon = 59,
        Equal = 61,

        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,

        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        GraveAccent = 96,

        World1 = 161,
        World2 = 162,

        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,

        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,

        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };

    public static class Input
	{
        /// <param name="code">The key to check</param>
        /// <returns>Whether or not the key <paramref name="code"/> is currently held down.</returns>
        public static bool IsKeyPressed(Keycode code)
		{
            return IsKeyPressed_Native(code);
		}
        /// <param name="code">The key to check</param>
        /// <returns>Whether or not the key <paramref name="code"/> was pressed this frame.</returns>
        public static bool WasKeyPressed(Keycode code)
        {
            return WasKeyPressed_Native(code);
        }
        /// <param name="code">The button to check</param>
        /// <returns>Whether or not the mouse button <paramref name="code"/> is currently held down.</returns>
        public static bool IsKeyPressed(Mousecode code)
        {
            return IsMousePressed_Native(code);
        }
        /// <param name="code">The button to check</param>
        /// <returns>Whether or not the mouse button <paramref name="code"/> was pressed this frame.</returns>
        public static bool WasKeyPressed(Mousecode code)
        {
            return WasMousePressed_Native(code);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static bool IsKeyPressed_Native(Keycode code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static bool WasKeyPressed_Native(Keycode code);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static bool IsMousePressed_Native(Mousecode code);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static bool WasMousePressed_Native(Mousecode code);
    }

}
