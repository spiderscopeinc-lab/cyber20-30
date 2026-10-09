/*
 CYBER 2.0 BLE motor starter. ESP32 Arduino core 3.x.
 Pin map: TB6612 AIN1=25 AIN2=26 PWMA=27; BIN1=14 BIN2=12 PWMB=13; STBY=33.
 Battery + -> VM, battery - -> GND, ESP32 GND -> same GND, VCC logic -> 3V3.
 Check motor current and wiring before powering. Test wheels lifted.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
const char* DEVICE_NAME="CYBER 2.0";
const char* SERVICE_UUID="0000ffe0-0000-1000-8000-00805f9b34fb";
const char* CHARACTERISTIC_UUID="0000ffe1-0000-1000-8000-00805f9b34fb";
const int AIN1=25,AIN2=26,PWMA=27,BIN1=14,BIN2=12,PWMB=13,STBY=33;
const uint32_t COMMAND_TIMEOUT_MS=900;
bool connected=false,emergency=false,autopilot=false;
int speedValue=90; uint32_t lastMotion=0; String lastCmd="S";
void motor(int a,int b,int pwm,int v,bool invert){if(invert)v=-v;v=constrain(v,-255,255);digitalWrite(a,v>0);digitalWrite(b,v<0);ledcWrite(pwm,abs(v));}
void stopMotors(){motor(AIN1,AIN2,PWMA,0,false);motor(BIN1,BIN2,PWMB,0,true);digitalWrite(STBY,LOW);lastCmd="S";}
void drive(int l,int r){if(emergency){stopMotors();return;}digitalWrite(STBY,HIGH);motor(AIN1,AIN2,PWMA,l,false);motor(BIN1,BIN2,PWMB,r,true);}
void handle(String c){c.trim();if(!c.length())return;if(c=="E"){emergency=true;autopilot=false;stopMotors();return;}if(c=="RESET"){emergency=false;stopMotors();return;}if(c.startsWith("V")){speedValue=constrain(c.substring(1).toInt(),0,255);return;}if(c=="A1"){if(!emergency)autopilot=true;stopMotors();return;}if(c=="A0"){autopilot=false;stopMotors();return;}if(c=="H"){stopMotors();return;}if(emergency){stopMotors();return;}if(c=="F"){drive(speedValue,speedValue);lastCmd="F";}else if(c=="B"){drive(-speedValue,-speedValue);lastCmd="B";}else if(c=="L"){drive(-speedValue,speedValue);lastCmd="L";}else if(c=="R"){drive(speedValue,-speedValue);lastCmd="R";}else if(c=="S"){stopMotors();}if(c=="F"||c=="B"||c=="L"||c=="R")lastMotion=millis();}
class ServerCallbacks:public BLEServerCallbacks{void onConnect(BLEServer*)override{connected=true;stopMotors();}void onDisconnect(BLEServer*)override{connected=false;autopilot=false;stopMotors();delay(100);BLEDevice::startAdvertising();}};
class CharCallbacks:public BLECharacteristicCallbacks{void onWrite(BLECharacteristic*c)override{std::string v=c->getValue();handle(String(v.c_str()));}};
void setup(){Serial.begin(115200);pinMode(AIN1,OUTPUT);pinMode(AIN2,OUTPUT);pinMode(BIN1,OUTPUT);pinMode(BIN2,OUTPUT);pinMode(STBY,OUTPUT);digitalWrite(STBY,LOW);ledcAttach(PWMA,18000,8);ledcAttach(PWMB,18000,8);stopMotors();BLEDevice::init(DEVICE_NAME);BLEServer*s=BLEDevice::createServer();s->setCallbacks(new ServerCallbacks());BLEService*sv=s->createService(SERVICE_UUID);BLECharacteristic*c=sv->createCharacteristic(CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE|BLECharacteristic::PROPERTY_WRITE_NR);c->setCallbacks(new CharCallbacks());c->setValue("READY");sv->start();BLEAdvertising*a=BLEDevice::getAdvertising();a->addServiceUUID(SERVICE_UUID);a->setScanResponse(true);BLEDevice::startAdvertising();lastMotion=millis();Serial.println("CYBER 2.0 ready");}
void loop(){if(connected&&lastCmd!="S"&&millis()-lastMotion>COMMAND_TIMEOUT_MS)stopMotors();delay(5);}
