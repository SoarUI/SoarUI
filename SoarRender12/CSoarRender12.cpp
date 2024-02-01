#include "pch.h"
#include "CSoarRender12.h"
#include <stdexcept>
#include "D3Dcompiler.h"
#include"d3dx12.h"
#include <vector>
#include <cassert>
#include "winerror.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include "windowsx.h"

//#pragma comment(lib, "legacy_stdio_definitions.lib")
#ifdef _DEBUG
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
//#pragma comment ( lib,"dxerr.lib")
#pragma comment(lib,"freetype.lib")
#else
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment ( lib,"dxerr.lib")
#pragma comment(lib,"freetype.lib")
#endif

CSoarRender12::CSoarRender12():d_bEnableAlphaBlend(false),
//////////////////////////////////////////////////////////////////////////
d_isCreatedMode(true)
{
	d_Idstring = "D3DRender12forSoar v1.0";
	d_rgbAFontColor = 0xFF00FF00;
#if defined(_DEBUG)
	UINT dxgiFactoryFlags = 0;
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
}
CSoarRender12::~CSoarRender12()
{
	Shutdown();
}
LPCTSTR CSoarRender12::getIdentityString(void) const
{
	return d_Idstring.c_str();
}
bool CSoarRender12::DisplayReset(void)
{
	updateparams();
	return true;
}
void CSoarRender12::updateparams()
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
bool CSoarRender12::SetTexture(CLeeString& textures, DWORD dwMaskARGB, DWORD dwFontARGB, bool usTransparent)
{
	d_imgFile = textures;
	d_rgbAMaskColor = XMCOLOR(dwMaskARGB);
	d_rgbAFontColor = XMCOLOR(dwFontARGB);
	vfontColor.x = d_rgbAFontColor.r / 255.0f;
	vfontColor.y = d_rgbAFontColor.g / 255.0f;
	vfontColor.z = d_rgbAFontColor.b / 255.0f;
	vMaskColor.x = d_rgbAMaskColor.r/255.0f;
	vMaskColor.y = d_rgbAMaskColor.g / 255.0f;
	vMaskColor.z = d_rgbAMaskColor.b / 255.0f;
	if (d3dDevice_)
	{
		if (!createTexture())
		{
			return false;
		}
	}

	return true;
}
bool CSoarRender12::Initialize(HWND hwnd)
{
	hwnd_ = hwnd;
	RECT dimensions;
	GetClientRect(hwnd, &dimensions);

	width_ = dimensions.right - dimensions.left;
	height_ = dimensions.bottom - dimensions.top;
	//全局顶点
	vertices_ = new VertexPos[6];
	vertices_[0].z = vertices_[1].z = vertices_[2].z = vertices_[3].z = vertices_[4].z = vertices_[5].z = 1.0f;

	// Update the viewport transform to cover the client area.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(width_);
	mScreenViewport.Height = static_cast<float>(height_);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, (long)width_, (long)height_ };
	//////////////////////////////////////////////////////////////////////////
	if (!createD3dDevice(false))
	{
		return false;
	}
	if(!CreateCommandQueue())
		return false;
	CreateSwapChain();
	mCameraRadius = 800.0f;
	mCameraRotationY = -XM_PI / 2.0f;
	mCameraHeight = 10.0f;
	return LoadFixContent();
}
bool CSoarRender12::InitializeEx(LPVOID d3dDevice, LPVOID DXGISwapChain)
{
	return true;
}
void CSoarRender12::Shutdown(void)
{
	UnloadContent();
	return;
}
void CSoarRender12::Release(void)
{
	delete this;
}
bool CSoarRender12::clearScene(void)
{
	if (d3dDevice_ == 0)
		return 0;
	
	//清空命令列表和分配器
	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));
	//切换缓冲 前台-》后台，准备绘制数据
	 // Set necessary state.
	
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate that the back buffer will be used as a render target.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, mRtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	
	mCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	//mCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	// Record commands. 在这里添加绘制命令：
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	//清除模板
	mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH , 1.0f, 0, 0, nullptr);
	
	
	return 1;
}
void CSoarRender12::beginScene(void) {
	if (d3dDevice_ == 0)
		return;
	setupMatrixMVP();

}
void CSoarRender12::endScene(void) {
	if (d3dDevice_ == 0)
		return;
	

	//将要呈现后台缓冲的内容;关闭命令列表的录制开关；意味一个批次的命令已经录制完毕；交给GPU绘制
	// Indicate that the back buffer will now be used to present.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	ThrowIfFailed(mCommandList->Close());
	
}
bool CSoarRender12::beginPatch(void)
{
	if (d3dDevice_ == 0)
		return 0;
	
	return 1;
}
bool CSoarRender12::endPatch(void)
{
	if (d3dDevice_ == 0)
		return 0;
	
	return 1;
}
void CSoarRender12::Present(void)
{
	if (d3dDevice_ == NULL ||
		mSwapChain == NULL)
		return;
	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	HRESULT hr = mSwapChain->Present(1, NULL);
	//等待上衣帧处理完
	WaitForPreviousFrame();
}
void CSoarRender12::Render(const RectF& destRect0, const RectF& texture_rect,
	ILeeTexture* texture, const PointF2D& PixelOffsetXY/*,IRenderCache*chache*/)
{
	if (d3dDevice_ == 0 )
		return;
	mCommandList->SetGraphicsRootSignature(m_ui_rootSignature.Get());
	if (d_bEnableAlphaBlend)
	{
		mCommandList->SetPipelineState(m_ui_blendpipelineState.Get());
	}
	else
	{
		mCommandList->SetPipelineState(m_ui_pipelineState.Get());
	}
	ID3D12DescriptorHeap* ppHeaps[] = { mCbvHeap.Get()};
	mCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	ID3D12DescriptorHeap* ppHeaps1[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(ppHeaps1), ppHeaps1);
	mCommandList->SetGraphicsRootDescriptorTable(1, mSrvHeap->GetGPUDescriptorHandleForHeapStart());
	RectF destRect = destRect0;
	destRect.scale(1.0, 1.0);
	int w = width_ / 2;
	int h = height_ / 2;
	destRect.offsetRect(-w * 1.0, -h * 1.0);
	
	//C:
	vertices_[0].x = 1.0f * destRect.d_left ;
	vertices_[0].y = 1.0f * destRect.d_top ;
	vertices_[0].z = 0;
	vertices_[0].Tex.x = 1.0f * texture_rect.d_left / TextureWidth;
	vertices_[0].Tex.y = 1.0f * texture_rect.d_top / TextureHeight;

	//vertices_[0].color=d_rgbAMaskColor;
	//B:
	vertices_[1].x = 1.0f * destRect.d_right ;
	vertices_[1].y = 1.0f * destRect.d_top ;
	vertices_[1].z = 0;
	vertices_[1].Tex.x = 1.0f * texture_rect.d_right / TextureWidth;
	vertices_[1].Tex.y = 1.0f * texture_rect.d_top / TextureHeight;
	//vertices_[1].color=d_rgbAMaskColor;
	//D:
	vertices_[2].x = 1.0f * destRect.d_left ;
	vertices_[2].y = 1.0f * destRect.d_bottom ;
	vertices_[2].z = 0;
	vertices_[2].Tex.x = 1.0f * texture_rect.d_left / TextureWidth;
	vertices_[2].Tex.y = 1.0f * texture_rect.d_bottom / TextureHeight;
	//vertices_[2].color=d_rgbAMaskColor;

	//D:
	vertices_[3].x = 1.0f * destRect.d_left ;
	vertices_[3].y = 1.0f * destRect.d_bottom ;
	vertices_[3].z = 0;
	vertices_[3].Tex.x = 1.0f * texture_rect.d_left / TextureWidth;
	vertices_[3].Tex.y = 1.0f * texture_rect.d_bottom / TextureHeight;
	//vertices_[3].color=d_rgbAMaskColor;
	//B:
	vertices_[4].x = 1.0f * destRect.d_right ;
	vertices_[4].y = 1.0f * destRect.d_top ;
	vertices_[4].z = 0;
	vertices_[4].Tex.x = 1.0f * texture_rect.d_right / TextureWidth;
	vertices_[4].Tex.y = 1.0f * texture_rect.d_top / TextureHeight;
	//vertices_[4].color=d_rgbAMaskColor;

	////A:
	vertices_[5].x = 1.0f * destRect.d_right ;
	vertices_[5].y = 1.0f * destRect.d_bottom ;
	vertices_[5].z = 0;
	vertices_[5].Tex.x = 1.0f * texture_rect.d_right / TextureWidth;
	vertices_[5].Tex.y = 1.0f * texture_rect.d_bottom / TextureHeight;
	//vertices_[5].color=d_rgbAMaskColor;
	//return;
	
	/*
	vertices_[0].x = -0.25f;
	vertices_[0].y = 0.25;
	vertices_[0].Tex.x = 0.0;
	vertices_[0].Tex.y = 1.0f;

	vertices_[1].x = 0.25f;
	vertices_[1].y = 0.25;
	vertices_[1].Tex.x = 1.0f;
	vertices_[1].Tex.y = 1.0f ;
	vertices_[2].x = -0.25f;
	vertices_[2].y = -0.25;
	vertices_[2].Tex.x = 0.0f;
	vertices_[2].Tex.y = 0.0f;
	vertices_[3].x = 0.25f;
	vertices_[3].y = 0.25;
	vertices_[3].Tex.x = 1.0f ;
	vertices_[3].Tex.y = 1.0f;
	vertices_[4].x = 0.25f;
	vertices_[4].y = -0.25;
	vertices_[4].Tex.x = 1.0f;
	vertices_[4].Tex.y = 0.0;
	vertices_[5].x = -0.25f;
	vertices_[5].y = -0.25;
	vertices_[5].Tex.x = 0.0f;
	vertices_[5].Tex.y = 0.0f ;
	*/
	//////////////////////////////////////////////////////////////////////////
	const UINT vertexBufferSize = 6 * sizeof(VertexPos);
	winstructbuffer bf;
	RECT rc;
	destRect.toWindowRect(rc);
	char skey[100] = { 0 };
	sprintf_s(skey, "%d-%d-%d-%d", rc.left, rc.top, rc.right, rc.bottom);
	std::map<std::string, winstructbuffer>::const_iterator it = winRectStructs.find(skey);
	if (it != winRectStructs.end())
	{
		bf.buffer = it->second.buffer.Get();
		bf.bufferView = it->second.bufferView;
	}
	else
	{
		bf = createVertexbuffer();
		winRectStructs[skey] = bf;
	}
	
	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin;
	CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
	ThrowIfFailed(bf.buffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
	memcpy(pVertexDataBegin, vertices_, vertexBufferSize);
	bf.buffer->Unmap(0, nullptr);
	//2024.1.26
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mCommandList->IASetVertexBuffers(0, 1, &bf.bufferView);
	mCommandList->DrawInstanced(6, 1, 0, 0);
}
void CSoarRender12::RenderText(const RectF& destRect0, const PointF2D& PixelOffsetXY,
	const CLeeString& szText, int vertTextFmt, int horzTextFmt)
{
	//开启后，版面不太好看。。。。
	//CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	//清除模板
	//mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	mCommandList->SetGraphicsRootSignature(m_font_rootSignature.Get());
	mCommandList->SetPipelineState(m_font_pipelineState.Get());
	
	ID3D12DescriptorHeap* ppHeaps[] = { mfontCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	mCommandList->SetGraphicsRootDescriptorTable(0, mfontCbvHeap->GetGPUDescriptorHandleForHeapStart());


	ID3D12DescriptorHeap* ppHeaps1[] = { mfontColorCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(ppHeaps1), ppHeaps1);
	mCommandList->SetGraphicsRootDescriptorTable(1, mfontColorCbvHeap->GetGPUDescriptorHandleForHeapStart());
	RectF destRect = destRect0;
	RECT rc;
	RECT rc1;
	destRect.scale(1.0, 1.0);
	int w = width_ / 2;
	int h = height_ / 2;
	destRect.offsetRect(-w * 1.0, -h * 1.0);
	destRect.toWindowRect(rc);
	///
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
		if (ch.TextureID.texture == NULL)
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
		winstructbuffer bf;
		char skey[100] = { 0 };
		sprintf_s(skey, "%d-%d-%d-%d", xpos, ypos, xpos + w, ypos + h);
		std::map<std::string, winstructbuffer>::const_iterator con_it = winTextStructs.find(skey);
		if (con_it != winTextStructs.end())
		{
			bf.buffer = con_it->second.buffer.Get();
			bf.bufferView = con_it->second.bufferView;
		}
		else
		{
			bf = createVertexbuffer();
			winTextStructs[skey] = bf;
		}
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		ThrowIfFailed(bf.buffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
		memcpy(pVertexDataBegin, fontvertices, sizeof(fontvertices));
		bf.buffer->Unmap(0, nullptr);
		
		// 绘制方块:设置shader-纹理  采样器
		if (ch.TextureID.texture)
		{
			ID3D12DescriptorHeap* ppHeaps[] = { ch.TextureID.SrvHeap.Get() };
			mCommandList->SetDescriptorHeaps(1, ppHeaps);
			mCommandList->SetGraphicsRootDescriptorTable(2, ch.TextureID.SrvHeap->GetGPUDescriptorHandleForHeapStart());
		}
		
		mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mCommandList->IASetVertexBuffers(0, 1, &bf.bufferView);
		mCommandList->DrawInstanced(6, 1, 0, 0);
		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6) * scale; //(2^6 = 64)
	}
}
bool CSoarRender12::LoadContent(void)
{

	return true;
}
void CSoarRender12::UnloadContent(void)
{
	WaitForPreviousFrame();
	winRectStructs.clear();
	winTextStructs.clear();
}
bool CSoarRender12::LoadFixContent(void)
{
	if (!CreateEffect())
		return false;
	creatFence();
	CreateCommandObjects();
	CreateRtvAndDsvDescriptorHeaps();
	CreateCBVDescriptorHeaps();
	CreateDepthStencilBuffer();
	createRectVertex();
	createTexture();
	createFont();
	return true;
}
bool CSoarRender12::EnableAlphaBlend(bool bEnable) {
	if (d_bEnableAlphaBlend != bEnable) {
		d_bEnableAlphaBlend = bEnable;
		return true;
	}
	return false;
}
bool CSoarRender12::GetAlphaBlendState() {
	return d_bEnableAlphaBlend;
}
bool CSoarRender12::createD3dDevice(bool bforce)
{

	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr, // REESE
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&d3dDevice_));
	if (FAILED(hardwareResult))
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter> pWarpAdapter;
		ThrowIfFailed(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
		ThrowIfFailed(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&d3dDevice_)));

	}
	
	
	
	


	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(d3dDevice_->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels, sizeof(msQualityLevels)));
	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
	return true;
}
bool CSoarRender12::CreateCommandQueue()
{
	cmd_queue_desc = { };
	cmd_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmd_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(d3dDevice_->CreateCommandQueue(
		&cmd_queue_desc, IID_PPV_ARGS(&mCommandQueue)));
	return true;
}

