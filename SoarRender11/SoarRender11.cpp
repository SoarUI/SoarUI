#include "pch.h"
#include "SoarRender11.h"
#include <windowsx.h>


#pragma warning(disable:4996)
#pragma comment(lib, "legacy_stdio_definitions.lib")
#if defined( DEBUG ) || defined( _DEBUG )
#pragma comment ( lib, "D3Dx11d.lib")
#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib,"dxerr.lib")
#pragma comment ( lib,"d3dcompiler.lib")
#pragma comment(lib,"freetype.lib")
//#pragma comment(lib,"winmm.lib")
//#pragma comment(lib,"comctl32.lib")
//#pragma comment(lib,"dxguid.lib")
#else
#pragma comment ( lib, "D3D11.lib")
#pragma comment ( lib, "D3Dx11.lib")
#pragma comment ( lib,"dxerr.lib")
#pragma comment ( lib,"d3dcompiler.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"freetype.lib")
#endif

CSoarRender11::CSoarRender11() :
	d3dDevice_(0), vertexBuffer_(0),
	colorMap_(0), d_bEnableAlphaBlend(false),
	//////////////////////////////////////////////////////////////////////////
	 d_isCreatedMode(true)
{
	d_Idstring = "D3DRender11forSoar v1.0";
	d_rgbAFontColor = 0xFF00FF00;

}
CSoarRender11::~CSoarRender11()
{
	Shutdown();
}
LPCTSTR CSoarRender11::getIdentityString(void) const
{
	return d_Idstring.c_str();
}
bool CSoarRender11::DisplayReset(void)
{
	updateparams();
	return true;
}
void CSoarRender11::updateparams()
{
	RECT dimensions;
	GetClientRect(hwnd_, &dimensions);

	int width_1 = dimensions.right - dimensions.left;
	int height_1 = dimensions.bottom - dimensions.top;

	if (width_1 != width_ ||
		height_1 != height_)
	{
		width_ = width_1;
		height_ = height_1;
		if (IsIconic(hwnd_))
		{
			return;
		}
		OnWindowChanged();

	}

}

