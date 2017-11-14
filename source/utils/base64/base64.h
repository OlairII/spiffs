/*
 * base64.h
 *
 *  Created on: Nov 14, 2017
 *      Author: houz03
 */

#ifndef SOURCE_UTILS_BASE64_BASE64_H_
#define SOURCE_UTILS_BASE64_BASE64_H_


char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);
unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);
void build_decoding_table();
void base64_cleanup();


#endif /* SOURCE_UTILS_BASE64_BASE64_H_ */
