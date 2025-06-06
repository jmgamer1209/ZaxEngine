using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public class ShaderProgram:Object
    {
        public ShaderProgram(string vertexPath, string fragmentPath)
        {
            Internal_Create_Path(this, vertexPath, fragmentPath);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_Create_Path(ShaderProgram shaderProgram, string vertexPath, string fragmentPath);
    }
}
