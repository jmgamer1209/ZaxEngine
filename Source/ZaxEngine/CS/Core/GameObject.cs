using System;
using System.Runtime.CompilerServices;


namespace ZaxEngine
{
    public class GameObject:Object
    {
        public List<Component> components = new List<Component>();
        public string name;
        public bool isActive = true;

        public void AddComponent(Component comp)
        {
            comp.gameObject = this;
            components.Add(comp);
        }


        #region Internal Call

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Component Internal_AddComponentWithType(Type componentType);

        #endregion
    }
}
