#include "FunctionWind.h"
#include<iostream>
#include<map>
#include<array>
#include"常用函数.hpp"
namespace CMath {
	UINT FunctionWind::m_WindowCount = 0;
	FunctionWind::FunctionWind(HINSTANCE hInstance, HWND parent) :
		m_MemBitmap(nullptr), m_MemDC(nullptr), m_OldBitmap(nullptr)
	{
		if (hInstance == nullptr)
		{
			hInstance = GetModuleHandle(NULL);
		}
		m_hInstance = hInstance;
		++m_WindowCount;
		if (m_WindowCount == 1)
		{
			WNDCLASSEX wc = { 0 };

			// 设置窗口类结构体的各个参数
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = this->WndProc;
			wc.hInstance = hInstance;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszClassName = G_FUNCTION_WINDOW_CLASS_NAME;

			if (!RegisterClassEx(&wc))
			{
				GetSystemLastErrorClass error;
				std::cout << "函数显示窗口类注册失败" << std::endl;
				std::cout << "错误代码：" << error.GetErrorCode() << std::endl;
				std::cout << error.GetErrorMsg() << std::endl;

			}
		}
		if (parent == nullptr)
		{
			m_hWnd = CreateWindow(
				G_FUNCTION_WINDOW_CLASS_NAME,
				TEXT("二维函数图像显示窗口"),
				WS_OVERLAPPEDWINDOW|WS_VISIBLE,
				100, 100, 800, 600,
				NULL,
				NULL,
				hInstance,
				this);
		}
		else
		{
			m_hWnd = CreateWindow(
				G_FUNCTION_WINDOW_CLASS_NAME,
				TEXT("二维函数图像显示窗口"),
				WS_CHILD | WS_VISIBLE,
				100, 100, 800, 600,
				parent,
				NULL,
				hInstance,
				this);
		}
		if (!m_hWnd)
		{
			GetSystemLastErrorClass error;
			std::cout << "二维函数图像显示窗口创建失败" << std::endl;
			std::cout << "错误代码：" << error.GetErrorCode() << std::endl;
			std::cout << error.GetErrorMsg() << std::endl;
		}
		m_CoordinateAxis = {
			0,20,//x
			0,20//y
		};
		m_XPrecision = 1.;
		m_YPrecision = 1.;
		
	}
	FunctionWind::~FunctionWind()
	{
		m_WindowCount--;
		CloseWindow(m_hWnd);
		if (m_WindowCount == 0)
		{
			UnregisterClass(G_FUNCTION_WINDOW_CLASS_NAME, m_hInstance);
		}
	}
	LRESULT CALLBACK FunctionWind::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static std::map<HWND, FunctionWind*>g_WindIndex;
		switch (message)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			auto fw_index = g_WindIndex.find(hWnd);
			if (fw_index == g_WindIndex.end())
			{
				EndPaint(hWnd, &ps);
				break;
			}
			FunctionWind* fw = fw_index->second;


			// 创建内存设备上下文（内存DC）
			HDC memDC = CreateCompatibleDC(hdc);
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;

			// 创建内存位图
			HBITMAP bitmap = CreateCompatibleBitmap(hdc, width, height);

