#include "eeprom.hpp"

void MEM::write_1_baht(uint16_t num) {
    EEPROM.write(1, (uint8_t)((num >> 8) & 0xFF));
    EEPROM.write(2, (uint8_t)(num & 0xFF));
};

void MEM::write_5_baht(uint16_t num) {
    EEPROM.write(3, (uint8_t)((num >> 8) & 0xFF));
    EEPROM.write(4, (uint8_t)(num & 0xFF));
};

void MEM::write_10_baht(uint16_t num) {
    EEPROM.write(5, (uint8_t)((num >> 8) & 0xFF));
    EEPROM.write(6, (uint8_t)(num & 0xFF));
};

uint16_t MEM::read_1_baht() {
    return (uint16_t)((EEPROM.read(1) << 8) | EEPROM.read(2));
};

uint16_t MEM::read_5_baht() {
    return (uint16_t)((EEPROM.read(3) << 8) | EEPROM.read(4));
};

uint16_t MEM::read_10_baht() {
    return (uint16_t)((EEPROM.read(5) << 8) | EEPROM.read(6));
};

void MEM::begin() {
    EEPROM.begin(10);
};

void MEM::reset() {
    for (uint8_t i = 0; i < 10; i++) {
        EEPROM.write(i, 0);
    };
};

bool MEM::commit() {
    return EEPROM.commit();
};

bool MEM::get_auto_lcd_off() {
    return EEPROM.read(7) & 0xFF;
};

void MEM::set_auto_lcd_off(bool _) {
    if (_ == 1) {
        EEPROM.write(7, 0xFF);
    } else {
        EEPROM.write(7, 0);
    };
    EEPROM.commit();
    delay(200);
};