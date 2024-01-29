#pragma once
//#include <windows.h>
#include "dxgi1_4.h"
#include<D3D12.h>
#include<wrl.h> //smart pointer
#include<xnamath.h>
#include<DxErr.h>
#include "../SoarHeader/ILeeTexture.h"
#include "../SoarHeader/IRawDataContainer.h"
#include "../SoarHeader/RectF.h"
#include "../SoarHeader/ILeeRender.h"
#include <vector>
#include <map>
#include"ft2build.h"
#include FT_FREETYPE_H


using Microsoft::WRL::ComPtr;
#ifndef DXFAILED_FUNC__H
#define DXFAILED_FUNC__H

inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
	HRESULT Error() const { return m_hr; }
private:
	const HRESULT m_hr;
};


inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw HrException(hr);
	}
}
#endif // !DXFAILED_FUNC__H
class CSoarRender12 : public ILeeRender
{
	/*D3D12 ����˳��
	* �� D3D12CreateDevice �� �� �� �� ID3Dp12Device �� �� ʵ �� ��
�� �� һ �� ID3D12Fence �� �� , �� �� ѯ �� �� �� �� �� С ��
�� �� �� �� �� �� �� 4X MSAA �� �� �� �� �� ֧ �� �� �� ��
�� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
�� �� �� �� �� �� �� �� ��
�� �� Ӧ �� �� �� �� �� �� �� �� �� �� ��
�� �� �� ̨ �� �� �� �� �� С , �� Ϊ �� �� �� �� Ⱦ Ŀ �� �� ͼ ��
�� �� �� �� / ģ �� �� �� �� �� �� ֮ �� �� �� �� �� / ģ �� �� ͼ ��
�� �� �� �� ( viewport ) �� �� �� �� �� ( scissor rectangle )��
	*/
public:
	struct CharacterTexture
	{
		// Unique material name for lookup.
		std::string Name;
		std::wstring Filename;
		Microsoft::WRL::ComPtr<ID3D12Resource> texture = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;
		ComPtr<ID3D12DescriptorHeap> SrvHeap; //shaderresourceview -texture
	};
	struct Character {
		CharacterTexture TextureID;  // ��������ID
		XMSHORT2 Size;       // ���δ��С
		XMFLOAT2 Bearing;    // ���λ��ڻ��ߺ�����λ��
		INT     Advance;    // ��㵽��һ���������ľ���
	};
	struct VertexPos
	{
		FLOAT x, y, z;//, rhw; // The transformed position for the vertex.
		XMFLOAT2 Tex;
	};
	struct VertexFont
	{
		FLOAT x, y, z;
		XMFLOAT2 Tex;
	};
	struct MVPConstantBuffer
	{
		XMFLOAT4X4 WorldViewProject;
	};
	struct fontConstantBuffer
	{
		XMFLOAT3 colorrgb;
		XMFLOAT3 maskrgb;
	};
	struct winstructbuffer
	{
		ComPtr<ID3D12Resource> buffer;
		D3D12_VERTEX_BUFFER_VIEW bufferView;
	};
	CSoarRender12();
	virtual ~CSoarRender12();
	bool Initialize(HWND hwnd);
	bool InitializeEx(LPVOID d3dDevice, LPVOID DXGISwapChain);
	void Shutdown(void);
	void Release(void);

	virtual bool LoadContent(void);
	virtual void UnloadContent(void);
	virtual bool LoadFixContent(void);
	virtual LPCTSTR getIdentityString(void) const;
	virtual bool beginPatch(void);
	virtual bool endPatch(void);
	virtual bool clearScene(void);
	virtual void beginScene(void);
	virtual void endScene(void);
	virtual void Present(void);
	virtual bool DisplayReset(void);
	//
	virtual void Render(const RectF& destRect, const RectF& texture_rect,
		ILeeTexture* texture, const PointF2D& PixelOffsetXY/*,IRenderCache*chache*/);
	virtual void RenderText(const RectF& destRect, const PointF2D& PixelOffsetXY,
		const CLeeString& szText, int vertTextFmt, int horzTextFmt);
	virtual void updateparams(void);
	virtual bool SetTexture(CLeeString& textures, DWORD dwMaskARGB, DWORD dwFontARGB, bool usTransparent);
	virtual bool EnableAlphaBlend(bool bEnableAlphaBlend);
	virtual bool GetAlphaBlendState();
private:
	void OnWindowChanged(void);
	bool createRectVertex(void);
	void creatFence();
	bool createTexture(void);
	bool createD3dDevice(bool bforce);
	bool CreateCommandQueue();
	void CreateCommandObjects();
	void CreateSwapChain();
	bool CreateDepthStencilBuffer();
	void WaitForPreviousFrame();
	void CreateRtvAndDsvDescriptorHeaps(); //��������view
	void CreateCBVDescriptorHeaps(); //��������view
	void CreateFontCBVDescriptorHeaps(); //��������view
	bool CreateEffect();
	void setupMatrixMVP();
	bool createFont(void);
	bool CreateFontEffect();
	bool createFontVertex(void);
	const Character GetCharacter(wchar_t c);
	std::wstring StringToWString(const std::string& gbkData);
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
	std::vector<UINT8> GenerateTextureData();
	CharacterTexture CreateTexture(FT_GlyphSlot& glyph);
	UINT CalcConstBufferByteSize(UINT byteSize);
	winstructbuffer createVertexbuffer(void);
private:
	HWND hwnd_;
	//
	CLeeString d_Idstring;
	//blend
	bool d_bEnableAlphaBlend;//�������
	bool d_isCreatedMode;
	unsigned int width_;
	unsigned int height_;
	//���㼯
	VertexPos* vertices_;
	CLeeString d_imgFile;
	XMCOLOR d_rgbAMaskColor;//texture͸��ɫ
	XMFLOAT3   vMaskColor;

