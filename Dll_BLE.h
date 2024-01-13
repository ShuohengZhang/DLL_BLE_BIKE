#pragma once

#include "targetver.h"
#include <pch.cpp>

struct DeviceUpdate {
	wchar_t id[100];
	bool isConnectable = false;
	bool isConnectableUpdated = false;
	wchar_t name[50];
	bool nameUpdated = false;
};

struct Service {
	wchar_t uuid[100];
};

struct Characteristic {
	wchar_t uuid[100];
	wchar_t userDescription[100];
};

struct BLEData {
	uint8_t buf[512];
	uint16_t size;
	wchar_t deviceId[256];
	wchar_t serviceUuid[256];
	wchar_t characteristicUuid[256];
};

struct ErrorMessage {
	wchar_t msg[1024];
};

enum class WriteType { Grade, Resistance };

extern "C" {

	__declspec(dllexport) void StartBleDeviceWatcher();
	__declspec(dllexport) void StopBleDeviceWatcher();
	__declspec(dllexport) void clearDeviceList();
	__declspec(dllexport) void OnAdvertisementReceived(winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher watcher, winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs eventArgs);
	__declspec(dllexport) void FindService(winrt::Windows::Devices::Bluetooth::BluetoothLEDevice CurrentDevice);
	__declspec(dllexport) void FindAndSubscribeCharacteristic(winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService service);
	__declspec(dllexport) void WriteValueToCharacteristic(winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic CurrentNotifyCharacteristic, int value);
	__declspec(dllexport) void WriteResistanceToCharacteristic(winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic CurrentNotifyCharacteristic, int value);
	__declspec(dllexport) void EnableNotifications(winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic characteristic);
	__declspec(dllexport) void UpdateBytes(int value, WriteType type, uint8_t& highByte, uint8_t& lowByte, uint8_t& resByte);
}