void CSoarRender12::CreateCommandObjects()
{
	ThrowIfFailed(d3dDevice_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf() )));

	ThrowIfFailed(d3dDevice_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT, mDirectCmdListAlloc.Get(),
		nullptr,
		IID_PPV_ARGS(mCommandList.GetAddressOf() ) ) );
	
}
void CSoarRender12::CreateSwapChain()
{
	if (!d_isCreatedMode)
		return;
	// Release the previous swapchain we will be recreating.
	mSwapChain.Reset();

	swap_chain_desc.BufferDesc.Width = width_;
	swap_chain_desc.BufferDesc.Height = height_;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.Format = mBackBufferFormat;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SampleDesc.Count =  m4xMssaaState ? 4 : 1; //注意 这个要和SwapEffect 一致
	swap_chain_desc.SampleDesc.Quality = m4xMssaaState ? (m4xMsaaQuality - 1) : 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = FrameCount;
	swap_chain_desc.OutputWindow = hwnd_;
	swap_chain_desc.Windowed = true;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	ComPtr<IDXGISwapChain> swapChain;
	ThrowIfFailed(mdxgiFactory->CreateSwapChain(
		mCommandQueue.Get(),
		&swap_chain_desc,
		&swapChain));
	/*ThrowIfFailed(mdxgiFactory->CreateSwapChainForHwnd(
		mCommandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
		hwnd_,
		(const DXGI_SWAP_CHAIN_DESC1*) & swap_chain_desc,
		nullptr,
		nullptr,
		&swapChain
	));*/
	ThrowIfFailed(swapChain.As(&mSwapChain));
	m_frameIndex = mSwapChain->GetCurrentBackBufferIndex();
}
void CSoarRender12::CreateRtvAndDsvDescriptorHeaps()
{
	// Create descriptor heaps.
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = FrameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(d3dDevice_->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));
	mRtvDescriptorSize = d3dDevice_->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	

	// Create frame resources
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	// Create a RTV for each frame.
	for (UINT n = 0; n < FrameCount; n++)
	{
		ThrowIfFailed(mSwapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
		d3dDevice_->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, mRtvDescriptorSize);
	}

	
	
}
void CSoarRender12::CreateCBVDescriptorHeaps()
{
	// Describe and create a constant buffer view (CBV) descriptor heap.
			// Flags indicate that this descriptor heap can be bound to the pipeline 
			// and that descriptors contained in it can be referenced by a root table.
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	ThrowIfFailed(d3dDevice_->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));
	mCbvUavDescriptorSize = d3dDevice_->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Create the constant buffer.
	{
		const UINT constantBufferSize = CalcConstBufferByteSize(sizeof(MVPConstantBuffer));    // CB size is required to be 256-byte aligned.

		ThrowIfFailed(d3dDevice_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_constantBuffer)));

		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBufferSize;
		d3dDevice_->CreateConstantBufferView(&cbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());

		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
		memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
	}
}
bool CSoarRender12::CreateDepthStencilBuffer()
{
	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer.Reset();
	}
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(d3dDevice_->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
	mDsvHeap->SetName(L"Depth/Stencil Resource Heap");

	mDsvDescriptorsize = d3dDevice_->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = width_;
	depthStencilDesc.Height = height_;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
	// the depth buffer.  Therefore, because we need to create two views to the same resource:
	//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
	// we need to create the depth buffer resource with a typeless format.  
	depthStencilDesc.Format = mDepthStencilFormat;

	depthStencilDesc.SampleDesc.Count =	m4xMssaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMssaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	ThrowIfFailed(
		d3dDevice_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),//默认堆，模板必须在默认堆
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,//资源初始状态
			&optClear,
			IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf()))
	);
	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	d3dDevice_->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	//资源状态转深度缓冲区
	//mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
	//	D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	return true;
}
bool CSoarRender12::createRectVertex(void)
{
	//这个是不需要创建描述堆的。。。
	return true;
}
bool CSoarRender12::CreateEffect()
{
	// Create an empty root signature.
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

		// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (FAILED(d3dDevice_->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}
		CD3DX12_DESCRIPTOR_RANGE1 SRVranges[1];
		SRVranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		CD3DX12_DESCRIPTOR_RANGE1 CBVranges[1];
		CBVranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

		CD3DX12_ROOT_PARAMETER1 rootParameters[2];
		//rootParameters[0].InitAsConstantBufferView(0); //b0
		rootParameters[0].InitAsDescriptorTable(1, &CBVranges[0], D3D12_SHADER_VISIBILITY_VERTEX);
		rootParameters[1].InitAsDescriptorTable(1, &SRVranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;//s0
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(_countof(rootParameters), //参数个数
			rootParameters,//参数
			1, //采样器数量
			&sampler, 
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
		ThrowIfFailed(d3dDevice_->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_ui_rootSignature)));
	}
	// Create the pipeline state, which includes compiling and loading shaders.
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		
		ThrowIfFailed(D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

		if (!CreateUIPipeline(vertexShader.Get(), pixelShader.Get()))
			return false;
		if (!CreateUIblendPipeline(vertexShader.Get(), pixelShader.Get()))
			return false;
	}
	
	return true;
}
bool CSoarRender12::CreateUIPipeline(ID3DBlob* VS, ID3DBlob* PS)
{
	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }

	};
	//现在 顶点布局直接嵌入资源中了
	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_ui_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(VS);
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(PS);
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // a default depth stencil state
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	ThrowIfFailed(d3dDevice_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_ui_pipelineState)));
	return true;
}

