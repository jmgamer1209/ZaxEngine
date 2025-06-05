using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace ZaxEngine
{
    public class Mesh:Object
    {
        public Mesh(string path)
        {
            Internal_Create_Path(this, path);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_Create_Path(Mesh scene,string path);
    }
}
