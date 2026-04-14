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
    }
}
