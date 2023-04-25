#pragma once
#include "IProductRegistrar.h"
#include "ProductFactory.h"

// 产品注册模板类，用于创建具体产品和从工厂里注册产品
// 模板参数 ProductType_t 表示的类是文件抽象类（基类），FileImpl_t 表示的类是具体产品（产品种类的子类）
template <class ProductType_t, class FileImpl_t>
class  CProductRegistrar :public IProductRegistrar<ProductType_t>
{
public:
	// 构造函数，用于注册产品到工厂，只能显示调用
    explicit CProductRegistrar (int iType)
	{
		// 通过工厂单例把产品注册到工厂
        CProductFactory<ProductType_t>::Instance().RegisterProduct(this, iType);
	}

	// 创建具体产品对象指针
	ProductType_t* CreateProduct()
	{
		return new FileImpl_t();
	}
};

