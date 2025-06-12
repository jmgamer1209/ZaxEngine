using System;
using System.Runtime.CompilerServices;

namespace ZaxEngine
{
    public class Transform:Component
    {
        public Vector3 position
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;
        }
        public Vector3 rotation 
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get; 
            [MethodImpl(MethodImplOptions.InternalCall)]
            set; 
        }
        public Vector3 scale { 
            [MethodImpl(MethodImplOptions.InternalCall)]
            get; 
            [MethodImpl(MethodImplOptions.InternalCall)]
            set; 
        }

        public Transform()
        {
            Internal_Create(this);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_Create(Transform obj);
    }
}
