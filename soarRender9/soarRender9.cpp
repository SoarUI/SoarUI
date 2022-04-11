/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    DirectX 11 Base Class - Used as base class for all DirectX 11 demos in this book.
*/


#include"SoarRender9.h"
#include<xnamath.h>
#include "d3dcompiler.h"
#include <D3DX9Effect.h>
#include <d3d9.h>
#include <Windowsx.h>
#include "../SoarHeader/leelog.h"


#if defined( DEBUG ) || defined( _DEBUG )
#pragma comment ( lib, "D3Dx9d.lib")
#pragma comment ( lib, "D3D9.lib")
#pragma comment ( lib,"dxerr.lib")
#pragma comment ( lib,"d3dcompiler.lib")
#else
#pragma comment ( lib, "D3D9.lib")
#pragma comment ( lib, "D3Dx9.lib")
#pragma comment ( lib,"dxerr.lib")
#pragma comment ( lib,"d3dcompiler.lib")
#endif



#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


CSoarRender9::CSoarRender9( ) : 
d3dDevice_( 0 ), vertexBuffer_( 0 ),
colorMap_( 0 ), d_bEnableAlphaBlend(false),
//////////////////////////////////////////////////////////////////////////
d3d9Font_(NULL),d_isCreatedMode(true)
{
    d_Idstring ="D3D9Render9forFlydream v1.0";

}


CSoarRender9::~CSoarRender9( )
{
    Shutdown( );
}


