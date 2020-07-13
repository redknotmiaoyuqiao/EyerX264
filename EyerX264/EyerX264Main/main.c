#include <stdio.h>
#include <stdlib.h>

#include "x264.h"

int main(int argc, const char * argv[]) {
    printf("Hello, World!\n");
    
    int width = 720;
    int height = 1080;
    int csp = X264_CSP_I420;
    
    x264_t * pHandle   = NULL;
    
    x264_param_t* pParam = (x264_param_t*)malloc(sizeof(x264_param_t));
    x264_param_default(pParam);
    pParam->i_width   = width;
    pParam->i_height  = height;
    
    pParam->i_csp = csp;
    x264_param_apply_profile(pParam, x264_profile_names[5]);
    
    pHandle = x264_encoder_open(pParam);
    
    free(pParam);
    
    return 0;
}
