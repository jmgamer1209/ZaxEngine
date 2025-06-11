using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public class Skybox:Component
    {
        public Skybox()
        {
            Internal_Create(this);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_Create(Skybox obj);

        public void SetCubeMap(Texture tex)
        {
            Internal_SetCubeMap(GetNativePtr(), tex.GetNativePtr());
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_SetCubeMap(IntPtr skybox, IntPtr tex);
    }
}
