using System;

namespace ZaxEngine
{
    public struct Color
    {
        float r;
        float g;
        float b;
        float a;

        public Color(float r, float g, float b, float a = 0)
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }

        //float* FloatPTR()
        //{
        //    return (float*)this;
        //}
    };

}
