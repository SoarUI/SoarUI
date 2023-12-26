#include "pch.h"
#include "SoarGLRender.h"
#ifdef USEGLEW
#include"glew.h"
#endif
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include <gl/GL.h>//图形库，提供最基本的函数，函数前缀gl；
#include <gl/GLU.h>//使用库，包含多个完成诸如设置视点矩阵和投影矩阵、执行多边形网格化以及渲染曲面等任务的函数；

#include "WinGLFunction.h"
#include<locale>
#include<codecvt>
#pragma warning(disable:4996)
//#pragma comment(lib, "legacy_stdio_definitions.lib")
#if defined( DEBUG ) || defined( _DEBUG )
#pragma comment(lib, "opengl32.lib") 
#pragma comment(lib, "glu32.lib")
#pragma comment(lib,"freetype.lib")
#ifdef USEGLEW
#pragma comment(lib, "glew32.lib")
#endif
#else
#pragma comment(lib, "opengl32.lib") 
#pragma comment(lib, "glu32.lib")
#pragma comment(lib,"freetype.lib")
#ifdef USEGLEW
#pragma comment(lib, "glew32.lib")
#endif
#endif


CSoarGLRender::CSoarGLRender() :
	m_hRC(0), 
	texture(0), d_bEnableAlphaBlend(false),
	 d_isCreatedMode(true)
{
	
	//loadOpenGL();
	//initOpenGL();
	d_Idstring = "OpenGLRenderforSoar v1.0";
	d_rgbAFontColor = 0xFF00FF00;
	
}
CSoarGLRender::~CSoarGLRender()
{
	Shutdown();
}
LPCTSTR CSoarGLRender::getIdentityString(void) const
{
	return d_Idstring.c_str();
}
bool CSoarGLRender::DisplayReset(void)
{
	updateparams();
	return true;
}
void CSoarGLRender::updateparams()
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

bool CSoarGLRender::SetTexture(CLeeString& textures, DWORD dwMaskARGB, DWORD dwFontARGB, bool usTransparent)
{
	d_imgFile = textures;
	d_rgbAColor = dwMaskARGB;
	d_rgbAFontColor = dwFontARGB;
	rgbaMaskcolor = glm::vec4( ((d_rgbAColor & 0x00FF0000) >> 16) /255.0f ,
		((d_rgbAColor & 0x0000FF00) >> 8)/255.0f, 
		(d_rgbAColor & 0x000000FF) / 255.0f,
		((d_rgbAColor & 0xFF000000) >> 24) / 255.0f
	);
	rgbafontcolor = glm::vec4(((d_rgbAFontColor & 0x00FF0000) >> 16) / 255.0f,
		((d_rgbAFontColor & 0x0000FF00) >> 8) / 255.0f,
		(d_rgbAFontColor & 0x000000FF) / 255.0f,
		((d_rgbAFontColor & 0xFF000000) >> 24) / 255.0f
	);
	if (m_hRC)
	{
		if (!createTexture())
		{
			return false;
		}
	}
	
	return true;
}
bool CSoarGLRender::Initialize(HWND hwnd)
{
	hwnd_ = hwnd;
	//获取DC
	m_hDC = GetDC(hwnd);
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
	rateX = 1.0;
	rateY = 1.0;
	//////////////////////////////////////////////////////////////////////////
	//设置渲染视口
	glViewport(0, 0, width_, height_);
	// Initialize the view matrix
	mCameraRadius = 600.0f;
	mCameraRotationY = glm::radians(180.0f) ;
	mCameraHeight = 6.0f;
	setupMatrixMVP();
	return LoadFixContent();
}
bool CSoarGLRender::InitializeEx(LPVOID d3dDevice, LPVOID DXGISwapChain)
{
	if (NULL == d3dDevice||
		DXGISwapChain == NULL)
		return false;
	if (m_hRC && d_isCreatedMode)
	{
		glDeleteContext(m_hRC);
		ReleaseDC(hwnd_, m_hDC);
		m_hRC = NULL;
	}
	m_hRC = (HGLRC)d3dDevice;
	//从创建参数找那个获取窗口
	d_isCreatedMode = false;
	m_hDC = glGetCurrentDC();
	hwnd_  = WindowFromDC(m_hDC);
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
	
	d_isCreatedMode = false;
	wglMakeCurrent(m_hDC, m_hRC);
	//init glad and fetch new opengl functions!! must make wglMakeCurrent First!!
	gladLoadGL();

#ifdef USEGLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, "Couldn't initialize GLEW!", "Fatal Error", MB_ICONERROR);
		return false;
	}
