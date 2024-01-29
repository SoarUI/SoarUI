#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include<DxErr.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>
//#include <DirectXMath.h>
#include <xnamath.h>
#include "../SoarHeader/ILeeTexture.h"
#include "../SoarHeader/IRawDataContainer.h"
#include "../SoarHeader/RectF.h"
#include "../SoarHeader/ILeeRender.h"
#include <D3DX10core.h>
#include <map>
#include"ft2build.h"
#include FT_FREETYPE_H
class CSoarRender11: public ILeeRender
{
public:
	struct VertexPos
	{
		FLOAT x, y, z;//, rhw; // The transformed position for the vertex.
		//DWORD color;        // The vertex color.
		//FLOAT tu, tv;   // The texture coordinates
		XMFLOAT2 Tex;
	};
	struct VertexFont
	{
		FLOAT x, y,z;
		XMFLOAT2 Tex;
	};
	struct Character {
		ID3D11ShaderResourceView* TextureID;  // 字形纹理ID
		XMSHORT2 Size;       // 字形大大小
		XMFLOAT2 Bearing;    // 字形基于基线和起点的位置
		INT     Advance;    // 起点到下一个字形起点的距离
	};
	struct CBNeverChanges
	{
		XMMATRIX mView;
		
	};
	struct CBfontNeverChanges
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
		XMFLOAT4 vfontColor;
	};
	struct CBChangeOnResize
	{
		XMMATRIX mProjection;
	};

	struct CBChangesEveryFrame
	{
		XMMATRIX mWorld;
		XMFLOAT3 maskrgb;
		XMFLOAT4 vMeshColor;
	};
	CSoarRender11();
	virtual ~CSoarRender11();
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
	
	bool createTexture(void);
	bool createD3dDevice(bool bforce);
	double angle_to_radian(double degree, double min, double second);
	void radian_to_angle(double rad, double ang[]);
	bool CreateEffect();
	void setupMatrixMVP();
	bool createFont(void);
	bool CreateFontEffect();
	bool createFontVertex(void);
	const Character GetCharacter(wchar_t c);
	std::wstring StringToWString(const std::string& gbkData);
protected:
	HWND hwnd_;
	ID3D11Device*			d3dDevice_;
	IDXGISwapChain*			pSwapChain = NULL;
	DXGI_SWAP_CHAIN_DESC	swap_chain_desc;
	ID3D11DeviceContext*	pImmediateContext;	//设备上下文
	ID3D11RenderTargetView* pRenderTargetView = NULL;
	ID3D11Texture2D*		pDepthStencilBuffer;	//深度模板缓冲区
	ID3D11DepthStencilView* pDepthStencilView;	//深度模板视图
	D3D_DRIVER_TYPE			driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL		featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11VertexShader*		pVertexShader = NULL;
	ID3D11PixelShader*		pPixelShader = NULL;
	ID3D11InputLayout* pVertexLayout = NULL;
	ID3D11InputLayout* pPrevVertexLayout = NULL;
	ID3D11RasterizerState* pNoCullRasterState = NULL;
	ID3D11SamplerState*		pSamplerLinear = NULL;
	
	CLeeString d_Idstring;
	//顶点ID3D10Buffer
	ID3D11Buffer* vertexBuffer_;

	//texture
	ID3D11Texture2D* colorMap_;
	ID3D11ShaderResourceView* colorMapRV;
	D3D11_TEXTURE2D_DESC      textureinfo;
	XMFLOAT3               vMaskColor;
	//顶点集
	VertexPos* vertices_; 
	unsigned int width_;
	unsigned int height_;
	double PI = 3.1415926;
	//////////////////////////////////////////////////////////////////////////
	
	
	//////////////////////////////////////////////////////////////////////////
	CLeeString d_imgFile;
	D3DXCOLOR d_rgbAColor;//texture透明色
	
	float rateX;
	float rateY;
	//blend
	bool d_bEnableAlphaBlend;//开启混合
	D3D11_BLEND_DESC blendDesc;
	ID3D11BlendState* mTransparentBS;
	
	//Font freetype
	D3DXCOLOR d_rgbAFontColor;//文字色
	FT_Library ft;
	FT_Face theface;
	ID3D11Buffer* fontvertexBuffer;
	ID3D11VertexShader* pfontVertexShader = NULL;
	ID3D11PixelShader* pfontPixelShader = NULL;
	ID3D11InputLayout* pfontVertexLayout = NULL;
	std::map<int, Character> Characters;
	VertexFont* vertices_font;
	ID3D11SamplerState* pFontSamplerLinear = NULL;
	ID3D11BlendState* mfontBlend;
private:
	bool d_isCreatedMode;
	XMMATRIX  m_matWorld;
	XMMATRIX	m_matView;
	XMMATRIX  m_matProject;
	XMMATRIX   m_ortho;//正交投影
	ID3D11Buffer* pCBNeverChanges = NULL;
	ID3D11Buffer* pCBChangeOnResize = NULL;
	ID3D11Buffer* pCBChangesEveryFrame = NULL;
	ID3D11Buffer* pfontCBNeverChanges = NULL;
	float mCameraRotationY;
	float mCameraRadius;
	float mCameraHeight;
};

