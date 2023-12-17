#include "camera_comm.h"
#include "object_detection.h"

CommSystem commSystem;

rslt resultData;
void setup() {
  Serial.begin(115200);
  init_object_detection();
  commSystem.init_comm();
}

void loop()
{
  if(!commSystem.command_available)
  {
    return;
  }

  commSystem.command_available = false;

  //TODO 未実装
  resultData = detect_object();
  if(resultData.result == false)
  {
    Serial.println("Object detection failed for some reason");
    return;
  }

  esp_err_t result = esp_now_send(masterMACAddress, (uint8_t *) &resultData.resultData, sizeof(resultData.resultData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(500);
}
