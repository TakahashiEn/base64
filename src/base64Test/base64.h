//
//  base64.h
//  base64Test
//
//  Created by Himari Takakura on 3/26/20.
//  Copyright © 2020 Himari Takakura. All rights reserved.
//

#ifndef base64_h
#define base64_h

#include <stdlib.h>
#include <string.h>

#define base64_str_len 64

static inline void str_print(char *str, uint32_t len){
    
    if(len <=0) printf("ERROR! STRING LEN < 0 !\n");
    for(int i=0; i<len; i++){
        printf("%c", str[i]);
    }
    printf("\n");
    
}

const char *base64_encode_lib = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


// use ascii as the index of array
const uint8_t base64_decode_lib[256] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 255,
    255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 253, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255,
    
    // '+'
    62,
    255, 255, 255,
    
    // '/'
    63,
    
    // 0 - 9
    52,  53,  54,  55,  56,  57,  58,  59,  60,  61,
    255, 255,255, 254, 255, 255, 255,
    
    // A - Z
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,
    12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,
    24,  25,
    255, 255, 255, 255, 255,255,
    
    // a - z
    26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
    37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,
    
    255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255
};

// 3 to 4
static inline void base64_group_encode(const char *src, char *dst){
    
    dst[0] = base64_encode_lib[ (uint8_t)src[0] >> 2 ];
    
    // 注意 + - 的优先级 比 & ｜ 要高！！！！！！！！！！！！
    dst[1] = base64_encode_lib[
                               (((uint8_t)src[0] << 4)&0x30) +
                               (((uint8_t)src[1] >> 4)&0x0f) ];
    
    dst[2] = base64_encode_lib[
                               (((uint8_t)src[1] << 2)&0x3c) +
                               (((uint8_t)src[2] >> 6)&0x03) ];
    
    dst[3] = base64_encode_lib[ ((uint8_t)src[2])&0x3f ];
    
}

static inline void base64_group_decode(char *src, char *dst){
    
    dst[0] = (char)(
                    (((base64_decode_lib[(uint8_t)src[0]]) << 2)&0xfc) +
                    (((base64_decode_lib[(uint8_t)src[1]]) >> 4)&0x03) );
    
    dst[1] = (char)(
                    (((base64_decode_lib[(uint8_t)src[1]]) << 4)&0xf0) +
                    (((base64_decode_lib[(uint8_t)src[2]]) >> 2)&0x0f) );
    
    dst[2] = (char)(
                    (((base64_decode_lib[(uint8_t)src[2]]) << 6)&0xc0) +
                    (((base64_decode_lib[(uint8_t)src[3]])     )&0x3f) );
    
}

char *base64_encode(char *src_str, uint32_t src_len, uint32_t *dst_len_ptr){
    
    if(src_len <= 0) return NULL;
    
    char *dst_str;
    int i;
    uint8_t rest = ((src_len) % 3);
    uint32_t count = (src_len - rest) / 3;
    
    *dst_len_ptr = (count + (rest - rest/2)) * 4;
    
    dst_str = (char *)calloc(*dst_len_ptr, sizeof(char));
    
    for(i=0; i<count; i++){
        base64_group_encode(&src_str[i * 3], &dst_str[i * 4]);
    }
    
    char *t_src = (char *)calloc(3, sizeof(char));
    memcpy(t_src, &src_str[i * 3], rest);
    base64_group_encode(t_src, &dst_str[i * 4]);
    dst_str[ i*4 + ( (rest + 1) - rest/3 ) ] = '=';
    dst_str[ i*4 + ( (rest + 2) - (rest+1)/3 ) ] = '=';
    
    return dst_str;
    
}

char *base64_decode(char *src_str, uint32_t src_len, uint32_t *dst_len_ptr){
    
    //
    
    if((src_len%4) != 0) return NULL;
    uint32_t count = src_len/4;
    *dst_len_ptr = count * 3;
    
    char *dst_str = (char *)calloc(*dst_len_ptr, sizeof(char));
    
    for (int i=0; i<count; i++) {
        base64_group_decode(&src_str[i * 4], &dst_str[i * 3]);
    }
    
    *dst_len_ptr = *dst_len_ptr - (src_str[src_len-1] == '=');

    *dst_len_ptr = *dst_len_ptr - (src_str[src_len-2] == '=');
    
    return dst_str;
}

#endif /* base64_h */
