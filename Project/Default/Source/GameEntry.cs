using System.ComponentModel;
using ZaxEngine;

public class GameEntry
{
    public static void GameStart()
    {
        var scene = new Scene();
        var woodenBox = new Mesh(Path.Combine(Application.contentPath, "Common", "WoodenCrate", "Wooden Crate.obj"));
        //Application::contentPath / "Shaders" / "Common" / "forward.vs", Application::contentPath / "Shaders" / "Common" / "forward.fs"
        var shaderProgram = new ShaderProgram(Path.Combine(Application.contentPath, "Shaders", "Common", "forward.vs"), Path.Combine(Application.contentPath, "Shaders", "Common", "forward.fs"));
        var mat = new Material(shaderProgram);

        // 加载纹理
        string baseColorPath = Path.Combine(Application.contentPath, "Common", "WoodenCrate", "Textures", "Wooden Crate_Crate_BaseColor.png");
        string normalPath = Path.Combine(Application.contentPath, "Common", "WoodenCrate", "Textures", "Wooden Crate_Crate_Normal.png");
        var woodenBoxAlbedoTexture = Texture.Load(baseColorPath);
        var woodenBoxNormalTexture = Texture.Load(normalPath);

        mat.SetProperty("AlbedoTexture", woodenBoxAlbedoTexture);
        mat.SetProperty("NormalMap", woodenBoxNormalTexture);
        mat.SetProperty("Test", 10);

        Material planeMat = new Material(shaderProgram);
        planeMat.SetProperty("AlbedoTexture", woodenBoxAlbedoTexture);

        Material transparentMat = new Material(shaderProgram);
        transparentMat.SetProperty("AlbedoTexture", woodenBoxAlbedoTexture);
        transparentMat.SetProperty("NormalMap", woodenBoxNormalTexture);
        transparentMat.SetProperty("SurfaceType", 1);
        transparentMat.SetProperty("Alpha", 0.5f);

        // 先创建天空盒
        var skyboxGO = new GameObject("Skybox");
        var t = new Transform();
        skyboxGO.AddComponent(t);
        var skybox = new Skybox();
        var folderPath = Path.Combine(Application.contentPath, "Common", "Skybox" , "1");
        var cubeMapTex = Texture.Load(folderPath, TextureType.CubeMap);
        skybox.SetCubeMap(cubeMapTex);
        skyboxGO.AddComponent(skybox);

        // 反射材质
        var reflectionShader = new ShaderProgram(Path.Combine(Application.contentPath,"Shaders", "Common" , "reflectionCube.vs"), Path.Combine(Application.contentPath,"Shaders" , "Common" , "reflectionCube.fs"));
        var reflectionMat = new Material(reflectionShader);
        var albedoTexture = woodenBoxAlbedoTexture;
        reflectionMat.SetProperty("albedoTexture", albedoTexture);
        reflectionMat.SetProperty("environmentTexture", cubeMapTex);
        reflectionMat.SetProperty("specularIntensity", 0.5f);
        reflectionMat.SetProperty("reflectionIntensity", 0.5f);

        // 创建渲染物体
        List<GameObject> boxes = new List<GameObject>(4);
        for (int i = 1; i <= 4; i++)
        {
            var box = new GameObject("Box"+i);
            boxes.Add(box);
            box.AddComponent(new Transform());
            if (i == 3) box.AddComponent(new MeshRenderer(woodenBox, transparentMat));
            else if (i < 4) box.AddComponent(new MeshRenderer(woodenBox, mat));
            else box.AddComponent(new MeshRenderer(woodenBox, reflectionMat));

            //Vector3 position(0, 0, 0);
            //Vector3 rotation(0, 0, 0);

        //if (i == 1)
        //{
        //    position = Vector3(-11.0f, -2.5f, -8.0f);
        //    rotation = Vector3(45.0f, -30.0f, 0);
        //}
        //else if (i == 2)
        //{
        //    position = Vector3(-2.5f, -2.5f, 0);
        //    rotation = Vector3(45.0f, 0, 0);
        //}
        //else if (i == 3)
        //{
        //    position = Vector3(6.5f, -2.5f, 0);
        //    rotation = Vector3(45.0f, 0, -30.0f);
        //}
        //else if (i == 4)
        //{
        //    position = Vector3(4.5f, 2.0f, -2.0f);
        //    rotation = Vector3(0, 35.0f, 0);
        //}
        //box->GetComponent<Transform>()->position = position;
        //box->GetComponent<Transform>()->rotation = rotation;
        }
    }
}