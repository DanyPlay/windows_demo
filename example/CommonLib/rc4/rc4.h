
#ifndef __WINDMILL__RC4__H__
#define __WINDMILL__RC4__H__


#ifdef __cplusplus
extern "C" {
#endif

typedef struct RC4_KEY{
    unsigned char abyState[256];
    unsigned char byX;
    unsigned char byY;
} RC4_KEY;

void RC4(RC4_KEY *key, int length, const void* pachIn, void* pachOut);
void RC4_set_key(RC4_KEY* key, int length, void * pachKeyData);

#ifdef __cplusplus
}
#endif

#endif /* __WINDMILL__RC4__H__ */

