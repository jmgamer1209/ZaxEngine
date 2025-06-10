using System.ComponentModel;
using ZaxEngine;

public class GameEntry
{
    public static void GameStart()
    {
        var scene = new Scene();
        var mesh = new Mesh(Path.Combine(Application.contentPath, "Common", "WoodenCrate", "Wooden Crate.obj"));
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
        //skyboxGO->AddComponent();
        //auto skybox = new Skybox();
        //skyboxGO->AddComponent(skybox);
    }
}