#include "IDevExcept.h"
#include <map>
// 设备类型使用
const uint32_t DEVICE_ERR_TYPE_UNKNOWN         = 0x00000001u;    //设备未知异常
const uint32_t DEVICE_ERR_TYPE_WAIT			   = 0x00000002u;    //设备异常需要等待完成提示操作
const uint32_t DEVICE_ERR_TYPE_MOUDLE          = 0x00000004u;	 //设备模块异常
const uint32_t DEVICE_ERR_TYPE_TESTER_PREPARE  = 0x00000008u;    //测试者准备内容异常
const uint32_t DEVICE_ERR_TYPE_TESTER		   = 0x00000010u;    //测试过程中测试者异常
const uint32_t DEVICE_ERR_TYPE_FOUL            = 0x00000020u;    //违规
const uint32_t DEVICE_ERR_TYPE_TEST_RESULT     = 0x00000040u;    //测试结果异常
const uint32_t DEVICE_ERR_TYPE_STOP            = 0x00000080u;    //测试状态异常，直接结束


IDevExcept* createExpectObj(uint32_t iType);

class CDevExcept : public IDevExcept
{
public:

	CDevExcept(const std::map<uint32_t, uint32_t>& exceptCodeTable, 
			   const std::map<uint32_t, uint32_t>& exceptTypeTable,
			   const std::map<uint32_t, string>& exceptInfo);
	~CDevExcept();

	virtual string getErrDesc(const uint32_t iErrCode) const override;

	uint32_t checkErr(const uint32_t value) override;

private:
	const std::map<uint32_t, uint32_t>  m_errCodeTable;
	const std::map<uint32_t, uint32_t>  m_errTypeTable;
	const std::map<uint32_t, string>    m_strErrInfo;
};
