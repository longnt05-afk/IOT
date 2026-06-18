#include "lcd_module.h"
#include "config.h" // Nạp file cấu hình chân cắm
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Khởi tạo đối tượng LCD
// Lưu ý: Địa chỉ 0x27 là phổ biến, nếu không chạy, hãy thử 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Khởi tạo I2C và màn hình LCD
void setupLCD() {
    // Khởi tạo I2C với các chân SDA, SCL tùy chỉnh
    Wire.begin(LCD_SDA_PIN, LCD_SCL_PIN);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("LCD Module OK!");
    delay(1500);
}

// Hiển thị 2 dòng văn bản lên LCD
void displayLCD(String line1, String line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}
