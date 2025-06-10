using System;
using System.Runtime.CompilerServices;


namespace ZaxEngine
{
    public class GameObject:ZObject
    {
        public List<Component> components = new List<Component>();
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
            components.Add(comp);
        }


        #region Internal Call

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Component Internal_AddComponentWithType(Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_Create();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_CreateWithName(string name);

        #endregion
    }
}