bool CSoarRender11::SetTexture(CLeeString& textures, DWORD dwMaskARGB, DWORD dwFontARGB, bool usTransparent)
{
	d_imgFile = textures;
	d_rgbAColor = D3DXCOLOR(dwMaskARGB);
	d_rgbAFontColor = D3DXCOLOR(dwFontARGB);
	D3DXCOLOR rgba = D3DXCOLOR(d_rgbAColor);
	vMaskColor.x = rgba.r;
	vMaskColor.y = rgba.g;
	vMaskColor.z = rgba.b;
	if (d3dDevice_)
	{
		if (!createTexture())
		{
			return false;
		}
	}
	
	return true;
}
bool CSoarRender11::Initialize(HWND hwnd)
{
	hwnd_ = hwnd;
	RECT dimensions;
	GetClientRect(hwnd, &dimensions);

	width_ = dimensions.right - dimensions.left;
	height_ = dimensions.bottom - dimensions.top;

	HRESULT result;
	unsigned int driver = 0;
	//全局顶点
	vertices_ = new VertexPos[6];
	vertices_[0].z = vertices_[1].z = vertices_[2].z = vertices_[3].z = vertices_[4].z = vertices_[5].z = 1.0f;
	//////////////////////////////////////////////////////////////////////////
	
	if (!createD3dDevice(false))
	{
		return false;
	}
	if (!CreateEffect())
		return false;
	rateX = 1.0;
	rateY = 1.0;
	//////////////////////////////////////////////////////////////////////////
	mCameraRadius = 10.0f;
	mCameraRotationY = -XM_PI / 2.0f;
	mCameraHeight = 0.0f;
	
	vertices_font = new VertexFont[6];
	return LoadFixContent();
}
bool CSoarRender11::InitializeEx(LPVOID d3dDevice, LPVOID DXGISwapChain)
{
	if (NULL == d3dDevice||
		DXGISwapChain == NULL)
		return false;
	if (d3dDevice_ && d_isCreatedMode)
	{
		d3dDevice_->Release();
		d3dDevice_ = NULL;
	}
	d3dDevice_ = (ID3D11Device*)d3dDevice;
	pSwapChain = (IDXGISwapChain*)DXGISwapChain;
	d3dDevice_->AddRef();
	pSwapChain->AddRef();
	//从创建参数找那个获取窗口
	if (FAILED(pSwapChain->GetDesc(&swap_chain_desc)))
	{
		return false;
	}
	//GetImmediateContext
	d3dDevice_->GetImmediateContext(&pImmediateContext);
	if (pImmediateContext == NULL)
		return false;
	//获取视口信息
	D3D11_VIEWPORT vp;
	pImmediateContext->RSGetViewports(NULL, &vp);
	pImmediateContext->OMGetRenderTargets(1, &pRenderTargetView, &pDepthStencilView);
	
	if (pRenderTargetView == NULL)
	{
		return false;
	}
	
	d_isCreatedMode = false;
	hwnd_ = swap_chain_desc.OutputWindow;
	HRESULT result;
	RECT dimensions;
	GetClientRect(hwnd_, &dimensions);

	width_ = dimensions.right - dimensions.left;
	height_ = dimensions.bottom - dimensions.top;
	unsigned int driver = 0;
	//全局顶点
	vertices_ = new VertexPos[6];
	vertices_[0].z = vertices_[1].z = vertices_[2].z = vertices_[3].z = vertices_[4].z = vertices_[5].z = 1.0f;
	vertices_font = new VertexFont[6];
	//////////////////////////////////////////////////////////////////////////
	//获取表面数据
	ID3D11Texture2D* ppBackBuffer = NULL;
	if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&ppBackBuffer) )) {
		D3D11_TEXTURE2D_DESC  surfaceDesc;
		ppBackBuffer->GetDesc(&surfaceDesc);

		ppBackBuffer->Release();
		//其实可以是用Viewport代替
		rateX = 1.0 * surfaceDesc.Width / width_;
		rateY = 1.0 * surfaceDesc.Height / height_;
	}
	d_isCreatedMode = false;
	//////////////////////////////////////////////////////////////////////////
	if (!CreateEffect())
		return false;
	rateX = 1.0;
	rateY = 1.0;
	//////////////////////////////////////////////////////////////////////////
	mCameraRadius = 10.0f;
	mCameraRotationY =XM_PI / 2.0f;
	mCameraHeight = 0.0f;
	return LoadFixContent();
}
bool CSoarRender11::createD3dDevice(bool bforce)
{
	if (pImmediateContext) 
		pImmediateContext->ClearState();
	if (pImmediateContext) 
		pImmediateContext->Release();
	if (pDepthStencilView)
	{
		pDepthStencilView->Release();
		pDepthStencilView = NULL;
	}
	if (pDepthStencilBuffer)
	{
		pDepthStencilBuffer->Release();
		pDepthStencilBuffer = NULL;
	}
	//视口
	if (pRenderTargetView)
	{
		pRenderTargetView->Release();
		pRenderTargetView = NULL;
	}
	//交换链
	if (pSwapChain)
	{
		pSwapChain->Release();
		pSwapChain = NULL;
	}
	if (d3dDevice_)
	{
		d3dDevice_->Release();
		d3dDevice_ = NULL;
	}
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = width_;
	swap_chain_desc.BufferDesc.Height = height_;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = hwnd_;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = S_OK;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swap_chain_desc, &pSwapChain, &d3dDevice_, &featureLevel, &pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return FALSE;

	hr = d3dDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return FALSE;
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width_;
	descDepth.Height = height_;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = d3dDevice_->CreateTexture2D(&descDepth, NULL, &pDepthStencilBuffer);
	if (FAILED(hr))
		return FALSE;
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = d3dDevice_->CreateDepthStencilView(pDepthStencilBuffer, &descDSV, &pDepthStencilView);
	if (FAILED(hr))
		return FALSE;
	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = width_;
	vp.Height = height_;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1, &vp);

	// Create the effect
	return true;
}
//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}
bool CSoarRender11::CreateEffect()
{
	
	if (pVertexLayout)
	{
		pVertexLayout->Release();
		pVertexLayout = NULL;
	}
	if (pNoCullRasterState)
	{
		pNoCullRasterState->Release();
		pNoCullRasterState = NULL;
	}
	if (pVertexShader)
	{
		pVertexShader->Release();
		pVertexShader = NULL;
	}
	if (pPixelShader)
	{
		pPixelShader->Release();
		pPixelShader = NULL;
	}
	HRESULT hr = S_OK;
	// Compile the pixel shader
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("soar11.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be located. or have some error in the FX file",
			"Error", MB_OK);
		return FALSE;
	}
	// Create the vertex shader
	hr = d3dDevice_->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return FALSE;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create the input layout
	hr = d3dDevice_->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &pVertexLayout);
	pVSBlob->Release();
	
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}
	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("soar11.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return FALSE;
	}

	// Create the pixel shader
	hr = d3dDevice_->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pPixelShader);
	if (FAILED(hr))
	{
		pPSBlob->Release();
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}
	pPSBlob->Release();
	
	
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FrontCounterClockwise = false;
	d3dDevice_->CreateRasterizerState(&rsDesc, &pNoCullRasterState);
	return true;
}
bool CSoarRender11::createRectVertex(void)
{
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
	}
	if (pCBNeverChanges)
	{
		pCBNeverChanges->Release();
		pCBNeverChanges = NULL;
	}
	if (pCBChangeOnResize)
	{
		pCBChangeOnResize->Release();
		pCBChangeOnResize = NULL;
	}
	if (pCBChangesEveryFrame)
	{
		pCBChangesEveryFrame->Release();
		pCBChangesEveryFrame = NULL;
	}
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexPos) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices_;
	HRESULT hr = d3dDevice_->CreateBuffer(&bd, &InitData, &vertexBuffer_);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}
	// Create the constant buffers
	//D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBNeverChanges);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	hr = d3dDevice_->CreateBuffer(&bd, NULL, &pCBNeverChanges);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}

	bd.ByteWidth = sizeof(CBChangeOnResize);
	hr = d3dDevice_->CreateBuffer(&bd, NULL, &pCBChangeOnResize);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}

	bd.ByteWidth = sizeof(CBChangesEveryFrame);
	hr = d3dDevice_->CreateBuffer(&bd, NULL, &pCBChangesEveryFrame);
	if (FAILED(hr))
		return FALSE;
	
	// Initialize the view matrix
	return true;
}
bool CSoarRender11::createTexture(void)
{
	if (pSamplerLinear != NULL)
	{
		pSamplerLinear->Release();
		pSamplerLinear = NULL;
	}
	if (colorMap_)
	{
		colorMap_->Release();
		colorMap_ = NULL;
	}
	if (colorMapRV)
	{
		colorMapRV->Release();
		colorMapRV = NULL;
	}
	if (mTransparentBS)
	{
		mTransparentBS->Release();
		mTransparentBS = NULL;
	}
	HRESULT d3dResult = D3DX11CreateShaderResourceViewFromFile(d3dDevice_,
		d_imgFile.c_str(), NULL, NULL, &colorMapRV,NULL);

	if (FAILED(d3dResult))
	{
		const TCHAR* errorString = DXGetErrorString(d3dResult);
		DXTRACE_ERR_MSGBOX(errorString, d3dResult);
		return false;
	}
	
	// Get the actual 2D texture from the resource view.
	//ID3D10Texture2D* tex;
	colorMapRV->GetResource((ID3D11Resource**)&colorMap_);
	// Get the description of the 2D texture.
	colorMap_->GetDesc(&textureinfo);
	//创建混合对象
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	d3dDevice_->CreateBlendState(&blendDesc, &mTransparentBS);
	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	d3dResult = d3dDevice_->CreateSamplerState(&sampDesc, &pSamplerLinear);
	if (FAILED(d3dResult))
	{
		const TCHAR* errorString = DXGetErrorString(d3dResult);
		DXTRACE_ERR_MSGBOX(errorString, d3dResult);
		return false;
	}
	return true;
}
bool CSoarRender11::createFont(void)
{
	if (FT_Init_FreeType(&ft))
		return false;
	if (FT_New_Face(ft, "msyh.ttc", 0, &theface))
		return false;
	//FT_Select_Charmap(theface, FT_ENCODING_UNICODE);
	FT_Set_Pixel_Sizes(theface, 0, 12);
	return CreateFontEffect();
}
bool CSoarRender11::CreateFontEffect()
{
	if (pfontVertexLayout)
	{
		pfontVertexLayout->Release();
		pfontVertexLayout = NULL;
	}
	
	if (pfontVertexShader)
	{
		pfontVertexShader->Release();
		pfontVertexShader = NULL;
	}
	if (pfontPixelShader)
	{
		pfontPixelShader->Release();
		pfontPixelShader = NULL;
	}
	HRESULT hr = S_OK;
	// Compile the pixel shader
	ID3DBlob* pVSBlob = NULL;
	hr = CompileShaderFromFile("font.fx", "VSFont", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be located. or have some error in the FX file",
			"Error", MB_OK);
		return FALSE;
	}
	// Create the vertex shader
	hr = d3dDevice_->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pfontVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return FALSE;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create the input layout
	hr = d3dDevice_->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &pfontVertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}
	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("font.fx", "PSFont", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return FALSE;
	}

	// Create the pixel shader
	hr = d3dDevice_->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pfontPixelShader);
	if (FAILED(hr))
	{
		pPSBlob->Release();
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}
	pPSBlob->Release();
	return createFontVertex();
}
bool CSoarRender11::createFontVertex(void)
{
	if (fontvertexBuffer)
	{
		fontvertexBuffer->Release();
		fontvertexBuffer = NULL;
	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexFont) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices_font;
	HRESULT hr = d3dDevice_->CreateBuffer(&bd, &InitData, &fontvertexBuffer);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}
	if (pfontCBNeverChanges)
	{
		pfontCBNeverChanges->Release();
		pfontCBNeverChanges = NULL;
	}
	//D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBfontNeverChanges);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	hr = d3dDevice_->CreateBuffer(&bd, NULL, &pfontCBNeverChanges);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}
	if (pFontSamplerLinear)
	{
		pFontSamplerLinear->Release();
		pFontSamplerLinear = NULL;
	}
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = d3dDevice_->CreateSamplerState(&sampDesc, &pFontSamplerLinear);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return false;
	}
	//创建混合对象
	if (mfontBlend != NULL)
	{
		mfontBlend->Release();
		mfontBlend = NULL;
	}
	D3D11_BLEND_DESC bdesc;
	ZeroMemory(&bdesc, sizeof(bdesc));
	bdesc.AlphaToCoverageEnable = false;
	bdesc.RenderTarget[0].BlendEnable = true;
	bdesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bdesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bdesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bdesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bdesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bdesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bdesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = d3dDevice_->CreateBlendState(&bdesc, &mfontBlend);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return false;
	}
	return true;
}

