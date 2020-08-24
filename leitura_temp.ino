#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
 
#define DS18B20_OneWire 2
 
OneWire oneWire(DS18B20_OneWire);

float maior = 0;

DeviceAddress sensorDS18B20;
 
DallasTemperature sensortemp(&oneWire);
 
int ndispositivos = 0;
float grausC;
int index = -1;
 
void setup()  {
	sensortemp.begin();
	Serial.begin(9600);
}
 
void loop() {  
	ndispositivos = sensortemp.getDeviceCount();
	sensortemp.requestTemperatures();
	maior=-1;
	
	Serial.print("Total de sensores : ");
	Serial.println(ndispositivos);
	for (int i = 0;  i < ndispositivos;  i++) {
		sensortemp.getAddress(sensorDS18B20, i);
		grausC = sensortemp.getTempCByIndex(i);
		if (grausC > maior)
		{
			maior = grausC;
			index = i;
		}
		String cmt = String(i);
		Serial.print(i);
		Serial.print(": ");
		//endereco(sensorDS18B20);
		String ad("");
				
		ad = enderecoToString(sensorDS18B20);
		//exportJSonCOM(ad, grausC, cmt);
		//Serial.print(" => ");
		//Serial.print(" : ");
		Serial.print(ad);
		Serial.print(" => ");
		exportJSonCOM(ad, grausC, "none");
	}


	Serial.print("	Maior: ");
	Serial.println(index);

	Serial.println("");
	
	delay(500);
}

void endereco(DeviceAddress deviceAddress)  { 
	for (uint8_t i = 0; i < 8; i++)  {
		if (deviceAddress[i] < 0x10) Serial.print("0");
			Serial.print(deviceAddress[i], HEX);
		if (i < 7) 
			Serial.print(", ");
	}
}

String enderecoToString(DeviceAddress deviceAddress)  {
	String endereco("");
	for (uint8_t i = 0; i < 8; i++)  {
		String addEndereco= String(16);
		addEndereco=String();
		if (deviceAddress[i] < 0x10) 
			addEndereco = String("0"+addEndereco);
		addEndereco = String(addEndereco+String(deviceAddress[i], HEX));
		endereco = String(addEndereco+endereco);
	}
	return endereco;
}

void exportJSonCOM(String endereco, float temperatura, String comment){
	DynamicJsonDocument doc(512);
	doc["sensor"] = "none";
	doc["address"] = endereco;
	doc["temp"]   = temperatura;
	doc["description"] = comment;
	serializeJson(doc, Serial);
	Serial.println();
}

