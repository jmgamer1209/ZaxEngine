using System;


namespace ZaxEngine
{
    public class GameObject
    {
        public List<Component> components = new List<Component>();
        public string name;
        public bool isActive = true;

        public void AddComponent(Component comp)
        {
            comp.gameObject = this;
            components.Add(comp);
        }
    }
}
