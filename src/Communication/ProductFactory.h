#pragma once
#include "IProductRegistrar.h"
#include <map>
#include <string>
//工厂模板类，用于获取和注册产品对象
// 模板参数 FileType_T 表示的类是产品抽象类
template <class ProductType_t>
class CProductFactory
{
public:
	// 获取工厂单例，工厂的实例是唯一的
	static CProductFactory<ProductType_t>& Instance()
	{
		static CProductFactory<ProductType_t> instance;
		return instance;
	}

	// 产品注册
    void RegisterProduct(IProductRegistrar<ProductType_t>* registrar, int type)
	{
        m_ProductRegistry[type] = registrar;
	}

    int getSize()
    {
        return m_ProductRegistry.size();
    }

    // 根据名字type，获取对应具体的产品对象
    ProductType_t* GetProduct(int type)
	{
		// 从map找到已经注册过的产品，并返回产品对象
        auto itFind = m_ProductRegistry.find(type);

        if (itFind != m_ProductRegistry.end() && itFind->second != nullptr)
		{
            return itFind->second->CreateProduct();
		}

        return nullptr;
	}

	void Clear(bool bNew)
	{
		if (bNew)
		{
            for(auto &elemetn : m_ProductRegistry)
            {
                delete elemetn.second;
                elemetn.second = nullptr;
            }
		}

		m_ProductRegistry.clear();
	}

private:
	// 禁止外部构造和虚构
	CProductFactory() { }
	~CProductFactory() {}

	// 禁止外部拷贝和赋值操作
	CProductFactory(const CProductFactory&);
	const CProductFactory& operator=(const CProductFactory&);

	// 保存注册过的产品，key:文件类型名 , value:对应文件类
    std::map<int, IProductRegistrar<ProductType_t>*> m_ProductRegistry;
};

