#pragma once
//基类,产品注册模板接口类 表示的类是文件处理类型抽象类
template<class ProductType_t>
class IProductRegistrar
{
public:
	//获取产品对象抽象接口
	virtual ProductType_t* CreateProduct() = 0;
    virtual ~IProductRegistrar() {}
protected:
	//禁止外部构造和虚构
	IProductRegistrar() {}

	//禁止外部拷贝和赋值操作
private:
	IProductRegistrar(const IProductRegistrar&);
	const IProductRegistrar& operator=(const IProductRegistrar&);
};

