/**************************************************
 * ESP32Cam Freenove ESPNow Transmitter
 * by @hpsaturn Copyright (C) 2024
 * This file is part ESP32S3 camera tests project:
 * https://github.com/hpsaturn/esp32s3-cam
**************************************************/

#include <Arduino.h>
#include "CamFreenove.h"
#include "ESPNowCam.h"
#include "Utils.h"

CamFreenove Camera;
ESPNowCam radio;

void processFrame() {
  if (Camera.get()) {
    uint8_t *out_jpg = NULL;
    size_t out_jpg_len = 0;
    frame2jpg(Camera.fb, 12, &out_jpg, &out_jpg_len);
    radio.sendData(out_jpg, out_jpg_len);
    // printFPS("CAM:");
    free(out_jpg);
    Camera.free();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  delay(1000);

  if (!Camera.begin()) {
    Serial.println("Camera Init Fail");
  }
  Camera.sensor->set_framesize(Camera.sensor, FRAMESIZE_QVGA); 

  if(psramFound()){
    size_t psram_size = esp_spiram_get_size() / 1048576;
    Serial.printf("PSRAM size: %dMb\r\n", psram_size);
  }
  radio.init();
  delay(500);
}

void loop() {
  processFrame();
}
