using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ZaxEngine;
using ZaxEngine.InputSystem;

public class TestComponent:ZaxEngine.GameComponent
{
    int test = 100;
    bool updatecall = false;
    string str = "this is string";
    float testf = 20f;

    public TestComponent() 
    {

    }

    void Update()
    {
        if (updatecall)
        {
            ZaxEngine.Debug.Log("Test Update Call");
        }
        if (ZaxEngine.InputSystem.Input.GetKeyDown(KeyCode.W))
        {
            ZaxEngine.Debug.Log("input w");
        }
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