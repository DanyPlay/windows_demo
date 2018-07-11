#include "stdafx.h"
#include "rc4.h"

static void swap_byte (unsigned char* a, unsigned char* b) {
	unsigned char bySwap; 
	bySwap = *a;
	*a = *b;
	*b = bySwap; 
}

void RC4(RC4_KEY *key, int length, const void* pachIn, void* pachOut) {
	int i;
	unsigned char byX;
	unsigned char byY;
	unsigned char* pabyState;
	unsigned char byXorIndex;

	const unsigned char* pin  = (const unsigned char*)pachIn;
	unsigned char* pout = (unsigned char*)pachOut;
	if (key == 0)
		return;

	byX = key->byX;
	byY = key->byY;
	pabyState = &key->abyState[0];

	for (i = 0; i < length; i++)
	{
		byX = (byX + 1) % 256;
		byY = (pabyState[byX] + byY) % 256;
		swap_byte(&pabyState[byX], &pabyState[byY]);
		byXorIndex = (pabyState[byX] + pabyState[byY]) % 256;

		if (pin != 0)
			pout[i] = pin[i] ^ pabyState[byXorIndex];
	}
	key->byX = byX;
	key->byY = byY;
}

void RC4_set_key(RC4_KEY* key, int length, void* pachKeyData) {
	int bSkipDiscard = 1;
	int i;
	unsigned char index1;
	unsigned char index2;
	unsigned char* pabyState;
	unsigned char* ptr = (unsigned char*)pachKeyData;
	if (!key) return;
	pabyState= &key->abyState[0];
	for (i = 0; i < 256; i++) {
		pabyState[i] = (unsigned char)i;
	}

	key->byX = 0;
	key->byY = 0;
	index1 = 0;
	index2 = 0;

	for (i = 0; i < 256; i++) {
		index2 = (ptr[index1] + pabyState[i] + index2) % 256;
		swap_byte(&pabyState[i], &pabyState[index2]);
		index1 = (unsigned char)((index1 + 1) % length);
	}
	
	if (!bSkipDiscard) {
		RC4(key, 1024, 0, 0);
	}
}

