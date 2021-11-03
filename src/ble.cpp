#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "ble.h"
#include "emg.h"

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "63b803e2-9201-47ee-968b-1405602a1b8e"
#define CHARACTERISTIC_UUID "46bfca8b-b8d8-40b1-87e7-c22116324c01"

#define MD1_OUT1 32 //GPIO #32
#define MD1_OUT2 33 //GPIO #33
#define MD2_OUT1 25 //GPIO #25
#define MD2_OUT2 26 //GPIO #26

char buf[100];

class MyServerCallbacks : public BLEServerCallbacks
{
private:
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
private:
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value_raw = pCharacteristic->getValue();

        std::string value = "E: 123.012345, F: 0.056789, E5";
        std::string delimiter = ",";

        size_t pos = 0;

        // ExtensorのRMS値を取得
        float e_value = 0.0;
        while ((pos = value.find(delimiter)) != std::string::npos)
        {
            std::string e_token = value.substr(3, 100); // 多めに100文字目まで取得
            const char *e_str = e_token.c_str();
            e_value = atof(e_str);
            value.erase(0, pos + delimiter.length());
            break;
        }

        // FlexorのRMS値を取得
        float f_value = 0.0;
        while ((pos = value.find(delimiter)) != std::string::npos)
        {
            std::string f_token = value.substr(4, 100); // 多めに100文字目まで取得
            const char *f_str = f_token.c_str();
            f_value = atof(f_str);
            value.erase(0, pos + delimiter.length());
            break;
        }

        extensor_value = e_value;
        flexor_value = f_value;

        // モニター出力
        sprintf(buf, "e_sp: %f\n", extensor_value);
        Serial.println(buf);
        sprintf(buf, "f_sp: %f\n", flexor_value);
        Serial.println(buf);
    }
};

void SetUpBLE()
{
    // Setup Pins
    pinMode(MD1_OUT1, OUTPUT);
    pinMode(MD1_OUT2, OUTPUT);
    pinMode(MD2_OUT1, OUTPUT);
    pinMode(MD2_OUT2, OUTPUT);

    // Create the BLE Device
    BLEDevice::init("ESP32 GET NOTI FROM DEVICE");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);

    pCharacteristic->setCallbacks(new MyCallbacks());

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

void UpdateBLEConnection()
{
    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}