// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include "TM1637.h"

#define GROUP_NO "ys709"

#define RTC_SDA A4
#define RTC_SCL A5

#define CLK 10
#define DIO 9

RTC_DS1307 rtc;
TM1637 display(CLK, DIO);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(9600);

  display.init();
  display.set(BRIGHTEST);
  display.clearDisplay();

  Wire.begin();

  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }

  // 第一次使用或时间丢失时，取消下面这行注释上传一次，之后再注释掉重新上传
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();

  Serial.print(F("Group: "));
  Serial.println(F(GROUP_NO));

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // 数码管显示：组号后两位 09 + 分钟，格式为 09:分钟
  int groupTail = 9;
  int minute = now.minute();

  int8_t digits[4];
  digits[0] = groupTail / 10;
  digits[1] = groupTail % 10;
  digits[2] = minute / 10;
  digits[3] = minute % 10;

  display.point(POINT_ON);
  display.display(digits);

  delay(1000);
}