//
//  Camera.cpp
//  flea3
//
//  Created by Gal Sasson on 5/30/14.
//
//

#include "Camera.h"

Camera::Camera()
{
    dc1394lib = NULL;
    camera = NULL;
}

Camera::~Camera()
{
    flea3_stop();
}

bool Camera::setup()
{
    dc1394lib = dc1394_new ();
    if (!dc1394lib) {
        return false;
    }
    
    if (!flea3_start()) {
        bInitialized = false;
        return false;
    }
    
    return true;
}

void Camera::update()
{
    if (!bInitialized) {
        return;
    }
    
    dc1394error_t err;
    dc1394video_frame_t *frame = NULL;
    
    flea3_set_params();
    
    // capture one frame
    err=dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_WAIT, &frame);
    
    if (err!=DC1394_SUCCESS || frame==NULL) {
        cout<<"error capturing frame"<<endl;
    }
    
    if (frame != NULL)
    {
//        cout<<frame->stride<<endl;
        simpleImage.setFromPixels(frame->image, frame->size[0], frame->size[1], OF_IMAGE_COLOR);
        dc1394_capture_enqueue(camera, frame);
        
        
        camImg.setFromPixels(frame->image, frame->size[0], frame->size[1]);

       // camImg = cvRaw8Img;
       // cvCvtColor(cvRaw8Img.getCvImage(), camImg.getCvImage(), CV_BayerBG2RGB);
//        camImg.flagImageChanged();
//        camImg.updateTexture();
//        img.setFromPixels(frame->image, frame->size[0], frame->size[1], OF_IMAGE_GRAYSCALE);
    }    
}

void Camera::draw()
{
//    cvRaw8Img.draw(0, 0);
  //  camImg.draw(0, 0);
    simpleImage.draw(0, 0);
//    img.draw(0, 0);
}

// returns camera guids
vector<uint64_t> Camera::getDeviceList()
{
    vector<uint64_t> guids;
    dc1394camera_list_t * list;
    dc1394error_t err;
    
    err=dc1394_camera_enumerate (dc1394lib, &list);
    if (err != DC1394_SUCCESS) {
        cout<<"error: dc1394 failed to enumerate cameras"<<endl;
        return guids;
    }
    
    cout<<"Cameras found: "<<list->num<<endl;
    
    for (int i=0; i<list->num; i++)
    {
        guids.push_back(list->ids[i].guid);
    }

    dc1394_camera_free_list (list);
    return guids;
}


bool Camera::flea3_start()
{
    dc1394featureset_t features;
    dc1394framerates_t framerates;
    dc1394video_modes_t video_modes;
    dc1394framerate_t framerate;
    dc1394color_coding_t coding;
//    unsigned int width, height;
//    dc1394video_frame_t *frame;
    
    dc1394error_t err;
    
    vector<uint64_t> guids = getDeviceList();
    if (guids.size() == 0) {
        cout<<"No cameras found"<<endl;
        return false;
    }
    
    
    camera = dc1394_camera_new (dc1394lib, guids[0]);
    if (!camera) {
        cout<<"Failed to initialize camera with guid "<< guids[0]<<endl;
        return false;
    }
    
    cout<<"Using camera with GUID "<< camera->guid<<endl;

    cout<<"Supported format7 modes:"<<endl;
    dc1394format7modeset_t info;
    dc1394_format7_get_modeset(camera, &info);
    for (int i=0; i<8; i++)
    {
        if (info.mode[i].present) {
            dc1394format7mode_t m = info.mode[i];
            cout<<i<<" :"<<endl<<"---"<<endl;
            cout<<m.max_size_x<<"x"<<m.max_size_y<<endl;
            for (int cc=0; cc<m.color_codings.num; cc++) {
                cout<<"Color coding: "<<m.color_codings.codings[cc]<<endl;
            }
            cout<<"Pixel num: "<<m.pixnum<<endl;
            cout<<"Size: "<<m.size_x<<"x"<<m.size_y<<endl;
            
            
        }
    }
    
    /*-----------------------------------------------------------------------
     *  get the best video mode and highest framerate. This can be skipped
     *  if you already know which mode/framerate you want...
     *-----------------------------------------------------------------------*/
    // get video modes:
    err=dc1394_video_get_supported_modes(camera,&video_modes);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Can't get video modes");
    /*
    for (int i=0; i<video_modes.num; i++)
    {
        cout<<"Supported mode: "<<video_modes.modes[i]<<endl;
    }
    
    cout<<"Checking video mode 92"<<endl;
    dc1394_get_color_coding_from_video_mode(camera, video_modes.modes[1], &coding);
    if (coding==DC1394_COLOR_CODING_RAW8) {
        video_mode=video_modes.modes[1];
    }

    if (!video_mode) {
        cout<<"Could not get a valid RAW8 mode"<<endl;
        dc1394_camera_free(camera);
        return false;
    }
    */
    
    video_mode=video_modes.modes[7];
    err = dc1394_format7_set_color_coding(camera, video_mode, DC1394_COLOR_CODING_RGB8);


    err=dc1394_format7_get_color_coding(camera, video_mode,&coding);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not get color coding");
    if (err != DC1394_SUCCESS) {
        return false;
    }
    cout<<"format 7 color coding = "<<coding<<endl;
    
    /*-----------------------------------------------------------------------
     *  setup capture
     *-----------------------------------------------------------------------*/
    
    err=dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_MAX);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not set iso speed");
    if (err != DC1394_SUCCESS) {
        return false;
    }
    
    err=dc1394_video_set_mode(camera, video_mode);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not set video mode");
    if (err != DC1394_SUCCESS) {
        return false;
    }
    
    err=dc1394_video_set_framerate(camera, DC1394_FRAMERATE_60);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not set framerate");
    if (err != DC1394_SUCCESS) {
        return false;
    }
    
    /*-----------------------------------------------------------------------
     *  report camera's features
     *-----------------------------------------------------------------------*/
    err=dc1394_feature_get_all(camera,&features);
    if (err!=DC1394_SUCCESS) {
        dc1394_log_warning("Could not get feature set");
    }
    else {
        dc1394_feature_print_all(&features, stdout);
    }

    unsigned int maxWidth, maxHeight;
    dc1394_format7_get_max_image_size(camera, video_mode, &maxWidth, &maxHeight);
    cout<<"Max size: "<<maxWidth<<"x"<<maxHeight<<endl;
    dc1394_format7_get_image_size(camera, video_mode, &maxWidth, &maxHeight);
    cout<<"Current size: "<<maxWidth<<"x"<<maxHeight<<endl;

