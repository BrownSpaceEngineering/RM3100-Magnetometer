#ifndef __RM3100_H
#define __RM3100_H

//pin definitions
#define PIN_DRDY 9 //Set pin D9 to be the Data Ready Pin
#define PIN_CS 10 //Chip Select (SS) is set to Pin 10

//internal register values without the R/W bit
#define RM3100_REVID_REG 0x36 // Hexadecimal address for the Revid internal register
#define RM3100_POLL_REG 0x00 // Hexadecimal address for the Poll internal register
#define RM3100_CMM_REG 0x01 // Hexadecimal address for the Continuous Measurement Mode internal register
#define RM3100_STATUS_REG 0x34 // Hexadecimal address for the Status internal register
#define RM3100_CCX1_REG 0x04 // Hexadecimal address for the Cycle Count X1 internal register
#define RM3100_CCX0_REG 0x05 // Hexadecimal address for the Cycle Count X0 internal register

//options
#define initialCC 200 // Set the cycle count
#define singleMode 0 //0 = use continuous measurement mode; 1 = use single measurement mode
#define useDRDYPin 1 //0 = not using DRDYPin ; 1 = using DRDYPin to wait for data

uint8_t revid;
uint16_t cycleCount;

void setup();
void loop();

void CS_HIGH(int CS);
void CS_LOW(int CS);

uint8_t readReg(uint8_t addr);
void writeReg(uint8_t addr, uint8_t data);
void changeCycleCount(uint16_t newCC);