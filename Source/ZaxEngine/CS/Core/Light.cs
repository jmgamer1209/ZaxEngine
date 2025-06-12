using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace ZaxEngine
{
    public enum LightType:byte
    {
        Directional = 0,
        Point = 1,
        Spot = 2
    }

    public class Light:Component
    {
        public Color color
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;
        }
        public float range {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set; 
        }
        public float innerAngle {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set; 
        }
        public float outerAngle {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set; 
        }
        public float shadowDepthBias {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set; 
        }

        public Light(LightType lightType)
        {
            Internal_Create_WithType(lightType);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void Internal_Create_WithType(LightType lightType);

    }
}