	DXGI_SWAP_CHAIN_DESC	swap_chain_desc;
	D3D12_COMMAND_QUEUE_DESC cmd_queue_desc;
	ComPtr < ID3D12Device> d3dDevice_;
	ComPtr < IDXGIFactory4> mdxgiFactory;
	ComPtr < IDXGISwapChain3> mSwapChain;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;

	//https://zhuanlan.zhihu.com/p/619906979  fenceͬ������
	ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;
	UINT64 mRtvDescriptorSize, 
		mDsvDescriptorsize, 
		mCbvUavDescriptorSize, 
		m4xMsaaQuality;
	bool m4xMssaaState = false;
	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	//ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	
	//https://zhuanlan.zhihu.com/p/149721407  ������Դ������֮��Ĺ�ϵ��
	//https://www.braynzarsoft.net/viewtutorial/q16390-directx-12-depth-testing ��ȼ��
	/*
	*	Constant Buffer/Shader Resource/Unordered Access view ( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		Sampler ( D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
		Render Target View ( D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		Depth Stencil View ( D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
	*/

	ComPtr<ID3D12DescriptorHeap> mRtvHeap; //Rendertargetview
	ComPtr<ID3D12DescriptorHeap> mDsvHeap;//Depth Stencil View
	ComPtr<ID3D12DescriptorHeap> mSrvHeap; //shaderresourceview -texture
	ComPtr<ID3D12DescriptorHeap> mCbvHeap;//--worldviewproject
	ComPtr<ID3D12DescriptorHeap> mfontCbvHeap;//--font_worldviewproject
	ComPtr<ID3D12DescriptorHeap> mfontColorCbvHeap;//font-mask-color

	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	D3D12_VIEWPORT mScreenViewport;//�ӿ�
	D3D12_RECT mScissorRect;//�ü�����
	static const UINT FrameCount = 2;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12Resource> mDepthStencilBuffer;
	ComPtr<ID3D12RootSignature> m_ui_rootSignature;
	ComPtr<ID3D12PipelineState> m_ui_pipelineState;//pso
	ComPtr<ID3D12RootSignature> m_font_rootSignature;
	// App resources.
	UINT8* pVertexDataBegin;
	ComPtr<ID3D12Resource> m_texture;

	UINT TextureWidth = 256;
	UINT TextureHeight = 256;
	UINT TexturePixelSize = 4;    // The number of bytes used to represent a pixel in the texture.
	int tex_width, tex_height, tex_nrChannels;
	//
	XMMATRIX  m_matWorld;
	XMMATRIX	m_matView;
	XMMATRIX   m_ortho;

	float mCameraRotationY;
	float mCameraRadius;
	float mCameraHeight;
	//Font
	XMCOLOR d_rgbAFontColor;//����ɫ
	XMFLOAT3 vfontColor;
	FT_Library ft;
	FT_Face theface;
	std::map<int, Character> Characters;
	ComPtr<ID3D12PipelineState> m_font_pipelineState;//pso
	VertexFont* vertices_font;
	//hlsl
	ComPtr<ID3D12Resource> m_constantBuffer;
	MVPConstantBuffer m_constantBufferData;
	UINT8* m_pCbvDataBegin;
	
	ComPtr<ID3D12Resource> m_constantFontBuffer;
	MVPConstantBuffer m_constantFontBufferData;
	UINT8* m_pCbvFontDataBegin;
	//fontcolor
	ComPtr<ID3D12Resource> m_constantFontColorBuffer;
	fontConstantBuffer m_constantFontColorBufferData;
	UINT8* m_pCbvFontColorDataBegin;
	//
	std::map<std::string, winstructbuffer> winRectStructs;
	std::map<std::string, winstructbuffer> winTextStructs;
};

