#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>


// Hier auf zusätzliche Header verweisen, die das Programm benötigt.

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <mutex>
#include <condition_variable>
#include <string>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Web.Syndication.h>

#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h"
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include "winrt/Windows.Devices.Enumeration.h"
#include "winrt/Windows.Storage.Streams.h"
