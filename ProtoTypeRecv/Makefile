#
#  ターゲットの指定（Makefile.targetで上書きされるのを防ぐため）
#
all:

#
#  アプリケーションファイル
#
APPL_CXXOBJS += rca_app.o
APPL_COBJS   +=
APPL_CFG      = rca_app.cfg

#
#  Serialの扱い
#   ARDUINO_SERIAL
#    NOUSE_SERIAL
#    USE_SERIAL
#
#ARDUINO_SERIAL = NOUSE_SERIAL
ARDUINO_SERIAL = USE_SERIAL

#
#  ライブラリの使用
#
#USE_ARDUINO_SPI = true
#USE_ARDUINO_NCESCAN = true
#USE_ARDUINO_SD  = true
#USE_ARDUINO_TFT = true
#USE_ETHERNET2 = true
#USE_NAXESMOTION = true
USE_WIRE = true

#
#  RCAのディレクトリの定義
#
RCA_DIR = ../../..

#
#  ASPのソースファイルのディレクトリの定義
#
SRCDIR = $(RCA_DIR)/asp_1.9.2

#
#  その他のオプション
#
APPL_CFLAGS +=
APPL_LIBS +=
APPL_DIR     +=

APPL_DIR     += $(RCA_DIR)/arduino_lib/libraries/ESP8266_Arudino_AT
APPL_CXXOBJS += ESP8266.o

APPL_DIR     += $(RCA_DIR)/arduino_lib/libraries/Milkcocoa_Arduino_SDK
APPL_DIR     += $(RCA_DIR)/arduino_lib/libraries/Milkcocoa_Arduino_SDK/aJson
APPL_DIR     += $(RCA_DIR)/arduino_lib/libraries/Milkcocoa_Arduino_SDK/include
APPL_DIR     += $(RCA_DIR)/arduino_lib/libraries/Milkcocoa_Arduino_SDK/include/aJson
APPL_DIR     += $(RCA_DIR)/arduino_lib/libraries/Milkcocoa_Arduino_SDK/include/Adafruit
APPL_CXXOBJS += aJSON.o Milkcocoa.o Adafruit_MQTT.o Adafruit_MQTT_Client.o
APPL_COBJS   += stringbuffer.o

APPL_DIR     += $(RCA_DIR)/examples/MultiTaskText/libraries/OLED_Display12864
APPL_CXXOBJS += SeeedOLED.o
APPL_COBJS   +=

APPL_DIR     += $(RCA_DIR)/examples/MultiTaskText/libraries/Grove_Digital_Light_Sensor
APPL_CXXOBJS += Digital_Light_TSL2561.o
APPL_COBJS   +=

APPL_DIR     += ..

include $(RCA_DIR)/lib/Makefile.rca
