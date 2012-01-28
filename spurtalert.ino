// SpurtAlert, by Adam Greig, Jan 2012

// ShiftBrite =============================================
const int CI = 10;
const int EI = 11;
const int LI = 12;
const int DI = 13;

boolean sb_enabled = true;

// Send a control packet.
void sb_send_packet(int address, int d1, int d2, int d3) {
  unsigned long pkt = address & 3;
  pkt = (pkt << 10) | (d1 & 1023);
  pkt = (pkt << 10) | (d2 & 1023);
  pkt = (pkt << 10) | (d3 & 1023);
  
  shiftOut(DI, CI, MSBFIRST, pkt >> 24);
  shiftOut(DI, CI, MSBFIRST, pkt >> 16);
  shiftOut(DI, CI, MSBFIRST, pkt >> 8);
  shiftOut(DI, CI, MSBFIRST, pkt);
  
  delay(1);
  digitalWrite(LI, HIGH);
  delay(1);
  digitalWrite(LI, LOW);
}

// Set the current limit per-channel. Integer 0 to 127.
void sb_set_current(int red, int green, int blue) {
  sb_send_packet(0x01, blue, red, green);
}

// Set the current colour. Integer 0 to 1023.
void sb_colour(int red, int green, int blue) {
  if(sb_enabled) {
    sb_send_packet(0x00, blue, red, green);
  } else {
    sb_send_packet(0x00, 0, 0, 0);
  }
}

void sb_enable() {
  sb_enabled = true;
}

void sb_disable() {
  sb_enabled = false;
}

void sb_toggle_enabled() {
  sb_enabled = !sb_enabled;
}

// Set up pins, set default levels and set current limit.
void sb_init() {
  pinMode(CI, OUTPUT);
  pinMode(EI, OUTPUT);
  pinMode(LI, OUTPUT);
  pinMode(DI, OUTPUT);
  
  digitalWrite(LI, LOW);
  digitalWrite(EI, LOW);
  
  sb_enabled = true;
  
  // Max current for each channel
  sb_set_current(127, 127, 127);
  
  // Indicate readyness
  sb_colour(0, 1023, 0);
  delay(100);
  sb_colour(0, 0, 0);
}