#endif
	//////////////////////////////////////////////////////////////////////////
	if (!CreateEffect())
		return false;
	rateX = 1.0;
	rateY = 1.0;
	//////////////////////////////////////////////////////////////////////////
	mCameraRadius = 600.0f;
	mCameraRotationY = -glm::radians(180.0f) / 2.0f;
	mCameraHeight = 0.0f;
	setupMatrixMVP();
	return LoadFixContent();
}
bool CSoarGLRender::createD3dDevice(bool bforce)
{
	//初始化像素格式
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),    //上述格式描述符的大小
		1,                                //版本号
		PFD_DRAW_TO_WINDOW |              //格式支持窗口
		PFD_SUPPORT_OPENGL |              //格式必须支持OpenGL
		PFD_DOUBLEBUFFER,                 //必须支持双缓冲
		PFD_TYPE_RGBA,                    //申请RGBA 格式
		24,                               //选定色彩深度
		0, 0, 0, 0, 0, 0,
		0, 
		0,           //忽略RGBA
		0,                                //无累加缓存
		0, 0, 0, 0,                       //忽略聚集位
		32,                               //32位Z-缓存(深度缓存)
		0,                                //无蒙板缓存
		0,                                //无辅助缓存
		PFD_MAIN_PLANE,                   //主绘图层
		0,                                //Reserved
		0, 0, 0                           //忽略层遮罩
	};
	//选择一个最适合pfd描述的像素格式索引值
	int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	//为DC设置像素格式
	bool succ=SetPixelFormat(m_hDC, nPixelFormat, & pfd);
	if (!succ)
	{
		::MessageBoxA(NULL, "ERROR", "ERR", MB_OK);
	}
	//创建RC
	m_hRC = wglCreateContext(m_hDC);
	if (m_hRC ==NULL)
	{
		::MessageBoxA(NULL, "ERROR", "cannot create opengl context", MB_OK);
		return false;
	}
	wglMakeCurrent(m_hDC, m_hRC);
	//init glad and fetch new opengl functions!! must make wglMakeCurrent First!!
	gladLoadGL();
	
#ifdef USEGLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, "Couldn't initialize GLEW!", "Fatal Error", MB_ICONERROR);
		return false;
	}
