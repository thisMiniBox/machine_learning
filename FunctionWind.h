#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include<sstream>
#include<cmath>
#include"CMathVec.h"
namespace CMath {
	HBITMAP LoadImageFile(HWND hWnd);



	constexpr auto G_FUNCTION_WINDOW_CLASS_NAME = L"Function_Window";
	class FunctionWind
	{
		static UINT m_WindowCount;
		HINSTANCE m_hInstance;
		HWND m_hWnd;

		std::vector<POINT> m_Point;
		RECT m_CoordinateAxis;//定义域x小到大，y小到大
		double m_XPrecision;//刻度
		double m_YPrecision;

		HDC m_MemDC;
		HBITMAP m_MemBitmap;
		HBITMAP m_OldBitmap;

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	public:
		FunctionWind(HINSTANCE hInstance = nullptr, HWND parent = nullptr);
		~FunctionWind();
	};

#define fiveCharToInt(c1, c2, c3, c4) ((static_cast<int>(c1)) | (static_cast<int>(c2) << 8) | (static_cast<int>(c3) << 16) | (static_cast<int>(c4) << 24))
#define intToChar(Int, Where) ((((int)Int >> (8 * Where)) & 0xFF))
#define FLOAT_TO_UCHAR_ARRAY(f, uchar_array)              \
    do {                                                  \
        union {                                           \
            float f;                                      \
            unsigned char uc[sizeof(float)];               \
        } u;                                              \
        u.f = (f);                                        \
        memcpy((uchar_array), u.uc, sizeof(float));       \
    } while (0)

#define UCHAR_ARRAY_TO_FLOAT(uchar_array, f)              \
    do {                                                  \
        union {                                           \
            unsigned char uc[sizeof(float)];               \
            float f;                                      \
        } u;                                              \
        memcpy(u.uc, (uchar_array), sizeof(float));       \
        (f) = u.f;                                        \
    } while (0)
	class Image
	{

	public:
		enum class ColorChannelOrder :int
		{
			G = fiveCharToInt(0, 0, 0, 0),

			RGB = fiveCharToInt(0, 1, 2, 0),
			RBG = fiveCharToInt(0, 2, 1, 0),
			GRB = fiveCharToInt(1, 0, 2, 0),
			GBR = fiveCharToInt(1, 2, 0, 0),
			BRG = fiveCharToInt(2, 0, 1, 0),
			BGR = fiveCharToInt(2, 1, 0, 0),

			RGBA = fiveCharToInt(0, 1, 2, 3),
			RBGA = fiveCharToInt(0, 2, 1, 3),
			GRBA = fiveCharToInt(1, 0, 2, 3),
			GBRA = fiveCharToInt(1, 2, 0, 3),
			BRGA = fiveCharToInt(2, 0, 1, 3),
			BGRA = fiveCharToInt(2, 1, 0, 3),

			ARGB = fiveCharToInt(3, 0, 1, 2),
			ARBG = fiveCharToInt(3, 0, 2, 1),
			AGRB = fiveCharToInt(3, 1, 0, 2),
			AGBR = fiveCharToInt(3, 1, 2, 0),
			ABRG = fiveCharToInt(3, 2, 0, 1),
			ABGR = fiveCharToInt(3, 2, 1, 0),
		};
		enum class ColorChannelCount
		{
			G_8,//8
			G_f,//32
			G_d,//64
			RGB_24,//24
			RGB_48,//48
			RGB_float,//96
			RGB_double,//192
			RGBA_32,//32
			RGBA_64,//64
			RGBA_float,//128
			RGBA_double//256
		};
		
		Image(const std::vector<unsigned char> Data = {}, UINT ImageWidth = 0, UINT ImageHeight = 0,
			ColorChannelCount BitCount = ColorChannelCount::RGB_24,
			ColorChannelOrder cco = ColorChannelOrder::RGB,//图片的通道顺序
			bool ColorChannelContinuous = false,//图片的通道是像素还是图片
			bool picturesflipping = false,//图片是否翻转
			bool TheOrderOfImagesIsColumns = false//图片是一列一列的还是一行一行的？
		);
		~Image();

		void SetImage(const std::vector<unsigned char> Data, UINT ImageWidth, UINT ImageHeight,
			ColorChannelCount BitCount = ColorChannelCount::RGB_24,
			ColorChannelOrder cco = ColorChannelOrder::RGB,//图片的通道顺序
			bool ColorChannelContinuous = false,//图片的通道是像素还是图片
			bool picturesflipping = false,//图片是否翻转
			bool TheOrderOfImagesIsColumns = false//图片是一列一列的还是一行一行的？
		);
		UINT GetImageWidth()const;
		UINT GetImageHeight()const;
		Color GetImagePixelColor(UINT x, UINT y)const;
		std::vector<unsigned char>& GetData();
		int GetPixelSize()const;
		int GetColorChannelSize()const;
		//会修改原数据
		//可能导致数据丢失
		const std::vector<UCHAR>& ScaleImage(int w, int h);
#ifdef WIN32
		//自动释放
		HBITMAP ToBitMap(int w = -1, int h = -1, bool autoDelete = true);
		//不修改原数据
		static HBITMAP ScaleBitmap(HBITMAP hBitmap, int newWidth, int newHeight, int mode = COLORONCOLOR);
		//不修改原数据
		HBITMAP ScaleBitmap(int newWidth, int newHeight);
		void DeleteBitMap();
		COLORREF GetImagePixel(UINT x, UINT y)const;
		void Draw(HDC hdc, int x, int y, int w = -1, int h = -1);
#endif // WIN32

	private:
		std::vector<UCHAR> m_Data;
		INT m_ImageWidth;
		INT m_ImageHeight;
		ColorChannelCount m_colorChannelCount;
#ifdef WIN32
		bool m_Auto;
		HBITMAP m_hBitmap;

		int m_hBitMapWidth;
		int m_hBitMapHeight;
#endif // WIN32
		bool IsSizeChange(int w, int h)const;
	};

	class ImageWind
	{
		static UINT g_WindCount;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HWND m_hWnd;
		HINSTANCE m_hInstance;
		std::vector<Image>m_ImageData;
		int m_shiftY;
		int m_ImageShowSize;
	public:
		ImageWind(HINSTANCE hInstance = nullptr, HWND parent = nullptr);
		~ImageWind();

		HWND GethWnd()const;
		void SetImage(const std::vector<Image> bitmap);
		void AddImage(const Image& image);
	};
}