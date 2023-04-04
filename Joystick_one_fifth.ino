#include <FUTABA_SBUS.h>
#include <Streaming.h>
#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD);  // No accelerator, brake, or steering
FUTABA_SBUS sBus;

bool analogToButton(int analogValue){
  if(analogValue > 800)
    return 1;
  return 0;
}

int analogToTristate(int analogValue){
  if(analogValue < 400)
    return 0;
  if(analogValue > 1400)
    return 2;
  return 1;
}

int toBitQuality(int analogValue){ 
  if(analogValue> 1800) analogValue = 1800;
  if(analogValue < 174) analogValue = 174;
  analogValue -= 992;
  return analogValue >= 0 ?  127*(double(analogValue)/(1800-992)) : 127*(double(analogValue)/(992-174));
}

void setup() {
  // put your setup code here, to run once:
  Joystick.begin();
  Joystick.setXAxisRange(-128, 127);
  Joystick.setYAxisRange(-128, 127);
  Serial.begin(115200);
  sBus.begin();

}

void loop() {
  sBus.FeedLine();
  if (sBus.toChannels == 1){
    sBus.UpdateChannels();
    sBus.toChannels = 0;  
    Joystick.setXAxis(toBitQuality(sBus.channels[3]));
    Joystick.setYAxis(toBitQuality(sBus.channels[1]));
    Joystick.setButton(0, analogToButton(sBus.channels[4]));
    Joystick.setButton(1, analogToButton(sBus.channels[7]));
  }
}
