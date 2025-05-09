using System;

namespace ZaxEngine
{
    public struct SceneLighting
    {
        public Color ambientColor;
        public float ambientIntensity;
    };

    public class Scene : Object
    {
        public List<GameObject> list = new List<GameObject>();
        public SceneLighting lightingSettings;

        public void AddGameObject(GameObject go)
        {
            list.Add(go);
        }
};
}
