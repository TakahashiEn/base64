//
//  main.c
//  base64Test
//
//  Created by Himari Takakura on 3/26/20.
//  Copyright © 2020 Himari Takakura. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"



int main(int argc, const char * argv[]) {
    
    uint32_t dst_len;
    char *dst = base64_encode((char *)argv[1], (uint32_t)strlen(argv[1]), &dst_len);;
    
    
    printf("src_len: %d\n", (int)strlen(argv[1]));
    
    printf("src: %s\n", argv[1]);
    
    printf("dst_len: %d\n", dst_len);
   
    printf("dst: ");
    str_print(dst, dst_len);
    
    uint32_t t_dst_len;
    char *t_dst = base64_decode(dst, dst_len, &t_dst_len);
    
    
    printf("t_dst_len: %d\n", t_dst_len);
    
    printf("t_dst: ");
    str_print(t_dst, t_dst_len);
    
    free(dst);
    return 0;
}
