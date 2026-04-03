using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public class Camera:Component
    {
        public Camera() 
        {
            Internal_Create();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void Internal_Create();
    }
}
