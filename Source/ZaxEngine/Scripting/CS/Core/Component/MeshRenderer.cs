using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public class MeshRenderer : Component
    {
        public MeshRenderer(Mesh mesh, Material mat)
        {
            Internal_Create_WithMeshMat(mesh.GetNativePtr(), mat.GetNativePtr());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void Internal_Create_WithMeshMat(IntPtr meshPtr, IntPtr matPtr);
    }
}
