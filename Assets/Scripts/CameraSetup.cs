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

    // Update is called once per frame
    void Update()
    {
        
    }
}
