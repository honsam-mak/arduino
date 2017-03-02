#include <DS1302.h>

// 初始化DS1302
// DS1302 rtc([RST], [DAT], [CLOCK]); 三個pin的接法
DS1302 rtc(16, 15, 14);

void setup() {
  // 設定時鐘為正常執行模式
  rtc.halt(false);

  //取消寫入保護，設定日期時要這行
  //rtc.writeProtect(false);

  // 設定Serial
  Serial.begin(9600);

  // 以下是設定時間的方法，在電池用完之前，只要設定一次就行了
  //rtc.setDOW(FRIDAY);        // 設定週幾，如FRIDAY
  //rtc.setTime(21, 35, 0);     // 設定時間 時，分，秒 (24hr format)
  //rtc.setDate(5, 8, 2016);   // 設定日期 日，月，年
}

void loop() {
  //將得到的日期時間顯示在Serial視窗

  // 週幾
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");

  // 日期
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // 時間
  Serial.println(rtc.getTimeStr());

  // 每次更新延遲1秒
  delay (1000);
}