bool CSoarRender11::LoadContent()
{
	// Override with demo specifics, if any...
	return true;
}

bool CSoarRender11::LoadFixContent()
{
	// Override with demo specifics, if any...
	createFont();
	createRectVertex();
	if (!createTexture())
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	setupMatrixMVP();
	//////////////////////////////////////////////////////////////////////////
	return true;
}
void CSoarRender11::UnloadContent()
{
	// Override with demo specifics, if any...
	if (mfontBlend != NULL)
	{
		mfontBlend->Release();
		mfontBlend = NULL;
	}
	if (pfontVertexLayout)
	{
		pfontVertexLayout->Release();
		pfontVertexLayout = NULL;
	}

	if (pfontVertexShader)
	{
		pfontVertexShader->Release();
		pfontVertexShader = NULL;
	}
	if (pfontPixelShader)
	{
		pfontPixelShader->Release();
		pfontPixelShader = NULL;
	}
	if (fontvertexBuffer)
	{
		fontvertexBuffer->Release();
		fontvertexBuffer = NULL;
	}
	if (pfontCBNeverChanges)
	{
		pfontCBNeverChanges->Release();
		pfontCBNeverChanges = NULL;
	}
	if (pFontSamplerLinear)
	{
		pFontSamplerLinear->Release();
		pFontSamplerLinear = NULL;
	}


	std::map<int, Character>::iterator it = Characters.begin();
	while (it != Characters.end())
	{
		if (it->second.TextureID)
			it->second.TextureID->Release();
		it = Characters.erase(it);
	}
	Characters.clear();
	if (fontvertexBuffer)
	{
		delete fontvertexBuffer;
		fontvertexBuffer = NULL;
	}
	
}
void CSoarRender11::Shutdown()
{
	UnloadContent();
	if (d3dDevice_ )
	{
		if (pDepthStencilView)
		{
			pDepthStencilView->Release();
			pDepthStencilView = NULL;
		}
		if (pImmediateContext)
			pImmediateContext->ClearState();
		if (pImmediateContext)
			pImmediateContext->Release();
		//视口
		if (pRenderTargetView)
		{
			pRenderTargetView->Release();
			pRenderTargetView = NULL;
		}
		//交换链
		if (pSwapChain)
		{
			pSwapChain->Release();
			pSwapChain = NULL;
		}
		if (d3dDevice_)
		{
			d3dDevice_->Release();
			d3dDevice_ = NULL;
		}
	}
	if (pVertexShader)
	{
		pVertexShader->Release();
		pVertexShader = NULL;
	}
	if (pPixelShader)
	{
		pPixelShader->Release();
		pPixelShader = NULL;
	}
	if (pNoCullRasterState)
	{
		pNoCullRasterState->Release();
		pNoCullRasterState = NULL;
	}
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
	}
	if (colorMap_)
	{
		colorMap_->Release();
		colorMap_ = NULL;
	}
	if (colorMapRV)
	{
		colorMapRV->Release();
		colorMapRV = NULL;
	}
	if (pCBNeverChanges)
	{
		pCBNeverChanges->Release();
		pCBNeverChanges = NULL;
	}
	if (pCBChangeOnResize)
	{
		pCBChangeOnResize->Release();
		pCBChangeOnResize = NULL;
	}
	if (pCBChangesEveryFrame)
	{
		pCBChangesEveryFrame->Release();
		pCBChangesEveryFrame = NULL;
	}
	if (mTransparentBS)
	{
		mTransparentBS->Release();
		mTransparentBS = NULL;
	}
	if(pSamplerLinear !=NULL)
	{
		pSamplerLinear->Release();
		pSamplerLinear = NULL;
	}
	if (pVertexLayout)
	{
		pVertexLayout->Release();
		pVertexLayout = NULL;
	}
	if (vertices_)
	{
		delete vertices_;
	}
	vertices_ = NULL;
	d3dDevice_ = 0;
}
void CSoarRender11::Release(void)
{
	delete this;
}
bool CSoarRender11::clearScene(void)
{
	if (d3dDevice_ == 0)
		return 0;
	// Clear the backbuffer
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // RGBA
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	return 1;
}
void CSoarRender11::beginScene(void) {
	if (d3dDevice_ == 0)
		return;
	//mCameraRotationY += 0.01f;
	setupMatrixMVP();
	
}
void CSoarRender11::endScene(void) {
	if (d3dDevice_ == 0)
		return;
}
bool CSoarRender11::beginPatch(void)
{
	if (d3dDevice_ == 0)
		return 0;
	
	return 1;
}
bool CSoarRender11::endPatch(void)
{
	if (d3dDevice_ == 0)
		return 0;
	
	return 1;
}
void CSoarRender11::Present(void)
{
	if (d3dDevice_ == NULL ||
		pSwapChain == NULL )
		return;
	HRESULT hr = pSwapChain->Present(NULL, NULL);
}
bool CSoarRender11::EnableAlphaBlend(bool bEnable) {
	if (d_bEnableAlphaBlend != bEnable) {
		d_bEnableAlphaBlend = bEnable;
		return true;
	}
	return false;
}
bool CSoarRender11::GetAlphaBlendState() {
	return d_bEnableAlphaBlend;
}
double CSoarRender11::angle_to_radian(double degree, double min, double second)
{
	double flag = (degree < 0) ? -1.0 : 1.0; //判断正负
	if (degree < 0)
	{
		degree = degree * (-1.0);
	}
	double angle = degree + min / 60 + second / 3600;
	double result = flag * (angle * PI) / 180;
	return result;
	//cout<<result<<endl;
}
//弧度转换为角度
void CSoarRender11::radian_to_angle(double rad, double ang[])
{
	double flag = (rad < 0) ? -1.0 : 1.0;
	if (rad < 0)
	{
		rad = rad * (-1.0);
	}
	double result = (rad * 180) / PI;
	double degree = int(result);
	double min = (result - degree) * 60;
	double second = (min - int(min)) * 60;
	ang[0] = flag * degree;
	ang[1] = int(min);
	ang[2] = second;
}
void CSoarRender11::Render(const RectF& destRect0, const RectF& texture_rect,
	ILeeTexture* texture, const PointF2D& PixelOffsetXY/*,IRenderCache*chache*/)
{
	if (d3dDevice_ == 0 ||
		vertexBuffer_ == NULL ||
		colorMap_ == NULL)
		return;
	//return;
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	RectF destRect = destRect0;
	destRect.scale(rateX, rateY);
	int w = width_ / 2;
	int h = height_ / 2;
	destRect.offsetRect(-w*1.0, - h*1.0);
	//C:
	vertices_[0].x = 1.0f * destRect.d_left;
	vertices_[0].y = 1.0f * destRect.d_top;;
	vertices_[0].z = 0;
	vertices_[0].Tex.x = 1.0f * texture_rect.d_left / textureinfo.Width;
	vertices_[0].Tex.y = 1.0f * texture_rect.d_top / textureinfo.Height;

	//vertices_[0].color=d_rgbAColor;
	//B:
	vertices_[1].x = 1.0f * destRect.d_right;;
	vertices_[1].y = 1.0f * destRect.d_top;;
	vertices_[1].z = 0;
	vertices_[1].Tex.x = 1.0f * texture_rect.d_right / textureinfo.Width;
	vertices_[1].Tex.y = 1.0f * texture_rect.d_top / textureinfo.Height;
	//vertices_[1].color=d_rgbAColor;
	//D:
	vertices_[2].x = 1.0f * destRect.d_left;;
	vertices_[2].y = 1.0f * destRect.d_bottom;
	vertices_[2].z = 0;
	vertices_[2].Tex.x = 1.0f * texture_rect.d_left / textureinfo.Width;
	vertices_[2].Tex.y = 1.0f * texture_rect.d_bottom / textureinfo.Height;
	//vertices_[2].color=d_rgbAColor;

	//D:
	vertices_[3].x = 1.0f * destRect.d_left;
	vertices_[3].y = 1.0f * destRect.d_bottom;
	vertices_[3].z = 0;
	vertices_[3].Tex.x = 1.0f * texture_rect.d_left / textureinfo.Width;
	vertices_[3].Tex.y = 1.0f * texture_rect.d_bottom / textureinfo.Height;
	//vertices_[3].color=d_rgbAColor;
	//B:
	vertices_[4].x = 1.0f * destRect.d_right;
	vertices_[4].y = 1.0f * destRect.d_top;
	vertices_[4].z = 0;
	vertices_[4].Tex.x = 1.0f * texture_rect.d_right / textureinfo.Width;
	vertices_[4].Tex.y = 1.0f * texture_rect.d_top / textureinfo.Height;
	//vertices_[4].color=d_rgbAColor;

	////A:
	vertices_[5].x = 1.0f * destRect.d_right;
	vertices_[5].y = 1.0f * destRect.d_bottom;
	vertices_[5].z = 0;
	vertices_[5].Tex.x = 1.0f * texture_rect.d_right / textureinfo.Width;
	vertices_[5].Tex.y = 1.0f * texture_rect.d_bottom / textureinfo.Height;
	//vertices_[5].color=d_rgbAColor;

	//////////////////////////////////////////////////////////////////////////
	
	// Set vertex buffer
	UINT stride = sizeof(VertexPos);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	VOID* pVertices;
	HRESULT hr = pImmediateContext->Map(vertexBuffer_,0, D3D11_MAP_WRITE_DISCARD,0, &mappedResource);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return;
	}
	memcpy(mappedResource.pData, vertices_, sizeof(VertexPos) * 6);
	pImmediateContext->Unmap(vertexBuffer_,0);
	//设置layout
	pImmediateContext->IAGetInputLayout(&pPrevVertexLayout);
	// Set the input layout
	pImmediateContext->IASetInputLayout(pVertexLayout);
	
	// Set primitive topology
	pImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//////////////////////////////////////////////////////////////////////////
	
	//设置混合状态
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if(d_bEnableAlphaBlend && mTransparentBS)
		pImmediateContext->OMSetBlendState(mTransparentBS, blendFactors, 0xffffffff);
	pImmediateContext->RSSetState(pNoCullRasterState);
	//////////////////////////////////////////////////////////////////////////
	//
	// Update variables that change once per frame
	//
	
	CBChangesEveryFrame cb;
	cb.mWorld = m_matWorld;
	cb.maskrgb = vMaskColor;
	if(pCBChangesEveryFrame)
		pImmediateContext->UpdateSubresource(pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);

	//
	// Render the cube
	//
	if(pVertexShader)
		pImmediateContext->VSSetShader(pVertexShader, NULL, 0);
	if (pCBNeverChanges)
		pImmediateContext->VSSetConstantBuffers(0, 1, &pCBNeverChanges);
	if (pCBChangeOnResize)
		pImmediateContext->VSSetConstantBuffers(1, 1, &pCBChangeOnResize);
	if (pCBChangesEveryFrame)
		pImmediateContext->VSSetConstantBuffers(2, 1, &pCBChangesEveryFrame);
	if(pPixelShader)
		pImmediateContext->PSSetShader(pPixelShader, NULL, 0);
	if(pCBChangesEveryFrame)
		pImmediateContext->PSSetConstantBuffers(2, 1, &pCBChangesEveryFrame);
	if(colorMapRV)
		pImmediateContext->PSSetShaderResources(0, 1, &colorMapRV);
	if(pSamplerLinear)
		pImmediateContext->PSSetSamplers(0, 1, &pSamplerLinear);
	pImmediateContext->Draw(6, 0);
	
	
	//恢复之前的状态
	pImmediateContext->RSSetState(0);//恢复之前的状态
	pImmediateContext->OMSetBlendState(0, blendFactors, 0xffffffff);
	//restore IALayout
	pImmediateContext->IASetInputLayout(pPrevVertexLayout);
	if (pPrevVertexLayout != NULL)
		pPrevVertexLayout->Release();
	pPrevVertexLayout = NULL;
}
void CSoarRender11::RenderText(const RectF& destRect0, const PointF2D& PixelOffsetXY,
	const CLeeString& szText, int vertTextFmt, int horzTextFmt)
{
	if (
		d3dDevice_ == NULL)
	{
		return;
	}
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	RectF destRect = destRect0;
	RECT rc;
	RECT rc1;
	destRect.scale(rateX, rateY);
	int w = width_ / 2;
	int h = height_ / 2;
	destRect.offsetRect(-w * 1.0, -h * 1.0);
	destRect.toWindowRect(rc);
	// Set vertex buffer
	UINT stride = sizeof(VertexFont);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &fontvertexBuffer, &stride, &offset);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	CBfontNeverChanges cb;
	cb.mWorld = m_matWorld;
	cb.mView = m_matView;
	cb.mProjection = m_ortho;
	cb.vfontColor = XMFLOAT4(d_rgbAFontColor);
	//cb.vMeshColor = vMeshColor;
	if (pfontCBNeverChanges)
		pImmediateContext->UpdateSubresource(pfontCBNeverChanges, 0, NULL, &cb, 0, 0);

	//设置layout
	pImmediateContext->IAGetInputLayout(&pPrevVertexLayout);
	// Set the input layout
	pImmediateContext->IASetInputLayout(pfontVertexLayout);

	// Set primitive topology
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (pfontVertexShader)
		pImmediateContext->VSSetShader(pfontVertexShader, NULL, 0);
	if (pfontCBNeverChanges)
		pImmediateContext->VSSetConstantBuffers(3, 1, &pfontCBNeverChanges);
	if (pfontPixelShader)
		pImmediateContext->PSSetShader(pfontPixelShader, NULL, 0);
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//if (d_bEnableAlphaBlend && mTransparentBS)
	pImmediateContext->OMSetBlendState(mfontBlend, blendFactors, 0xffffffff);
	pImmediateContext->RSSetState(pNoCullRasterState);
	// 恢复为默认值
	//pImmediateContext->OMSetDepthStencilState(0, 0);
	
	float scale = 1.0f;
	float x = rc.left;
	float y = rc.top;
	int rcw = rc.right - rc.left;
	int rch = rc.bottom - rc.top;
	if (rcw <= 0 ||
		rch <= 0)
	{
		return;
	}
	int vfmt = DT_LEFT;
	switch (vertTextFmt) {
	case 1:
	{
		vfmt = DT_TOP;
		break;
	}
	case 2:
	{
		vfmt = DT_VCENTER | DT_SINGLELINE;
		y = rc.top + (rc.bottom - rc.top) / 2.0f;
		break;
	}
	case 3:
	{
		vfmt = DT_BOTTOM;
		y = rc.bottom;
		break;
	}
	default:
		vfmt = DT_VCENTER;
		y = rc.top + (rc.bottom - rc.top) / 2.0f;
	}
	int hfmt = DT_LEFT;
	switch (horzTextFmt) {
	case 4:
	{
		hfmt = DT_LEFT;
		x = rc.left;
		break;
	}
	case 5:
	{
		hfmt = DT_RIGHT;
		x = rc.left;
		break;
	}
	case 6:
	{
		hfmt = DT_CENTER;
		x = rc.left + (rc.right - rc.left) / 2.0f;
		break;
	}
	case 7:
	{
		hfmt = DT_SINGLELINE;
		x = rc.left;
		break;
	}
	case 8:
	{
		hfmt = DT_WORDBREAK;
		break;
	}
	default:
		hfmt = DT_LEFT;
	}

	// 对文本中的所有字符迭代
	std::wstring utf8strings;
