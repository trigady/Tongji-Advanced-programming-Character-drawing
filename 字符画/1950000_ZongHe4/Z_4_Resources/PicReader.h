/******************************************************************
* ��ע�⣡                                                         *
* ��ͷ�ļ���Ϊ���װ��WinAPI��WIC�ײ㺯�������������ͼƬ��ȡ��������  *
* ���װ�������ⲿ�⣬����������һ����Լ��������������ϸ�Ķ����¹涨  *
*     ��ͷ�ļ����κ�û��TO-DO�ĵط����㲻Ҫ�޸ģ��������������⣬      *
* �뼰ʱ��ϵ��ʦ�����̣�                                            *
*     ÿһ��TO-DO����TO-DO��˵�� (TO-DO) END ����������ɿ��·�����  *
*     readPic()����Ϊ���װ��WinAPI�еķ��������Խ�ͼƬ��ȡΪRGBA��   *
* bitmap���ݣ����Ⲣ�����������ͨ���޸��������ֱ�Ӵﵽ��ȡ�Ҷ�ͼ��   *
* Ŀ�ġ�                                                           *
*     getData()����������Ҫ���Ƶĺ���������ȡ������һάBYTE����ת��   *
* ����ʵ�ֵ�Array�ࡣ                                              *
*     testReader()��demo���ṩ��ȡ���ݵ�����һ��˼·��               *
******************************************************************/
#ifndef PIC_READER_H
#define PIC_READER_H

#include <windows.h>
#include <wincodec.h>
#include <commdlg.h>
#include <iostream>


