#include "pch.h"
#include "SoarRender10.h"


#pragma warning(disable:4996)
#pragma comment(lib, "legacy_stdio_definitions.lib")
#if defined( DEBUG ) || defined( _DEBUG )
#pragma comment ( lib, "D3Dx10d.lib")
#pragma comment ( lib, "D3D10.lib")
#pragma comment ( lib,"dxerr.lib")
#pragma comment ( lib,"d3dcompiler.lib")
#else
#pragma comment ( lib, "D3D10.lib")
#pragma comment ( lib, "D3Dx10.lib")
#pragma comment ( lib,"dxerr.lib")
#pragma comment ( lib,"d3dcompiler.lib")
#endif

CSoarRender10::CSoarRender10() :
	d3dDevice_(0), vertexBuffer_(0),
	colorMap_(0), d_bEnableAlphaBlend(false),
	//////////////////////////////////////////////////////////////////////////
	d3d10Font_(NULL), d_isCreatedMode(true)
{
	d_Idstring = "D3D9Render10forSoar v1.0";
	d_rgbAFontColor = 0xFF00FF00;
}
CSoarRender10::~CSoarRender10()
{
	Shutdown();
}
const  CLeeString& CSoarRender10::getIdentityString(void) const
{
	return d_Idstring;
}
bool CSoarRender10::DisplayReset(void)
{
	updateparams();
	return true;
}
void CSoarRender10::updateparams()
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

