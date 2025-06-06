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

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Internal_Create(Scene scene);



        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void AddGameObject(GameObject go);

    };
}
