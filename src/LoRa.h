// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef LORA_H
#define LORA_H

#include "xspips.h"

#ifdef ARDUINO_SAMD_MKRWAN1300
#define LORA_DEFAULT_SPI           SPI1
#define LORA_DEFAULT_SPI_FREQUENCY 250000
#define LORA_DEFAULT_SS_PIN        LORA_IRQ_DUMB
#define LORA_DEFAULT_RESET_PIN     -1
#define LORA_DEFAULT_DIO0_PIN      -1
#else
#define LORA_DEFAULT_SPI           SPI
#define LORA_DEFAULT_SPI_FREQUENCY 8E6 
#define LORA_DEFAULT_SS_PIN        10
#define LORA_DEFAULT_RESET_PIN     9
#define LORA_DEFAULT_DIO0_PIN      2
#endif

#define PA_OUTPUT_RFO_PIN          0
#define PA_OUTPUT_PA_BOOST_PIN     1

class LoRaClass {
public:
  LoRaClass();

  s32 begin(long frequency);
  void end();

  s32 beginPacket(s32 implicitHeader = false);
  s32 endPacket(bool async = false);

  s32 parsePacket(s32 size = 0);
  s32 packetRssi();
  float packetSnr();
  long packetFrequencyError();

  // from Print
  virtual size_t write(u8 byte);
  virtual size_t write(const u8 *buffer, size_t size);

  // from Stream
  virtual s32 available();
  virtual s32 read();
  virtual s32 peek();
  virtual void flush();

#ifndef ARDUINO_SAMD_MKRWAN1300
  void onReceive(void(*callback)(s32));

  void receive(s32 size = 0);
#endif
  void idle();
  void sleep();

  void setTxPower(s32 level, s32 outputPin = PA_OUTPUT_PA_BOOST_PIN);
  void setFrequency(long frequency);
  void setSpreadingFactor(s32 sf);
  void setSignalBandwidth(long sbw);
  void setCodingRate4(s32 denominator);
  void setPreambleLength(long length);
  void setSyncWord(s32 sw);
  void enableCrc();
  void disableCrc();
  void enableInvertIQ();
  void disableInvertIQ();
  
  void setOCP(u8 mA); // Over Current Protection control

  // deprecated
  void crc() { enableCrc(); }
  void noCrc() { disableCrc(); }

  s8 random();

  void setPins(s32 ss = LORA_DEFAULT_SS_PIN, s32 reset = LORA_DEFAULT_RESET_PIN, s32 dio0 = LORA_DEFAULT_DIO0_PIN);
  void setSPI(XSpiPs& spi);
  void setSPIFrequency(uint32_t frequency);

//  void dumpRegisters(Stream& out);

private:
  void explicitHeaderMode();
  void implicitHeaderMode();

  void handleDio0Rise();
  bool isTransmitting();

  s32 getSpreadingFactor();
  long getSignalBandwidth();

  void setLdoFlag();

  u8 readRegister(u8 address);
  void writeRegister(u8 address, u8 value);
  u8 singleTransfer(u8 address, u8 value);

  static void onDio0Rise();

private:
  XSpiPs_Config _spiSettings;
  XSpiPs* _spi;
  s32 _ss;
  s32 _reset;
  s32 _dio0;
  long _frequency;
  s32 _packetIndex;
  s32 _implicitHeaderMode;
  void (*_onReceive)(s32);
};

extern LoRaClass LoRa;

#endif