#endif
	
	// Create the effect
	return CreateEffect();
}
bool CSoarGLRender::CreateEffect()
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexCoord;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 matworld;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"   gl_Position =projection * view * matworld * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"	TexCoord = aTexCoord;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform vec4 vMaskColor;\n"
		"uniform sampler2D texture1;\n"
		"void main()\n"
		"{\n"
		"	vec4 texColor = texture(texture1, TexCoord);\n"
		"	//if(abs(texColor.x-vMaskColor.r)<0.01 && abs(texColor.y-vMaskColor.g)<0.01 && abs(texColor.z-vMaskColor.b)<0.01)\n"
		"	//discard;\n"
		"   FragColor = texColor;\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attach the shader source code to the shader object and compile the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	//frame shader <=> pixelshader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetProgramiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	// final linked version of multiple shaders combined
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return true;
}
bool CSoarGLRender::createRectVertex(void)
{
	if (VBO!=0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	
	vertices_[0].x = 1.0f ;
	vertices_[0].y = 1.0f ;
	vertices_[0].z = 1;
	vertices_[0].Tex.x = 1.0f ;
	vertices_[0].Tex.y = 1.0f ;

	
	vertices_[1].x = 1.0f ;
	vertices_[1].y = -1.0f ;
	vertices_[1].z = 1;
	vertices_[1].Tex.x = 1.0f;
	vertices_[1].Tex.y = 0.0f;
	
	vertices_[2].x = -1.0f ;
	vertices_[2].y = -1.0f ;
	vertices_[2].z = 1;
	vertices_[2].Tex.x = 0.0;
	vertices_[2].Tex.y = 0.0;
	
	vertices_[3].x = -1.0f ;
	vertices_[3].y = 1.0f ;
	vertices_[3].z = 1;
	vertices_[3].Tex.x = 0.0f;
	vertices_[3].Tex.y = 1.0f ;
	
	vertices_[4].x = 1.0f ;
	vertices_[4].y = 1.0f ;
	vertices_[4].z = 1;
	vertices_[4].Tex.x = 1.0f ;
	vertices_[4].Tex.y = 1.0f ;
	
	vertices_[5].x = -1.0f ;
	vertices_[5].y = -1.0f ;
	vertices_[5].z = 1;
	vertices_[5].Tex.x = 0.0f ;
	vertices_[5].Tex.y = 0.0f ;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(VertexPos), vertices_, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	return true;
}
bool CSoarGLRender::createTexture(void)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	unsigned char* data = stbi_load(d_imgFile.c_str(), &tex_width, &tex_height, &tex_nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}
	stbi_image_free(data);
	return true;
}
bool CSoarGLRender::createFont(void)
{
	if (FT_Init_FreeType(&ft))
		return false;
	if (FT_New_Face(ft, "msyh.ttc", 0, &theface))
		return false;
	FT_Set_Pixel_Sizes(theface, 0, 12);
	return CreateFontEffect();
}
bool CSoarGLRender::CreateFontEffect()
{
	const char* vertexFontShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"   gl_Position =projection * vec4(vertex.xy, 0.0, 1.0);\n"
		"	TexCoords = vertex.zw;\n"
		"}\0";
	const char* fragmentFontShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoords;\n"
		"uniform sampler2D text;\n"
		"uniform vec3 textColor;\n"
		"void main()\n"
		"{\n"
		"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
		"   FragColor = vec4(textColor, 1.0) * sampled;\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attach the shader source code to the shader object and compile the shader
	glShaderSource(vertexShader, 1, &vertexFontShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	//frame shader <=> pixelshader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentFontShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetProgramiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	// final linked version of multiple shaders combined
	fontshaderProgram = glCreateProgram();
	glAttachShader(fontshaderProgram, vertexShader);
	glAttachShader(fontshaderProgram, fragmentShader);
	glLinkProgram(fontshaderProgram);
	glGetProgramiv(fontshaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(fontshaderProgram, 512, NULL, infoLog);
		//std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return createFontVertex();
}
bool CSoarGLRender::createFontVertex(void)
{
	if (fontVBO != 0)
	{
		glDeleteBuffers(1, &fontVBO);
		fontVBO = 0;
	}
	if (fontVAO != 0)
	{
		glDeleteVertexArrays(1, &fontVAO);
		fontVAO = 0;
	}
	glGenVertexArrays(1, &fontVAO);
	glGenBuffers(1, &fontVBO);
	glBindVertexArray(fontVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	return true;
}
bool CSoarGLRender::LoadContent()
{
	// Override with demo specifics, if any...
	return true;
}

bool CSoarGLRender::LoadFixContent()
{
	// Override with demo specifics, if any...
	if (!createFont())
	{
		return false;
	}
	createRectVertex();
	if (!createTexture())
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	return true;
}
void CSoarGLRender::UnloadContent()
{
	// Override with demo specifics, if any...
	FT_Done_Face(theface);
	FT_Done_FreeType(ft);
	
}
void CSoarGLRender::Shutdown()
{
	UnloadContent();
	if (m_hRC && d_isCreatedMode)
	{
		//视口
		if (m_hRC)
		{
			wglDeleteContext(m_hRC);
			m_hRC = NULL;
		}
	}
		
	m_hRC = 0;
}
void CSoarGLRender::Release(void)
{
	delete this;
}
bool CSoarGLRender::clearScene(void)
{
	if (m_hRC == 0)
		return 0;
	// Clear the backbuffer
	float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f }; // RGBA
	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return 1;
}
void CSoarGLRender::beginScene(void) {
	if (m_hRC == 0)
		return;
	//选择RC作为当前线程的RC
	if (m_hDC != wglGetCurrentDC() ||
		wglGetCurrentContext() != m_hRC)
	{
		glMakeCurrent(m_hDC, m_hRC);
	}
	
	
	//mCameraRotationY += 0.09f;
	//setupMatrixMVP();
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	glUseProgram(shaderProgram); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	// configure global opengl state
   // -----------------------------
	//glEnable(GL_DEPTH_TEST);
	// maskrgb
	unsigned int projLoc = glGetUniformLocation(shaderProgram, "vMaskColor");
	glUniform4fv(projLoc, 4,glm::value_ptr(rgbaMaskcolor));
	
}
void CSoarGLRender::endScene(void) {
	if (m_hRC == 0)
		return;
}
bool CSoarGLRender::beginPatch(void)
{
	if (m_hRC == 0)
		return 0;
	return 1;
}
bool CSoarGLRender::endPatch(void)
{
	if (m_hRC == 0)
		return 0;
	
	return 1;
}
void CSoarGLRender::Present(void)
{
	if (m_hRC == NULL ||
		m_hDC == NULL )
		return;
	//交换当前缓冲区和后台缓冲区
	SwapBuffers(m_hDC);
	//取消当前线程选中的RC
	//glMakeCurrent(NULL, NULL);
}
bool CSoarGLRender::EnableAlphaBlend(bool bEnable) {
	if (d_bEnableAlphaBlend != bEnable) {
		d_bEnableAlphaBlend = bEnable;
		return true;
	}
	return false;
}
bool CSoarGLRender::GetAlphaBlendState() {
	return d_bEnableAlphaBlend;
}
double CSoarGLRender::angle_to_radian(double degree, double min, double second)
{
	double flag = (degree < 0) ? -1.0 : 1.0; //判断正负
	if (degree < 0)
	{
		degree = degree * (-1.0);
	}
	double angle = degree + min / 60 + second / 3600;
	double result = flag * (angle * PI) / 180;
	return result;
}
//弧度转换为角度
void CSoarGLRender::radian_to_angle(double rad, double ang[])
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
void CSoarGLRender::Render(const RectF& destRect0, const RectF& texture_rect,
	ILeeTexture* targettexture, const PointF2D& PixelOffsetXY/*,IRenderCache*chache*/)
{
	if (m_hRC == 0 ||
		VBO == NULL ||
		this->texture == NULL)
		return;
	RectF destRect = destRect0;
	destRect.scale(rateX, rateY);

	//C:
	//ScreenToViewPort(destRect.d_left, destRect.d_top,vertices_[0]);
	vertices_[0].x = 1.0f * destRect.d_left;
	vertices_[0].y = 1.0f * destRect.d_top ;
	vertices_[0].z = 0;
	vertices_[0].Tex.x = 1.0f * texture_rect.d_left  / tex_width;
	vertices_[0].Tex.y = 1.0f * texture_rect.d_top / tex_height;

	//vertices_[0].color=d_rgbAColor;
	//B:
	//ScreenToViewPort(destRect.d_right, destRect.d_top, vertices_[1]);
	vertices_[1].x = 1.0f * destRect.d_right ;
	vertices_[1].y = 1.0f * destRect.d_top ;
	vertices_[1].z = 0;
	vertices_[1].Tex.x = 1.0f * texture_rect.d_right  / tex_width;
	vertices_[1].Tex.y = 1.0f * texture_rect.d_top / tex_height;
	//vertices_[1].color=d_rgbAColor;
	//D:
	//ScreenToViewPort(destRect.d_left, destRect.d_bottom, vertices_[2]);
	vertices_[2].x = 1.0f * destRect.d_left ;
	vertices_[2].y = 1.0f * destRect.d_bottom ;
	vertices_[2].z = 0;
	vertices_[2].Tex.x = 1.0f * texture_rect.d_left  / tex_width;
	vertices_[2].Tex.y = 1.0f * texture_rect.d_bottom / tex_height;
	//vertices_[2].color=d_rgbAColor;

	//D:
	//ScreenToViewPort(destRect.d_left, destRect.d_bottom, vertices_[3]);
	vertices_[3].x = 1.0f * destRect.d_left ;
	vertices_[3].y = 1.0f * destRect.d_bottom ;
	vertices_[3].z = 0;
	vertices_[3].Tex.x = 1.0f * texture_rect.d_left  / tex_width;
	vertices_[3].Tex.y = 1.0f * texture_rect.d_bottom / tex_height;
	//vertices_[3].color=d_rgbAColor;
	//B:
	//ScreenToViewPort(destRect.d_right, destRect.d_top, vertices_[4]);
	vertices_[4].x = 1.0f * destRect.d_right ;
	vertices_[4].y = 1.0f * destRect.d_top ;
	vertices_[4].z = 0;
	vertices_[4].Tex.x = 1.0f * texture_rect.d_right  / tex_width;
	vertices_[4].Tex.y = 1.0f * texture_rect.d_top / tex_height;
	//vertices_[4].color=d_rgbAColor;

	////A:
	//ScreenToViewPort(destRect.d_right, destRect.d_bottom, vertices_[5]);
	vertices_[5].x = 1.0f * destRect.d_right ;
	vertices_[5].y = 1.0f * destRect.d_bottom ;
	vertices_[5].z = 0;
	vertices_[5].Tex.x = 1.0f * texture_rect.d_right  / tex_width;
	vertices_[5].Tex.y = 1.0f * texture_rect.d_bottom / tex_height;
	//vertices_[5].color=d_rgbAColor;

	//////////////////////////////////////////////////////////////////////////
	if(d_bEnableAlphaBlend)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	// Set vertex buffer
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0,6 * sizeof(VertexPos), (const void*)vertices_);
	//activate the vertex attributes 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//解绑VAO
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//解绑EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//////////////////////////////////////////////////////////////////////////
	
	// Update Variables that never change
	
	//设置混合状态
	
	
	//////////////////////////////////////////////////////////////////////////
	
	
}
void CSoarGLRender::RenderText(const RectF& destRect0, const PointF2D& PixelOffsetXY,
	const CLeeString& szText, int vertTextFmt, int horzTextFmt)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 激活合适的渲染状态
	glUseProgram(fontshaderProgram);
	glUniform3f(glGetUniformLocation(fontshaderProgram, "textColor"), rgbafontcolor.x, rgbafontcolor.y, rgbafontcolor.z);
	glUniform1i(glGetUniformLocation(fontshaderProgram, "text"), 0);
	unsigned int projLoc = glGetUniformLocation(fontshaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_matProject));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(fontVAO);
	RectF destRect = destRect0;
	RECT rc;
	destRect.scale(rateX, rateY);
	destRect.toWindowRect(rc);
	float scale = 1.0f;
	GLfloat x = rc.left;
	GLfloat y = rc.top;
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
		y = rc.top+(rc.bottom-rc.top)/2.0f;
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
		x = rc.left+(rc.right-rc.left)/2.0f;
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
	GLfloat totalw=0.0f, totalh=0.0f;
	 for (c = utf8strings.begin(); c != utf8strings.end(); c++)
	{
		std::map<GLint, Character>::const_iterator it = Characters.find(*c);
		if (it == Characters.end())
		{
			Characters[*c] = GetCharacter(*c);
		}
		Character ch = Characters[*c];
		totalh = totalh<ch.Size.y * scale? ch.Size.y * scale:totalh;
		// 更新位置到下一个字形的原点，注意单位是1/64像素
		totalw += (ch.Advance >> 6) * scale; //(2^6 = 64)
	}
	 //caculate current line
	 GLfloat suggest_linew = totalw > rcw ? rcw : totalw;
	 GLfloat suggest_lineh = totalh > rch ? rch : totalh;
	 if (hfmt & DT_CENTER)
		 x -= suggest_linew * 0.5;
	 if (vfmt & DT_VCENTER)
		 y -= suggest_lineh * 0.5;
	 GLfloat target_line_startx = x;
	 GLfloat target_firstline_starty = y;
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
		
		std::map<GLint, Character>::const_iterator it = Characters.find(*c);
		if (it == Characters.end())
		{
			Characters[*c]=GetCharacter(*c);
		}
		Character ch = Characters[*c];
		if (*c == '\r')
		{
			continue;
		}
		if (*c == '\n')
		{
			y += suggest_lineh+2;
			x = target_line_startx;
			continue;
		}
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (ch.Size.y - ch.Bearing.y) * scale+ (suggest_lineh-h);

		
		// 当前字符的VBO
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos,     ypos,       0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 }
		};
		// 在方块上绘制字形纹理
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// 更新当前字符的VBO
		glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// 绘制方块
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6) * scale; //(2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void CSoarGLRender::OnWindowChanged(void)
{

	//////////////////////////////////////////////////////////////////////////
	//自己创建的设备，：reset或者重新创建设备
	
	//重新获取窗口大小
	RECT dimensions;
	GetClientRect(hwnd_, &dimensions);
	width_ = dimensions.right - dimensions.left;
	height_ = dimensions.bottom - dimensions.top;
	//获取表面数据重新计算比例
	rateX = 1.0 ;
	rateY = 1.0 ;
	if(d_isCreatedMode)
	glViewport(0, 0, width_, height_);
	setupMatrixMVP();
	return;
}
void CSoarGLRender::setupMatrixMVP()
{
	glm::mat4 mx = glm::mat4(1.0f);
	m_matWorld = glm::mat4(1.0f);
	float camX = sin(mCameraRotationY) * mCameraRadius;
	float camZ = cos(mCameraRotationY) * mCameraRadius;
	m_matView = glm::lookAtLH(glm::vec3(camX, mCameraHeight, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	m_ortho= glm::orthoLH(0.0f, 1.0f * width_, 1.0f * height_, 0.0f, 0.1f, 1000.0f);;
	//初始化正交矩形
	//m_matProject=glm::perspectiveLH(glm::radians(45.0f), (float)width_ / (float)height_, 0.1f, 1000.0f);
	m_matProject = glm::orthoLH(0.0f, 1.0f*width_, 1.0f*height_, 0.0f, 0.1f, 1000.0f);
	m_ortho = m_matProject * m_matView * m_matWorld;
	glUseProgram(shaderProgram);
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "matworld");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_matProject));
	// pass them to the shaders (3 different ways)
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_matWorld));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &m_matView[0][0]);
	
}
void CSoarGLRender::ScreenToViewPort(float x, float y, VertexPos& vout)
{
	/*
	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 v;
	v.x = (((2.0f * x) / width_) - 1) / m_ortho.11;
	v.y = -(((2.0f * y) / height_) - 1) / m_ortho._22;
	v.z = 1.0f;

	// Get the inverse view matrix
	const D3DXMATRIX matView = m_matView;
	const D3DXMATRIX matWorld = m_matWorld;
	D3DXMATRIX mWorldView = matWorld * matView;
	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);

	// Transform the screen space pick ray into 3D space
	vout.x = (v.x * m._11 + v.y * m._21 + v.z * m._31)* width_;
	vout.y = (v.x * m._12 + v.y * m._22 + v.z * m._32)* height_;
	vout.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	//vPickRayOrig.x = m._41;
	//vPickRayOrig.y = m._42;
	//vPickRayOrig.z = m._43;
	*/
}
const CSoarGLRender::Character CSoarGLRender::GetCharacter(wchar_t c)
{
	Character ch;
	//FT_Select_Charmap(theface, ft_encoding_unicode);
	//修改颜色数据读取位数，默认是4字节，这里由于字体
	//贴图是一个灰度图，只占1字节，所以这里修改
	//一下颜色数据的读取位数，防止出现段错误
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//从字体中加载字符c的字符信息
	FT_Load_Char(theface, c, FT_LOAD_RENDER);
	
	GLuint tex;//储存字符贴图
	glGenTextures(1, &tex);//生成字符贴图
	glBindTexture(GL_TEXTURE_2D, tex);//绑定字符贴图，以支持后续操作
	//将读取到的字符贴图数据加载到字符贴图中（从内存加载到显存）
	//这里的字符贴图数据都储存到r通道
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		theface->glyph->bitmap.width,
		theface->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		theface->glyph->bitmap.buffer
	);
	//设置环绕方式和采样方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//将字符信息储存到我们定义好的数据结构中
	ch.TextureID = tex;
	ch.Size = glm::ivec2(theface->glyph->bitmap.width, theface->glyph->bitmap.rows);
	ch.Bearing = glm::ivec2(theface->glyph->bitmap_left, theface->glyph->bitmap_top);
	ch.Advance = theface->glyph->advance.x;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	return ch;
}
std::wstring CSoarGLRender::StringToWString(const std::string& gbkData)
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
std::string CSoarGLRender::StringToUTF8(const std::string& gbkData)
{
	if (IsTextUTF8(gbkData))
		return gbkData;
	const char* GBK_LOCALE_NAME = "CHS";  //GBK在windows下的locale name(.936, CHS ), linux下的locale名可能是"zh_CN.GBK"

	std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>>
		conv(new std::codecvt<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));
	std::wstring wString = conv.from_bytes(gbkData);    // string => wstring

	std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
	std::string utf8str = convert.to_bytes(wString);     // wstring => utf-8

	return utf8str;
}
bool CSoarGLRender::IsTextUTF8(const std::string& str)
{
	char nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr;
	bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8

	for (int i = 0; i < str.length(); i++)
	{
		chr = str[i];

		// 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,
		// 但用一个字节存,最高位标记为0,o0xxxxxxx
		if ((chr & 0x80) != 0)
			bAllAscii = false;

		if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)   nBytes = 6;
				else if (chr >= 0xF8)         nBytes = 5;
				else if (chr >= 0xF0)         nBytes = 4;
				else if (chr >= 0xE0)         nBytes = 3;
				else if (chr >= 0xC0)         nBytes = 2;
				else {
					return false;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			nBytes--;
		}
	}

	if (nBytes > 0) //违返规则
		return false;

	if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
		return false;

	return true;
}