template<typename T = int>class Array {
public:
	T* data = NULL;
	int index;
	int shape[16] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
	int axisNum;
	int nowAxis;
	int capcity;
	bool flag = true;

	Array() {}

	template<typename ...Args>
	Array(Args... args) {
		// ��ȡ��������С��ת��Ϊsize_t����
		auto num = sizeof...(args);
		size_t list[] = { args... };

		int size = 1;
		axisNum = num;
		for (int i = 0; i < axisNum; i++) {
			shape[i] = list[i];
			size *= shape[i];
		}

		capcity = size;
		data = new T[size];

		index = 0;
		nowAxis = axisNum;
	}

	template<typename ...Args>
	Array at(Args... args) {
		// ��ȡ��������С��ת��Ϊsize_t����
		auto num = sizeof...(args);
		size_t list[] = { args... };

		if (num > this->nowAxis) {
			exit(-1);
		}

		int size = 0, size1 = 1;
		for (int i = 0; i < num; i++) {
			size1 = list[i];
			for (int j = i + 1; j < this->nowAxis; j++) {
				size1 *= shape[i];
			}
			size += size1;
		}

		Array temp;
		temp.axisNum = this->axisNum;
		temp.data = this->data;
		temp.index = this->index + size;
		temp.nowAxis = this->nowAxis - num;
		temp.capcity = this->capcity;

		int uhi = 0;
		for (int i = 0; i < this->nowAxis; i++) {
			if (i < num) {
				continue;
			}
			temp.shape[uhi] = this->shape[i];
			uhi++;
		}

		return temp;
	}

	template<typename ...Args>
	void reshape(Args... args) {
		// ��ȡ��������С��ת��Ϊsize_t����
		auto num = sizeof...(args);
		size_t list[] = { args... };

		int uhi = 1, uhj = 1;
		for (int i = 0; i < num; i++) {
			uhi *= list[i];
			uhj *= this->shape[i];
		}
		if (uhi != uhj) {
			exit(-1);
		}

		for (int i = 0; i < 16; i++) {
			if (i < num) {
				shape[i] = list[i];
			}
			else {
				shape[i] = 1;
			}
		}
		axisNum = num;
		nowAxis = axisNum;
		this->index = 0;
	}

	T* get_content() {
		return data;
	}

	void set(int value) {
		this->data[this->index] = value;
	}

	Array operator[](int index) {
		// �������޸��Ӿ����nowAxis��ֵ�Լ������б�Ҫ��ֵ���Է���һ���Ӿ���
		if (this->nowAxis == 0) {
			exit(-1);
		}

		int size = index;
		for (int i = 1; i < this->nowAxis; i++) {
			size *= this->shape[i];
		}

		Array temp;
		temp.axisNum = this->axisNum;
		temp.data = this->data;
		temp.index = this->index + size;
		temp.nowAxis = this->nowAxis - 1;
		temp.capcity = this->capcity;

		for (int i = 1; i < this->nowAxis; i++) {
			temp.shape[i - 1] = this->shape[i];
		}

		return temp;
	}

	~Array() {
		if (axisNum == nowAxis) {
			delete[] data;
		}
	}

	Array& operator=(Array sp) {
		this->axisNum = sp.axisNum;
		this->capcity = sp.capcity;
		this->index = sp.index;
		this->nowAxis = sp.nowAxis;
		for (int i = 0; i < this->axisNum; i++) {
			this->shape[i] = sp.shape[i];
		}
		this->data = new T[this->capcity];
		for (int i = 0; i < this->capcity; i++) {
			this->data[i] = sp.data[i];
		}
		this->flag = true;
		return *this;
	}

	Array(const Array& sp) {
		this->axisNum = sp.axisNum;
		this->capcity = sp.capcity;
		this->index = sp.index;
		this->nowAxis = sp.nowAxis;
		for (int i = 0; i < this->axisNum; i++) {
			this->shape[i] = sp.shape[i];
		}
		if (sp.flag) {
			this->data = sp.data;
		}
		else {
			this->data = new T[this->capcity];
			for (int i = 0; i < this->capcity; i++) {
				this->data[i] = sp.data[i];
			}
			this->flag = true;
		}
	}

	Array operator=(T data) {
		this->data[index] = data;
		return *this;
	}

	operator int() {
		return data[index];
	}

	Array operator+(Array &sp) {
		if (this->capcity != sp.capcity) {
			exit(-1);
		}

		Array temp;
		temp.axisNum = this->axisNum;
		temp.capcity = this->capcity;
		temp.index = this->index;
		temp.nowAxis = this->nowAxis;
		temp.data = new T[this->capcity];
		temp.flag = false;

		for (int i = 0; i < this->axisNum; i++) {
			temp.shape[i] = this->shape[i];
		}

		for (int i = 0; i < this->capcity; i++) {
			temp.data[i] = this->data[i] + sp.data[i];
		}

		return temp;
	}

	Array operator+(int sp) {

		Array temp;
		temp.axisNum = this->axisNum;
		temp.capcity = this->capcity;
		temp.index = this->index;
		temp.nowAxis = this->nowAxis;
		temp.data = new T[this->capcity];
		temp.flag = false;

		for (int i = 0; i < this->axisNum; i++) {
			temp.shape[i] = this->shape[i];
		}

		for (int i = 0; i < this->capcity; i++) {
			temp.data[i] = this->data[i] + sp;
		}

		return temp;
	}

	Array operator-(Array &sp) {
		if (this->capcity != sp.capcity) {
			exit(-1);
		}

		Array temp;
		temp.axisNum = this->axisNum;
		temp.capcity = this->capcity;
		temp.index = this->index;
		temp.nowAxis = this->nowAxis;
		temp.data = new T[this->capcity];
		temp.flag = false;

		for (int i = 0; i < this->axisNum; i++) {
			temp.shape[i] = this->shape[i];
		}

		for (int i = 0; i < this->capcity; i++) {
			temp.data[i] = this->data[i] - sp.data[i];
		}

		return temp;
	}

	Array operator-(int sp) {

		Array temp;
		temp.axisNum = this->axisNum;
		temp.capcity = this->capcity;
		temp.index = this->index;
		temp.nowAxis = this->nowAxis;
		temp.data = new T[this->capcity];
		temp.flag = false;

		for (int i = 0; i < this->axisNum; i++) {
			temp.shape[i] = this->shape[i];
		}

		for (int i = 0; i < this->capcity; i++) {
			temp.data[i] = this->data[i] - sp;
		}

		return temp;
	}

	Array operator*(Array &sp) {
		if (this->capcity != sp.capcity) {
			exit(-1);
		}

		Array temp;
		temp.axisNum = this->axisNum;
		temp.capcity = this->capcity;
		temp.index = this->index;
		temp.nowAxis = this->nowAxis;
		temp.data = new T[this->capcity];
		temp.flag = false;

		for (int i = 0; i < this->axisNum; i++) {
			temp.shape[i] = this->shape[i];
		}

		for (int i = 0; i < this->capcity; i++) {
			temp.data[i] = this->data[i] * sp.data[i];
		}

		return temp;
	}

	Array operator*(int sp) {

		Array temp;
		temp.axisNum = this->axisNum;
		temp.capcity = this->capcity;
		temp.index = this->index;
		temp.nowAxis = this->nowAxis;
		temp.data = new T[this->capcity];
		temp.flag = false;

		for (int i = 0; i < this->axisNum; i++) {
			temp.shape[i] = this->shape[i];
		}

		for (int i = 0; i < this->capcity; i++) {
			temp.data[i] = this->data[i] * sp;
		}

		return temp;
	}

	Array operator/(Array &sp) {
		if (this->capcity != sp.capcity) {
			exit(-1);
		}

		Array temp;
		temp.axisNum = this->axisNum;
		temp.capcity = this->capcity;
		temp.index = this->index;
		temp.nowAxis = this->nowAxis;
		temp.data = new T[this->capcity];
		temp.flag = false;

		for (int i = 0; i < this->axisNum; i++) {
			temp.shape[i] = this->shape[i];
		}

		for (int i = 0; i < this->capcity; i++) {
			temp.data[i] = this->data[i] / sp.data[i];
		}

		return temp;
	}

	Array operator/(int sp) {

		Array temp;
		temp.axisNum = this->axisNum;
		temp.capcity = this->capcity;
		temp.index = this->index;
		temp.nowAxis = this->nowAxis;
		temp.data = new T[this->capcity];
		temp.flag = false;

		for (int i = 0; i < this->axisNum; i++) {
			temp.shape[i] = this->shape[i];
		}

		for (int i = 0; i < this->capcity; i++) {
			temp.data[i] = this->data[i] / sp;
		}

		return temp;
	}
};

