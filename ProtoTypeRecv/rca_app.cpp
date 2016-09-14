#include "rca.h"

#include "ESP8266.h"
#include "Milkcocoa.h"
#include "Client_ESP8266.h"
#include "../examples_gdef.h"

#define LED_RED 2
#define TOUCH_SW 3

//#define MILKCOCOA_PUSH
#define MILKCOCOA_ON

#define MILKCOCOA_SERVERPORT  1883

ESP8266Client wifi_client;

const char MQTT_SERVER[] PROGMEM	= MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM	= __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&wifi_client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

extern void onpush(DataElement *elem);

void setup() {
	int ret;

	Serial.begin(115200);
	Serial.print("Milkcocoa SDK demo");

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

#ifdef MILKCOCOA_ON
	if (milkcocoa.on(MILKCOCOA_DATASTORE, "push", onpush)) {
//	if (milkcocoa.on(MILKCOCOA_DATASTORE, "send", onpush)){
		Serial.println("milkcocoa on sucesss");
	} else {
		Serial.println("milkcocoa on failure");
	}

	pinMode(LED_RED, OUTPUT);
	digitalWrite(LED_RED, LOW);
#endif /* MILKCOCOA_ON  */

	pinMode(TOUCH_SW, INPUT);

	Serial.println("setup end\r\n");
}

void loop() {
#ifdef MILKCOCOA_ON
	int8_t ret;

	while ((ret = milkcocoa.loop(1)) != 0) {
		Serial.println("Milkcocoa.loop() connection error.");
		Serial.println(milkcocoa.connectErrorString(ret));
		Serial.println(ret);
		Serial.println("Retrying MQTT connection in 5 seconds...");
		delay(5000);
	}
	Serial.print(".");
#endif /* MILKCOCOA_ON  */

#ifdef MILKCOCOA_PUSH
	int8_t push_ret;
	int sw_push = 0;
	static int sw_st = 1;

	switch (sw_st) {
	case 0:
		if (digitalRead(TOUCH_SW)) {
			sw_push = 1;
			sw_st = 1;
		}
		break;
	default:
		if (!digitalRead(TOUCH_SW)) {
			sw_push = 1;
			sw_st = 0;
		}
		break;
	}

	if (sw_push) {
		DataElement elem = DataElement();

		if (sw_st) {
			Serial.println("Push data to Milkcocoa : Key:LED, Value:ON");
			elem.setValue("LED", "ON");
		} else {
			Serial.println("Push data to Milkcocoa : Key:LED, Value:OFF");
			elem.setValue("LED", "OFF");
		}

		do {
			push_ret = milkcocoa.push(MILKCOCOA_DATASTORE, &elem);
			if (push_ret != 0) {
				Serial.println("Milkcocoa.push() error.");
				Serial.println(milkcocoa.pushErrorString(push_ret));
				Serial.println(push_ret);
				Serial.println("Retrying MQTT push in 5 seconds...");
				delay(5000);
			}
		} while (push_ret != 0);

		Serial.println("Push success.");
	}
#endif /* MILKCOCOA_PUSH */

	if (Serial.available() > 0) {
		Serial.read();
		Serial.print("Pause  : Input any char to continue.\n\r");
		while (!(Serial.available() > 0));
		Serial.print("Resume.\n\r");
		Serial.read();
	}
}

#ifdef MILKCOCOA_ON
void onpush(DataElement *pelem) {
	//char *data;
	int data_p;
	float data_t;
	int data_s;
	static int ln = 0;

	//if(!pelem->getString("LED", &data)) {
	if (!pelem->getInt("PULSE", &data_p)) {
		Serial.print("onpush : key PULSE is not found.");
		return;
	};
	// Serial.println(pelem->getFloat("TEMP"));
	if (!pelem->getFloat("TEMP", &data_t)) {
		Serial.print("onpush : key TEMP is not found.");
		return;
	};

	if (!pelem->getInt("STATE", &data_s)) {
		Serial.print("onpush : key STATE is not found.");
		return;
	};


	Serial.print("onpush : {PULSE, ");
	//Serial.write(*data_p);
	Serial.print(data_p);
	Serial.println("}.");

	Serial.print("onpush : {TEMP, ");
	//Serial.write(*data_t);
	Serial.print(data_t);
	Serial.println("}.");

	Serial.print("onpush : {STATE, ");
	//Serial.write(*data_s);
	Serial.print(data_s);
	Serial.println("}.");

enum states{
	NORMAL = 0,
	ILLPULSE = 1,
	ILLTEMP = 2,
	ILLPULSEANDTEMP = 3,
};

	switch (data_s){
		case NORMAL:
			break;
		case ILLPULSE:
			break;
		case ILLTEMP:
			break;
		case ILLPULSEANDTEMP:
			break;
	}
	
	/*
	if (strcmp(data, "ON") == 0) {
		Serial.println("LED : ON!");
		digitalWrite(LED_RED, HIGH);
	}
	else if(strcmp(data, "OFF") == 0) {
		Serial.println("LED : OFF!");
		digitalWrite(LED_RED, LOW);
	}
	*/

}
#endif /* MILKCOCOA_ON */
