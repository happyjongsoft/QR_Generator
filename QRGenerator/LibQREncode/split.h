#ifndef __SPLIT_H__
#define __SPLIT_H__

#include "qrencode.h"

/**
 * Split the input string (null terminated) into QRinput.
 * @param string input string
 * @param hint give QR_MODE_KANJI if the input string contains Kanji character encoded in Shift-JIS. If not, give QR_MODE_8.
 * @param casesensitive 0 for case-insensitive encoding (all alphabet characters are replaced to UPPER-CASE CHARACTERS.
 * @retval 0 success.
 * @retval -1 an error occurred. errno is set to indicate the error. See
 *               Exceptions for the details.
 * @throw EINVAL invalid input object.
 * @throw ENOMEM unable to allocate memory for input objects.
 */
extern int Split_splitStringToQRinput(const char *string, QRinput *input,
		QRencodeMode hint, int casesensitive);

#endif /* __SPLIT_H__ */
