#define  _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "winmm.lib")
#include "PicReader.h"
#include "FastPrinter.h"
#include "mmsystem.h"
#include <stdio.h>

const static char asciiStrength[15] = { 
	'M','N','H','Q','$','O','C','?','7','>','!',':','-',';','.' 
};

char get_ascii(int sp) {
	return asciiStrength[sp / 18];
}

//这里定义高度
int HEI = 60;

void draw_(const char addr[]) {
	PicReader imread;
	BYTE *data = nullptr;
	UINT x, y;

	imread.readPic(addr);
	Array<int> test;
	UINT xx = 0, yy = 0;
	imread.getData(test, xx, yy);

	int times = yy / HEI;
	if (times == 0) {
		HEI = yy;
		times = 1;
	}
	int LEN = xx / times;
	if (LEN > 190) {
		times = xx / 190;
		HEI = yy / times;
		LEN = 190;
	}
	int temp;

	FastPrinter printer(LEN, HEI);
	char *databuffer = new char[LEN*HEI];
	WORD *colorbuffer = new WORD[LEN*HEI];

	printer.cleanSrceen();
	for (int j = 0; j < LEN; j++) {
		for (int i = 0; i < HEI; i++) {
			temp = 0;
			for (int ii = i * times; ii < (i + 1)*times; ii++) {
				for (int jj = j * times; jj < (j + 1)*times; jj++) {
					temp += test[jj][ii];
				}
			}
			temp = (temp + times * times / 2) / (times*times);

			databuffer[i*LEN + j] = get_ascii(temp);
			colorbuffer[i*LEN + j] = fp_color::b_white | fp_color::f_black;
		}
	}

	printer.setData(databuffer, colorbuffer);
	printer.draw(true);
	getchar();

	HEI = 60;
}

const int NUM = 1011;

struct the_id {
	char *databuffer;
}ids[NUM];

void video() {
	PicReader imread;
	BYTE *data = nullptr;
	UINT x, y;

	Array<int> test;
	UINT xx = 0, yy = 0;

	char addr[20] = "video\\sea 0001.jpg";

	int times, HEI = 60, LEN, temp;

	WORD *colorbuffer;

	imread.readPic((const char*)addr);
	imread.getData(test, xx, yy);

	times = yy / HEI;
	if (times == 0) {
		HEI = yy;
		times = 1;
	}
	LEN = xx / times;
	if (LEN > 190) {
		times = xx / 190;
		HEI = yy / times;
		LEN = 190;
	}

	colorbuffer = new WORD[LEN*HEI];
	for (int j = 0; j < LEN; j++) {
		for (int i = 0; i < HEI; i++) {
			colorbuffer[i*LEN + j] = fp_color::b_white | fp_color::f_black;
		}
	}

	FastPrinter printer(LEN, HEI);

	for (int uhi = 1; uhi <= NUM; uhi++) {
		addr[10] = '0' + uhi / 1000;
		addr[11] = '0' + (uhi % 1000) / 100;
		addr[12] = '0' + (uhi % 100) / 10;
		addr[13] = '0' + uhi % 10;

		imread.readPic((const char*)addr);
		imread.getData(test, xx, yy);

		ids[uhi].databuffer = new char[LEN*HEI];

		for (int j = 0; j < LEN; j++) {
			for (int i = 0; i < HEI; i++) {
				temp = 0;
				for (int ii = i * times; ii < (i + 1)*times; ii++) {
					for (int jj = j * times; jj < (j + 1)*times; jj++) {
						temp += test[jj][ii];
					}
				}
				temp = (temp + times * times / 2) / (times*times);

				ids[uhi].databuffer[i*LEN + j] = get_ascii(temp);
				colorbuffer[i*LEN + j] = fp_color::b_white | fp_color::f_black;
			}
		}
		HEI = 60;
	}

	printer.cleanSrceen();
	COORD textXY;
	textXY.X = 10;
	textXY.Y = 10;
	printer.setText(textXY, "按下回车开始播放");
	printer.draw(true);
	getchar();

	DWORD tic, tic2;

	tic = GetTickCount();
	PlaySound("sound\\sea.wav", NULL, SND_FILENAME | SND_ASYNC);
	for (int uhi = 1; uhi <= NUM; uhi++) {
		for (;;) {
			tic2 = GetTickCount();
			if (tic2 - tic >= 40) {
				tic += 40;
				printer.cleanSrceen();
				printer.setData(ids[uhi].databuffer, colorbuffer);
				printer.draw(true);
				break;
			}
		}
	}
}

int main() {
	char ccc;
	
	while (true) {
		printf("************************** 高 程 大 作 业 **************************\n");
		printf(" * 请输入您想要查看的内容编号：                                   * \n");
		printf(" * a) airplane.jpg       b) baboon.jpg         c) barbara.jpg     * \n");
		printf(" * d) cameraman.jpg      e) compa.png          f) goldhill.jpg    * \n");
		printf(" * g) lena.jpg           h) lena1.jpg          i) milkdrop.jpg    * \n");
		printf(" * j) peppers.jpg        k) woman.jpg          l) sea.mp4         * \n");
		printf(" *                       q)   退出                                * \n");
		printf("************************** 1852979 李至霖 **************************\n");

		scanf("%c", &ccc);
		getchar();

		switch (ccc) {
			case 'a':
				draw_("classic_picture\\airplane.jpg");
				break;
			case 'b':
				draw_("classic_picture\\baboon.jpg");
				break;
			case 'c':
				draw_("classic_picture\\barbara.jpg");
				break;
			case 'd':
				draw_("classic_picture\\cameraman.jpg");
				break;
			case 'e':
				draw_("classic_picture\\compa.png");
				break;
			case 'f':
				draw_("classic_picture\\goldhill.jpg");
				break;
			case 'g':
				draw_("classic_picture\\lena.jpg");
				break;
			case 'h':
				draw_("classic_picture\\lena1.jpg");
				break;
			case 'i':
				draw_("classic_picture\\milkdrop.jpg");
				break;
			case 'j':
				draw_("classic_picture\\peppers.jpg");
				break;
			case 'k':
				draw_("classic_picture\\woman.jpg");
				break;
			case 'l':
				video();
				break;
		}

		if (ccc == 'q') {
			break;
		}
	}


	return 0;
}