bool CSoarRender10::SetTexture(CLeeString& textures, DWORD dwRGBA, bool usTransparent)
{
	d_imgFile = textures;
	d_rgbAColor = dwRGBA;
	if (d3dDevice_)
	{
		if (!createTexture())
		{
			return false;
		}
	}
	
	return true;
}
bool CSoarRender10::Initialize(HWND hwnd)
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
	//vertices_[0].color = vertices_[1].color = vertices_[2].color = vertices_[3].color = vertices_[4].color = vertices_[5].color = 0xffffffff;
	//vertices_[0].rhw = vertices_[1].rhw = vertices_[2].rhw = vertices_[3].rhw = vertices_[4].rhw = vertices_[5].rhw = 1.0f;
	//////////////////////////////////////////////////////////////////////////
	
	if (!createD3dDevice(false))
	{
		return false;
	}
	rateX = 1.0;
	rateY = 1.0;
	//////////////////////////////////////////////////////////////////////////
	D3DXMatrixIdentity( &m_matWorld);
	// Initialize the view matrix
	D3DXMatrixIdentity(&m_matView);
	//初始化正交矩形
	D3DXMatrixOrthoLH(&m_ortho, width_, height_, 0.1f, 1000.f);
	return LoadFixContent();
}
bool CSoarRender10::InitializeEx(LPVOID d3dDevice, LPVOID DXGISwapChain)
{
	if (NULL == d3dDevice||
		DXGISwapChain == NULL)
		return false;
	if (d3dDevice_ && d_isCreatedMode)
	{
		d3dDevice_->Release();
		d3dDevice_ = NULL;
	}
	d3dDevice_ = (ID3D10Device*)d3dDevice;
	pSwapChain = (IDXGISwapChain*)DXGISwapChain;
	//从创建参数找那个获取窗口
	if (FAILED(pSwapChain->GetDesc(&swap_chain_desc)))
	{
		return false;
	}
	//获取视口信息
	D3D10_VIEWPORT vp;
	d3dDevice_->RSGetViewports(NULL, &vp);
	d3dDevice_->OMGetRenderTargets(1, &pRenderTargetView, NULL);
	
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
	//vertices_[0].color = vertices_[1].color = vertices_[2].color = vertices_[3].color = vertices_[4].color = vertices_[5].color = 0xffffffff;
	//vertices_[0].rhw = vertices_[1].rhw = vertices_[2].rhw = vertices_[3].rhw = vertices_[4].rhw = vertices_[5].rhw = 1.0f;
	//////////////////////////////////////////////////////////////////////////
	//获取表面数据
	ID3D10Texture2D* ppBackBuffer = NULL;
	if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&ppBackBuffer) )) {
		D3D10_TEXTURE2D_DESC  surfaceDesc;
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
	D3DXMatrixIdentity(&m_matWorld);
	// Initialize the view matrix
	D3DXMatrixIdentity(&m_matView);
	//初始化正交矩形
	D3DXMatrixOrthoLH(&m_ortho, width_, height_, 0.1f, 1000.f);
	return LoadFixContent();
}
bool CSoarRender10::createD3dDevice(bool bforce)
{
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
	D3D10_DRIVER_TYPE driverTypes[] =
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

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
		hr = D3D10CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags,
			D3D10_SDK_VERSION, &swap_chain_desc, &pSwapChain, &d3dDevice_);
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
	ID3D10Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return FALSE;

	hr = d3dDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return FALSE;
	d3dDevice_->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// Setup the viewport
	D3D10_VIEWPORT vp;
	vp.Width = width_;
	vp.Height = height_;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d3dDevice_->RSSetViewports(1, &vp);

	// Create the effect
	return CreateEffect();
}
bool CSoarRender10::CreateEffect()
{
	if (pEffect)
	{
		pEffect->Release();
		pEffect = NULL;
	}
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
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
	hr = D3DX10CreateEffectFromFile("soar.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0,
		d3dDevice_, NULL, NULL, &pEffect, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be located. or have some error in the FX file",
			"Error", MB_OK);
		return FALSE;
	}

	// Obtain the technique
	pTechnique = pEffect->GetTechniqueByName("Render");
	// Obtain the variables
	pWorldVariable = pEffect->GetVariableByName("World")->AsMatrix();
	pViewVariable = pEffect->GetVariableByName("View")->AsMatrix();
	pProjectionVariable = pEffect->GetVariableByName("Projection")->AsMatrix();
	pSkinTexShaderRSVariable = pEffect->GetVariableByName("txDiffuse")->AsShaderResource();
	pmaskrgbVariable = pEffect->GetVariableByName("maskrgb")->AsVector();

	// Define the input layout
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		//{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},

	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	hr = d3dDevice_->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &pVertexLayout);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}


	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));
	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_NONE;
	rsDesc.FrontCounterClockwise = false;
	d3dDevice_->CreateRasterizerState(&rsDesc, &pNoCullRasterState);
	return true;
}
bool CSoarRender10::createRectVertex(void)
{
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
	}
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexPos) * 6;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices_;
	HRESULT hr = d3dDevice_->CreateBuffer(&bd, &InitData, &vertexBuffer_);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return FALSE;
	}
	
	return true;
}
bool CSoarRender10::createTexture(void)
{
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
	HRESULT d3dResult = D3DX10CreateShaderResourceViewFromFile(d3dDevice_,
		d_imgFile.c_str(), NULL, NULL, &colorMapRV,NULL);

	if (FAILED(d3dResult))
	{
		const TCHAR* errorString = DXGetErrorString(d3dResult);
		DXTRACE_ERR_MSGBOX(errorString, d3dResult);
		return false;
	}
	pSkinTexShaderRSVariable->SetResource(colorMapRV);
	
	// Get the actual 2D texture from the resource view.
	//ID3D10Texture2D* tex;
	colorMapRV->GetResource((ID3D10Resource**)&colorMap_);
	// Get the description of the 2D texture.
	colorMap_->GetDesc(&textureinfo);
	//创建混合对象
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.BlendEnable[0] = true;
	blendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	d3dDevice_->CreateBlendState(&blendDesc, &mTransparentBS);
	return true;
}
bool CSoarRender10::createFont(void)
{
	if (d3d10Font_)
	{
		d3d10Font_->Release();
		d3d10Font_ = NULL;
	}
	ZeroMemory(&d3dFontparam_, sizeof(D3DX10_FONT_DESC));
	d3dFontparam_.Height = 12;    // in logical units
	d3dFontparam_.Width = 12;    // in logical units    
	d3dFontparam_.Weight = 500;   // boldness, range 0(light) - 1000(bold)
	d3dFontparam_.Italic = false;
	d3dFontparam_.CharSet = DEFAULT_CHARSET;
	d3dFontparam_.Quality = 0;
	d3dFontparam_.PitchAndFamily = 0;
	strncpy(d3dFontparam_.FaceName, "Times New Roman", 30); // font style
	HRESULT hr = D3DX10CreateFontIndirect(d3dDevice_, &d3dFontparam_, &d3d10Font_);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return false;
	}
	return true;
}


