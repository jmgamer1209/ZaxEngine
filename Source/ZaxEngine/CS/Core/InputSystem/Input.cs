using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine.InputSystem
{
    public enum KeyCode: int
    {
        W = 87,
        A = 65,
        S = 83,
        D = 68
    }

    public enum MouseButton: int
    {
        Left = 0,
        Right = 1
    }

    public static class Input
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyDown(KeyCode key);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetMouseDown(MouseButton key);
    }
}