//    dc1394_format7_set_packet_size(camera, video_mode, 6596);
    // packet size: 6596
    
    flea3_set_params();
    
    err=dc1394_capture_setup(camera,4, DC1394_CAPTURE_FLAGS_DEFAULT);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not setup camera-\nmake sure that the video mode and framerate are\nsupported by your camera");
    if (err != DC1394_SUCCESS) {
        return false;
    }
    
    /*-----------------------------------------------------------------------
     *  have the camera start sending us data
     *-----------------------------------------------------------------------*/
    err=dc1394_video_set_transmission(camera, DC1394_ON);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not start camera iso transmission");
    if (err != DC1394_SUCCESS) {
        return false;
    }
    
    /*-----------------------------------------------------------------------
     *  get frame size and allocate images
     *-----------------------------------------------------------------------*/
    
    dc1394_get_image_size_from_video_mode(camera, video_mode, &imgWidth, &imgHeight);
    cout<<"Frame size: "<<imgWidth<<"x"<<imgHeight<<endl;
    camImg.allocate(imgWidth, imgHeight);
    cvRaw8Img.allocate(imgWidth, imgHeight);
    frameSize = ofVec2f(imgWidth, imgHeight);
    
    bInitialized = true;
    return true;
}

void Camera::flea3_set_params()
{
    dc1394_format7_set_image_position(camera, video_mode, 40, 118);
    dc1394_format7_set_image_size(camera, video_mode, 960, 540);

    // set parameters
    dc1394_feature_set_mode(camera, DC1394_FEATURE_SHUTTER, DC1394_FEATURE_MODE_MANUAL);
    dc1394_feature_set_value(camera, DC1394_FEATURE_SHUTTER, Params::camShutter);
    
    dc1394_feature_set_mode(camera, DC1394_FEATURE_EXPOSURE, DC1394_FEATURE_MODE_MANUAL);
    dc1394_feature_set_value(camera, DC1394_FEATURE_EXPOSURE, Params::camExposure);

    dc1394_feature_set_mode(camera, DC1394_FEATURE_SHARPNESS, DC1394_FEATURE_MODE_MANUAL);
    dc1394_feature_set_value(camera, DC1394_FEATURE_SHARPNESS, Params::camSharpness);

//    dc1394_feature_set_mode(camera, DC1394_FEATURE_GAMMA, DC1394_FEATURE_MODE_MANUAL);
//    dc1394_feature_set_value(camera, DC1394_FEATURE_GAMMA, Params::camGamma);
//
    //
    //    err = dc1394_feature_set_mode(camera, DC1394_FEATURE_FRAME_RATE, DC1394_FEATURE_MODE_MANUAL);
    //    err = dc1394_feature_set_value(camera, DC1394_FEATURE_FRAME_RATE, 100);
    
    
}

void Camera::flea3_stop()
{
    if (!bInitialized) {
        return;
    }
    
    dc1394error_t err;
    
    /*-----------------------------------------------------------------------
     *  stop data transmission
     *-----------------------------------------------------------------------*/
    err=dc1394_video_set_transmission(camera,DC1394_OFF);
    DC1394_ERR_CLN_RTN(err,cleanup_and_exit(camera),"Could not stop the camera");
    
    /*-----------------------------------------------------------------------
     *  close camera
     *-----------------------------------------------------------------------*/
    dc1394_video_set_transmission(camera, DC1394_OFF);
    dc1394_capture_stop(camera);
    dc1394_camera_free(camera);
    dc1394_free (dc1394lib);
}

void Camera::cleanup_and_exit(dc1394camera_t *camera)
{
    dc1394_video_set_transmission(camera, DC1394_OFF);
    dc1394_capture_stop(camera);
    dc1394_camera_free(camera);
}




