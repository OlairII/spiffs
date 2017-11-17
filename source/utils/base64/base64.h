/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef SOURCE_UTILS_BASE64_BASE64_H_
#define SOURCE_UTILS_BASE64_BASE64_H_


unsigned char * base64_encode(const unsigned char *src, size_t len, size_t *out_len);


#endif /* SOURCE_UTILS_BASE64_BASE64_H_ */
