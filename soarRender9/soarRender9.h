/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    DirectX 11 Base Class - Used as base class for all DirectX 11 demos in this book.
*/


#ifndef _Soar_Render_H_
#define _Soar_Render_H_

#include<d3d9.h>
#include<d3dx9.h>
#include<DxErr.h>
#include "../SoarHeader/ILeeTexture.h"
#include "../SoarHeader/IRawDataContainer.h"
#include "../SoarHeader/RectF.h"
#include "../SoarHeader/ILeeRender.h"
#include <xnamath.h>
#include <gdiplus.h>
#include <d3dx9core.h>

class CSoarRender9:
	public ILeeRender
{
    public:
		struct VertexPos
		{
			FLOAT x, y, z, rhw; // The transformed position for the vertex.
			DWORD color;        // The vertex color.
			FLOAT tu, tv;   // The texture coordinates
		};
        CSoarRender9();
        virtual ~CSoarRender9();

        bool Initialize( HWND hwnd );
		bool InitializeEx(LPVOID d3dDevice, LPVOID DXGISwapChain);
        void Shutdown( void );
		void Release(void);

        virtual bool LoadContent( void );
        virtual void UnloadContent( void );
        virtual bool LoadFixContent( void );
	virtual LPCTSTR   getIdentityString(void) const;
	 virtual bool beginPatch(void);
	 virtual bool endPatch(void);
	 virtual bool clearScene(void);
	 virtual void beginScene(void);
	virtual void endScene(void);
	 virtual void Present(void);
	 virtual bool DisplayReset(void);
	 //
	 virtual void Render(const RectF & destRect,const RectF&texture_rect,
		 ILeeTexture* texture,const PointF2D& PixelOffsetXY/*,IRenderCache*chache*/);
	 virtual void RenderText(const RectF & destRect,const PointF2D& PixelOffsetXY,
		 const CLeeString& szText,int vertTextFmt,int horzTextFmt);
	 virtual void updateparams(void) ;
	 virtual bool SetTexture(CLeeString & textures, DWORD dwMaskARGB, DWORD dwFontARGB, bool usTransparent) ;
	 //
	  virtual bool EnableAlphaBlend(bool bEnableAlphaBlend);
	  virtual bool GetAlphaBlendState();
private:
	void OnWindowChanged(void);
	bool createRectVertex(void);
	bool createFont(void);
	bool createTexture(void);
	bool createD3dDevice(bool bforce);
    protected:
        HWND hwnd_;
        LPDIRECT3DDEVICE9 d3dDevice_;
		//窗口参数
		D3DPRESENT_PARAMETERS d3dpp_;
		D3DXFONT_DESC d3dFontparam_;
		CLeeString d_Idstring;
		//顶点
		IDirect3DVertexBuffer9* vertexBuffer_;

		//texture
		LPDIRECT3DTEXTURE9 colorMap_;
		D3DXIMAGE_INFO     textureinfo;
		//顶点集
		VertexPos * vertices_;
		unsigned int width_;
		unsigned int height_;
		//////////////////////////////////////////////////////////////////////////
		LPD3DXFONT d3d9Font_;
		//////////////////////////////////////////////////////////////////////////
		CLeeString d_imgFile;
		D3DXCOLOR d_rgbAColor ;//透明色
		D3DXCOLOR d_rgbAFontColor;
		float rateX;
		float rateY;
		bool d_bEnableAlphaBlend;//开启混合
private:
	bool d_isCreatedMode;
	D3DXMATRIX  m_matWorld;
	D3DXMATRIX	m_matView;
	D3DXMATRIX  m_matProject;
	D3DXMATRIX   m_ortho;//正交投影
};

#endif