bool CSoarRender10::LoadContent()
{
	// Override with demo specifics, if any...
	return true;
}

bool CSoarRender10::LoadFixContent()
{
	// Override with demo specifics, if any...
	createFont();
	createRectVertex();
	if (!createTexture())
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	return true;
}
void CSoarRender10::UnloadContent()
{
	// Override with demo specifics, if any...

	if (colorMap_)
		colorMap_->Release();
	if (vertexBuffer_)
		vertexBuffer_->Release();
	if (d3d10Font_)
	{
		d3d10Font_->Release();
	}
	if (vertices_)
	{
		delete vertices_;
	}
	vertices_ = NULL;
	colorMap_ = 0;
	vertexBuffer_ = 0;
	d3d10Font_ = NULL;
}
void CSoarRender10::Shutdown()
{
	UnloadContent();
	if (d3dDevice_ && d_isCreatedMode)
	{
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
		
	d3dDevice_ = 0;
}
void CSoarRender10::Release(void)
{
	delete this;
}
bool CSoarRender10::clearScene(void)
{
	if (d3dDevice_ == 0)
		return 0;
	// Clear the backbuffer
	float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f }; // RGBA
	d3dDevice_->ClearRenderTargetView(pRenderTargetView, ClearColor);
	return 1;
}
void CSoarRender10::beginScene(void) {
	if (d3dDevice_ == 0)
		return;
	D3DXCOLOR rgba = D3DXCOLOR(d_rgbAColor);
	vMaskColor.x = rgba.r;
	vMaskColor.y = rgba.g;
	vMaskColor.z = rgba.b;
	// maskrgb
	pmaskrgbVariable->SetFloatVector((float*)vMaskColor);
	D3DXMatrixIdentity(&m_matWorld);
	//窗口坐标系与DX坐标系 Y方向相反，中心为原点
	D3DXMatrixRotationX(&m_matWorld, angle_to_radian(180, 0, 0));
	//初始化正交矩形
	D3DXMatrixOrthoLH(&m_ortho, width_, height_, 0.1f, 1000.f);
}
void CSoarRender10::endScene(void) {
	if (d3dDevice_ == 0)
		return;
}
bool CSoarRender10::beginPatch(void)
{
	if (d3dDevice_ == 0)
		return 0;
	return 1;
}
bool CSoarRender10::endPatch(void)
{
	if (d3dDevice_ == 0)
		return 0;
	
	return 1;
}
void CSoarRender10::Present(void)
{
	if (d3dDevice_ == NULL ||
		pSwapChain == NULL )
		return;
	HRESULT hr = pSwapChain->Present(NULL, NULL);
}
bool CSoarRender10::EnableAlphaBlend(bool bEnable) {
	if (d_bEnableAlphaBlend != bEnable) {
		d_bEnableAlphaBlend = bEnable;
		return true;
	}
	return false;
}
bool CSoarRender10::GetAlphaBlendState() {
	return d_bEnableAlphaBlend;
}
double CSoarRender10::angle_to_radian(double degree, double min, double second)
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
void CSoarRender10::radian_to_angle(double rad, double ang[])
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
void CSoarRender10::Render(const RectF& destRect0, const RectF& texture_rect,
	ILeeTexture* texture, const PointF2D& PixelOffsetXY/*,IRenderCache*chache*/)
{
	if (d3dDevice_ == 0 ||
		vertexBuffer_ == NULL ||
		colorMap_ == NULL)
		return;
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
	vertices_[1].Tex.y = 1.0f * texture_rect.d_top / textureinfo.Width;
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
	d3dDevice_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	VOID* pVertices;
	HRESULT hr = vertexBuffer_->Map(D3D10_MAP_WRITE_DISCARD,0, (void**)&pVertices);
	if (FAILED(hr))
	{
		const TCHAR* errorString = DXGetErrorString(hr);
		DXTRACE_ERR_MSGBOX(errorString, hr);
		return;
	}
	memcpy(pVertices, vertices_, sizeof(VertexPos) * 6);
	vertexBuffer_->Unmap();
	//设置layout
	d3dDevice_->IAGetInputLayout(&pPrevVertexLayout);
	// Set the input layout
	d3dDevice_->IASetInputLayout(pVertexLayout);
	
	// Set primitive topology
	d3dDevice_->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//////////////////////////////////////////////////////////////////////////
	
	// Update Variables that never change
	pWorldVariable->SetMatrix((float*)&m_matWorld);
	pViewVariable->SetMatrix((float*)&m_matView);
	pProjectionVariable->SetMatrix((float*)&m_ortho);
	//设置混合状态
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if(d_bEnableAlphaBlend)
		d3dDevice_->OMSetBlendState(mTransparentBS, blendFactors, 0xffffffff);
	d3dDevice_->RSSetState(pNoCullRasterState);
	
	//////////////////////////////////////////////////////////////////////////
	
	// Render objects
	D3D10_TECHNIQUE_DESC techDesc;
	pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		pTechnique->GetPassByIndex(p)->Apply(0);
		d3dDevice_->Draw(6, 0);
	}
	
	//恢复之前的状态
	d3dDevice_->RSSetState(0);//恢复之前的状态
	d3dDevice_->OMSetBlendState(0, blendFactors, 0xffffffff);
	//restore IALayout
	d3dDevice_->IASetInputLayout(pPrevVertexLayout);
	if (pPrevVertexLayout != NULL)
		pPrevVertexLayout->Release();
	pPrevVertexLayout = NULL;
}
void CSoarRender10::RenderText(const RectF& destRect0, const PointF2D& PixelOffsetXY,
	const CLeeString& szText, int vertTextFmt, int horzTextFmt)
{
	if (d3d10Font_ == 0 ||
		d3dDevice_ == NULL)
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
		break;
	}
	case 3:
	{
		vfmt = DT_BOTTOM;
		break;
	}
	default:
		vfmt = DT_VCENTER;
	}
	int hfmt = DT_LEFT;
	switch (horzTextFmt) {
	case 4:
	{
		hfmt = DT_LEFT;
		break;
	}
	case 5:
	{
		hfmt = DT_RIGHT;
		break;
	}
	case 6:
	{
		hfmt = DT_CENTER;
		break;
	}
	case 7:
	{
		hfmt = DT_SINGLELINE;
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
	RectF destRect = destRect0;
	RECT rc;
	RECT rc1;
	destRect.scale(rateX, rateY);
	destRect.toWindowRect(rc);
	d3d10Font_->DrawText(NULL, szText.c_str(), -1, &rc, vfmt | hfmt, D3DXCOLOR(d_rgbAFontColor));
}

void CSoarRender10::OnWindowChanged(void)
{

	//////////////////////////////////////////////////////////////////////////
	//自己创建的设备，：reset或者重新创建设备
	if (d_isCreatedMode)
	{
		d3d10Font_->Release();
		d3d10Font_ = NULL;
		colorMap_->Release();
		colorMap_ = NULL;
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
		createD3dDevice(true);
		
		LoadFixContent();
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
	}
	D3DXMatrixIdentity(&m_matWorld);
	//窗口坐标系与DX坐标系 Y方向相反，中心为原点
	D3DXMatrixRotationX(&m_matWorld, angle_to_radian(180, 0, 0));
	//初始化正交矩形
	D3DXMatrixOrthoLH(&m_ortho, width_, height_, 0.1f, 1000.f);
	return;
}