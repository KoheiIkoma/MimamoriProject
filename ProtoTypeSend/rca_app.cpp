#include "rca.h"

#include "ESP8266.h"
#include "Milkcocoa.h"
#include "Client_ESP8266.h"
#include "../examples_gdef.h"

// 温室度センサのインクルードファイル
#include <arduino.h>
#include <Wire.h>
#include "TH02.h"

#define LIGHT_SIG A0

#define IFTTT_KEY	"1XTd_bIv4-LMbVOhIL4Z5"
#define EVENT_NAME	"test"

#define HOST_NAME	"maker.ifttt.com"
#define HOST_PORT	(80)

#define PULSE_BORDER 100

#define MILKCOCOA_SERVERPORT  1883

ESP8266Client wifi_client;

const char MQTT_SERVER[] PROGMEM	= MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM	= __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&wifi_client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

TH02 th02(TH02_I2C_ADDR);
boolean TH02_found = false;

/* ======================================================================
Function: printhex
Purpose : print hex value in 2 digit format
Input   : value
Output  :
Comments:
====================================================================== */
void printhex(uint8_t c) {
	if (c < 16)
		Serial.print('0');
	Serial.print(c, HEX);
}

/* ======================================================================
Function: i2cScan
Purpose : scan I2C bus
Input   : -
Output  : number of I2C devices seens
Comments: global var TH02_found set to true if found TH02
====================================================================== */
int i2cScan() {
	byte error, address;
	int nDevices;

	Serial.println("Scanning I2C bus ...");

	nDevices = 0;
	for (address = 1; address < 127; address++ ) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0) {
			Serial.print(F("I2C device found at address 0x"));
			printhex(address);

			if (address == 0x40) {

				Serial.println(F("-> TH02 !"));
				TH02_found = true;
			} else
				Serial.println(F("-> Unknown device !"));

			nDevices++;
		} else if (error == 4) {
			Serial.print(F("Unknow error at address 0x"));
			if (address < 16)
				Serial.print('0');
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0)
		Serial.println(F("No I2C devices found\n"));
	else
		Serial.println(F("Scan done"));

	return nDevices;
}



void setup() {
	int ret;

	Serial.begin(115200);
	// Serial.print("Milkcocoa SDK demo");

	// Connect to WiFi access point.
	Serial.println(); Serial.println();
	Serial.print("Connecting to ");
	Serial.println(STA_SSID);

	ret = WiFi.begin(Serial5, 115200);

	if (ret == 1) {
		Serial.print("Cannot communicate with ESP8266.");
		while (1);
	} else if (ret == 2) {
		Serial.println("FW Version mismatch.");
		Serial.print("FW Version:");
		Serial.println(WiFi.getVersion().c_str());
		Serial.print("Supported FW Version:");
		Serial.println(ESP8266_SUPPORT_VERSION);
		while (1);
	} else {
		Serial.print("begin ok\r\n");
	}

	Serial.print("FW Version:");
	Serial.println(WiFi.getVersion().c_str());

	if (WiFi.setOprToStation()) {
		Serial.print("to station ok\r\n");
	} else {
		Serial.print("to station err\r\n");
	}

	if (WiFi.joinAP(STA_SSID, STA_PASSWORD)) {
		Serial.print("Join AP success\r\n");
		Serial.print("IP: ");
		Serial.println(WiFi.getLocalIP().c_str());
	} else {
		Serial.print("Join AP failure\r\n");
	}

	if (WiFi.stopServer()) {
		Serial.print("Stop server ok\r\n");
	} else {
		Serial.print("Stop server err\r\n");
	}

	if (WiFi.disableMUX()) {
		Serial.print("single ok\r\n");
	} else {
		Serial.print("single err\r\n");
	}

	// 光センサ(脈拍の代用)の設定
	pinMode(LIGHT_SIG, INPUT);

	// 温湿度センサの設定
	Wire.begin();

	uint8_t devID;
	uint8_t err;
	uint8_t status;
	uint8_t config;
	// Loop until we found TH02 module
	while (!TH02_found) {
		// scan I2C bus
		i2cScan();

		// We found it ?
		if (TH02_found) {
			// TH02 ID
			err = th02.getId(&devID);

			if (err) {
				Serial.print(F("TH02 getId error = 0x"));
				printhex(err);
				Serial.println();
			} else {
				Serial.print(F("TH02 device ID = 0x"));
				printhex(devID);
				Serial.println();

				if (devID == 0x50) {
					Serial.println(F("TH02 device ID match !"));

					if ( (err = th02.getStatus(&status)) != 0) {
						Serial.print(F("TH02 Status error = 0x"));
						printhex(err);
					} else {
						Serial.print(F("TH02 Status = 0x"));
						printhex(status);
					}
					Serial.println();

					if ( (err = th02.getConfig(&config)) != 0) {
						Serial.print(F("TH02 Config error = 0x"));
						printhex(err);
					} else {
						Serial.print(F("TH02 Config = 0x"));
						printhex(config);
					}
					Serial.println();
				}
			}
		}

		// wait until next search
		if (!TH02_found) {
			Serial.println(F("Will retry to find TH02 in 5 sec."));
			delay(5000);
		}

	} // While not found

	Serial.println("setup end\r\n");
}



