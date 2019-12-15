void LcdPrint(char* str) {
  lcd.print(str);
}

void LcdInit() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  //         12345678901234567890
  lcd.print("MegaHand by Chkalov!");
}