bool CSoarRender12::CreateUIblendPipeline(ID3DBlob* VS, ID3DBlob* PS)
{
	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }

	};
	//现在 顶点布局直接嵌入资源中了
	// Describe and create the graphics pipeline state object (PSO).
	CD3DX12_BLEND_DESC blender(D3D12_DEFAULT);
	blender.RenderTarget[0].BlendEnable = true;
	blender.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blender.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blender.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blender.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blender.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blender.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blender.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_ui_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(VS);
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(PS);
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = blender;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // a default depth stencil state
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	ThrowIfFailed(d3dDevice_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_ui_blendpipelineState)));
	return true;
}
// Generate a simple black and white checkerboard texture.
std::vector<UINT8> CSoarRender12::GenerateTextureData()
{
	unsigned char* srcdata = stbi_load(d_imgFile.c_str(), &tex_width, &tex_height, &tex_nrChannels, 0);
	TextureWidth = tex_width;
	TextureHeight = tex_height;
	//TexturePixelSize = tex_nrChannels;
	const UINT rowPitch = TextureWidth * tex_nrChannels;
	const UINT textureSize = rowPitch * TextureHeight;
	
		const UINT rowPitchRGBA = TextureWidth * TexturePixelSize;
		const UINT cellPitchRGBA = rowPitchRGBA >> 3;        // The width of a cell in the checkboard texture.
		const UINT cellHeightRGBA = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
		const UINT textureSizeRGBA = rowPitchRGBA * TextureHeight;

		std::vector<UINT8> dataRGBA(textureSizeRGBA);
		UINT8* pData = &dataRGBA[0];
		if (TexturePixelSize == tex_nrChannels)
		{
			memcpy(pData, srcdata, textureSize);
			return dataRGBA;
		}
		int j = 0;
		for (UINT n = 0; n < textureSize; n += tex_nrChannels)
		{
			//memcpy(pData+j, srcdata+n, tex_nrChannels);
			pData[j] = srcdata[n];        // R
			pData[j + 1] = srcdata[n+1];    // G
			pData[j + 2] = srcdata[n+2];    // B
			pData[j + 3] = 0xff;    // A
			j += TexturePixelSize;
		}
	return dataRGBA;
}
void CSoarRender12::creatFence()
{
	// Create synchronization objects.
	ThrowIfFailed(d3dDevice_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	mCurrentFence = 1;
}
bool CSoarRender12::createTexture(void)
{
	
	if (m_texture)
	{
		if (m_fenceEvent)
		{
			WaitForPreviousFrame();
			clearScene();
		}
		m_texture.Reset();
		mSrvHeap.Reset();
	}
	// Describe and create a shader resource view (SRV) heap for the texture.
	{
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(d3dDevice_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap)));
	}
	/*
	* 注意：ComPtr 是 CPU 对象，但此资源需要保留在范围内，直到引用它的命令列表在 GPU 上完成执行。我们将在此方法结束时刷新 GPU，以确保资源不会过早销毁
	*/
	ComPtr<ID3D12Resource> textureUploadHeap;

	// Create the texture.
	{
		std::vector<UINT8> texture = GenerateTextureData();
		// Describe and create a Texture2D.
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.Width = TextureWidth;
		textureDesc.Height = TextureHeight;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		ThrowIfFailed(d3dDevice_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),//默认堆 只有gpu可以访问
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_texture)));

		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);

		// Create the GPU upload buffer.
		ThrowIfFailed(d3dDevice_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //上传堆：经cpu上传到gpu
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&textureUploadHeap)));

		// Copy data to the intermediate upload heap and then schedule a copy 
		// from the upload heap to the Texture2D.
		

		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = &texture[0];
		textureData.RowPitch = TextureWidth * TexturePixelSize;
		textureData.SlicePitch = textureData.RowPitch * TextureHeight;

		UpdateSubresources(mCommandList.Get(), m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
		//资源状态从只写(渲染目标状态)转只读(着色器资源状态)
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		// Describe and create a SRV for the texture.
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping =D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		d3dDevice_->CreateShaderResourceView(m_texture.Get(), &srvDesc, mSrvHeap->GetCPUDescriptorHandleForHeapStart());
		
	}
	//首先要将命令列表置于关闭状态。这是因为在第一次引用命令列表时，我们要对它进行重置，
	//而在调用重置方法之前又需要先将其关闭
	 // Close the command list and execute it to begin the initial GPU setup.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* ppCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	
	// Wait for the command list to execute; we are reusing the same command 
	   // list in our main loop but for now, we just want to wait for setup to 
	   // complete before continuing.
	WaitForPreviousFrame();
	return true;
}
bool CSoarRender12::createFont(void)
{
	if (FT_Init_FreeType(&ft))
		return false;
	if (FT_New_Face(ft, "msyh.ttc", 0, &theface))
		return false;
	//FT_Select_Charmap(theface, FT_ENCODING_UNICODE);
	FT_Set_Pixel_Sizes(theface, 0, 12);
	vertices_font = new VertexFont[6];
	CreateFontCBVDescriptorHeaps();
	return CreateFontEffect();
}
bool CSoarRender12::CreateFontEffect()
{
	
	HRESULT hr = S_OK;
	// Create an empty root signature.
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

		// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (FAILED(d3dDevice_->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}
		//
		CD3DX12_DESCRIPTOR_RANGE1 SRVranges[1];
		SRVranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		CD3DX12_DESCRIPTOR_RANGE1 CBVranges[1];
		CBVranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		CD3DX12_DESCRIPTOR_RANGE1 CBVranges1[1];
		CBVranges1[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		//根参数描述
		CD3DX12_ROOT_PARAMETER1 rootParameters[3];
		
		rootParameters[0].InitAsDescriptorTable(1, &CBVranges[0], D3D12_SHADER_VISIBILITY_VERTEX);
		rootParameters[1].InitAsDescriptorTable(1, &CBVranges1[0], D3D12_SHADER_VISIBILITY_PIXEL);
		//rootParameters[1].InitAsConstantBufferView(1);
		rootParameters[2].InitAsDescriptorTable(1, &SRVranges[0], D3D12_SHADER_VISIBILITY_PIXEL);

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;//s0
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(_countof(rootParameters), //参数个数
			rootParameters,//参数
			1, //采样器数量
			&sampler,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
		ThrowIfFailed(d3dDevice_->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_font_rootSignature)));
	}
	// Compile the pixel shader
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		ThrowIfFailed(D3DCompileFromFile(L"fontshaders.hlsl", nullptr, nullptr, "VSFontMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(L"fontshaders.hlsl", nullptr, nullptr, "PSFontMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

		if (!CreateFontPipeline(vertexShader.Get(), pixelShader.Get()))
			return false;
	}
	return createFontVertex();
}
bool CSoarRender12::CreateFontPipeline(ID3DBlob* VS, ID3DBlob* PS)
{
	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }

	};
	//现在 顶点布局直接嵌入资源中了
	// Describe and create the graphics pipeline state object (PSO).
	CD3DX12_BLEND_DESC blender(D3D12_DEFAULT);
	blender.RenderTarget[0].BlendEnable = true;
	blender.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blender.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blender.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blender.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blender.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blender.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blender.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_font_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(VS);
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(PS);
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = blender;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // a default depth stencil state
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	ThrowIfFailed(d3dDevice_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_font_pipelineState)));
	return true;
}
bool CSoarRender12::createFontVertex(void)
{
	return true;
}
void CSoarRender12::CreateFontCBVDescriptorHeaps()
{
	// Describe and create a constant buffer view (CBV) descriptor heap.
			// Flags indicate that this descriptor heap can be bound to the pipeline 
			// and that descriptors contained in it can be referenced by a root table.
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	ThrowIfFailed(d3dDevice_->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mfontCbvHeap)));
	mCbvUavDescriptorSize = d3dDevice_->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Create the constant buffer.
	{
		const UINT constantBufferSize = CalcConstBufferByteSize(sizeof(MVPConstantBuffer));    // CB size is required to be 256-byte aligned.

		ThrowIfFailed(d3dDevice_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_constantFontBuffer)));

		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_constantFontBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBufferSize;
		d3dDevice_->CreateConstantBufferView(&cbvDesc, mfontCbvHeap->GetCPUDescriptorHandleForHeapStart());

		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		ThrowIfFailed(m_constantFontBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvFontDataBegin)));
		memcpy(m_pCbvFontDataBegin, &m_constantFontBufferData, sizeof(m_constantFontBufferData));
	}
	//fontcolor
	D3D12_DESCRIPTOR_HEAP_DESC fontcolorHeapDesc = {};
	fontcolorHeapDesc.NumDescriptors = 1;
	fontcolorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	fontcolorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	ThrowIfFailed(d3dDevice_->CreateDescriptorHeap(&fontcolorHeapDesc, IID_PPV_ARGS(&mfontColorCbvHeap)));
	

	// Create the constant buffer.
	{
		const UINT constantBufferSize = CalcConstBufferByteSize(sizeof(fontConstantBuffer));    // CB size is required to be 256-byte aligned.

		ThrowIfFailed(d3dDevice_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_constantFontColorBuffer)));

		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_constantFontColorBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBufferSize;
		d3dDevice_->CreateConstantBufferView(&cbvDesc, mfontColorCbvHeap->GetCPUDescriptorHandleForHeapStart());

		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		ThrowIfFailed(m_constantFontColorBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvFontColorDataBegin)));
		memcpy(m_pCbvFontColorDataBegin, &m_constantFontColorBufferData, sizeof(m_constantFontColorBufferData));
	}
}
void CSoarRender12::WaitForPreviousFrame()
{
	// Advance the fence value to mark commands up to this fence point.
	mCurrentFence++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mCurrentFence));

	// Wait until the GPU has completed commands up to this fence point.
	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		// Fire event when GPU hits current fence.  
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFence, m_fenceEvent));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
	m_frameIndex = mSwapChain->GetCurrentBackBufferIndex();
}
void CSoarRender12::OnWindowChanged()
{
	assert(d3dDevice_);
	assert(mSwapChain);
	assert(mDirectCmdListAlloc);

	// Flush before changing any resources.
	WaitForPreviousFrame();

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	// Release the previous resources we will be recreating.
	for (int i = 0; i < FrameCount; ++i)
		m_renderTargets[i].Reset();
	

	// Resize the swap chain.
	ThrowIfFailed(mSwapChain->ResizeBuffers(
		FrameCount,
		width_, height_,
		mBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	m_frameIndex = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FrameCount; i++)
	{
		ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i])));
		d3dDevice_->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.ptr +=1*mRtvDescriptorSize;
	}

	// Create the depth/stencil buffer and view.
	CreateDepthStencilBuffer();
	// Execute the resize commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	WaitForPreviousFrame();

	// Update the viewport transform to cover the client area.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(width_);
	mScreenViewport.Height = static_cast<float>(height_);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, (long)width_, (long)height_ };
}
D3D12_CPU_DESCRIPTOR_HANDLE CSoarRender12::CurrentBackBufferView()const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		m_frameIndex,
		mRtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE CSoarRender12::DepthStencilView()const
{
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}
void CSoarRender12::setupMatrixMVP()
{
	if (IsMinimized(hwnd_))
	{
		return;
	}
	//mCameraRotationY += 0.01;
	XMMATRIX mx = XMMatrixIdentity();
	//窗口坐标系与DX坐标系 Y方向相反，中心为原点
	m_matWorld = mx;
	mx = XMMatrixRotationX(XM_PI);
	m_matWorld = XMMatrixMultiply(m_matWorld, mx);
	float x = mCameraRadius * cosf(mCameraRotationY);
	float z = mCameraRadius * sinf(mCameraRotationY);
	FXMVECTOR pos = XMVectorSet(x, mCameraHeight, z, 1.0f);
	FXMVECTOR target = XMVectorZero();
	FXMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_matView = XMMatrixLookAtLH(pos, target, up);
	m_ortho = XMMatrixOrthographicLH(width_, height_, 0.1f, 1000.f);
	//16字节对齐的
	// https://www.cnblogs.com/xrunning/p/5247409.html
	//mvp
	XMMATRIX vtemp = m_matView;
	XMMATRIX WVP0= XMMatrixMultiply(m_matWorld, vtemp);
	vtemp = m_ortho;
	XMMATRIX WVP = XMMatrixMultiply(WVP0, vtemp);
	XMStoreFloat4x4(&m_constantBufferData.WorldViewProject,
		XMMatrixTranspose(WVP));
	memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
	XMStoreFloat4x4(&m_constantFontBufferData.WorldViewProject,
		XMMatrixTranspose(WVP));
	memcpy(m_pCbvFontDataBegin, &m_constantFontBufferData, sizeof(m_constantFontBufferData));
	m_constantFontColorBufferData.maskrgb = vMaskColor;
	m_constantFontColorBufferData.colorrgb = vfontColor;
	memcpy(m_pCbvFontColorDataBegin, &m_constantFontColorBufferData, sizeof(m_constantFontColorBufferData));
}
const CSoarRender12::Character CSoarRender12::GetCharacter(wchar_t c)
{
	
	Character ch;
	//FT_Select_Charmap(theface, ft_encoding_unicode);
	//修改颜色数据读取位数，默认是4字节，这里由于字体
	//贴图是一个灰度图，只占1字节，所以这里修改
	//一下颜色数据的读取位数，防止出现段错误
	
	//从字体中加载字符c的字符信息
	FT_Load_Char(theface, c, FT_LOAD_RENDER);
	if (theface->glyph->bitmap.buffer == NULL)
	{
		ch.TextureID.texture=nullptr;
		ch.Size = XMSHORT2((SHORT)theface->glyph->bitmap.width, (SHORT)theface->glyph->bitmap.rows);
		ch.Bearing = XMFLOAT2(theface->glyph->bitmap_left, theface->glyph->bitmap_top);
		ch.Advance = theface->glyph->advance.x;
		return ch;
	}
	
	//将读取到的字符贴图数据加载到字符贴图中（从内存加载到显存）
	//这里的字符贴图数据都储存到r通道
	ch.TextureID = CreateTexture(theface->glyph);
	//将字符信息储存到我们定义好的数据结构中
	ch.Size = XMSHORT2((SHORT)theface->glyph->bitmap.width, (SHORT)theface->glyph->bitmap.rows);
	ch.Bearing = XMFLOAT2(theface->glyph->bitmap_left, theface->glyph->bitmap_top);
	ch.Advance = theface->glyph->advance.x;
	return ch;
}
std::wstring CSoarRender12::StringToWString(const std::string& gbkData)
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
CSoarRender12::CharacterTexture CSoarRender12::CreateTexture(FT_GlyphSlot& glyph)
{
	CSoarRender12::CharacterTexture chTexture;
	// Describe and create a shader resource view (SRV) heap for the texture.
	ComPtr<ID3D12DescriptorHeap> SrvHeap;
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(d3dDevice_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&SrvHeap)));
	// Note: ComPtr's are CPU objects but this resource needs to stay in scope until
	// the command list that references it has finished executing on the GPU.
	// We will flush the GPU at the end of this method to ensure the resource is not
	// prematurely destroyed.
	ComPtr<ID3D12Resource> textureUploadHeap;

	// Create the texture.
	{
		ComPtr<ID3D12Resource> tmptexture;
		// Describe and create a Texture2D.
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_A8_UNORM;
		textureDesc.Width = glyph->bitmap.width;
		textureDesc.Height = glyph->bitmap.rows;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		ThrowIfFailed(d3dDevice_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&tmptexture)));

		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(tmptexture.Get(), 0, 1);

		// Create the GPU upload buffer.
		ThrowIfFailed(d3dDevice_->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&textureUploadHeap)));

		// Copy data to the intermediate upload heap and then schedule a copy 
		// from the upload heap to the Texture2D.
		std::vector<UINT8> texture = GenerateTextureData();

		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = glyph->bitmap.buffer;
		textureData.RowPitch = glyph->bitmap.pitch;
		textureData.SlicePitch = textureData.RowPitch * glyph->bitmap.rows;

		UpdateSubresources(mCommandList.Get(), tmptexture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(tmptexture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		// Describe and create a SRV for the texture.
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		d3dDevice_->CreateShaderResourceView(tmptexture.Get(), &srvDesc, SrvHeap->GetCPUDescriptorHandleForHeapStart());
		
		chTexture.texture = tmptexture.Get();
		chTexture.UploadHeap = textureUploadHeap.Get();
		chTexture.SrvHeap = SrvHeap.Get();
	}
	return chTexture;
}
UINT CSoarRender12::CalcConstBufferByteSize(UINT byteSize)
{
	/*
	* assume 300 
	* （300+255） &~255
	* 555 & ~255
	* 0x022B & ~0x00ff
	* 0x022B & 0xff00
	* 0x0200
	* 512
	*/
	return (byteSize + 255) & ~255;
}
CSoarRender12::winstructbuffer CSoarRender12::createVertexbuffer(void)
{
	winstructbuffer buffer;
	const UINT vertexBufferSize = 6 * sizeof(VertexPos);
	// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
	ThrowIfFailed(d3dDevice_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer.buffer)));

	// Initialize the vertex buffer view.
	D3D12_VERTEX_BUFFER_VIEW bv;
	bv.BufferLocation = buffer.buffer->GetGPUVirtualAddress();
	bv.StrideInBytes = sizeof(VertexPos);
	bv.SizeInBytes = vertexBufferSize;
	buffer.bufferView = bv;
	
	return buffer;
}