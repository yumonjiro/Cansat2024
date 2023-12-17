#ifndef __CAMERAA_COMM__
#define __CAMERAA_COMM__

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

// for communication system
typedef struct {
  bool isTargetFound;
  float value;
  float x;
  float y;
} result_message;
result_message not_found = {false, 0, 0, 0};

typedef struct {
  bool result;
  result_message resultData; 
} rslt;

rslt result_failed = { false, not_found};

extern uint8_t masterMACAddress[];
extern uint8_t cameraMACAddress[];

typedef enum camera_mode
{
  object_detection = 0,
  color_detection = 1,
} camera_mode;
camera_mode camera_mode_default = camera_mode::object_detection;
typedef struct
{
  camera_mode mode;
} camera_command;
camera_command command_default = {camera_mode_default};

class CommSystem
{
  public:
    static bool command_available;
    static camera_command command_received;
    static esp_now_peer_info_t peerInfo;

    // Callback when data is sent
    
    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

    // Callback when data is received
    static void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

    void init_comm();
};

#endif