using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace ZaxEngine
{
    public class Mesh:ZObject
    {
        public Mesh(string path)
        {
            Internal_Create_Path(this, path);
        }

        public static Mesh GetQuadMesh()
        {
            return Internal_GetQuadMesh();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_Create_Path(Mesh obj,string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Mesh Internal_GetQuadMesh();
    }
}
