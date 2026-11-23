/
  tm1637.display(3,13);
  delay(3000);
}

int hour, minute, second;
unsigned char ClockPoint = 1;
 
void loop() {
  DateTime theTime = RTC.now();
  second = theTime.second();  // get seconds
  minute = theTime.minute();  // get minutes

  TimeDisp[0] = minute / 10;
  TimeDisp[1] = minute % 10;
  TimeDisp[2] = second / 10;
  TimeDisp[3] = second % 10;
  
  tm1637.display(0,TimeDisp[0]);
  tm1637.display(1,TimeDisp[1]); 
  tm1637.display(2,TimeDisp[2]);
  tm1637.display(3,TimeDisp[3]);
  Serial.print("Set to ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.print(second);
  Serial.println();
  ClockPoint = (~ClockPoint) & 0x01;
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); 
  delay(1000);  
}