// SerLCD =================================================
char lcd_char_sgl_up_arrow[8] = {0, 0, 0, 0, 4, 14, 31, 0};
char lcd_char_dbl_up_arrow[8] = {4, 14, 31, 0, 4, 14, 31, 0};
char lcd_char_sgl_down_arrow[8] = {0, 31, 14, 4, 0, 0, 0, 0};
char lcd_char_dbl_down_arrow[8] = {0, 31, 14, 4, 0, 31, 14, 4};
char lcd_char_no_arrow[8] = {0, 0, 0, 31, 31, 0, 0, 0};
char lcd_char_question_mark[8] = {0x0,0xe,0x11,0x2,0x4,0x4,0x0,0x4};
char lcd_char_unknown[8] = {0x2,0x7,0x10,0x8,0x10,0x7,0x12,0x0};
char lcd_char_stormy_1[8] = {0x0, 0x0, 0x0, 0x0, 0x1, 0x7, 0x7, 0x3};
char lcd_char_stormy_2[8] = {0x0, 0x0, 0x0, 0x0, 0x1c, 0x1f, 0x1f, 0x1e};
char lcd_char_stormy_3[8] = {0x0, 0x4, 0x8, 0x0, 0x2, 0x4, 0x0, 0x0};
char lcd_char_stormy_4[8] = {0x4, 0x4, 0xc, 0x18, 0x10, 0x12, 0x4, 0x0};
char lcd_char_rain_1[8] = {0x0, 0x0, 0x0, 0x0, 0x1, 0x7, 0x7, 0x3};
char lcd_char_rain_2[8] = {0x0, 0x0, 0x0, 0x0, 0x1c, 0x1f, 0x1f, 0x1e};
char lcd_char_rain_3[8] = {0x0, 0x1, 0x2, 0x0, 0x0, 0x2, 0x4, 0x0};
char lcd_char_rain_4[8] = {0x0, 0x1, 0x2, 0x8, 0x10, 0x2, 0x4, 0x0};
char lcd_char_very_dry_1[8] = {0x0, 0x4, 0x2, 0x1, 0x0, 0x0, 0x7, 0x0};
char lcd_char_very_dry_2[8] = {0x0, 0x8, 0x8, 0x9, 0x0, 0x1c, 0x1d, 0x1c};
char lcd_char_very_dry_3[8] = {0x1, 0x2, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_very_dry_4[8] = {0x9, 0x8, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_fair_1[8] = {0x0, 0x0, 0x0, 0x0, 0x6, 0x1f, 0x1f, 0xf};
char lcd_char_fair_2[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x1b};
char lcd_char_fair_3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_fair_4[8] = {0x5, 0x9, 0x11, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_change_1[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_change_2[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_change_3[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_change_4[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_spurt_1[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0x6, 0x0};
char lcd_char_spurt_2[8] = {0x0, 0x0, 0x10, 0x18, 0xa, 0x2, 0x6, 0x0};
char lcd_char_spurt_3[8] = {0x0, 0x0, 0x0, 0x0, 0x4, 0xc, 0x8, 0x0};
char lcd_char_spurt_4[8] = {0x0, 0xe, 0xb, 0x0, 0x0, 0x0, 0x0, 0x0};
char lcd_char_spurt_5[8] = {0x10, 0x1b, 0x2, 0x0, 0xe, 0xe, 0xe, 0xe};
char lcd_char_spurt_6[8] = {0x0, 0x0, 0xc, 0x6, 0x0, 0x0, 0x0, 0x0};

void lcd_clear() {
  Serial.write(0xFE);
  Serial.write(0x01);
  delay(500);
}

void lcd_line_one(int pos=0) {
  Serial.write(0xFE);
  Serial.write((char)(0x80 + pos));
  //delay(200);
}

void lcd_line_two(int pos=0) {
  Serial.write(0xFE);
  Serial.write((char)(0xC0 + pos));
  //delay(200);
}

void lcd_store_char(char address, char data[]) {
  Serial.write(0xFE);
  Serial.write((char)(0x40 | (address << 3)));
  int i;
  for(i=0; i<8; i++) {
    Serial.write((char)(data[i] + 32));
  }
  lcd_line_one();
}

void lcd_load_change(int dir) {
  lcd_line_one();
  switch(dir) {
    case -2:
      lcd_store_char(4, lcd_char_dbl_down_arrow);
      break;
    case -1:
      lcd_store_char(4, lcd_char_sgl_down_arrow);
      break;
    case 0:
      lcd_store_char(4, lcd_char_no_arrow);
      break;
    case 1:
      lcd_store_char(4, lcd_char_sgl_up_arrow);
      break;
    case 2:
      lcd_store_char(4, lcd_char_dbl_up_arrow);
      break;
    case 3:
      lcd_store_char(4, lcd_char_unknown);
      break;
    default:
      lcd_store_char(4, lcd_char_question_mark);
  }
}

void lcd_show_pressure(int pressure) {
  lcd_line_one(0);
  Serial.print(pressure);
  Serial.print(" mbar");
  lcd_line_one(10);
  Serial.print((char)4);
  
  lcd_line_one(14);
  Serial.print((char)0);
  Serial.print((char)1);
  lcd_line_two(14);
  Serial.print((char)2);
  Serial.print((char)3);
}

void lcd_show_stormy(int pressure, int dir) {
  lcd_load_change(dir);
  lcd_store_char(0, lcd_char_stormy_1);
  lcd_store_char(1, lcd_char_stormy_2);
  lcd_store_char(2, lcd_char_stormy_3);
  lcd_store_char(3, lcd_char_stormy_4);
  lcd_line_two(4);
  Serial.print("Stormy  ");
  lcd_show_pressure(pressure);
}

void lcd_show_rain(int pressure, int dir) {
  lcd_load_change(dir);
  lcd_store_char(0, lcd_char_rain_1);
  lcd_store_char(1, lcd_char_rain_2);
  lcd_store_char(2, lcd_char_rain_3);
  lcd_store_char(3, lcd_char_rain_4);
  lcd_line_two(4);
  Serial.print("Rain    ");
  lcd_show_pressure(pressure);
}

void lcd_show_change(int pressure, int dir) {
  lcd_load_change(dir);
  lcd_store_char(0, lcd_char_change_1);
  lcd_store_char(1, lcd_char_change_2);
  lcd_store_char(2, lcd_char_change_3);
  lcd_store_char(3, lcd_char_change_4);
  lcd_line_two(4);
  Serial.print("Change  ");
  lcd_show_pressure(pressure);
}

void lcd_show_fair(int pressure, int dir) {
  lcd_load_change(dir);
  lcd_store_char(0, lcd_char_fair_1);
  lcd_store_char(1, lcd_char_fair_2);
  lcd_store_char(2, lcd_char_fair_3);
  lcd_store_char(3, lcd_char_fair_4);
  lcd_line_two(4);
  Serial.print("Fair    ");
  lcd_show_pressure(pressure);
}

void lcd_show_very_dry(int pressure, int dir) {
  lcd_load_change(dir);
  lcd_store_char(0, lcd_char_very_dry_1);
  lcd_store_char(1, lcd_char_very_dry_2);
  lcd_store_char(2, lcd_char_very_dry_3);
  lcd_store_char(3, lcd_char_very_dry_4);
  lcd_line_two(4);
  Serial.print("Very Dry");
  lcd_show_pressure(pressure);
}

void lcd_show_spurt_alert() {
  lcd_clear();
  
  lcd_store_char(0, lcd_char_spurt_1);
  lcd_store_char(1, lcd_char_spurt_2);
  lcd_store_char(2, lcd_char_spurt_3);
  lcd_store_char(3, lcd_char_spurt_4);
  lcd_store_char(4, lcd_char_spurt_5);
  lcd_store_char(5, lcd_char_spurt_6);
  
  lcd_line_one(1);
  Serial.print((char)0);
  Serial.print((char)1);
  Serial.print((char)2);
  lcd_line_two(1);
  Serial.print((char)3);
  Serial.print((char)4);
  Serial.print((char)5);
  lcd_line_one(6);
  Serial.print("SPURT");
  lcd_line_two(6);
  Serial.print("ALERT");
  lcd_line_one(13);
  Serial.print((char)0);
  Serial.print((char)1);
  Serial.print((char)2);
  lcd_line_two(13);
  Serial.print((char)3);
  Serial.print((char)4);
  Serial.print((char)5);
}

void lcd_init() {
  Serial.begin(9600);
  lcd_clear();
  lcd_line_one();
}

// BMP085 =================================================
#include <Wire.h>
#define BMP085_ADDRESS 0x77
const unsigned char OSS = 3;

int ac1, ac2, ac3, b1, b2, mb, mc, md;
unsigned int ac4, ac5, ac6;
long b5;

char bmp_read(unsigned char address) {
  unsigned char data;
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write((byte)address);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available());
  return Wire.read();
}

int bmp_read_int(unsigned char address) {
  unsigned char msb, lsb;
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write((byte)address);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available() < 2);
  msb = Wire.read();
  lsb = Wire.read();
  return (int)msb<<8 | lsb;
}

unsigned int bmp_read_ut() {
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  delay(5);
  return bmp_read_int(0xF6);
}

unsigned long bmp_read_up() {
  unsigned char msb, lsb, xlsb;
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  delay(2 + (3<<OSS));
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  while(Wire.available() < 3);
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  return (((unsigned long)msb << 16) | ((unsigned long)lsb << 8) | (unsigned long)xlsb) >> (8-OSS);
}

void bmp_init() {
  Wire.begin();
  ac1 = bmp_read_int(0xAA); ac2 = bmp_read_int(0xAC);
  ac3 = bmp_read_int(0xAE); ac4 = bmp_read_int(0xB0);
  ac5 = bmp_read_int(0xB2); ac6 = bmp_read_int(0xB4);
  b1  = bmp_read_int(0xB6); b2  = bmp_read_int(0xB8);
  mb  = bmp_read_int(0xBA); mc  = bmp_read_int(0xBC);
  md  = bmp_read_int(0xBE);
}

short bmp_calc_temperature(unsigned int ut) {
  long x1, x2;
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;
  return ((b5 + 8)>>4);
}

long bmp_calc_pressure(unsigned long up) {
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000) {
    p = (b7<<1)/b4;
  } else {
    p = (b7/b4)<<1;
  }
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  return p;
}

long bmp_pressure() {
  bmp_calc_temperature(bmp_read_ut());
  return bmp_calc_pressure(bmp_read_up());
}

// Moisture Probe =========================================
const int PROBE = A0;
const int MOISTURE_THRESHOLD = 650;

boolean moisture_detected() {
  return analogRead(PROBE) > MOISTURE_THRESHOLD;
}

// Push Button ============================================
const int BUTTON = 9;
boolean button_pushed() {
  return digitalRead(BUTTON) == LOW;
}

// Pressure History =======================================
int pressure_history[23] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1};
int last_reading_index = -1;

void pressure_store(int pressure) {
  int index = (last_reading_index + 1) % 23;
  last_reading_index = index;
  pressure_history[index] = pressure;
}

int pressure_retrieve() {
  int index = (last_reading_index + 1) % 23;
  return pressure_history[index];
}

int pressure_direction(int pressure) {
  int old = pressure_retrieve();
  if(old == -1) {
    return 3;
  }
  
  int delta = pressure - old;
  if(delta < -50) {
    return -2;
  } else if(delta < -10) {
    return -1;
  } else if(delta < 10)  {
    return 0;
  } else if(delta < 50)  {
    return 1;
  } else {
    return 2;
  }
}

// Time Management ========================================
unsigned long last_minute = 0;
unsigned long last_hour = 0;

boolean time_one_minute() {
  unsigned long delta = millis() - last_minute;
  last_minute = millis();
  if(delta > 60000UL) {
    return true;
  } else {
    return false;
  }
}

boolean time_one_hour() {
  unsigned long delta = millis() - last_hour;
  last_hour = millis();
  if(delta > 3600000UL) {
    return true;
  } else {
    return false;
  }
}

// Spurt Alert ============================================
void spurt_alert() {
  lcd_show_spurt_alert();
  while(!button_pushed()) {
    sb_colour(1023, 0, 0);
    delay(500);
    sb_colour(0, 0, 0);
    delay(500);
  }
  lcd_clear();
}

// Pressure Display =======================================
void display_pressure() {
  int pressure = (int)(bmp_pressure() / 100);
  int dir = pressure_direction(pressure);
  render_pressure(pressure, dir);
  if(time_one_hour()) {
    pressure_store(pressure);
  }
}

void render_pressure(int pressure, int dir) {
  if(pressure < 965) {
    sb_colour(512, 0, 0);
    lcd_show_stormy(pressure, dir);
  } else if(pressure < 985) {
    sb_colour(128, 0, 512);
    lcd_show_rain(pressure, dir);
  } else if(pressure < 1015) {
    sb_colour(0, 256, 0);
    lcd_show_change(pressure, dir);
  } else if(pressure < 1035) {
    sb_colour(0, 256, 256);
    lcd_show_fair(pressure, dir);
  } else {
    sb_colour(256, 256, 0);
    lcd_show_very_dry(pressure, dir);
  }
}

// Demo Mode ==============================================
void demo_mode() {
  lcd_clear();
  
  render_pressure(960, -2);
  delay(2000);
  
  render_pressure(980, -1);
  delay(2000);
  
  render_pressure(1010, 0);
  delay(2000);
  
  render_pressure(1030, 1);
  delay(2000);
  
  render_pressure(1050, 2);
  delay(2000);
  
  lcd_show_spurt_alert();
  sb_colour(1023, 0, 0);
  delay(2000);
  
  lcd_clear();
}

// ========================================================
void setup() {
  lcd_init();
  bmp_init();
  sb_init();
  
  display_pressure();
}

// ========================================================
void loop() {
  if(time_one_minute()) {
    display_pressure();
  }
  
  if(button_pushed()) {
    delay(500);
    if(button_pushed()) {
      demo_mode();
      display_pressure();
    } else {
      sb_toggle_enabled();
      display_pressure();
    }
  }
  
  if(moisture_detected()) {
    spurt_alert();
    display_pressure();
  }
}
