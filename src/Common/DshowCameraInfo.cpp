#include "DshowCameraInfo.h"
#include <dshow.h>
#include <QString>
#include <QDebug>

#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "OleAut32.lib")

CDshowCameraInfo::CDshowCameraInfo()
{
}


vector<string> & CDshowCameraInfo::Enumerate_Cameras_Name()
{
    //清空已经记录的信息
    if (m_vCameraInfo.size() != 0)
	{
        m_vCameraInfo.clear();
	}

    CoInitialize(nullptr);

    //创建设备枚举
    HRESULT hr = S_FALSE;
    ICreateDevEnum *pDevEnum = nullptr;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum,  nullptr,	CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, reinterpret_cast<void **>(&pDevEnum));
    if(FAILED(hr) || nullptr == pDevEnum)
    {
        return m_vCameraInfo;
    }

    // 创建视频输入设备枚举
    IEnumMoniker *pEnum = nullptr;
    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);

    if(FAILED(hr) || nullptr == pEnum)
    {
        if(nullptr != pDevEnum)
        {
            pDevEnum->Release();
            pDevEnum = nullptr;
        }
        return m_vCameraInfo;
    }

    // 遍历设备并获取设备信息
    IMoniker     *pMoniker = nullptr;
    IPropertyBag *pPropBag = nullptr;
    VARIANT      varName;
    VariantInit(&varName);

    while (pEnum->Next(1, &pMoniker, nullptr) == S_OK)
    {
        if(nullptr == pMoniker)
        {
            continue;
        }

        hr = pMoniker->BindToStorage(nullptr, nullptr, IID_IPropertyBag, reinterpret_cast<void **>(&pPropBag));
        if(FAILED(hr) || nullptr == pPropBag)
        {
            if(nullptr != pMoniker)
            {
                pMoniker->Release();
                pMoniker = nullptr;
            }
            continue;
        }

        //读取设备名称
        hr = pPropBag->Read(L"FriendlyName", &varName, nullptr);
        if(SUCCEEDED(hr))
        {
            // 存储设备名称
            wstring strDevName = varName.bstrVal;
            m_vCameraInfo.push_back(QString::fromStdWString(strDevName).toStdString());
            VariantClear(&varName);
        }

        if(nullptr != pPropBag)
        {
            pPropBag->Release();
            pPropBag = nullptr;
        }


        if(nullptr != pMoniker)
        {
            pMoniker->Release();
            pMoniker = nullptr;
        }

    }


    if(nullptr != pEnum)
    {
        pEnum->Release();
        pEnum = nullptr;
    }

    if(nullptr != pDevEnum)
    {
        pDevEnum->Release();
        pDevEnum = nullptr;
    }

    CoUninitialize();

    return m_vCameraInfo;
}


int CDshowCameraInfo::Get_Camera_Index(const string& strName)
{
    int iFindIndex = -1;
    for(size_t i = 0; i <  m_vCameraInfo.size(); i++)
    {
        if(m_vCameraInfo.at(i) == (strName))
        {
            iFindIndex = int(i);
            break;
        }
    }
    return  iFindIndex;
}


