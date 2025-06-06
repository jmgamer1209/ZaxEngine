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
    }
}