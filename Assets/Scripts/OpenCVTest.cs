using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class OpenCVTest : MonoBehaviour
{
    public int port = 0;

    private bool _ready;

    // Start is called before the first frame update
    void Start()
    {
        int result = OpenCVInterop.Init(ref port);
        if (result == -1)
        {
            Debug.LogWarningFormat("[{0}] Failed to open the camera", GetType());
            return;
        }

        _ready = true;
    }

    // Update is called once per frame
    void Update()
    {
        if (!_ready)
            return;

        OpenCVInterop.Detect();
    }

    void OnApplicationQuit()
    {
        if (_ready)
        {
            OpenCVInterop.Close();
        }
    }
}

// Define the functions which can be called from the .dll.
internal static class OpenCVInterop
{
    [DllImport("CalibrationCamera")]
    internal static extern int Init(ref int capturePort);

    [DllImport("CalibrationCamera")]
    internal static extern int Close();

    [DllImport("CalibrationCamera")]
    internal static extern void Detect();
}
