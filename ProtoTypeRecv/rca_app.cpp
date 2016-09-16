#include "rca.h"

#include "ESP8266.h"
#include "Milkcocoa.h"
#include "Client_ESP8266.h"
#include "../examples_gdef.h"

#define MILKCOCOA_SERVERPORT  1883

#define VCC2_1 12
#define VCC2_2 11
#define GREEN1 2
#define GREEN2 3
#define GREEN3 4
#define RED1 6
#define RED2 7
#define RED3 8
#define RED4 9

#define	NORMAL          100
#define	ILLPULSE        101
#define	ILLTEMP         102
#define	ILLPULSEANDTEMP 103
#define UNCONNECTEDNETWORK 120

int illPulseTime = 100;
int normalPulseTime = 100;
int pulse = 100;
int state = UNCONNECTEDNETWORK;
int count = 0;

ESP8266Client wifi_client;

const char MQTT_SERVER[] PROGMEM	= MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM	= __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&wifi_client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

extern void onpush(DataElement *elem);

void setup() {
	int ret;

	Serial.begin(115200);
	Serial.print("MimaMori Recv");

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

	if (milkcocoa.on(MILKCOCOA_DATASTORE, "push", onpush)) {
		Serial.println("milkcocoa on sucesss");
	} else {
		Serial.println("milkcocoa on failure");
	}

	pinMode(VCC2_1, OUTPUT);
	pinMode(VCC2_2, OUTPUT);
	pinMode(GREEN1, OUTPUT);
	pinMode(GREEN2, OUTPUT);
	pinMode(GREEN3, OUTPUT);
	pinMode(RED1, OUTPUT);
	pinMode(RED2, OUTPUT);
	pinMode(RED3, OUTPUT);
	pinMode(RED4, OUTPUT);

	Serial.println("setup end\r\n");
}

void offGreen() {
	digitalWrite(GREEN1, HIGH);
	digitalWrite(GREEN2, HIGH);
	digitalWrite(GREEN3, HIGH);
}

void allGreen() {
	digitalWrite(GREEN1, LOW);
	digitalWrite(GREEN2, LOW);
	digitalWrite(GREEN3, LOW);
}

void allRed() {
	digitalWrite(RED1, LOW);
	digitalWrite(RED2, LOW);
	digitalWrite(RED3, LOW);
	digitalWrite(RED4, LOW);
}

void initialize() {
	digitalWrite(VCC2_1, LOW);
	digitalWrite(VCC2_2, LOW);
	offGreen();
	digitalWrite(RED1, HIGH);
	digitalWrite(RED2, HIGH);
	digitalWrite(RED3, HIGH);
	digitalWrite(RED4, HIGH);
}

void normal() {
	initialize();
	digitalWrite(GREEN1, LOW);
	delay(normalPulseTime);
	digitalWrite(GREEN2, LOW);
	delay(normalPulseTime);
	digitalWrite(VCC2_1, HIGH);
	digitalWrite(VCC2_2, HIGH);
	digitalWrite(GREEN3, LOW);
	delay(normalPulseTime);
	digitalWrite(VCC2_1, LOW);
	digitalWrite(VCC2_2, LOW);
	digitalWrite(GREEN3, HIGH);
	delay(normalPulseTime);
	digitalWrite(GREEN2, HIGH);
	delay(normalPulseTime);
	initialize();
}

void illTemp() {
	offGreen();
	digitalWrite(VCC2_1, HIGH);
	digitalWrite(VCC2_2, HIGH);
	allRed();
}

void illPulse() {
	initialize();
	delay(illPulseTime);
	digitalWrite(VCC2_1, HIGH);
	digitalWrite(VCC2_2, HIGH);
	allRed();
	delay(illPulseTime);
}

void illPulseAndTemp() {
	digitalWrite(VCC2_1, LOW);
	digitalWrite(VCC2_2, LOW);
	delay(illPulseTime);
	digitalWrite(GREEN1, LOW);
	digitalWrite(GREEN2, LOW);
	digitalWrite(GREEN3, HIGH);
	digitalWrite(VCC2_1, HIGH);
	digitalWrite(VCC2_2, HIGH);
	allRed();
}

void unConnectedNetwork() {
	digitalWrite(VCC2_1, HIGH);
	digitalWrite(VCC2_2, HIGH);
	allGreen();
	allRed();
}

void loop() {
	int8_t ret;

	while ((ret = milkcocoa.loop(1)) != 0) {
		state = UNCONNECTEDNETWORK;
		unConnectedNetwork();
		Serial.println("Milkcocoa.loop() connection error.");
		Serial.println(milkcocoa.connectErrorString(ret));
		Serial.println(ret);
		Serial.println("Retrying MQTT connection in 5 seconds...");
		delay(5000);
	}
	Serial.print(".");

	if (Serial.available() > 0) {
		Serial.read();
		Serial.print("Pause  : Input any char to continue.\n\r");
		while (!(Serial.available() > 0));
		Serial.print("Resume.\n\r");
		Serial.read();
	}
	count++;
	if (count > 30) state = UNCONNECTEDNETWORK;
	switch (state){
		case NORMAL:
			normal();
			break;
		case ILLPULSE:
			illPulse();
			break;
		case ILLTEMP:
			illTemp();
			break;
		case ILLPULSEANDTEMP:
			illPulseAndTemp();
			break;
		case UNCONNECTEDNETWORK:
			unConnectedNetwork();
		break;
		default:
			break;
	}
	delay(100);
}

void onpush(DataElement *pelem) {
	int p, s;
	count = 0;
	if (!pelem->getInt("PULSE", &p)) {
		Serial.print("onpush : key PULSE is not found.");
		return;
	};
	if (!pelem->getInt("STATE", &s)) {
		Serial.print("onpush : key STATE is not found.");
		return;
	};

	Serial.print("onpush : {PULSE, ");
	Serial.print(p);
	Serial.println("}.");

	Serial.print("onpush : {STATE, ");
	Serial.print(s);
	Serial.println("}.");

	if(p != 0) {
		illPulseTime    = (60000 / p) / 2;
		normalPulseTime = (60000 / p) / 6;
	}
	pulse = p;
	switch(s) {
		case 0:
		state = NORMAL;
		break;
		case 1:
		state = ILLPULSE;
		break;
		case 2:
		state = ILLTEMP;
		break;
		case 3:
		state = ILLPULSEANDTEMP;
		break;
	}

}
