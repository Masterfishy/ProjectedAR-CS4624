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
    private bool _finishedCenteringShotgun = false;
    private bool _finishedCentering = false;
    private bool doPan = true;

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

        #region
        ///////////////////////////////
        // Center the shotgun spread //
        ///////////////////////////////
        
        if (!_finishedCenteringShotgun)
        {
            int seenId = OpenCVInterop.GetSeenId();

            cameraCtrl.centerShotgun(seenId);
            cameraCtrl.panSensitivity = 0.05f;

            _finishedCenteringShotgun = seenId == 16;
            return;
        }
        #endregion

        #region
        //////////////////
        // Pan marker16 //
        //////////////////

        if (!_finishedCentering)
        {
            if (doPan)
            {
                double xRes = OpenCVInterop.xOffset();
                double yRes = OpenCVInterop.yOffset();

                float distance = Mathf.Sqrt((float)(xRes * xRes + yRes * yRes));
                if(distance < 100)
                {
                    cameraCtrl.panSensitivity = 0.01f;
                }
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

                doPan = !(xRes == 0 && yRes == 0);
            }

            if (!doPan)
            {
                double scaleDif = OpenCVInterop.ScaleDifference();
                if(Mathf.Abs(scaleDif) < 10)
                {
                    cameraCtrl.zoomSensitivity = 0.01f;
                }

                if (scaleDif > 0)
                {
                    cameraCtrl.zoomControl(true, true);
                }
                else if (scaleDif < 0)
                {
                    cameraCtrl.zoomControl(true, false);
                }
                else
                {
                    Debug.Log("Stop zooming");
                    cameraCtrl.zoomControl(false, false);
                }

                Debug.Log("Scale Diff: " + scaleDif);
                doPan = scaleDif == 0;
            }

            //double botRes = OpenCVInterop.BotCornerOffset();
            //if (botRes > 0)
            //{
            //    cameraCtrl.zoomControl(true, true);
            //}
            //else if (botRes < 0)
            //{
            //    cameraCtrl.zoomControl(true, false);
            //}
            //else
            //{
            //    Debug.Log("Stop zooming");
            //    cameraCtrl.zoomControl(false, false);
            //}

            //_finishedCentering = xRes == 0 && yRes == 0;
        }
        #endregion
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

        cameraCtrl.panXControl(false, false);
        cameraCtrl.panYControl(false, false);
        cameraCtrl.zoomControl(false, false);
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
    internal static extern double xOffset();

    [DllImport("CalibrationCamera")]
    internal static extern double yOffset();

    [DllImport("CalibrationCamera")]
    internal static extern double TopCornerOffset();

    [DllImport("CalibrationCamera")]
    internal static extern double BotCornerOffset();

    [DllImport("CalibrationCamera")]
    internal static extern int GetSeenId();

    [DllImport("CalibrationCamera")]
    internal static extern double ScaleDifference();
}
