// Dll_BLE.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.

#include "pch.h"
#include "Dll_BLE.h"

#define __WFILE__ L"Dll_BLE.cpp"

using namespace std;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Web::Syndication;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Storage::Streams;
using namespace winrt::Windows::Devices::Bluetooth::Advertisement;
using namespace winrt::Windows::Foundation;

uint8_t highByte = 0x00;
uint8_t lowByte = 0x00;
uint8_t resByte = 0x00;

BluetoothLEAdvertisementWatcher watcher;
//GattCharacteristic CurrentNotifyCharacteristic;
std::vector<winrt::Windows::Devices::Bluetooth::BluetoothLEDevice> DeviceList;


void StartBleDeviceWatcher() {
    watcher = BluetoothLEAdvertisementWatcher();

    watcher.ScanningMode(BluetoothLEScanningMode::Active);

    // Set the signal strength filter
    watcher.SignalStrengthFilter().InRangeThresholdInDBm(-80);
    watcher.SignalStrengthFilter().OutOfRangeThresholdInDBm(-90);
    watcher.SignalStrengthFilter().OutOfRangeTimeout(TimeSpan(5000));
    watcher.SignalStrengthFilter().SamplingInterval(TimeSpan(2000));

    // Register callback for when advertisements are received
    watcher.Received([](BluetoothLEAdvertisementWatcher const& watcher, BluetoothLEAdvertisementReceivedEventArgs const& args) {
        // Callback logic goes here
        });

    // Start watching for advertisements
    std::cout << "1. Start watching for devices..." << std::endl;
    watcher.Start();
}

void StopBleDeviceWatcher() {
    watcher.Stop();
}

void OnAdvertisementReceived(BluetoothLEAdvertisementWatcher watcher, BluetoothLEAdvertisementReceivedEventArgs eventArgs) {
    using namespace winrt::Windows::Devices::Bluetooth;
    using namespace winrt::Windows::Foundation;

    BluetoothLEDevice::FromBluetoothAddressAsync(eventArgs.BluetoothAddress()).Completed([watcher](IAsyncOperation<BluetoothLEDevice> asyncInfo, AsyncStatus asyncStatus) {
        if (asyncStatus == AsyncStatus::Completed) {
            BluetoothLEDevice currentDevice = asyncInfo.GetResults();

            if (currentDevice && currentDevice.Name() == L"KICKR BIKE 65E9") {
                // 停止扫描
                std::wcout << L"1.2 KICKR BIKE 65E9 is found" << std::endl;
                watcher.Stop();

                // 检查设备是否已在列表中
                auto it = std::find_if(DeviceList.begin(), DeviceList.end(), [&currentDevice](const BluetoothLEDevice& device) {
                    return device.DeviceId() == currentDevice.DeviceId();
                    });

                if (it == DeviceList.end()) {
                    DeviceList.push_back(currentDevice);

                    // TODO: 替换为相应的事件触发机制
                    // DeviceWatcherChanged(MsgType::BleDevice, currentDevice);
                }

                // StartMatching(currentDevice); // 需要将 StartMatching 转换为 C++/WinRT
            }
        }
        });
}

winrt::fire_and_forget EnableNotificationsAsync(GattCharacteristic characteristic) {
    auto MessAgeChanged = [](const std::wstring& message) {
        // 替换为消息处理逻辑
        std::wcout << message << std::endl;
    };

    MessAgeChanged(L"3.1 Checking for GATT communication status...");
    auto status = co_await characteristic.WriteClientCharacteristicConfigurationDescriptorAsync(winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattClientCharacteristicConfigurationDescriptorValue::Notify);

    if (status == winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::Success) {
        MessAgeChanged(L"3.2 GATT communication success");

        // 订阅特征值改变事件
        characteristic.ValueChanged([](winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic sender, winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs args) {
            // 在这里处理特征值改变事件
            });
    }
}

void clearDeviceList() {
    DeviceList.clear();
}

winrt::fire_and_forget FindAndSubscribeCharacteristicAsync(GattDeviceService service) {
    auto MessAgeChanged = [](const std::wstring& message) {
        // 替换为消息处理逻辑
        std::wcout << message << std::endl;
    };

    MessAgeChanged(L"3. Start to subscribe...");
    auto characteristicsResult = co_await service.GetCharacteristicsAsync();

    for (auto&& characteristic : characteristicsResult.Characteristics()) {
        if (characteristic.Uuid() == winrt::guid("00002ad9-0000-1000-8000-00805f9b34fb")) {
            auto CurrentNotifyCharacteristic = characteristic;
            EnableNotificationsAsync(characteristic);
            MessAgeChanged(L"3.3 Write characteristic subscribe successful.");
            // co_await Write(); // 写入数据
            break;
        }
    }
}