template <typename T>
inline void SafeRelease(T *&p) {
	if (nullptr != p) {
		p->Release();
		p = nullptr;
	}
}

class PicReader {
public:
	PicReader();
	~PicReader();

	void readPic(LPCSTR);
	void getData(Array<int> &spp, UINT &xx, UINT &yy);
	void testReader(BYTE *&,UINT &, UINT &);
private:
	void init();
	bool checkHR(HRESULT);
	void quitWithError(LPCSTR);

	HWND                    hWnd;
	HANDLE                  hFile;
	IWICImagingFactory     *m_pIWICFactory;
	IWICFormatConverter    *m_pConvertedSourceBitmap;

	/*TO-DO��������ܻ���������Ҫ���ڲ���Ա END*/
};

PicReader::PicReader() : m_pConvertedSourceBitmap(nullptr), m_pIWICFactory(nullptr) {
	init();
}

PicReader::~PicReader() {
	if (hFile != NULL) CloseHandle(hFile);
	SafeRelease(m_pConvertedSourceBitmap);
	SafeRelease(m_pIWICFactory);
	CoUninitialize();
}

bool PicReader::checkHR(HRESULT hr) {
	return (hr < 0);
}

void PicReader::quitWithError(LPCSTR message) {
	MessageBoxA(hWnd, message, "Application Error", MB_ICONEXCLAMATION | MB_OK);
	quick_exit(0xffffffff);
}

void PicReader::init() {
	hWnd = GetForegroundWindow();

	// Enables the terminate-on-corruption feature.
	HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

	HRESULT hr = S_OK;

	//Init the WIC
	hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	// Create WIC factory
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);

	// Throw error if create factor failed
	if (checkHR(hr)) { quitWithError("Init Reader Failed"); }
}

void PicReader::readPic(LPCSTR fileName) {
	HRESULT hr = S_OK;

	// Create a File Handle (WinAPI method not std c)
	if (hFile != NULL) CloseHandle(hFile);
	hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_NOT_FOUND) {
		quitWithError("Cannot find such file, please retry or check the access");
	}

	// Create a decoder
	IWICBitmapDecoder *pDecoder = nullptr;
	hr = m_pIWICFactory->CreateDecoderFromFileHandle((ULONG_PTR)hFile, nullptr, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if (checkHR(hr)) { quitWithError("Create Decoder Failed"); }

	// Retrieve the first frame of the image from the decoder
	IWICBitmapFrameDecode *pFrame = nullptr;
	hr = pDecoder->GetFrame(0, &pFrame);
	if (checkHR(hr)) { quitWithError("Get Frame Failed"); }

	// Format convert the frame to 32bppRGBA
	SafeRelease(m_pConvertedSourceBitmap);
	hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
	if (checkHR(hr)) { quitWithError("Get Format Converter Failed"); }

	hr = m_pConvertedSourceBitmap->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeCustom);
	if (checkHR(hr)) { quitWithError("Init Bitmap Failed"); }

	// Clean memory
	SafeRelease(pDecoder);
	SafeRelease(pFrame);
}

