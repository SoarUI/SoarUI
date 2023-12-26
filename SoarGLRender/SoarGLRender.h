#pragma once
#include "glad.h"
#include <windows.h>
#include "../SoarHeader/ILeeTexture.h"
#include "../SoarHeader/IRawDataContainer.h"
#include "../SoarHeader/RectF.h"
#include "../SoarHeader/ILeeRender.h"


// Include GLM
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include"ft2build.h"
#include FT_FREETYPE_H
class CSoarGLRender : public ILeeRender
{
public:
	struct VertexPos
	{
		FLOAT x, y, z;//, rhw; // The transformed position for the vertex.
		//DWORD color;        // The vertex color.
		//FLOAT tu, tv;   // The texture coordinates
		glm::vec2 Tex;
	};
	struct Character {
		GLuint     TextureID;  // ��������ID
		glm::ivec2 Size;       // ���δ��С
		glm::ivec2 Bearing;    // ���λ��ڻ��ߺ�����λ��
		GLuint     Advance;    // ��㵽��һ���������ľ���
	};

	CSoarGLRender();
	virtual ~CSoarGLRender();
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
	bool createFont(void);
	bool createTexture(void);
	bool createD3dDevice(bool bforce);
	double angle_to_radian(double degree, double min, double second);
	void radian_to_angle(double rad, double ang[]);
	bool CreateEffect();
	bool CreateFontEffect();
	bool createFontVertex(void);
	void setupMatrixMVP();
	void ScreenToViewPort(float x, float y, VertexPos& v);
	const Character GetCharacter(wchar_t c);
	std::string StringToUTF8(const std::string& gbkData);
	std::wstring StringToWString(const std::string& gbkData);
	bool IsTextUTF8(const std::string& str);
protected:
	HWND hwnd_;
	HGLRC    m_hRC;    //OpenGL��Ⱦ�����Ļ������
	HDC      m_hDC;    //Windows�豸�������
	CLeeString d_Idstring;
	unsigned int shaderProgram;
	//����ID3D10Buffer
	unsigned int VBO;
	//layout just ad directx inputlayout
	unsigned int VAO;
	//texture
	unsigned int texture;
	int tex_width, tex_height, tex_nrChannels;

	//texture
	glm::vec4              vMaskColor;
	//���㼯
	VertexPos* vertices_;
	unsigned int width_;
	unsigned int height_;
	double PI = 3.1415926;
	//////////////////////////////////////////////////////////////////////////
	//������������
	//D3DX10_FONT_DESC d3dFontparam_;
	//ID3DX10Font* d3d10Font_;
	DWORD d_rgbAFontColor;//����ɫ
	//////////////////////////////////////////////////////////////////////////
	CLeeString d_imgFile;
	DWORD d_rgbAColor;//texture͸��ɫ
	
	float rateX;
	float rateY;
	//blend
	bool d_bEnableAlphaBlend;//�������
	//Font freetype
	FT_Library ft;
	FT_Face theface;
	unsigned int fontshaderProgram;
	GLuint fontVAO, fontVBO;
	glm::vec4 rgbafontcolor;
	glm::vec4 rgbaMaskcolor;
	std::map<GLint, Character> Characters;
private:
	bool d_isCreatedMode;
	glm::mat4  m_matWorld;
	glm::mat4	m_matView;
	glm::mat4  m_matProject;
	glm::mat4   m_ortho;//����ͶӰ
	float mCameraRotationY;
	float mCameraRadius;
	float mCameraHeight;
};

