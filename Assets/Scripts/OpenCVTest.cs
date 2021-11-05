using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

// Good azure camera expoure: 16670, lights at 2 dots

public class OpenCVTest : MonoBehaviour
{
    public int port = 0;
    public CameraSetup cameraCtrl;

    private bool _ready = false;

    // Start is called before the first frame update
    void Start()
    {
        CaptureCamera();
    }

    // Update is called once per frame
    void Update()
    {
        if (!_ready)
            return;

        // Detect markers and draw on borders
        OpenCVInterop.Detect();

        double xRes = OpenCVInterop.xDistance();
        // If xRes is negative, marker 0 is right of marker 3
        if (xRes > 0)
        {
            // Go left
            cameraCtrl.panXControl(true, true);
        }
        else if (xRes < 0)
        {
            // Go right
            cameraCtrl.panXControl(true, false);
        }
        else
        {
            // Don't move on the x axis
            Debug.Log("Stop panning X");
            cameraCtrl.panXControl(false, false);
        }

        double yRes = OpenCVInterop.yDistance();
        // If yRes is negative, marker 1 is below marker 3
        if (yRes > 0)
        {
            // Go up
            cameraCtrl.panYControl(true, true);
        }
        else if (yRes < 0)
        {
            // Go down
            cameraCtrl.panYControl(true, false);
        }
        else
        {
            // Don't move on the y axis
            Debug.Log("Stop panning Y");
            cameraCtrl.panYControl(false, false);
        }

        Debug.Log($"X Distance: {xRes}");
        Debug.Log($"Y Distance: {yRes}");
    }

    void OnApplicationQuit()
    {
        if (_ready)
        {
            OpenCVInterop.Close();
        }
    }

    void OnEnable()
    {
        if (!_ready) //add check to see if camera is open by another application, if yes, return error to make sure unity doesn't crashf
        {
            CaptureCamera();
        }
    }

    void OnDisable()
    {
        if (_ready)
        {
            OpenCVInterop.Close();

            _ready = false;
        }
    }

    private void CaptureCamera()
    {
        int result = OpenCVInterop.Init(ref port);
        if (result == -1)
        {
            Debug.LogWarningFormat("[{0}] Failed to open the camera", GetType());
            return;
        }

        _ready = true;
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

    [DllImport("CalibrationCamera")]
    internal static extern double xDistance();

    [DllImport("CalibrationCamera")]
    internal static extern double yDistance();
}
