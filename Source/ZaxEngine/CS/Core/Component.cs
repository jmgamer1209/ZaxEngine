using System;
using System.Runtime.CompilerServices;

namespace ZaxEngine
{
    public class Component:ZObject
    {
        public GameObject gameObject;
        public bool enabled 
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;
        }
    }
}