bool CSoarRender9::Initialize(  HWND hwnd )
{
    hwnd_ = hwnd;
    RECT dimensions;
    GetClientRect( hwnd, &dimensions );

    width_ = dimensions.right - dimensions.left;
    height_ = dimensions.bottom - dimensions.top;

    HRESULT result;
    unsigned int driver = 0;
    //全局顶点
	vertices_ =new VertexPos[6];
	vertices_[0].z =vertices_[1].z=vertices_[2].z=vertices_[3].z=vertices_[4].z=vertices_[5].z=1.0f;
	vertices_[0].color =vertices_[1].color=vertices_[2].color=vertices_[3].color=vertices_[4].color=vertices_[5].color=0xffffffff;
	vertices_[0].rhw=vertices_[1].rhw=vertices_[2].rhw=vertices_[3].rhw=vertices_[4].rhw=vertices_[5].rhw=1.0f;
	//////////////////////////////////////////////////////////////////////////
	ZeroMemory( &d3dpp_, sizeof(d3dpp_) );
	d3dpp_.Windowed = TRUE;
	d3dpp_.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp_.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp_.hDeviceWindow =hwnd_;
	d3dpp_.BackBufferHeight=height_;//可以用 GetSystemMetrics(SM_CXSCREEN) 获得屏幕宽
	d3dpp_.BackBufferWidth=width_;    //可以用 GetSystemMetrics(SM_CYSCREEN) 获得屏幕高
	d3dpp_.FullScreen_RefreshRateInHz=0;//显示器刷新率，窗口模式该值必须为0
    if (!createD3dDevice(false))
    {
		return false;
    }
	 rateX =1.0;
	 rateY=1.0;
	//////////////////////////////////////////////////////////////////////////
	//初始化正交矩形
	D3DXMatrixOrthoLH(&m_ortho,width_,height_,0.1f,1000.f);
    return LoadFixContent( );
}
bool CSoarRender9::InitializeEx(LPVOID d3dDevice  )
{
	if(NULL==d3dDevice)
		return false;
	 if (d3dDevice_ && d_isCreatedMode)
	{
		d3dDevice_->Release();
		d3dDevice_ =NULL ;
	}
	 d3dDevice_ = (LPDIRECT3DDEVICE9) d3dDevice; 
	 //从创建参数找那个获取窗口
	 D3DDEVICE_CREATION_PARAMETERS d3dCreateParams;
	if(D3D_OK !=d3dDevice_->GetCreationParameters(&d3dCreateParams)){
		return false;
	}
	hwnd_=d3dCreateParams.hFocusWindow;
    HRESULT result;
	RECT dimensions;
    GetClientRect( hwnd_, &dimensions );

    width_ = dimensions.right - dimensions.left;
    height_ = dimensions.bottom - dimensions.top;
    unsigned int driver = 0;
    //全局顶点
	vertices_ =new VertexPos[6];
	vertices_[0].z =vertices_[1].z=vertices_[2].z=vertices_[3].z=vertices_[4].z=vertices_[5].z=1.0f;
	vertices_[0].color =vertices_[1].color=vertices_[2].color=vertices_[3].color=vertices_[4].color=vertices_[5].color=0xffffffff;
	vertices_[0].rhw=vertices_[1].rhw=vertices_[2].rhw=vertices_[3].rhw=vertices_[4].rhw=vertices_[5].rhw=1.0f;
	//////////////////////////////////////////////////////////////////////////
	//获取表面数据
	IDirect3DSurface9* ppBackBuffer=NULL;
	if(d3dDevice_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &ppBackBuffer)==D3D_OK && ppBackBuffer!=NULL ){
		D3DSURFACE_DESC  surfaceDesc;
	  ppBackBuffer->GetDesc(&surfaceDesc);

	  ppBackBuffer->Release();
		rateX =1.0*surfaceDesc.Width/width_;
		rateY=1.0*surfaceDesc.Height/height_;
	  width_ =surfaceDesc.Width;
	  height_ = surfaceDesc.Height;
	}
	  
	ZeroMemory( &d3dpp_, sizeof(d3dpp_) );
	d_isCreatedMode=false;
	//////////////////////////////////////////////////////////////////////////
	//初始化正交矩形
	D3DXMatrixOrthoLH(&m_ortho,width_,height_,0.1f,1000.f);
	
    return LoadFixContent( );
}
bool CSoarRender9::createD3dDevice(bool bforce)
{
	if (d3dDevice_)
	{
		d3dDevice_->Release();
		d3dDevice_ =NULL ;
	}
	IDirect3D9 * d3d_ = Direct3DCreate9( D3D_SDK_VERSION ) ;
	if( FAILED( d3d_->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd_,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp_, &d3dDevice_ ) ) )
	{
		d3d_->Release();
		return false;
	}
	d_isCreatedMode =true;
	d3d_->Release();
	return true ;
}
bool CSoarRender9::createRectVertex(void)
{
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ =NULL;
	}
	HRESULT hr = d3dDevice_->CreateVertexBuffer( 6*sizeof(VertexPos),
		0 /*Usage*/, D3DFVF_CUSTOMVERTEX, D3DPOOL_SYSTEMMEM, &vertexBuffer_, NULL ) ;
	if( FAILED( hr ) )
	{
		const TCHAR* errorString = DXGetErrorString(hr) ;
		DXTRACE_ERR_MSGBOX(errorString, hr) ;
		return false;
	}
	return true;
}
bool CSoarRender9::createFont(void)
{
	if (d3d9Font_)
	{
		d3d9Font_->Release();
		d3d9Font_ =NULL;
	}
	ZeroMemory(&d3dFontparam_, sizeof(D3DXFONT_DESC));
	d3dFontparam_.Height         = 12;    // in logical units
	d3dFontparam_.Width          = 12;    // in logical units    
	d3dFontparam_.Weight         = 500;   // boldness, range 0(light) - 1000(bold)
	d3dFontparam_.Italic         = false;      
	d3dFontparam_.CharSet        = DEFAULT_CHARSET;          
	d3dFontparam_.Quality        = 0;           
	d3dFontparam_.PitchAndFamily = 0;           
	strncpy(d3dFontparam_.FaceName, "Times New Roman",30); // font style
	HRESULT hr =D3DXCreateFontIndirect(  d3dDevice_, &d3dFontparam_, &d3d9Font_);
		if( FAILED( hr ) )
		{
			const TCHAR* errorString = DXGetErrorString(hr) ;
			DXTRACE_ERR_MSGBOX(errorString, hr) ;
			return false;
		}
		return true;
}
bool CSoarRender9::createTexture(void)
{
	if (colorMap_)
	{
		colorMap_->Release();
		colorMap_ =NULL ;
	}
	HRESULT d3dResult = D3DXCreateTextureFromFileEx( d3dDevice_,
		d_imgFile.c_str(),D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,
		D3DFMT_UNKNOWN,D3DPOOL_DEFAULT, D3DX_FILTER_TRIANGLE, D3DX_FILTER_TRIANGLE,d_rgbAColor,&textureinfo,NULL,&colorMap_ );

	if( FAILED( d3dResult ) )
	{
		const TCHAR* errorString = DXGetErrorString(d3dResult) ;
		DXTRACE_ERR_MSGBOX(errorString, d3dResult) ;
		return false;
	}
	return true ;
}
bool CSoarRender9::LoadContent( )
{
    // Override with demo specifics, if any...
    return true;
}

