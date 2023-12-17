#include "camera_comm.h"

uint8_t masterMACAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t cameraMACAddress[] = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

bool CommSystem::command_available;
camera_command CommSystem::command_received;
esp_now_peer_info_t CommSystem::peerInfo;

void CommSystem::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    Serial.println("Delivery Success :)");
  }
  else{
    Serial.println("Delivery Fail :(");
  }
}
// Callback when data is received
void CommSystem::OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&CommSystem::command_received, incomingData, sizeof(CommSystem::command_received));
  Serial.print("Bytes received: ");
  Serial.println(len);

  //command_receivedは一時的に読み取る用で、この関数外からは使わない
  CommSystem::command_available = true;
}
void CommSystem::init_comm()
{
  command_available = false;
  
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  memcpy(peerInfo.peer_addr, masterMACAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}