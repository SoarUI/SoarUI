#pragma once
#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include<DxErr.h>
#include "../SoarHeader/ILeeTexture.h"
#include "../SoarHeader/IRawDataContainer.h"
#include "../SoarHeader/RectF.h"
#include "../SoarHeader/ILeeRender.h"
class CSoarRender10: public ILeeRender
{
public:
	struct VertexPos
	{
		FLOAT x, y, z;//, rhw; // The transformed position for the vertex.
		//DWORD color;        // The vertex color.
		//FLOAT tu, tv;   // The texture coordinates
		D3DXVECTOR2 Tex;
	};
	CSoarRender10();
	virtual ~CSoarRender10();
	bool Initialize(HWND hwnd);
	bool InitializeEx(LPVOID d3dDevice, LPVOID DXGISwapChain);
	void Shutdown(void);
	void Release(void);

	virtual bool LoadContent(void);
	virtual void UnloadContent(void);
	virtual bool LoadFixContent(void);
	const virtual CLeeString& getIdentityString(void) const;
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
	virtual bool SetTexture(CLeeString& textures, DWORD dwRGBA, bool usTransparent);
	virtual bool EnableAlphaBlend(bool bEnableAlphaBlend);
	virtual bool GetAlphaBlendState();

private:
	void OnWindowChanged(void);
	bool createRectVertex(void);
	bool createFont(void);
	bool createTexture(void);
	bool createD3dDevice(bool bforce);
	double angle_to_radian(double degree, double min, double second);
	void radian_to_angle(double rad, double ang[]);
	bool CreateEffect();
	void setupMatrixMVP();
	void ScreenToViewPort(float x, float y, VertexPos& v);
protected:
	HWND hwnd_;
	ID3D10Device* d3dDevice_;
	IDXGISwapChain* pSwapChain = NULL;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	D3D10_DRIVER_TYPE       driverType = D3D10_DRIVER_TYPE_NULL;
	ID3D10Effect* pEffect = NULL;
	ID3D10EffectTechnique* pTechnique = NULL;
	ID3D10InputLayout* pVertexLayout = NULL;
	ID3D10InputLayout* pPrevVertexLayout = NULL;
	ID3D10RasterizerState* pNoCullRasterState = NULL;
	
	CLeeString d_Idstring;
	//顶点ID3D10Buffer
	ID3D10Buffer* vertexBuffer_;

	//texture
	ID3D10Texture2D* colorMap_;
	ID3D10ShaderResourceView* colorMapRV;
	D3D10_TEXTURE2D_DESC      textureinfo;
	ID3D10EffectShaderResourceVariable* pSkinTexShaderRSVariable = NULL;
	ID3D10EffectVectorVariable* pmaskrgbVariable = NULL;
	D3DXVECTOR3               vMaskColor;
	//顶点集
	VertexPos* vertices_;
	unsigned int width_;
	unsigned int height_;
	double PI = 3.1415926;
	//////////////////////////////////////////////////////////////////////////
	//字体描述参数
	D3DX10_FONT_DESC d3dFontparam_;
	ID3DX10Font* d3d10Font_;
	DWORD d_rgbAFontColor;//文字色
	//////////////////////////////////////////////////////////////////////////
	CLeeString d_imgFile;
	DWORD d_rgbAColor;//texture透明色
	
	float rateX;
	float rateY;
	//blend
	bool d_bEnableAlphaBlend;//开启混合
	D3D10_BLEND_DESC blendDesc;
	ID3D10BlendState* mTransparentBS;
private:
	bool d_isCreatedMode;
	D3DXMATRIX  m_matWorld;
	D3DXMATRIX	m_matView;
	D3DXMATRIX  m_matProject;
	D3DXMATRIX   m_ortho;//正交投影
	ID3D10EffectMatrixVariable* pWorldVariable = NULL;
	ID3D10EffectMatrixVariable* pViewVariable = NULL;
	ID3D10EffectMatrixVariable* pProjectionVariable = NULL;
	float mCameraRotationY;
	float mCameraRadius;
	float mCameraHeight;
};