			// 将位图选入内存DC
			HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);

			// 在内存DC上进行绘图操作
			// ...

			// 将内存DC上绘制好的图像拷贝到窗口DC上进行显示
			BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

			// 清理资源
			SelectObject(memDC, oldBitmap);
			DeleteObject(bitmap);
			DeleteDC(memDC);

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_SIZE:
		{
			auto fw_index = g_WindIndex.find(hWnd);
			if (fw_index == g_WindIndex.end())
			{
				break;
			}
			FunctionWind* fw = fw_index->second;
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			HDC memDC = fw->m_MemDC;
			if (memDC)
			{ 
				SelectObject(memDC, fw->m_OldBitmap);
				DeleteObject(fw->m_MemBitmap);
				DeleteDC(memDC);
			}
			break;
		}
		case WM_CREATE:
		{
			CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			LPVOID lpData = pCreateStruct->lpCreateParams;
			FunctionWind* fw = static_cast<FunctionWind*>(lpData);
			if (fw)
			{
				g_WindIndex.insert(std::make_pair(hWnd, fw));
			}
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return 0;
	}

	constexpr auto G_IMAGE_WINDOW_CLASS_NAME = L"Image_Show_Window";
	UINT ImageWind::g_WindCount = 0;
	ImageWind::ImageWind(HINSTANCE hInstance, HWND parent) :m_shiftY(0), m_ImageShowSize(-1)
	{
		if (hInstance == nullptr)
		{
			hInstance = GetModuleHandle(NULL);
		}
		m_hInstance = hInstance;
		++g_WindCount;
		if (g_WindCount == 1)
		{
			WNDCLASSEX wc = { 0 };

			// 设置窗口类结构体的各个参数
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = this->WndProc;
			wc.hInstance = hInstance;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszClassName = G_IMAGE_WINDOW_CLASS_NAME;

			if (!RegisterClassEx(&wc))
			{
				GetSystemLastErrorClass error;
				std::cout << "图片显示窗口类注册失败" << std::endl;
				std::cout << "错误代码：" << error.GetErrorCode() << std::endl;
				std::cout << error.GetErrorMsg() << std::endl;

			}
		}
		if (parent == nullptr)
		{
			m_hWnd = CreateWindow(
				G_IMAGE_WINDOW_CLASS_NAME,
				TEXT("图像显示窗口"),
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				100, 100, 800, 600,
				NULL,
				NULL,
				hInstance,
				this);
		}
		else
		{
			m_hWnd = CreateWindow(
				G_IMAGE_WINDOW_CLASS_NAME,
				TEXT("图像显示窗口"),
				WS_CHILD | WS_VISIBLE,
				100, 100, 800, 600,
				parent,
				NULL,
				hInstance,
				this);
		}
		if (!m_hWnd)
		{
			GetSystemLastErrorClass error;
			std::cout << "图像显示窗口创建失败" << std::endl;
			std::cout << "错误代码：" << error.GetErrorCode() << std::endl;
			std::cout << error.GetErrorMsg() << std::endl;
		}
	}
	ImageWind::~ImageWind()
	{
		g_WindCount--;
		CloseWindow(m_hWnd);
		if (g_WindCount == 0)
		{
			UnregisterClass(G_IMAGE_WINDOW_CLASS_NAME, m_hInstance);
		}
	}
	void ImageWind::SetImage(const std::vector<Image> bitmap)
	{
		m_ImageData = bitmap;
		InvalidateRect(m_hWnd, NULL, true);
	}
	HWND ImageWind::GethWnd()const
	{
		return m_hWnd;
	}
	void ImageWind::AddImage(const Image& image)
	{
		m_ImageData.push_back(image);
	}
	LRESULT CALLBACK ImageWind::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static std::map<HWND, ImageWind*>g_WindIndex;
		switch (message)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			ImageWind* iw = g_WindIndex[hWnd];
			if (!iw || iw->m_ImageData.empty())
			{
				EndPaint(hWnd, &ps);
				break;
			}
			auto& bitmaps = iw->m_ImageData;
			if (bitmaps.empty())
			{
				EndPaint(hWnd, &ps);
				break;
			}
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);

			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

			int x = 0;
			int y = iw->m_shiftY;

			int w;
			if (iw->m_ImageShowSize < 0)
			{
				w = bitmaps[0].GetImageWidth();
			}
			else
			{
				w = iw->m_ImageShowSize;
			}
			int h = bitmaps[0].GetImageHeight() / bitmaps[0].GetImageWidth() * w;
			for (int i = 0; i < bitmaps.size(); i++) {
				if (y < -h)
				{
					bitmaps[i].DeleteBitMap();
					x += w;
					if (x >= clientRect.right)
					{
						x = 0;
						y += h;
					}
					if (y >= clientRect.bottom)
						break;
					continue;
				}
				bitmaps[i].Draw(hdcMem, x, y, w, h);
				x += w;
				if (x >= clientRect.right)
				{
					x = 0;
					y += h;
				}
				if (y >= clientRect.bottom)
					break;
			}

			BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcMem, 0, 0, SRCCOPY);

			SelectObject(hdcMem, hOldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(hdcMem);

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_ERASEBKGND:
			return TRUE;
		case WM_CREATE:
		{
			CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			LPVOID lpData = pCreateStruct->lpCreateParams;
			ImageWind* iw = static_cast<ImageWind*>(lpData);
			if (iw)
			{
				g_WindIndex.insert(std::make_pair(hWnd, iw));
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			short delta = GET_WHEEL_DELTA_WPARAM(wParam) / 6;
			ImageWind* iw = g_WindIndex[hWnd];
			if (!iw || iw->m_ImageData.empty())
			{
				break;
			}
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				if (iw->m_ImageShowSize < 0 && delta>0)
				{
					iw->m_ImageShowSize = 32;
				}
				else
					iw->m_ImageShowSize += (float)delta / 12;
				InvalidateRect(hWnd, NULL, false);
				break;
			}
			if (iw->m_shiftY + delta < 10)
				iw->m_shiftY += delta;
			InvalidateRect(hWnd, NULL, false);
			break;
		}
		case WM_CLOSE:
			DestroyWindow(hWnd);
			return 0;
			break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return 0;
	}
	HBITMAP LoadImageFile(HWND hWnd)
	{
		OPENFILENAME ofn = {};
		WCHAR szFile[MAX_PATH] = {};

		// 设置对话框参数
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFilter = L"All Files\0*.*\0";
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		// 显示打开文件对话框
		if (GetOpenFileName(&ofn))
		{
			// 加载位图文件
			return (HBITMAP)LoadImage(NULL, szFile, IMAGE_BITMAP | LR_LOADFROMFILE, 0, 0, LR_LOADFROMFILE);
		}

		return NULL;
	}

	Image::Image(const std::vector<unsigned char> Data, UINT ImageWidth, UINT ImageHeight,
		ColorChannelCount BitCount, //图片通道数
		ColorChannelOrder cco,//图片的通道顺序
		bool ColorChannelContinuous,//图片的通道是像素还是图片
		bool picturesflipping,//图片是否翻转
		bool TheOrderOfImagesIsColumns)//图片是一列一列的还是一行一行的？
	{
#ifdef WIN32
		m_hBitmap = nullptr;
#endif // WIN32
		m_hBitMapHeight = 0;
		m_hBitMapWidth = 0;
		if (ImageWidth == 0)
		{
			m_Data = {};
			m_ImageHeight = 0;
			m_ImageWidth = 0;
			return;
		}
		SetImage(Data, ImageWidth, ImageHeight, BitCount, cco, ColorChannelContinuous, picturesflipping, TheOrderOfImagesIsColumns);
	}
	Image::~Image()
	{
#ifdef WIN32
		if (m_hBitmap)
			DeleteObject(m_hBitmap);
		m_hBitmap = nullptr;
#endif // WIN32

	}
	void Image::SetImage(const std::vector<unsigned char> Data, UINT ImageWidth, UINT ImageHeight,
		ColorChannelCount BitCount,
		ColorChannelOrder cco,//图片的通道顺序
		bool ColorChannelContinuous ,//图片的通道是像素还是图片
		bool picturesflipping,//图片是否翻转
		bool TheOrderOfImagesIsColumns//图片是一列一列的还是一行一行的？
	)
	{
#ifdef WIN32
		DeleteBitMap();
		m_hBitmap = nullptr;
#endif // WIN32

		m_colorChannelCount = BitCount;
		if (ImageWidth == 0 || ImageHeight == 0)
		{
			m_Data = {};
			m_ImageHeight = 0;
			m_ImageWidth = 0;
			return;
		}
		m_Data.resize(Data.size());
		m_ImageWidth = ImageWidth;
		m_ImageHeight = ImageHeight;

		auto color = cco;
		size_t ptr;
		size_t ImageSize = (size_t)m_ImageHeight * m_ImageWidth;
		int ColorChanneCount = GetColorChannelSize();
		if (picturesflipping)
		{
			ptr = ImageSize - 1;
		}
		else
		{
			ptr = 0;
		}
		for (size_t x = 0; x < m_ImageWidth; ++x)
		{
			for (size_t y = 0; y < m_ImageHeight; ++y)
			{
				if (ColorChannelContinuous)
				{
					if (TheOrderOfImagesIsColumns)
					{
						for (int i = 0; i < ColorChanneCount; ++i)
						{
							m_Data[(x + m_ImageWidth * y) * ColorChanneCount + (intToChar(color, i))] = Data[ptr + ImageSize * i];
						}
					}
					else
					{
						for (int i = 0; i < ColorChanneCount; ++i)
						{
							m_Data[(x * m_ImageHeight + y) * ColorChanneCount + (intToChar(color, i))] = Data[ptr + ImageSize * i];
						}
					}
				}
				else
				{
					if (TheOrderOfImagesIsColumns)
					{
						size_t shift = ptr * 3;
						for (int i = 0; i < ColorChanneCount; ++i)
						{
							m_Data[(x + m_ImageWidth * y) * ColorChanneCount + (intToChar(color, i))] = Data[shift + i];
						}
					}
					else
					{
						size_t shift = ptr * 3;
						for (int i = 0; i < ColorChanneCount; ++i)
						{
							m_Data[(x * m_ImageHeight + y) * ColorChanneCount + (intToChar(color, i))] = Data[shift + i];
						}
					}
				}
				if (picturesflipping)
				{
					ptr--;
				}
				else
				{
					ptr++;
				}
			}
		}
		ptr;
	}
	HBITMAP Image::ScaleBitmap(HBITMAP hBitmap, int newWidth, int newHeight, int mode)
	{
		// 获取原始位图信息
		BITMAP bitmap = {};
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		// 创建设备上下文
		HDC hdcScreen = GetDC(NULL);
		if (!hdcScreen)
		{
			// 处理错误
			return nullptr;
		}
		HDC hdcSrc = CreateCompatibleDC(hdcScreen);
		if (!hdcSrc)
		{
			// 处理错误
			ReleaseDC(NULL, hdcScreen);
			return nullptr;
		}
		HDC hdcDst = CreateCompatibleDC(hdcScreen);
		if (!hdcDst)
		{
			// 处理错误
			DeleteDC(hdcSrc);
			ReleaseDC(NULL, hdcScreen);
			return nullptr;
		}

		// 创建新的位图
		BITMAPINFOHEADER bih;
		memset(&bih, 0, sizeof(BITMAPINFOHEADER));
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biWidth = newWidth;
		bih.biHeight = -newHeight;  // 负数表示顶部向下扫描
		bih.biPlanes = 1;
		bih.biBitCount = bitmap.bmBitsPixel;
		bih.biCompression = BI_RGB;

		// 创建与新尺寸匹配的DIBSection位图
		HBITMAP hNewBitmap = CreateDIBSection(hdcDst, (BITMAPINFO*)&bih, DIB_RGB_COLORS, NULL, NULL, 0);
		if (!hNewBitmap)
		{
			// 处理错误
			DeleteDC(hdcSrc);
			DeleteDC(hdcDst);
			ReleaseDC(NULL, hdcScreen);
			return nullptr;
		}

		// 将新位图选入设备上下文
		HBITMAP hOldSrcBitmap = (HBITMAP)SelectObject(hdcSrc, hBitmap);
		HBITMAP hOldDstBitmap = (HBITMAP)SelectObject(hdcDst, hNewBitmap);

		// 设置插值模式
		DWORD oldInterpolationMode = 0;
		oldInterpolationMode = SetStretchBltMode(hdcDst, mode);  // 不使用抗锯齿插值模式

		// 执行位图缩放
		StretchBlt(hdcDst, 0, 0, newWidth, newHeight, hdcSrc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

		// 恢复原始位图和设备上下文
		SelectObject(hdcSrc, hOldSrcBitmap);
		SelectObject(hdcDst, hOldDstBitmap);

		// 恢复原始的插值模式
		SetStretchBltMode(hdcDst, oldInterpolationMode);

		// 释放资源
		DeleteDC(hdcSrc);
		DeleteDC(hdcDst);
		ReleaseDC(NULL, hdcScreen);

		return hNewBitmap;
	}

	HBITMAP Image::ScaleBitmap(int w, int h)
	{
		if (!m_hBitmap)
			return nullptr;
		HBITMAP newBitMap = ScaleBitmap(m_hBitmap, w, h, false);
		DeleteBitMap();
		m_hBitmap = newBitMap;
		return newBitMap;
	}
	bool Image::IsSizeChange(int w, int h)const
	{
		if (w == m_hBitMapWidth && h == m_hBitMapHeight)
			return false;
		return true;
	}
	HBITMAP Image::ToBitMap(int w, int h,bool Auto)
	{
		if (w <= 0 || h <= 0)
		{
			w = m_ImageWidth;
			h = m_ImageHeight;
		}
		if (m_hBitmap && !IsSizeChange(w, h))
			return m_hBitmap;

		BITMAPINFO bmpInfo;
		memset(&bmpInfo, 0, sizeof(BITMAPINFO));
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = m_ImageWidth;
		bmpInfo.bmiHeader.biHeight = -m_ImageHeight;  // 负数表示顶部向下扫描
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = this->GetPixelSize() * 8;
		void* pPixels = nullptr;
		HDC hdcScreen = GetDC(nullptr);
		HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmpInfo, DIB_RGB_COLORS, &pPixels, nullptr, 0);

		if (hBitmap != nullptr)
		{
			int bytesPerPixel = GetColorChannelSize();  // 每像素的字节数（RGB）
			int pitch = m_ImageWidth * bytesPerPixel;  // 行字节数
			BYTE* srcPixels = m_Data.data();
			BYTE* destPixels = static_cast<BYTE*>(pPixels);

			for (int y = 0; y < m_ImageHeight; y++)
			{
				memcpy(destPixels, srcPixels, pitch);
				srcPixels += pitch;
				destPixels += pitch;
			}
			srcPixels = static_cast<BYTE*>(pPixels);
			for (int y = 0; y < m_ImageHeight; y++)
			{
				for (int x = 0; x < m_ImageWidth; x++)
				{
					// 获取像素索引
					int pixelIndex = (y * m_ImageWidth + x) * GetColorChannelSize();

					// 交换红色和蓝色通道的值
					BYTE temp = srcPixels[pixelIndex];
					srcPixels[pixelIndex] = srcPixels[pixelIndex + 2];
					srcPixels[pixelIndex + 2] = temp;
				}
			}
		}

		ReleaseDC(nullptr, hdcScreen);
		if (!Auto)
		{
			m_hBitmap = nullptr;
			return hBitmap;
		}
		m_hBitMapHeight = h;
		m_hBitMapWidth = w;
		m_hBitmap = hBitmap;
		return ScaleBitmap(w, h);
	}
	void Image::DeleteBitMap()
	{
		if (!m_hBitmap)
			return;
		DeleteObject(m_hBitmap);
		m_hBitmap = nullptr;
	}
	COLORREF Image::GetImagePixel(UINT x, UINT y)const
	{
		CMath::Color color = GetImagePixelColor(x, y);
		return RGB(color.x, color.y, color.z);
	}
	void Image::Draw(HDC hdc, int x, int y, int w, int h)
	{
		if (w == 0 || h == 0)
		{
			return;
		}
		if (w < 0)
		{
			w = m_ImageWidth;
		}
		if (h < 0)
		{
			h = m_ImageHeight;
		}
		if (w != m_hBitMapWidth || h != m_hBitMapHeight)
		{
			m_hBitMapHeight = h;
			m_hBitMapWidth = w;
			DeleteBitMap();
		}
		if (m_colorChannelCount < ColorChannelCount::RGBA_32)
		{
			HBITMAP hCurBitmap = ToBitMap(w, h);
			// 创建临时的兼容设备上下文
			HDC hdcMemTemp = CreateCompatibleDC(hdc);

			// 将当前HBITMAP选入临时设备上下文
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMemTemp, hCurBitmap);

			// 获取临时设备上下文中位图的大小
			BITMAP bm;
			GetObject(hCurBitmap, sizeof(BITMAP), &bm);
			int curBitmapWidth = bm.bmWidth;
			int curBitmapHeight = bm.bmHeight;

			// 在主设备上下文中绘制当前位图
			BitBlt(hdc, x, y, curBitmapWidth, curBitmapHeight, hdcMemTemp, 0, 0, SRCCOPY);

			// 恢复原始位图和设备上下文，并释放临时设备上下文资源
			SelectObject(hdcMemTemp, hOldBitmap);
			DeleteDC(hdcMemTemp);
			//DeleteBitMap();
		}
	}

	UINT Image::GetImageWidth()const
	{
		return m_ImageWidth;
	}
	UINT Image::GetImageHeight()const
	{
		return m_ImageHeight;
	}
	Color Image::GetImagePixelColor(UINT x, UINT y)const
	{
		if (x >= m_ImageWidth || y >= m_ImageHeight)
			return Color();
		int channelSize = GetPixelSize();
		size_t ptr = ((size_t)m_ImageWidth * y + x) * channelSize;
		switch (GetColorChannelSize())
		{
		case 1:
			return Color(m_Data[ptr], m_Data[ptr], m_Data[ptr], 255);
			break;
		case 3:
			return Color(m_Data[ptr], m_Data[ptr + 1], m_Data[ptr + 2], 255);
			break;
		case 4:
			return Color(m_Data[ptr], m_Data[ptr + 1], m_Data[ptr + 2], m_Data[ptr + 3]);
			break;
		default:
			return Color(255);
			break;
		}
		
	}
	std::vector<unsigned char>& Image::GetData()
	{
		return m_Data;
	}
	int Image::GetPixelSize()const
	{
		switch (m_colorChannelCount)
		{
		case CMath::Image::ColorChannelCount::G_8:
			return 1;
			break;
		case CMath::Image::ColorChannelCount::RGB_24:
			return 3;
			break;
		case CMath::Image::ColorChannelCount::RGBA_32:
		case CMath::Image::ColorChannelCount::G_f:
			return 4;
			break;
		case CMath::Image::ColorChannelCount::RGB_48:
			return 6;
			break;
		case CMath::Image::ColorChannelCount::RGBA_64:
		case CMath::Image::ColorChannelCount::G_d:
			return 8;
			break;
		case CMath::Image::ColorChannelCount::RGB_float:
			return 12;
			break;
		case CMath::Image::ColorChannelCount::RGBA_float:
			return 16;
			break;
		case CMath::Image::ColorChannelCount::RGB_double:
			return 24;
			break;
		case CMath::Image::ColorChannelCount::RGBA_double:
			return 32;
			break;
		}
		return 0;
	}
	int Image::GetColorChannelSize()const
	{
		switch (m_colorChannelCount)
		{
		case CMath::Image::ColorChannelCount::G_8:
		case CMath::Image::ColorChannelCount::G_f:
		case CMath::Image::ColorChannelCount::G_d:
			return 1;
			break;
		case CMath::Image::ColorChannelCount::RGB_24:
		case CMath::Image::ColorChannelCount::RGB_48:
		case CMath::Image::ColorChannelCount::RGB_float:
		case CMath::Image::ColorChannelCount::RGB_double:
			return 3;
			break;
		case CMath::Image::ColorChannelCount::RGBA_32:
		case CMath::Image::ColorChannelCount::RGBA_64:
		case CMath::Image::ColorChannelCount::RGBA_float:
		case CMath::Image::ColorChannelCount::RGBA_double:
			return 4;
			break;
		}
	}
	const std::vector<UCHAR>& Image::ScaleImage(int w, int h)
	{
		if (!IsSizeChange(w, h))
			return m_Data;
		std::vector<UCHAR> NewImageData(w * h * GetPixelSize());
		int channelSize = GetColorChannelSize();
		for (int x = 0; x < w; ++x)
		{
			for (int y = 0; y < h; ++y)
			{
				float sx = (float)x / (w - 1) * (m_ImageWidth - 1);
				float sy = (float)y / (h - 1) * (m_ImageHeight - 1);
				Color color = GetImagePixel(sx, sy);
				int ptr = (y * w + x) * channelSize;
				for (int i = 0; i < channelSize; ++i)
				{
					NewImageData[ptr + i] = color[i];
				}
			}
		}
		DeleteBitMap();
		m_Data = NewImageData;
		return m_Data;
	}
}
