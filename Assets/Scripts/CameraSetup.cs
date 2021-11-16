using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraSetup : MonoBehaviour
{
    public Camera cam;
    // Start is called before the first frame update
    void Start()
    {
        cam.usePhysicalProperties = true; //physcial camera        
        cam.transform.eulerAngles = new Vector3(-2f, 75.89f, 91.73f); //rotation
        cam.transform.position = new Vector3(0f, 0f, 0f); //position
        cam.focalLength = 26.8f; //focal length
        cam.sensorSize = new Vector2(16f, 9f); //aspect ratio
    }

    private bool zooming = false;
    private bool panX = false;
    private bool panY = false;
    private bool panZ = false;

    private bool zoomDir = false;
    private bool panXDir = false;
    private bool panYDir = false;
    private bool panZDir = false;

    public float zoomSensitivity = 0.01f; //focal length per frame
    public float panSensitivity = 0.05f; //vector angle per frame

    // Update is called once per frame
    void Update()
    {
        if (zooming)
        {
            if (zoomDir) //true for zoom in, increase focal length
                cam.focalLength += zoomSensitivity;
            else
                cam.focalLength -= zoomSensitivity; //false for zoom out, reduce focal length
        }
        if (panX)
        {
            if (panXDir) //true for left, rotate along Y axis to pan in X direction
            {
                Vector3 newAngle = new Vector3(cam.transform.eulerAngles.x, cam.transform.eulerAngles.y + panSensitivity, cam.transform.eulerAngles.z);
                cam.transform.eulerAngles = newAngle;
            }
            else
            {
                Vector3 newAngle = new Vector3(cam.transform.eulerAngles.x, cam.transform.eulerAngles.y - panSensitivity, cam.transform.eulerAngles.z);
                cam.transform.eulerAngles = newAngle;
            }
        }
        if (panY)
        {
            if (panYDir) //true for up, rotate along X axis to pan in Y direction
            {
                Vector3 newAngle = new Vector3(cam.transform.eulerAngles.x + panSensitivity, cam.transform.eulerAngles.y, cam.transform.eulerAngles.z);
                cam.transform.eulerAngles = newAngle;
            }
            else
            {
                Vector3 newAngle = new Vector3(cam.transform.eulerAngles.x - panSensitivity, cam.transform.eulerAngles.y, cam.transform.eulerAngles.z);
                cam.transform.eulerAngles = newAngle;
            }
        }
        if (panZ)
        {
            if (panZDir) //true for clockwise, increase Z value
            {
                Vector3 newAngle = new Vector3(cam.transform.eulerAngles.x, cam.transform.eulerAngles.y, cam.transform.eulerAngles.z - panSensitivity);
                cam.transform.eulerAngles = newAngle;
            }
            else
            {
                Vector3 newAngle = new Vector3(cam.transform.eulerAngles.x, cam.transform.eulerAngles.y, cam.transform.eulerAngles.z + panSensitivity);
                cam.transform.eulerAngles = newAngle;
            }
        }
    }

    // Starts or stops zooming in the given direction
    public void zoomControl(bool running, bool direction)
    {
        zooming = running;
        zoomDir = direction;
    }

    public void panXControl(bool running, bool direction)
    {
        panX = running;
        panXDir = direction;
    }

    public void panYControl(bool running, bool direction)
    {
        panY = running;
        panYDir = direction;
    }

    public void panZControl(bool running, bool direction)
    {
        panZ = running;
        panZDir = direction;
    }

    // takes in an aruco ID and pans x and y accordingly
    public void centerShotgun(int id)
    {
        //convert id to 0 based first, starting ID in the shotgun marker matrix is 4
        id -=4;
        int row = id / 5; //5x5 matrix starting at top left and going down
        int col = id % 5;

        if (col != 2)
        {
            panXControl(true, col < 2); //less than 3 means go to the left, panX control takes true to left
        }
        else
        {
            panXControl(false, false); //stop panning X
        }

        if (row != 2)
        {
            panYControl(true, row < 2); //less than 3 means go up, panY control takes true to go up
        }
        else
        {
            panYControl(false, false); //stop panning Y
        }
    }
}
