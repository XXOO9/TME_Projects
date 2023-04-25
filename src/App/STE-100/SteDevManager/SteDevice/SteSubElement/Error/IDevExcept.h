#pragma once
#include <string>
#include <stdint.h>

using namespace std;

const uint32_t DEVICE_NORMAL = 0;           // 设备正常运行
const uint32_t TYPE_NORMAL   = 0;           // 类型正常

class IDevExcept
{
public:
    IDevExcept() {}
    virtual ~IDevExcept() {}

    uint32_t getErrCode()     const { return m_iErrCode;     }

    uint32_t getErrType()     const { return m_iErrType;     }

    uint32_t getLastErrCode() const { return m_iLastErrCode; }

    uint32_t getLastErrType() const { return m_iLastErrType; }

    void     clearErr() {	m_iErrType = TYPE_NORMAL; m_iErrCode = DEVICE_NORMAL; m_iLastErrType = TYPE_NORMAL; m_iLastErrCode = DEVICE_NORMAL;}

	virtual  string   getErrDesc(const uint32_t iErrCode) const = 0;

	virtual  uint32_t checkErr(const uint32_t value) = 0;

protected:
    void     setErrCode(const uint32_t iErrCode)     { m_iErrCode     = iErrCode; }
    void     setErrType(const uint32_t iErrType)     { m_iErrType     = iErrType; }
    void     setLastErrCode(const uint32_t iErrCode) { m_iLastErrCode = iErrCode; }
    void     setLastErrType(const uint32_t iErrType) { m_iLastErrType = iErrType; }

private:

    uint32_t m_iErrType     = TYPE_NORMAL;
    uint32_t m_iErrCode     = DEVICE_NORMAL;
    uint32_t m_iLastErrType = TYPE_NORMAL;
    uint32_t m_iLastErrCode = DEVICE_NORMAL;
};