void PicReader::getData(Array<int> &spp, UINT &xx, UINT &yy) {
	HRESULT hr = S_OK;

	// Get the size of Image
	UINT x, y;
	hr = m_pConvertedSourceBitmap->GetSize(&x, &y);
	if (checkHR(hr)) { quitWithError("Check Bitmap Size Failed"); }

	// Create the buffer of pixels, the type of BYTE is unsigned char
	BYTE *data;
	data = new BYTE[x * y * 4];
	memset(data, 0, x * y * 4);

	// Copy the pixels to the buffer
	UINT stride = x * 4;
	hr = m_pConvertedSourceBitmap->CopyPixels(nullptr, stride, x * y * 4, data);
	if (checkHR(hr)) { quitWithError("Copy Pixels Failed"); }

	/******************************************************************
	*  TO-DO:                                                         *
	*                                                                 *
	*  ʵ��һ��Array�࣬���������dataת�������Array��                  *
	*                                                                 *
	*  ����˵������Bitmap Copy���������ݣ�ÿ4��Ϊһ�����һ������         *
	*           ����Ϊһ������Ϊͼ���(��*��*4)��һά����                *
	*           �������Ų�Ϊ R G B A R G B A R G B A.....              *
	*                                                                 *
	*  ��ע�⣡  �������ֻ�Ķ��Ӵ˿�ʼ����һ��TO-DO ENDλ�õĴ��룡       *
	******************************************************************/

	Array<BYTE> sp(x, y, 4);	
	int hh = 0;
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			for (int k = 0; k < 4; k++) {
				sp[j][i][k] = data[hh];
				hh++;
			}
		}
	}

	spp.axisNum = 2;
	spp.capcity = y / 2 * x;
	spp.index = 0;
	spp.nowAxis = 2;
	spp.shape[0] = x;
	spp.shape[1] = y / 2;
	spp.data = new int[spp.capcity];

	int t1, t2, t3;
	int G1, G2;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y / 2; j++) {
			t1 = sp[i][2 * j][0];
			t2 = sp[i][2 * j][1];
			t3 = sp[i][2 * j][2];
			G1 = (t1 * 299 + t2 * 587 + t3 * 114 + 500) / 1000;

			t1 = sp[i][2 * j + 1][0];
			t2 = sp[i][2 * j + 1][1];
			t3 = sp[i][2 * j + 1][2];
			G2 = (t1 * 299 + t2 * 587 + t3 * 114 + 500) / 1000;

			spp[i][j] = (G1 + G2 + 1) / 2;
		}
	}

	xx = x;
	yy = y / 2;

	delete[] data;

	/******************************************************************
	*  TO-DO END                                                      *
	******************************************************************/

	// Close the file handle
	CloseHandle(hFile);
	hFile = NULL;
}

void PicReader::testReader(BYTE* &_out, UINT& _x, UINT& _y){
	HRESULT hr = S_OK;

	// Get the size of Image
	UINT x, y;
	hr = m_pConvertedSourceBitmap->GetSize(&x, &y);
	if (checkHR(hr)) { quitWithError("Check Bitmap Size Failed"); }

	// Create the buffer of pixels, the type of BYTE is unsigned char
	BYTE *data;
	data = new BYTE[x * y * 4];
	memset(data, 0, x * y * 4);

	// Copy the pixels to the buffer
	UINT stride = x * 4;
	hr = m_pConvertedSourceBitmap->CopyPixels(nullptr, stride, x * y * 4, data);
	if (checkHR(hr)) { quitWithError("Copy Pixels Failed"); }


	_out = data; _x = x; _y = y;

	// Close the file handle
	CloseHandle(hFile);
	hFile = NULL;
}

#endif