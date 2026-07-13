using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using ZaxEngine;
using ZaxEngine.InputSystem;

public class UIComponent:ZaxEngine.GameComponent
{
    int test = 100;
    bool updatecall = false;
    string str = "this is string";
    float testf = 20f;
    public event System.Action OnStartGame;

    public UIComponent() 
    {
        AudioManager.Play(Path.Combine(ZaxEngine.Application.contentPath, "Audio", "game-music-loop.mp3"), true);
    }

    void Start()
    {
        ZaxEngine.Debug.Log("Test Start Call");
    }

    void OnColliderEnter(Collider collider)
    {
        ZaxEngine.Debug.Log("Test Collider Enter");
        AudioManager.Play(Path.Combine(ZaxEngine.Application.contentPath, "Audio", "winning-a-coin.wav"), false);
    }

    void OnGui()
    {
        ImGui.SetCursorPos(100, 100);
        if (ImGui.Button("进入游戏", new ZaxEngine.Vector2(150f, 50f)))
        {
            Debug.Log("Test Button Click");
            OnStartGame();
        }
    }

    void Update()
    {
        if (updatecall)
        {
            ZaxEngine.Debug.Log("Test Update Call");
        }
        if (ZaxEngine.InputSystem.Input.GetKeyDown(KeyCode.W))
        {
            var transform = gameObject.GetComponent<Transform>();
            if (transform == null)
            {
                Debug.Log("No Transform!");
            }
            
            //var position = transform.position;
            //position.z = position.z - 1;
            //transform.position = position;
        }
        //if (ZaxEngine.InputSystem.Input.GetKeyDown(KeyCode.S))
        //{
        //    var transform = gameObject.GetComponent<Transform>();
        //    var position = transform.position;
        //    position.z = position.z + 1;
        //    transform.position = position;
        //}
        //if (ZaxEngine.InputSystem.Input.GetKeyDown(KeyCode.A))
        //{
        //    var transform = gameObject.GetComponent<Transform>();
        //    var position = transform.position;
        //    position.x = position.x - 1;
        //    transform.position = position;
        //}
        //if (ZaxEngine.InputSystem.Input.GetKeyDown(KeyCode.D))
        //{
        //    var transform = gameObject.GetComponent<Transform>();
        //    var position = transform.position;
        //    position.x = position.x + 1;
        //    transform.position = position;
        //}
        if (ZaxEngine.InputSystem.Input.GetMouseDown(MouseButton.Left))
        {
            ZaxEngine.Debug.Log("鼠标 left");
        }
        if (ZaxEngine.InputSystem.Input.GetMouseDown(MouseButton.Right))
        {
            ZaxEngine.Debug.Log("mouse right");
        }
    }
}