#include<Mouse.h>

int enableButton = 14; // 搖杆按鍵，用作滑鼠功能使能按鍵
int leftButton = 15; //左按鍵，模擬滑鼠左鍵
int rightButton = 9; // 右按鍵，模擬滑鼠右鍵
int upButton = 10;
int downButton = 2;
int xAxis = A0; // 遙感X軸
int yAxis = A1; // 遙感Y軸
int mouseSensitivity = 20; // 滑鼠靈敏度
int wheelSensitivity = 1;
boolean enable = false; // 模擬滑鼠功能是否可用
boolean lastEnableButtonState = HIGH; // 上一次使能按鍵讀值
int LED = 16;


void setup() {
  pinMode(enableButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT);
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);
  pinMode(LED,OUTPUT);

  Serial.begin(9600);
  
  // 開始控制滑鼠
  Mouse.begin();
}

void loop() {
  boolean EnableButtonState = digitalRead(enableButton);

  if((EnableButtonState == LOW)  && (EnableButtonState != lastEnableButtonState)){
    enable = !enable;
  }
  
  lastEnableButtonState = EnableButtonState;
  
  if(enable){
    digitalWrite(LED, HIGH);
    // 讀取滑鼠偏移值
    int x = readAxis(xAxis);
    int y = readAxis(yAxis);
    
    // 讀取滑鼠滾輪
    int wheel = 0;
    if(digitalRead(upButton) == LOW)
      wheel = wheelSensitivity;
    else if(digitalRead(downButton) == LOW)
      wheel = -wheelSensitivity;

    // 移動滑鼠位置
    Mouse.move(x,y,wheel);

    // 點擊滑鼠左右鍵
    isClickButton(leftButton,MOUSE_LEFT);
    isClickButton(rightButton,MOUSE_RIGHT);
    
  }else{
    digitalWrite(LED, LOW);
  }
  delay(20);
}

int readAxis(int thisAxis){
  int reading = analogRead(thisAxis);
  reading = map(reading, 0, 1023, 0, mouseSensitivity);
  int distance = reading - mouseSensitivity/2;
//  int threshold = mouseSensitivity / 4;
//  if(abs(distance)<threshold){
//    distance = 0;
//  }
  return distance;
}

void isClickButton(int ButtonPin, uint8_t Button){
  if(digitalRead(ButtonPin)==LOW){
    if(!Mouse.isPressed(Button)){
      Mouse.press(Button);
    }
  }else if(Mouse.isPressed(Button)){
    Mouse.release(Button);
  }
}
