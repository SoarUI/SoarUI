#ifndef LEE_CALLBACK_SDK_H
#define LEE_CALLBACK_SDK_H
/********************************************************************
    created:    2015/02/10
    created:    2015/2/10::   12:07
    filename:   E:\2015it\LeeSDK\headers\Global\leecallback.h
    file path:  E:\2015it\LeeSDK\headers\Global
    file base:  leecallback
    file ext:   h
    author:     nicklisir/lee
    copyright:  nicklisir@gmail.com QQ327681967
    purpose:    为回调而设计的类，包括类回调/自由回调，支持两类参数格式：1个参数或无参数
*********************************************************************/
#include<assert.h>
template< typename Ret ,typename EventArgs >
class ICallbackFactorSheet
{
public:

	ICallbackFactorSheet(void)
	{
	}
public:

	virtual ~ICallbackFactorSheet(void)
	{
	}
	virtual Ret operator()( EventArgs &Args ) const
	{
      return Ret();
	}
	virtual Ret operator()( void ) const
	{
		return Ret();
	}
};

template<typename Ret,typename EventArgs >
class FreeCallbackFactor:public ICallbackFactorSheet< Ret , EventArgs >
{
public:
	//定义自由函数回调格式
	typedef Ret (CALLBACK* FreeFunction_ptr )( EventArgs &args );
	typedef Ret (CALLBACK* FreeFunctionV_ptr )( void );
	FreeCallbackFactor( FreeFunction_ptr func ):ICallbackFactorSheet< Ret , EventArgs >( ) , 
		ptrFactor_impl( func ),ptrFactorV_impl(NULL)
	{

	}
	FreeCallbackFactor( FreeFunctionV_ptr func ):ICallbackFactorSheet< Ret , EventArgs >( ) , 
		ptrFactorV_impl( func ),ptrFactor_impl(NULL)
	{

	}
	virtual ~FreeCallbackFactor( )
	{
        
	}
	virtual Ret operator( )( EventArgs &Args ) const
	{
		if ( ptrFactor_impl )
		{
			return ptrFactor_impl( Args );
		}
		else
		{
			return Ret();
		}
       
	}
	virtual Ret operator( )( void) const
	{
		if ( ptrFactorV_impl )
		{
			return ptrFactorV_impl( );
		}
		else
		{
			return Ret();
		}

	}
protected:
FreeFunction_ptr ptrFactor_impl ;
FreeFunctionV_ptr ptrFactorV_impl ;
};
template< class TClass , typename Ret ,typename EventArgs >
class MemberCallbackFactor:public ICallbackFactorSheet< Ret , EventArgs >
{
public:
	//成员回调格式
	typedef Ret ( TClass::*MemberFunction_ptr )( EventArgs &args );
	typedef Ret ( TClass::*MemberFunctionV_ptr )( void );
	MemberCallbackFactor( MemberFunction_ptr func , TClass* pobj ) : CLS_Owner( pobj ) ,
		ptrFactor_impl( func ),ptrFactorV_impl(NULL)
	{
	}
	MemberCallbackFactor( MemberFunctionV_ptr func , TClass* pobj ) : CLS_Owner( pobj ) , 
		ptrFactorV_impl( func ),ptrFactor_impl(NULL)
	{
	}
	virtual ~MemberCallbackFactor()
	{

	}
	virtual Ret operator ( )( EventArgs &args ) const
	{
		if ( CLS_Owner 
			&& ptrFactor_impl )
		{
			//this->func(args);
			return ( CLS_Owner->*ptrFactor_impl )( args );
		}
		else
		{
			return Ret();
		}
      
	}
	virtual Ret operator ( )( void ) const
	{
		if ( CLS_Owner 
			&& ptrFactorV_impl )
		{
			//this->func();
			return ( CLS_Owner->*ptrFactorV_impl )( );
		}
		else
		{
			return Ret();
		}

	}
protected:
	MemberFunction_ptr ptrFactor_impl ;
	MemberFunctionV_ptr ptrFactorV_impl ;
	TClass * CLS_Owner ;
};
////对回调执行函数进行封装,适配器
template<typename Ret,typename EventArgs >
class CLeeCallbackcompiler
{
public:
	CLeeCallbackcompiler( ) : ptrFactor_impler( NULL ),ptrFactorV_impler(NULL)
	{}
	virtual Ret operator () ( EventArgs &args ) const
	{
		if ( ptrFactor_impler )
		{
			return ( *ptrFactor_impler )( args ) ;
		}
		else
		{
			return Ret();
		}
		
	} 
	virtual Ret operator () ( void ) const
	{
		if ( ptrFactorV_impler )
		{
			return ( *ptrFactorV_impler )(  ) ;
		}
		else
		{
			return Ret();
		}

	} 
	bool RegisterCallback( typename FreeCallbackFactor< Ret , EventArgs >::FreeFunction_ptr func )
	{
		UnRegisterCallback();
		ptrFactor_impler = new FreeCallbackFactor< Ret , EventArgs >( func ) ;
		if ( ptrFactor_impler )
		{
			return true;
		}
		return false;
	}
	bool RegisterCallback( typename FreeCallbackFactor< Ret , EventArgs >::FreeFunctionV_ptr funcV )
	{
		UnRegisterCallback();
		ptrFactorV_impler = new FreeCallbackFactor< Ret , EventArgs >( funcV ) ;
		if ( ptrFactorV_impler )
		{
			return true;
		}
		return false;
	}
	template< typename TClass>
	bool RegisterCallback( typename MemberCallbackFactor< TClass , Ret , EventArgs >::MemberFunction_ptr MemberFunc , TClass * obj )
	{
		UnRegisterCallback() ;
     ptrFactor_impler = new MemberCallbackFactor< TClass , Ret , EventArgs >( MemberFunc , obj ) ;
	 if ( ptrFactor_impler )
	 {
		 return true;
	 }
	 return false;
	}
	template< typename TClass>
	bool RegisterCallback( typename MemberCallbackFactor< TClass , Ret , EventArgs >::MemberFunctionV_ptr MemberFuncV , TClass * obj )
	{
		UnRegisterCallback() ;
		ptrFactorV_impler = new MemberCallbackFactor< TClass , Ret , EventArgs >( MemberFuncV , obj ) ;
		if ( ptrFactorV_impler )
		{
			return true;
		}
		return false;
	}
	bool UnRegisterCallback(void)
	{
		if ( ptrFactor_impler )
		{
			delete ptrFactor_impler ;
			ptrFactor_impler = NULL ;
			return true ;
		}
		if ( ptrFactorV_impler )
		{
			delete ptrFactorV_impler ;
			ptrFactorV_impler = NULL ;
			return true ;
		}
		return false;
	}
	virtual ~CLeeCallbackcompiler()
	{
		delete ptrFactor_impler ;
		delete ptrFactorV_impler ;
		ptrFactor_impler = NULL ;
		ptrFactorV_impler = NULL ;
	}
private:
	ICallbackFactorSheet< Ret , EventArgs > *ptrFactor_impler ;
	ICallbackFactorSheet< Ret , EventArgs > *ptrFactorV_impler ;
};

#endif