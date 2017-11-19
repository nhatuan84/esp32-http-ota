#include <WiFi.h>
#include <HttpOTA.h>
#include <mySD.h>

const char* ssid = "dd-wrt";
const char* password = "0000000000";
File file;

void saveData(uint8_t *buffer, int bytes){
  file.write(buffer, bytes);
}
int readData(uint8_t *buffer, int bytes){
  return file.read(buffer, bytes);
}
void progress(DlState state, int percent){
  Serial.printf("state = %d - percent = %d\n", state, percent);
}
void error(char *message){
  printf("%s\n", message);
}

void startDl(void){
  //write bin file to sdcard
  SD.remove("fw.bin");
  file = SD.open("fw.bin", FILE_WRITE);
}
void endDl(void){
  file.close();
}
void startFl(void){
  //write bin file to sdcard
  file = SD.open("fw.bin", FILE_READ);
}
void endFl(void){
  file.close();
}
void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Connecting to "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  
  Serial.print("WiFi connected, IP address: "); 
  Serial.println(WiFi.localIP());
  
  if (!SD.begin(32, 14, 12, 27)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  DlInfo info;
  info.url = "http://192.168.1.107/upload/led.bin";
  info.md5 = "6bd07139c21f572370242905c4465056";
  info.startDownloadCallback =  startDl;
  info.endDownloadCallback =    endDl;
  info.startFlashingCallback =  startFl;
  info.endFlashingCallback =    endFl;
  
  info.saveDataCallback = saveData;
  info.readDataCallback = readData;
  info.progressCallback  = progress;
  info.errorCallback     = error;
  httpOTA.start(info);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