winrt::fire_and_forget FindServiceAsync(BluetoothLEDevice CurrentDevice) {
    auto MessAgeChanged = [](const std::wstring& message) {
        // 替换为消息处理逻辑
        std::wcout << message << std::endl;
    };

    MessAgeChanged(L"2. Start finding write service...");
    auto servicesResult = co_await CurrentDevice.GetGattServicesAsync();

    for (auto&& service : servicesResult.Services()) {
        if (service.Uuid() == winrt::guid("00001826-0000-1000-8000-00805f9b34fb")) {
            FindAndSubscribeCharacteristicAsync(service);
            break;
        }
    }
}

void FindService(BluetoothLEDevice CurrentDevice) {
    FindServiceAsync(CurrentDevice);
}



void FindAndSubscribeCharacteristic(GattDeviceService service) {
    FindAndSubscribeCharacteristicAsync(service);
}

winrt::fire_and_forget WriteValueToCharacteristicAsync(GattCharacteristic CurrentNotifyCharacteristic, int value) {
    auto MessAgeChanged = [](const std::wstring& message) {
        // 替换为消息处理逻辑
        std::wcout << message << std::endl;
    };

    // 更新字节
    // 这里需要根据您的 UpdateBytes 函数实现来更新字节
    // byte highByte = (value >> 8) & 0xFF;
    // byte lowByte = value & 0xFF;

    if (CurrentNotifyCharacteristic) {
        std::vector<uint8_t> data = { 0x11, 0x00, 0x00, highByte, lowByte, resByte, 0x00 };

        // 创建 DataWriter 来准备数据
        winrt::Windows::Storage::Streams::DataWriter dataWriter;
        for (auto& b : data) {
            dataWriter.WriteByte(b);
        }

        auto buffer = dataWriter.DetachBuffer();

        auto status = co_await CurrentNotifyCharacteristic.WriteValueAsync(buffer);

        if (status == winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::Success) {
            MessAgeChanged(L"4.3 Grade successfully written to characteristic.");
        }
        else {
            MessAgeChanged(L"4.3 Failed to write grade to characteristic.");
        }
    }
}

void WriteValueToCharacteristic(GattCharacteristic CurrentNotifyCharacteristic, int value) {
    WriteValueToCharacteristicAsync(CurrentNotifyCharacteristic, value);
}


winrt::fire_and_forget WriteResistanceToCharacteristicAsync(GattCharacteristic CurrentNotifyCharacteristic, int value) {
    auto MessAgeChanged = [](const std::wstring& message) {
        // 替换为消息处理逻辑
        std::wcout << message << std::endl;
    };

    // 更新字节
    // 这里需要根据您的 UpdateBytes 函数实现来更新字节
    // byte resByte = value & 0xFF;

    if (CurrentNotifyCharacteristic) {
        std::vector<uint8_t> data = { 0x11, 0x00, 0x00, highByte, lowByte, resByte, 0x00 };

        // 创建 DataWriter
        winrt::Windows::Storage::Streams::DataWriter dataWriter;
        for (auto& b : data) {
            dataWriter.WriteByte(b);
        }

        auto buffer = dataWriter.DetachBuffer();

        auto status = co_await CurrentNotifyCharacteristic.WriteValueAsync(buffer);

        if (status == winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus::Success) {
            MessAgeChanged(L"4.3 Resistance successfully written to characteristic.");
        }
        else {
            MessAgeChanged(L"4.3 Failed to write resistance to characteristic.");
        }
    }
}

void WriteResistanceToCharacteristic(GattCharacteristic CurrentNotifyCharacteristic, int value) {
    WriteResistanceToCharacteristicAsync(CurrentNotifyCharacteristic, value);
}

void EnableNotifications(GattCharacteristic characteristic) {
    EnableNotificationsAsync(characteristic);
}


void UpdateBytes(int value, WriteType type, uint8_t& highByte, uint8_t& lowByte, uint8_t& resByte) {
    switch (type) {
    case WriteType::Grade:
        highByte = static_cast<uint8_t>((value >> 8) & 0xFF);
        lowByte = static_cast<uint8_t>(value & 0xFF);
        break;
    case WriteType::Resistance:
        resByte = static_cast<uint8_t>(value & 0xFF);
        break;
    }
}
