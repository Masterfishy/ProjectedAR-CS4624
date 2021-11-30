using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

// Good azure camera expoure: 16670, lights at 2 dots in the perform studio if lighting becomes an issue.

public class Calibrator : MonoBehaviour
{
    public int capturePort = 0;
    public CameraSetup cameraCtrl;

    public double panThreshold = 0.0;
    public double zoomThreshold = 0.0;

    private bool ready = false;
    private bool finishedCenteringScatter = false;
    private bool finishedCentering = false;
    private bool doPan = true;

    // Start is called before the first frame update
    private void Start()
    {
        CaptureCamera();
    }

    // Update is called once per frame
    private void Update()
    {
        if (!ready)
            return;

        // Detect markers and draw on borders
        OpenCVInterop.Detect();

        #region Scatter Board
        //////////////////////////////
        // Center the scatter board //
        //////////////////////////////

        if (!finishedCenteringScatter)
        {
            int seenId = OpenCVInterop.GetSeenId();

            cameraCtrl.centerShotgun(seenId);
            cameraCtrl.panSensitivity = 0.05f;

            finishedCenteringScatter = seenId == 16;
            return;
        }
        #endregion

        #region Panning and Zooming
        ////////////////////////////
        // Pan and zoom marker 16 //
        ////////////////////////////

        if (!finishedCentering)
        {
            double xRes = OpenCVInterop.xOffset();
            double yRes = OpenCVInterop.yOffset();
            double scaleDif = OpenCVInterop.ScaleDifference();

            if (doPan)
            {
                // Pan maker 16 towards the center of the calibration board.

                Debug.Log("Panning...");

                float distance = Mathf.Sqrt((float)(xRes * xRes + yRes * yRes));
                if (distance < 10)
                {
                    cameraCtrl.panSensitivity = 0.01f;
                }

                // If xRes is greater than the threshold, marker 16 is right of the center
                if (xRes > panThreshold)
                {
                    // Go left
                    cameraCtrl.panXControl(true, true);
                }
                else if (xRes < -panThreshold)
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

                // If yRes is greater than the threshold, marker 16 is below the center
                if (yRes > panThreshold)
                {
                    // Go up
                    cameraCtrl.panYControl(true, true);
                }
                else if (yRes < -panThreshold)
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

                // Stop panning if both xRes and yRes are within the threshold
                doPan = !(WithinThreshold(xRes, panThreshold) && WithinThreshold(yRes, panThreshold));
            }

            if (!doPan)
            {
                Debug.Log("Zooming...");

                if (Mathf.Abs((float)scaleDif) < 2)
                {
                    cameraCtrl.zoomSensitivity = 0.01f;
                }

                // If scaleDif is greater than the threshold, marker 16 is too small
                if (scaleDif > zoomThreshold)
                {
                    // Zoom in
                    cameraCtrl.zoomControl(true, true);
                }
                else if (scaleDif < -zoomThreshold)
                {
                    // Zoom out
                    cameraCtrl.zoomControl(true, false);
                }
                else
                {
                    Debug.Log("Stop zooming");
                    cameraCtrl.zoomControl(false, false);
                }

                Debug.Log("Scale Difference: " + scaleDif);

                // Stop zoomming if scaleDif is within the threshold
                doPan = WithinThreshold(scaleDif, zoomThreshold);
            }

            // Finish panning and zooming if all values are within the desired thresholds
            finishedCentering = WithinThreshold(scaleDif, zoomThreshold) && WithinThreshold(xRes, panThreshold) && WithinThreshold(yRes, panThreshold);
        }
        else // Finished calibrating
        {
            Debug.Log("Calibrated");
        }
        #endregion
    }

    // Release the camera when the game ends.
    private void OnApplicationQuit()
    {
        if (ready)
        {
            OpenCVInterop.Close();
        }
    }

    // Capture the camera when this script is enabled.
    private void OnEnable()
    {
        if (!ready)
        {
            CaptureCamera();
        }

        finishedCenteringScatter = false;
        finishedCentering = false;
    }

    // Release the camera when this script is disabled.
    private void OnDisable()
    {
        if (ready)
        {
            OpenCVInterop.Close();

            ready = false;
        }

        cameraCtrl.panXControl(false, false);
        cameraCtrl.panYControl(false, false);
        cameraCtrl.zoomControl(false, false);
    }

    // Capture the camera so that OpenCV functions can work.
    private void CaptureCamera()
    {
        int result = OpenCVInterop.Init(ref capturePort);
        if (result == -1)
        {
            Debug.LogWarningFormat("[{0}] Failed to open the camera", GetType());
            return;
        }

        ready = true;
    }

    // Determine if value is below the threshold
    private bool WithinThreshold(double value, double threshold)
    {
        return Mathf.Abs((float)value) <= threshold;
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
    internal static extern int GetSeenId();

    [DllImport("CalibrationCamera")]
    internal static extern double ScaleDifference();
}
