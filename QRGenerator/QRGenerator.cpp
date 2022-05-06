

//	-------------------------------------------------------
//	Includes
//	-------------------------------------------------------

#include "stdafx.h"
#include <string.h>
#include <errno.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include "qrencode.h"
#include "rc4.h"

#include <windows.h>

//	-------------------------------------------------------


//	-------------------------------------------------------
//	DEFines
//	-------------------------------------------------------

#define OUT_FILE_PIXEL_PRESCALER	3					// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

#define PIXEL_COLOR_R				0							// Color of bmp pixels
#define PIXEL_COLOR_G				0
#define PIXEL_COLOR_B				50 //xff

#define STRING_LENGTH				15
//#define ID_BMP					0x4D42	//"BM" //0x4D42

// BMP defines

typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef signed long		LONG;

#define BI_RGB			0L

#pragma pack(push, 2)


#pragma pack(pop)
//	-------------------------------------------------------
BYTE COMTCP_KEY[] = "COMTCP_V9.0_KEY_GODAVARTHI_SURESH_BOND247";

void exit_message(const char* error_message, int error)
{
	// Print an error message
	fprintf(stderr, error_message);
	fprintf(stderr, "\n");
	// Exit the program
	exit(error);
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Arguments is failed\n");
		return -1;
	}

	char*			szSourceSring = new char[strlen(argv[3])];
	strcpy(szSourceSring, argv[3]);
	int imgWidth = atoi(argv[2]);

	if (imgWidth <= 0)
	{
		printf("Size argument is wrong!\n");
		return -1;
	}
	if (imgWidth % 4)
		imgWidth = (imgWidth / 4 + 1) * 4;

	unsigned int	unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char*	pRGBData, *pSourceData, *pDestData;
	QRcode*			pQRC;

	char qrcontent[100];
	char filename[100];


	{
		// Compute QRCode
		if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_H, QR_MODE_8, 1))
		{
			unWidth = pQRC->width;
			unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 3;
			if (unWidthAdjusted % 4)
				unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
			unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;

			// Allocate pixels buffer
			if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
			{
				printf("Out of memory");
				exit(-1);
			}

			// Preset to white
			memset(pRGBData, 0xff, unDataBytes);

			// Prepare bmp headers
			BITMAPFILEHEADER bfh;
			BITMAPINFOHEADER bih;

			memcpy((char *)&bfh.bfType, "BM", 2);
			bfh.bfSize = sizeof(bfh) + sizeof(bih) + imgWidth * imgWidth * 3;
			bfh.bfReserved1 = 0;
			bfh.bfReserved2 = 0;
			bfh.bfOffBits = sizeof(bfh) + sizeof(bih);


			bih.biSize = sizeof(bih);
			bih.biWidth = imgWidth;// unWidth * OUT_FILE_PIXEL_PRESCALER;
			bih.biHeight = imgWidth;// -((int)unWidth * OUT_FILE_PIXEL_PRESCALER);
			bih.biPlanes = 1;
			bih.biBitCount = 24;
			bih.biCompression = BI_RGB; // uncompressed 24-bit RGB
			bih.biSizeImage = 0; // can be zero for BI_RGB bitmaps
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed = 0;
			bih.biClrImportant = 0;


			// Convert QrCode bits to bmp pixels
			pSourceData = pQRC->data;
			for (y = 0; y < unWidth; y++)
			{
				pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
				for (x = 0; x < unWidth; x++)
				{
					if (*pSourceData & 1)
					{
						for (l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
						{
							for (n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
							{
								*(pDestData + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_B;
								*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_G;
								*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_R;
							}
						}
					}
					pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
					pSourceData++;
				}
			}

			unsigned char* pImgData = new unsigned char[imgWidth * imgWidth * 3];

			int w = unWidth * OUT_FILE_PIXEL_PRESCALER;

			for (y = 0; y < imgWidth; y++)
			{
				for (x = 0; x < imgWidth; x++)
				{

					*(pImgData + (y * imgWidth + x) * 3) = *(pRGBData + ((int)((imgWidth - y - 1) * w / (imgWidth) * (w + 1)) + int(x * w / (imgWidth))) * 3);
					*(pImgData + (y * imgWidth + x) * 3 + 1) = *(pRGBData + ((int)((imgWidth - y - 1) * w / (imgWidth) * (w + 1)) + int(x * w / (imgWidth))) * 3 + 1);
					*(pImgData + (y * imgWidth + x) * 3 + 2) = *(pRGBData + ((int)((imgWidth - y - 1) * w / (imgWidth) * (w + 1)) + int(x * w / (imgWidth))) * 3 + 2);

				}
			}

			// Output the bmp file
			FILE *file;
			file = fopen(argv[1], "wb");

			// Write bitmap file header
			fwrite(&bfh, 1, sizeof(bfh), file);
			fwrite(&bih, 1, sizeof(bih), file);
			//			fwrite(pRGBData, sizeof(unsigned char), unDataBytes, file);
			fwrite(pImgData, sizeof(unsigned char), imgWidth * imgWidth * 3, file);

			fclose(file);


			// Exit normally
			return 0;
		}
		free(pRGBData);
		QRcode_free(pQRC);

	}

}


