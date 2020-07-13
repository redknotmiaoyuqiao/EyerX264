#include <stdio.h>
#include <stdlib.h>

#include "x264.h"

int main(int argc, const char * argv[]) {
    int width = 640;
    int height = 368;
    int csp = X264_CSP_I420;
    char * yuvPath = "/Users/lichi/Desktop/ads.yuv";
    char * h264Path = "/Users/lichi/Desktop/ads.h264";
    
    FILE * yuvF = fopen(yuvPath, "rb");
    FILE * h264F = fopen(h264Path, "wb");
    
    x264_t * pHandle = NULL;
    
    x264_param_t* pParam = (x264_param_t*)malloc(sizeof(x264_param_t));
    x264_param_default(pParam);
    pParam->i_width   = width;
    pParam->i_height  = height;
    pParam->i_csp = csp;
    x264_param_apply_profile(pParam, x264_profile_names[5]);
    
    pHandle = x264_encoder_open(pParam);
    
    x264_nal_t * pNals = NULL;
    int iNal = 0;
    x264_picture_t* pPic_in = (x264_picture_t*)malloc(sizeof(x264_picture_t));
    x264_picture_t* pPic_out = (x264_picture_t*)malloc(sizeof(x264_picture_t));
    
    x264_picture_init(pPic_out);
    x264_picture_alloc(pPic_in, csp, pParam->i_width, pParam->i_height);
    
    int frameIndex = 0;
    while(1){
        if(yuvF == NULL){
            break;
        }
        
        int ret = 0;
        
        int y_size = width * height;
        ret = fread(pPic_in->img.plane[0], y_size, 1, yuvF);       //Y
        ret = fread(pPic_in->img.plane[1], y_size/4, 1, yuvF);     //U
        ret = fread(pPic_in->img.plane[2], y_size/4, 1, yuvF);     //V
        
        pPic_in->i_pts = frameIndex;
        
        if(ret <= 0){
            break;
        }
        
        ret = x264_encoder_encode(pHandle, &pNals, &iNal, pPic_in, pPic_out);
        
        frameIndex++;
        
        for (int j = 0; j < iNal; j++){
            fwrite(pNals[j].p_payload, 1, pNals[j].i_payload, h264F);
        }
    }
    
    //flush encoder
    while(1){
       int ret = x264_encoder_encode(pHandle, &pNals, &iNal, NULL, pPic_out);
       if(ret == 0){
            break;
       }
       printf("Flush 1 frame.\n");
       for (int j = 0; j < iNal; ++j){
           fwrite(pNals[j].p_payload, 1, pNals[j].i_payload, h264F);
       }
       frameIndex++;
     }
    
    
    
    if(pHandle != NULL){
        x264_encoder_close(pHandle);
        pHandle = NULL;
    }
    
    free(pPic_in);
    free(pPic_out);
    
    if(pParam != NULL){
        free(pParam);
        pParam = NULL;
    }
    
    if(yuvF != NULL){
        fclose(yuvF);
        yuvF = NULL;
    }
    
    if(h264F != NULL){
        fclose(h264F);
        h264F = NULL;
    }
    
    return 0;
}