#ifdef UNICODE
	utf8strings = szText;
#else
	utf8strings = StringToWString(szText);
#endif // UNICODE
	std::wstring::const_iterator c;
	//caculate the avg height and one line width
	float totalw = 0.0f, totalh = 0.0f;
	for (c = utf8strings.begin(); c != utf8strings.end(); c++)
	{
		std::map<int, Character>::const_iterator it = Characters.find(*c);
		if (it == Characters.end())
		{
			Characters[*c] = GetCharacter(*c);
		}
		Character ch = Characters[*c];
		totalh = totalh < ch.Size.y * scale ? ch.Size.y * scale : totalh;
		// 更新位置到下一个字形的原点，注意单位是1/64像素
		totalw += (ch.Advance >> 6) * scale; //(2^6 = 64)
	}
	//caculate current line
	float suggest_linew = totalw > rcw ? rcw : totalw;
	float suggest_lineh = totalh > rch ? rch : totalh;
	if (hfmt & DT_CENTER)
		x -= suggest_linew * 0.5;
	if (vfmt & DT_VCENTER)
		y -= suggest_lineh * 0.5;
	float target_line_startx = x;
	float target_firstline_starty = y;
	//draw font texture
	for (c = utf8strings.begin(); c != utf8strings.end(); c++)
	{
		//exeed break
		if (x > target_line_startx + rcw)
		{

			if (hfmt & DT_SINGLELINE)
			{
				break;
			}
			else //multilines
			{
				y += suggest_lineh + 2;
				x = target_line_startx;
				if (y > target_firstline_starty + rch - suggest_lineh)
				{
					break;
				}
				continue;
			}
		}

		std::map<int, Character>::const_iterator it = Characters.find(*c);
		if (it == Characters.end())
		{
			Characters[*c] = GetCharacter(*c);
		}
		Character ch = Characters[*c];
		if (*c == '\r')
		{
			continue;
		}
		if (*c == '\n')
		{
			y += suggest_lineh + 2;
			x = target_line_startx;
			continue;
		}
		if (ch.TextureID==NULL)
		{
			// 更新位置到下一个字形的原点，注意单位是1/64像素
			x += (ch.Advance >> 6) * scale; //(2^6 = 64)
			continue;
		}
		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		float xpos = x + ch.Bearing.x * scale;
		float ypos = y + (ch.Size.y - ch.Bearing.y) * scale + (suggest_lineh - h);


		// 当前字符的VBO
		VertexFont fontvertices[6] = {
			
			{xpos,     ypos + h, 0.0,   XMFLOAT2(0.0, 1.0)},
			{ xpos,     ypos,	 0.0,   XMFLOAT2(0.0, 0.0) },
			{ xpos + w, ypos,    0.0,   XMFLOAT2(1.0, 0.0) },

			{ xpos,     ypos + h,0.0,   XMFLOAT2(0.0, 1.0) },
			{ xpos + w, ypos,    0.0,   XMFLOAT2(1.0, 0.0) },
			{ xpos + w, ypos + h,0.0,   XMFLOAT2(1.0, 1.0) }
			

		};
		// 在方块上绘制字形纹理
		
		// 更新当前字符的VBO
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		VOID* pVertices;
		HRESULT hr = pImmediateContext->Map(fontvertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			const TCHAR* errorString = DXGetErrorString(hr);
			DXTRACE_ERR_MSGBOX(errorString, hr);
			return;
		}
		memcpy(mappedResource.pData, fontvertices, sizeof(fontvertices));
		pImmediateContext->Unmap(fontvertexBuffer, 0);
		// 绘制方块
		
		if (ch.TextureID)
			pImmediateContext->PSSetShaderResources(1, 1, &ch.TextureID);
		if (pFontSamplerLinear)
			pImmediateContext->PSSetSamplers(1, 1, &pFontSamplerLinear);
		pImmediateContext->Draw(6, 0);
		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6) * scale; //(2^6 = 64)
	}
	//恢复之前的状态
	pImmediateContext->RSSetState(0);//恢复之前的状态
	pImmediateContext->OMSetBlendState(0, blendFactors, 0xffffffff);
	pImmediateContext->IASetInputLayout(pPrevVertexLayout);
	if (pPrevVertexLayout != NULL)
		pPrevVertexLayout->Release();
	pPrevVertexLayout = NULL;
}

