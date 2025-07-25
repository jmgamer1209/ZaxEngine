using System;
using System.Runtime.CompilerServices;


namespace ZaxEngine
{
    public class GameObject:ZObject
    {
        public string name;
        public bool isActive = true;

        public GameObject()
        {

        }

        public GameObject(string name)
        {
            this.name = name;
            Internal_CreateWithName(name);
        }

        public void AddComponent(Component comp)
        {
            comp.gameObject = this;
            Internal_AddComponent(GetNativePtr(), comp.GetNativePtr());
        }

        public T GetComponent<T>() where T : Component
        {
            var component = Internal_GetComponent(typeof(T));
            return (T)component;
        }

        #region Internal Call

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Component Internal_AddComponentWithType(Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_AddComponent(IntPtr gameobjectPtr, IntPtr componentPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_Create();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_CreateWithName(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Component Internal_GetComponent(System.Type type);

        #endregion
    }
}
