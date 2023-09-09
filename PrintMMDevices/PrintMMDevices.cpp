#include <Windows.h>
#include <mmdeviceapi.h>
#include <iostream>
#include <Functiondiscoverykeys_devpkey.h>

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

int main() {
    // Initialize COM for this thread
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::cerr << "Error: CoInitializeEx failed." << std::endl;
        return 1;
    }

    IMMDeviceEnumerator* pEnumerator = NULL;
    hr = CoCreateInstance(
        CLSID_MMDeviceEnumerator, NULL,
        CLSCTX_ALL, IID_IMMDeviceEnumerator,
        (void**)&pEnumerator
    );
    if (FAILED(hr)) {
        std::cerr << "Error: CoCreateInstance failed." << std::endl;
        CoUninitialize();
        return 1;
    }

    // Enumerate audio output devices (eRender)
    IMMDeviceCollection* pDeviceCollection = NULL;
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDeviceCollection);
    if (FAILED(hr)) {
        std::cerr << "Error: EnumAudioEndpoints failed." << std::endl;
        pEnumerator->Release();
        CoUninitialize();
        return 1;
    }

    UINT deviceCount;
    hr = pDeviceCollection->GetCount(&deviceCount);
    if (FAILED(hr)) {
        std::cerr << "Error: GetCount failed." << std::endl;
        pDeviceCollection->Release();
        pEnumerator->Release();
        CoUninitialize();
        return 1;
    }

    std::cout << "Audio Output Devices:" << std::endl;

    for (UINT i = 0; i < deviceCount; i++) {
        IMMDevice* pDevice = NULL;
        hr = pDeviceCollection->Item(i, &pDevice);
        if (FAILED(hr)) {
            std::cerr << "Error: Item failed." << std::endl;
            break;
        }

        LPWSTR pwszID = NULL;
        hr = pDevice->GetId(&pwszID);
        if (FAILED(hr)) {
            std::cerr << "Error: GetId failed." << std::endl;
            pDevice->Release();
            break;
        }

        IPropertyStore* pProps = NULL;
        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (FAILED(hr)) {
            std::cerr << "Error: OpenPropertyStore failed." << std::endl;
            CoTaskMemFree(pwszID);
            pDevice->Release();
            break;
        }

        PROPVARIANT varName;
        PropVariantInit(&varName);
        hr = pProps->GetValue(PKEY_Device_DeviceDesc, &varName);
        if (SUCCEEDED(hr)) {
            std::wcout << "Device " << i + 1 << ": " << varName.pwszVal << std::endl;
            PropVariantClear(&varName);
        }

        pProps->Release();
        CoTaskMemFree(pwszID);
        pDevice->Release();
    }

    pDeviceCollection->Release();
    pEnumerator->Release();

    // Uninitialize COM for this thread
    CoUninitialize();

    return 0;
}