// void loop() {
// 	int8_t ret;
// 	int8_t push_ret;
//
// 	DataElement elem1 = DataElement();
//
// 	Serial.print("Push data to Milkcocoa : Key:PULSE, Value:");
// 	int pulse = analogRead(LIGHT_SIG);
// 	Serial.println(pulse);
// 	elem1.setValue("PULSE", pulse);
//
// 	Serial.print("Push data to Milkcocoa : Key:TEMP, Value:");
// 	th02.startTempConv();
// 	th02.waitEndConversion();
// 	th02.getConversionValue();
// 	double temp = th02.getLastRawTemp() / 100.0;
// 	Serial.println(temp);
// 	elem1.setValue("TEMP", temp);
//
// 	do {
// 		push_ret = milkcocoa.push(MILKCOCOA_DATASTORE, &elem1);
// 		if (push_ret != 0) {
// 			Serial.println("Milkcocoa.push() error.");
// 			Serial.println(milkcocoa.pushErrorString(push_ret));
// 			Serial.println(push_ret);
// 			Serial.println("Retrying MQTT push in 5 seconds...");
// 			delay(5000);
// 		}
// 		delay(1000);
// 	} while (push_ret != 0);
//
// 	Serial.println("Push success.");
// 	delay(2000);
//
// //	delay(10000);
//
// 	if (Serial.available() > 0) {
// 		Serial.read();
// 		Serial.print("Pause  : Input any char to continue.\n\r");
// 		while (!(Serial.available() > 0));
// 		Serial.print("Resume.\n\r");
// 		Serial.read();
// 	}
//
//
// }

void pushData(int pulse, double temp, double humi, int state) {
	int8_t ret;
	int8_t push_ret;

	DataElement elem1 = DataElement();

	// DataElementに各センサの値をセットする
	Serial.print("Push data to Milkcocoa : Key:PULSE, Value:");
	Serial.println(pulse);
	elem1.setValue("PULSE", pulse);

	Serial.print("Push data to Milkcocoa : Key:TEMP, Value:");
	Serial.println(temp);
	elem1.setValue("TEMP", temp);

	Serial.print("Push data to Milkcocoa : Key:HUMI, Value:");
	Serial.println(humi);
	elem1.setValue("HUMI", humi);

	Serial.print("Push data to Milkcocoa : Key:STATE, Value:");
	Serial.println(state);
	elem1.setValue("STATE", state);


	// プッシュする
	do {
		push_ret = milkcocoa.push(MILKCOCOA_DATASTORE, &elem1);
		if (push_ret != 0) {
			Serial.println("Milkcocoa.push() error.");
			Serial.println(milkcocoa.pushErrorString(push_ret));
			Serial.println(push_ret);
			Serial.println("Retrying MQTT push in 5 seconds...");
			delay(5000);
		}
		delay(1000);
	} while (push_ret != 0);

	Serial.println("Push success.");
	delay(2000);

//	delay(10000);

	if (Serial.available() > 0) {
		Serial.read();
		Serial.print("Pause  : Input any char to continue.\n\r");
		while (!(Serial.available() > 0));
		Serial.print("Resume.\n\r");
		Serial.read();
	}


}

// 温度センサから読み出した値を返す
double getTemperature() {
		th02.startTempConv();
		th02.waitEndConversion();
		th02.getConversionValue();
		return th02.getLastRawTemp() / 100.0;
}

// 湿度センサから読み出した値を返す
double getHumidity() {
		th02.startRHConv();
		th02.waitEndConversion();
		th02.getConversionValue();
		return th02.getLastRawRH() / 100.0;
}

// アラートが必要ならtrue
boolean isAlertNeeded(int pulse, double temp, double humi) {
  if (pulse == 0) {
    return true;
  }

  if (temp > 32 && humi > 70) {
    return true;
  }

  return false;
}

char buffer[1024];
const char request1[] = "GET /trigger/" EVENT_NAME "/with/key/" IFTTT_KEY "/";
const char request2[] = " HTTP/1.1\r\nHost: " HOST_NAME "\r\nConnection: close\r\n\r\n";

void sendAlertMail(int pulse, int temp, int humi) {

  if (!WiFi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.println("create tcp err");
    return;
  }

  sprintf(buffer, "%s?value1=%d&value2=%d&value3=%d%s", request1, pulse, temp, humi, request2);

  WiFi.send((const uint8_t*)buffer, strlen(buffer));

  uint32_t len = WiFi.recv((uint8_t*)buffer, sizeof(buffer), 10000);


  if (!WiFi.releaseTCP()) {
    Serial.println("release tcp err");
  }

}

int getPulse(){
	int pulseCnt = 0;
	int result = 0;
	unsigned long start=0,end=0;
	static int tmp[2] = {0};

 	do {
 	start = micros();
 	pulseCnt++;
 	int pulse = analogRead(LIGHT_SIG);
 	tmp[0] = tmp[1];
 	tmp[1] = pulse;

 	if ((tmp[0] < PULSE_BORDER ) && (tmp[1] > PULSE_BORDER )){
 		result = 1;
 		end = micros();
 		double lap = end - start;
 		break;
 		}
 	end = micros();
 	double lap = end - start;
 	//delayMicroseconds(500-lap);
	} 
	while (pulseCnt<100);
	return result;
}
void loop() {
		static int cnt = 0;
		static int pulse = 0; 
		static unsigned long start=0,end=0;
		static double  totaltime = 0;
		int state = 0;
		
		start = millis();
		/* 毎フレームの処理 */
		pulse += getPulse();	
		/* 毎フレームの処理、ここまで */
		end = millis();
		
 		cnt++;
		totaltime += end - start;
		
		// 60秒に1回の処理
		if (cnt == 600) {
			double weight = 60 / (totaltime/1000);
			pulse = pulse * weight;
			double temp = getTemperature();
			double humi = getHumidity();
			
		// Milkcocoaにデータをプッシュする
		pushData(pulse, temp, humi, state);
      	if (isAlertNeeded(pulse, temp, humi) == true) {
        Serial.println("send alert");
        // sendAlertMail(pulse, temp, humi);
      }
			
			pulse = 0;
			cnt = 0;
			start = 0;
			end = 0;
			totaltime = 0;
		}
}
