using System;
using System.Runtime.CompilerServices;

namespace ZaxEngine
{
    public struct SceneLighting
    {
        public Color ambientColor;
        public float ambientIntensity;
    };

    public class Scene : ZObject
    {
        public List<GameObject> list = new List<GameObject>();
        public SceneLighting lightingSettings;

        public Scene()
        {
            Internal_Create(this);
        }

        public void AddGameObject(GameObject go)
        {
            Internal_AddGameObject(go);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_Create(Scene scene);


        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Internal_AddGameObject(GameObject go);

    };
}
