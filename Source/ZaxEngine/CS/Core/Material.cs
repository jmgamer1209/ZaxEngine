using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public class Material:ZObject
    {
        public ShaderProgram shaderProgram;
        public Material(ShaderProgram shaderProgram)
        {
            this.shaderProgram = shaderProgram;
            Internal_Create_WithShader(this, shaderProgram.GetNativePtr());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_Create_WithShader(Material mat, IntPtr shaderPtr);
    }
}
