using System.ComponentModel;
using ZaxEngine;

public class GameEntry
{
    public static void GameStart()
    {
        var scene = new Scene();
        var mesh = new Mesh(Path.Combine(Application.contentPath, "Common", "WoodenCrate", "Wooden Crate.obj"));
    }
}