bool CSoarRender9::LoadFixContent( )
{
	// Override with demo specifics, if any...
	createFont();
	createRectVertex();
	if (! createTexture())
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	
	
    //////////////////////////////////////////////////////////////////////////
	return true;
}
void CSoarRender9::UnloadContent( )
{
    // Override with demo specifics, if any...
	
	if( colorMap_ ) 
		colorMap_->Release( );
	if( vertexBuffer_ ) 
		vertexBuffer_->Release( );
	if (d3d9Font_)
	{
		d3d9Font_->Release();
	}
	if (vertices_)
	{
		delete vertices_;
	}
	vertices_ =NULL;
	colorMap_ = 0 ;
	vertexBuffer_ = 0;
	d3d9Font_ =NULL;
}


void CSoarRender9::Shutdown( )
{
    UnloadContent( );
    if( d3dDevice_ && d_isCreatedMode) d3dDevice_->Release( );    
    d3dDevice_ = 0;
}
void CSoarRender9::Release(void)
{
	delete this;
}
bool CSoarRender9::clearScene(void)
{
	if( d3dDevice_ == 0 )
		return 0;
    
	 // Clear the back buffer to a blue color
	 d3dDevice_->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );
	return 1;
}
void CSoarRender9::beginScene(void){
	if( d3dDevice_ == 0 )
		return ;
	 d3dDevice_->BeginScene();
}
void CSoarRender9::endScene(void){
	if( d3dDevice_ == 0 )
		return ;
	d3dDevice_->EndScene();
}
bool CSoarRender9::beginPatch(void)
{
	if( d3dDevice_ == 0 )
		return 0;
	
	d3dDevice_->GetTransform(D3DTS_PROJECTION, &m_matProject); //m_matWorld
	d3dDevice_->GetTransform(D3DTS_WORLD, &m_matWorld);
	d3dDevice_->GetTransform(D3DTS_VIEW,&m_matView);
	D3DXMATRIX matWorld;
	 D3DXMatrixIdentity( &matWorld );
	d3dDevice_->SetTransform(D3DTS_WORLD,&matWorld);
	 //view
	D3DXVECTOR3 vEyePt   ( 0.0f, 3.0f,-5.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec   ( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	d3dDevice_->SetTransform( D3DTS_VIEW, &matView );
	d3dDevice_->SetTransform( D3DTS_PROJECTION, &m_ortho);
	return 1;
}
bool CSoarRender9::endPatch(void)
{
	if( d3dDevice_ == 0 )
		return 0;
	d3dDevice_->SetTransform(D3DTS_WORLD,&m_matWorld);
	d3dDevice_->SetTransform(D3DTS_PROJECTION, &m_matProject); 
	d3dDevice_->SetTransform(D3DTS_VIEW ,&m_matView);
	return 1;
}
void CSoarRender9::Present(void)
{
	if( d3dDevice_ == 0 )
		return ;
	HRESULT hr =d3dDevice_->Present( NULL, NULL, NULL, NULL );
}
bool CSoarRender9::EnableAlphaBlend(bool bEnable){
	if(d_bEnableAlphaBlend != bEnable){
		 d_bEnableAlphaBlend =bEnable;
		 //开启透明效果 alpha纹理混合:纹理alpha值》材质》顶点颜色的alpha值
	d3dDevice_->SetRenderState( D3DRS_ALPHABLENDENABLE,  d_bEnableAlphaBlend );
		 return true;
	 }
	 return false;
}
bool CSoarRender9::GetAlphaBlendState(){
	return d_bEnableAlphaBlend;
	  }
//
void CSoarRender9::Render(const RectF & destRect0,const RectF&texture_rect,
					ILeeTexture* texture,const PointF2D& PixelOffsetXY/*,IRenderCache*chache*/)
{
	if( d3dDevice_ == 0 ||
		vertexBuffer_ ==NULL ||
		colorMap_ ==NULL)
		return ;
	/*
	X=(2X-width)/width  Y=(height-2y)/height
	*/
	/*VertexPos vertices[] =
	{
		{ XMFLOAT3(  1.0f,  1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },A
		{ XMFLOAT3(  1.0f, 0.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },B
		{ XMFLOAT3( 0.0f, 0.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },C

		{ XMFLOAT3( 0.0f,0.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },C
		{ XMFLOAT3( 0.0f,  1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },D
		{ XMFLOAT3(  1.0f,  1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },A
	};*/
	RectF destRect=destRect0;
	destRect.scale(rateX,rateY);
	//C:
	vertices_[0].x =1.0f * destRect.d_left;
	vertices_[0].y =1.0f * destRect.d_top;;
	vertices_[0].tu=1.0f * texture_rect.d_left/textureinfo.Width ;
	vertices_[0].tv=1.0f * texture_rect.d_top/textureinfo.Height ;
	//vertices_[0].color=d_rgbAColor;
	//B:
	vertices_[1].x =1.0f * destRect.d_right;;
	vertices_[1].y =1.0f * destRect.d_top;;
	vertices_[1].tu=1.0f * texture_rect.d_right/textureinfo.Width ;
	vertices_[1].tv=1.0f * texture_rect.d_top/textureinfo.Width;
	//vertices_[1].color=d_rgbAColor;
	//D:
	vertices_[2].x =1.0f * destRect.d_left;;
	vertices_[2].y =1.0f * destRect.d_bottom;;
	vertices_[2].tu=1.0f * texture_rect.d_left/textureinfo.Width ;
	vertices_[2].tv=1.0f * texture_rect.d_bottom/textureinfo.Height ;
	//vertices_[2].color=d_rgbAColor;

	//D:
	vertices_[3].x =1.0f * destRect.d_left;
	vertices_[3].y =1.0f * destRect.d_bottom;
	vertices_[3].tu=1.0f * texture_rect.d_left/textureinfo.Width ;
	vertices_[3].tv=1.0f * texture_rect.d_bottom/textureinfo.Height ;
	//vertices_[3].color=d_rgbAColor;
	//B:
	vertices_[4].x =1.0f * destRect.d_right;
	vertices_[4].y =1.0f * destRect.d_top;
	vertices_[4].tu=1.0f * texture_rect.d_right/textureinfo.Width ;
	vertices_[4].tv=1.0f * texture_rect.d_top/textureinfo.Height;
	//vertices_[4].color=d_rgbAColor;
	
	////A:
	vertices_[5].x =1.0f * destRect.d_right;
	vertices_[5].y =1.0f * destRect.d_bottom;
	vertices_[5].tu=1.0f * texture_rect.d_right/textureinfo.Width;
	vertices_[5].tv=1.0f * texture_rect.d_bottom/textureinfo.Height;
	//vertices_[5].color=d_rgbAColor;
	
	//////////////////////////////////////////////////////////////////////////
	VOID* pVertices;
	HRESULT hr =vertexBuffer_->Lock( 0, sizeof(VertexPos)*6, (void**)&pVertices, 0 ) ;
	if( FAILED(hr ) )
	{
		const TCHAR* errorString = DXGetErrorString(hr) ;
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return ;
	}

	memcpy( pVertices, vertices_, sizeof(VertexPos)*6 );

	vertexBuffer_->Unlock();
	//////////////////////////////////////////////////////////////////////////
	
	//颜色混合
	d3dDevice_->SetTexture( 0, colorMap_);
	d3dDevice_->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );//颜色混合：相乘
	d3dDevice_->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );//颜色来源---材质
	d3dDevice_->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );//颜色来源---漫反射
	//d3dDevice_->SetTextureStageState( 0,D3DTSS_COLOROP, D3DTOP_SELECTARG1 );//使用第一个颜色作为第一个texturestage的输出
	//开启透明效果 alpha纹理混合:纹理alpha值》材质》顶点颜色的alpha值
	//d3dDevice_->SetRenderState( D3DRS_ALPHABLENDENABLE,  true );
	//对于通过程序顶点构造的多边形渲染时必须开启ALPHA通道
	d3dDevice_->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );//ALPHA混合：相乘
	
	d3dDevice_->SetTextureStageState( 0, D3DTSS_ALPHAARG1,   D3DTA_TEXTURE );//ALPHA来源---纹理
	d3dDevice_->SetTextureStageState( 0, D3DTSS_ALPHAARG2,   D3DTA_DIFFUSE );//ALPHA来源---漫反射
	//设置为普通Alpha混合
	d3dDevice_->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
	d3dDevice_->SetRenderState( D3DRS_DESTBLEND,   D3DBLEND_INVSRCALPHA );
	d3dDevice_->SetRenderState( D3DRS_ALPHAFUNC,   D3DCMP_NOTEQUAL );
	d3dDevice_->SetRenderState( D3DRS_ALPHAREF,   d_rgbAColor );
	// Set the image states to get a good quality image.
	// 设置放大过滤器为线性过滤器
	d3dDevice_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	// 设置缩小过滤器为线性过滤器
	d3dDevice_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// Set default rendering states.
	d3dDevice_->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3dDevice_->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//设置线框模式
	//d3dDevice_->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	//关闭Zbuffer
	d3dDevice_->SetRenderState(D3DRS_ZENABLE, FALSE);
	//////////////////////////////////////////////////////////////////////////
	unsigned int stride = sizeof( VertexPos );
	unsigned int offset = 0;
	d3dDevice_->SetStreamSource( 0, vertexBuffer_, offset, stride );
	d3dDevice_->SetFVF( D3DFVF_CUSTOMVERTEX );
	d3dDevice_->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
	d3dDevice_->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	
}
void CSoarRender9::RenderText(const RectF & destRect0,const PointF2D& PixelOffsetXY,
						const CLeeString& szText,int vertTextFmt,int horzTextFmt)
{
	if (d3d9Font_==0 || 
		d3dDevice_ ==NULL)
	{
		return ;
	}
	int vfmt=DT_LEFT;
	switch(vertTextFmt){
	case 1:
		{
			vfmt=DT_TOP;
			break;
		}
		case 2:
		{
			vfmt=DT_VCENTER|DT_SINGLELINE;
			break;
		}
		case 3:
		{
			vfmt=DT_BOTTOM;
			break;
		}
		default:
			vfmt=DT_VCENTER;
	}
	int hfmt=DT_LEFT;
	switch(horzTextFmt){
	case 4:
		{
			hfmt=DT_LEFT;
			break;
		}
		case 5:
		{
			hfmt=DT_RIGHT;
			break;
		}
		case 6:
		{
			hfmt=DT_CENTER;
			break;
		}
		case 7:
		{
			hfmt=DT_SINGLELINE;
			break;
		}
		case 8:
		{
			hfmt=DT_WORDBREAK;
			break;
		}
		default:
			hfmt=DT_LEFT;
	}
	RectF destRect=destRect0;
	RECT rc ;
	d3dDevice_->SetRenderState(D3DRS_ZENABLE, FALSE);
	destRect.scale(rateX,rateY);
	destRect.toWindowRect(rc);
	d3d9Font_->DrawText(NULL,szText.c_str(),-1,&rc,vfmt|hfmt,D3DXCOLOR(0,1.0,0,1.0));
  	d3dDevice_->SetRenderState(D3DRS_ZENABLE, TRUE);
}
const  CLeeString& CSoarRender9::getIdentityString(void) const
{ 
	return d_Idstring;
}
 bool CSoarRender9::DisplayReset(void)
 {
	updateparams();
	 return true;
 }
