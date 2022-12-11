#include <Arduino.h>

#include "lcd.hpp"
#include "eeprom.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool request_lcd_refresh = 0;

bool sensor_1_baht = 0;
bool sensor_5_baht = 0;
bool sensor_10_baht = 0;

bool pin_reset = 0;
time_t pin_reset_t;

uint16_t coin_1 = 0;
uint16_t coin_5 = 0;
uint16_t coin_10 = 0;

bool beep = 0;
bool get_beep = 0;
time_t beep_t;

void setup() {
    LCD::init();
    MEM::begin();
    coin_1 = MEM::read_1_baht();
    coin_5 = MEM::read_5_baht();
    coin_10 = MEM::read_10_baht();
    LCD::set_1_baht(coin_1);
    LCD::set_5_baht(coin_5);
    LCD::set_10_baht(coin_10);
    pinMode(A0, INPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, INPUT);
    pinMode(D6, INPUT);
    pinMode(D7, INPUT);
    LCD::refresh();
    digitalWrite(D4, HIGH);
};

void loop() {
    if (digitalRead(D5) == LOW) {
        if (sensor_1_baht == 0) {
            sensor_1_baht = 1;
            coin_1 += 1;
            get_beep = 1;
            MEM::write_1_baht(coin_1);
            request_lcd_refresh = 1;
        };
    } else {
        sensor_1_baht = 0;
    };
    if (digitalRead(D6) == LOW) {
        if (sensor_5_baht == 0) {
            sensor_5_baht = 1;
            coin_5 += 1;
            get_beep = 1;
            MEM::write_5_baht(coin_5);
            request_lcd_refresh = 1;
        };
    } else {
        sensor_5_baht = 0;
    };
    if (digitalRead(D7) == LOW) {
        if (sensor_10_baht == 0) {
            sensor_10_baht = 1;
            coin_10 += 1;
            get_beep = 1;
            MEM::write_10_baht(coin_10);
            request_lcd_refresh = 1;
        };
    } else {
        sensor_10_baht = 0;
    };
    if (analogRead(A0) > 512) {
        get_beep = 1;
        if (pin_reset == 0) {
            pin_reset = 1;
            pin_reset_t = millis();
        } else {
            if (millis() - pin_reset_t > 5000) {
                pin_reset_t += 60000;
                MEM::reset();
                coin_1 = 0;
                coin_5 = 0;
                coin_10 = 0;
                request_lcd_refresh = 1;
            };
        };
    } else {
        pin_reset = 0;
    };
    if (request_lcd_refresh) {
        LCD::set_1_baht(coin_1);
        LCD::set_5_baht(coin_5);
        LCD::set_10_baht(coin_10);
        LCD::refresh();
        request_lcd_refresh = 0;
    };
    if (get_beep) {
        get_beep = 0;
        beep = 1;
        beep_t = millis();
        digitalWrite(D4, LOW);
    };
    if (beep) {
        if (millis() - beep_t > 50) {
            digitalWrite(D4, HIGH);
            beep = 0;
        };
    };
};
