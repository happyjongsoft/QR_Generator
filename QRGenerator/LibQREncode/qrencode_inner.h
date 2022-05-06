#ifndef __QRENCODE_INNER_H__
#define __QRENCODE_INNER_H__

/**
 * This header file includes definitions for test use.
 */

/******************************************************************************
 * Raw code
 *****************************************************************************/

typedef struct {
	int dataLength;
	unsigned char *data;
	int eccLength;
	unsigned char *ecc;
} RSblock;

typedef struct {
	int version;
	int dataLength;
	int eccLength;
	unsigned char *datacode;
	unsigned char *ecccode;
	int b1;
	int blocks;
	RSblock *rsblock;
	int count;
} QRRawCode;

extern QRRawCode *QRraw_new(QRinput *input);
extern unsigned char QRraw_getCode(QRRawCode *raw);
extern void QRraw_free(QRRawCode *raw);

/******************************************************************************
 * Raw code for Micro QR Code
 *****************************************************************************/

typedef struct {
	int version;
	int dataLength;
	int eccLength;
	unsigned char *datacode;
	unsigned char *ecccode;
	RSblock *rsblock;
	int oddbits;
	int count;
} MQRRawCode;

extern MQRRawCode *MQRraw_new(QRinput *input);
extern unsigned char MQRraw_getCode(MQRRawCode *raw);
extern void MQRraw_free(MQRRawCode *raw);

/******************************************************************************
 * Frame filling
 *****************************************************************************/
extern unsigned char *FrameFiller_test(int version);
extern unsigned char *FrameFiller_testMQR(int version);

/******************************************************************************
 * QR-code encoding
 *****************************************************************************/
extern QRcode *QRcode_encodeMask(QRinput *input, int mask);
extern QRcode *QRcode_encodeMaskMQR(QRinput *input, int mask);
extern QRcode *QRcode_new(int version, int width, unsigned char *data);

#endif /* __QRENCODE_INNER_H__ */
