using System;
using System.Runtime.CompilerServices;

namespace ZaxEngine
{
    public class Transform:Component
    {
        public Vector3 position;
        public Vector3 rotation;
        public Vector3 scale;

        public Transform()
        {
            Internal_Create(this);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_Create(Transform obj);
    }
}
