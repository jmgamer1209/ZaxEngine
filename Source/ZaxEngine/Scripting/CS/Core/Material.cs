using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
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

        public void SetProperty(string name, Texture value)
        {
            Internal_SetProperty_Texture(GetNativePtr(), name, value.GetNativePtr());
        }

        public void SetProperty(string name, float value)
        {
            Internal_SetProperty_Float(GetNativePtr(), name, value);
        }

        public void SetProperty(string name, int value)
        {
            Internal_SetProperty_Int(GetNativePtr(), name, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_SetProperty_Texture(IntPtr mat,string name,IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_SetProperty_Float(IntPtr mat, string name, float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_SetProperty_Int(IntPtr mat, string name, int value);
    }
}