void CSoarRender11::OnWindowChanged(void)
{

	//////////////////////////////////////////////////////////////////////////
	//自己创建的设备，：reset或者重新创建设备
	if (d_isCreatedMode)
	{
		createD3dDevice(true);
	}
	else {
		//重新获取窗口大小
		RECT dimensions;
		GetClientRect(hwnd_, &dimensions);
		width_ = dimensions.right - dimensions.left;
		height_ = dimensions.bottom - dimensions.top;
		//获取表面数据重新计算比例
		swap_chain_desc.BufferDesc.Width = width_;
		swap_chain_desc.BufferDesc.Height = height_;
		//获取表面数据-由宿主改变
		//pSwapChain->ResizeBuffers(swap_chain_desc.BufferCount,width_,height_, swap_chain_desc.BufferDesc.Format,swap_chain_desc.Flags);
		ID3D10Texture2D* ppBackBuffer = NULL;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&ppBackBuffer))) {
			D3D10_TEXTURE2D_DESC  surfaceDesc;
			ppBackBuffer->GetDesc(&surfaceDesc);

			ppBackBuffer->Release();
			//其实可以是用Viewport代替
			rateX = 1.0 * surfaceDesc.Width / width_;
			rateY = 1.0 * surfaceDesc.Height / height_;
		}
		d3dDevice_->GetImmediateContext(&pImmediateContext);
		if (pImmediateContext == NULL)
			throw ;
		//获取视口信息
		D3D11_VIEWPORT vp;
		pImmediateContext->RSGetViewports(NULL, &vp);
		if (pRenderTargetView != NULL)
		{
			pRenderTargetView->Release();
		}
		if (pDepthStencilView != NULL)
		{
			pDepthStencilView->Release();
		}
		pImmediateContext->OMGetRenderTargets(1, &pRenderTargetView, &pDepthStencilView);
	}
	CreateEffect();
	LoadFixContent();
	std::map<int, Character>::iterator it = Characters.begin();
	while(it!= Characters.end())
	{
		if (it->second.TextureID)
			it->second.TextureID->Release();
		it = Characters.erase(it);
	}
	Characters.clear();
	setupMatrixMVP();
	return;
}
void CSoarRender11::setupMatrixMVP()
{
	if (IsMinimized(hwnd_))
	{
		return;
	}
	XMMATRIX mx = XMMatrixIdentity();
	//窗口坐标系与DX坐标系 Y方向相反，中心为原点
	m_matWorld = mx;
	mx = XMMatrixRotationX( XM_PI);
	m_matWorld=XMMatrixMultiply( m_matWorld,mx);
	float x = mCameraRadius * cosf(mCameraRotationY);
	float z = mCameraRadius * sinf(mCameraRotationY);
	FXMVECTOR pos =XMVectorSet(x, mCameraHeight, z,0.0f);
	FXMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	FXMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_matView = XMMatrixLookAtLH( pos, target, up);
	
	//初始化正交矩形
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = m_matView;
	pImmediateContext->UpdateSubresource(pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
	//初始化正交矩形
	m_ortho = XMMatrixOrthographicLH(width_, height_, 0.1f, 1000.f);
	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = (m_ortho);
	pImmediateContext->UpdateSubresource(pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	CBfontNeverChanges cb;
	cb.mWorld = m_matWorld;
	cb.mView = m_matView;
	cb.mProjection = m_ortho;
	cb.vfontColor = XMFLOAT4(d_rgbAFontColor);
	if (pfontCBNeverChanges)
		pImmediateContext->UpdateSubresource(pfontCBNeverChanges, 0, NULL, &cb, 0, 0);
}
const CSoarRender11::Character CSoarRender11::GetCharacter(wchar_t c)
{
	Character ch;
	//FT_Select_Charmap(theface, ft_encoding_unicode);
	//修改颜色数据读取位数，默认是4字节，这里由于字体
	//贴图是一个灰度图，只占1字节，所以这里修改
	//一下颜色数据的读取位数，防止出现段错误
	ID3D11ShaderResourceView* __result{ nullptr };
	//从字体中加载字符c的字符信息
	FT_Load_Char(theface, c, FT_LOAD_RENDER);
	if (theface->glyph->bitmap.buffer==NULL)
	{
		ch.TextureID = NULL;
		ch.Size = XMSHORT2((SHORT)theface->glyph->bitmap.width, (SHORT)theface->glyph->bitmap.rows);
		ch.Bearing = XMFLOAT2(theface->glyph->bitmap_left, theface->glyph->bitmap_top);
		ch.Advance = theface->glyph->advance.x;
		return ch;
	}
	D3D11_SUBRESOURCE_DATA __subData;
	__subData.pSysMem = theface->glyph->bitmap.buffer;
	__subData.SysMemPitch = theface->glyph->bitmap.width;
	__subData.SysMemSlicePitch = theface->glyph->bitmap.width * theface->glyph->bitmap.rows; // 只对3D纹理有效

	ID3D11Texture2D* Tex2D;
	D3D11_TEXTURE2D_DESC  Tex2Dtdesc;

	Tex2Dtdesc.Width = theface->glyph->bitmap.width;
	Tex2Dtdesc.Height = theface->glyph->bitmap.rows;
	Tex2Dtdesc.MipLevels = 1;
	Tex2Dtdesc.ArraySize = 1;

	Tex2Dtdesc.SampleDesc.Count = 1;
	Tex2Dtdesc.SampleDesc.Quality = 0;
	Tex2Dtdesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2Dtdesc.Format = DXGI_FORMAT_A8_UNORM;
	Tex2Dtdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	Tex2Dtdesc.CPUAccessFlags = 0;
	Tex2Dtdesc.MiscFlags = 0;
	//将读取到的字符贴图数据加载到字符贴图中（从内存加载到显存）
	//这里的字符贴图数据都储存到r通道
	if (FAILED(d3dDevice_->CreateTexture2D(&Tex2Dtdesc, &__subData, &Tex2D))) {
		throw std::runtime_error("CreateTexture2D");
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = Tex2Dtdesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = Tex2Dtdesc.MipLevels;
	viewDesc.Texture2D.MostDetailedMip = 0;

	HRESULT hr = d3dDevice_->CreateShaderResourceView(Tex2D, &viewDesc, &ch.TextureID);
	if (FAILED(hr)) {
		
		throw std::runtime_error("CreateShaderResourceView");
	}
	if (Tex2D !=NULL)
	{
		Tex2D->Release();
	}
	//设置环绕方式和采样方式
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	//将字符信息储存到我们定义好的数据结构中
	ch.Size = XMSHORT2((SHORT)theface->glyph->bitmap.width, (SHORT)theface->glyph->bitmap.rows);
	ch.Bearing = XMFLOAT2(theface->glyph->bitmap_left, theface->glyph->bitmap_top);
	ch.Advance = theface->glyph->advance.x;

	return ch;
}
std::wstring CSoarRender11::StringToWString(const std::string& gbkData)
{


	int  len = 0;
	len = gbkData.length();
	int  unicodeLen = MultiByteToWideChar(CP_ACP,
		0,
		gbkData.c_str(),
		-1,
		NULL,
		0);
	wchar_t* pUnicode;
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,
		0,
		gbkData.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	std::wstring  rt;
	rt = (wchar_t*)pUnicode;
	delete  pUnicode;

	return  rt;

}