void CSoarRender9::updateparams()
{
	RECT dimensions;
	GetClientRect( hwnd_, &dimensions );

	int width_1= dimensions.right - dimensions.left;
	int height_1 = dimensions.bottom - dimensions.top;

	if (width_1 != width_ ||
		height_1 != height_)
	{
		width_ =width_1;
		height_ =height_1;
		if (IsMinimized(hwnd_))
		{
			return ;
		}
		OnWindowChanged();
	
	}
	
}
void CSoarRender9::OnWindowChanged(void)
{
	
	//////////////////////////////////////////////////////////////////////////
	//自己创建的设备，：reset或者重新创建设备
	if(d_isCreatedMode)
	{
		d3d9Font_->Release();
		d3d9Font_ =NULL;
		colorMap_->Release();
		colorMap_ =NULL;
		vertexBuffer_->Release();
		vertexBuffer_ =NULL;
		//createD3dDevice(true);
	
		ZeroMemory( &d3dpp_, sizeof(d3dpp_) );
		d3dpp_.Windowed = TRUE;
		d3dpp_.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp_.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp_.hDeviceWindow =hwnd_;
		d3dpp_.BackBufferHeight=height_;
		d3dpp_.BackBufferWidth=width_;    
		d3dpp_.FullScreen_RefreshRateInHz=0;
		d3dDevice_->Reset(&d3dpp_);
		rateX =1.0;
		rateY=1.0;
		LoadFixContent();
	}
	else{
		//获取表面数据重新计算比例
		IDirect3DSurface9* ppBackBuffer=NULL;
		if(d3dDevice_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &ppBackBuffer)==D3D_OK && ppBackBuffer!=NULL )
		{
			D3DSURFACE_DESC  surfaceDesc;
		  ppBackBuffer->GetDesc(&surfaceDesc);

		  ppBackBuffer->Release();
		   rateX =1.0*surfaceDesc.Width/width_;
		  rateY=1.0*surfaceDesc.Height/height_;
		  width_ =surfaceDesc.Width;
		  height_ = surfaceDesc.Height;
		}
	}
	//初始化正交矩形
	D3DXMatrixOrthoLH(&m_ortho,width_,height_,0.1f,1000.f);
	return  ;
}
bool CSoarRender9::SetTexture(CLeeString & textures,DWORD dwRGBA,bool usTransparent) 
{
	d_imgFile =textures;
	d_rgbAColor =dwRGBA;
	if(d3dDevice_)
	{
		if (! createTexture())
		{
			return false;
		}
	}
	return true;
}