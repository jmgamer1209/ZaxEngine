using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public static class ImGui
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool Button(string label);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool Button(string label, Vector2 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool SetCursorPos(float x, float y);
    }
}
