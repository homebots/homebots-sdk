#ifndef _ESP_SHA_
#define _ESP_SHA_

typedef struct
{
  uint32_t state[5];
  uint32_t count[2];
  unsigned char buffer[64];
} SHA1_CTX;

void MOVE_TO_FLASH SHA1Transform(uint32_t state[5], const unsigned char buffer[64]);
void MOVE_TO_FLASH SHA1Init(SHA1_CTX *context);
void MOVE_TO_FLASH SHA1Update(SHA1_CTX *context, const unsigned char *data, uint32_t len);
void MOVE_TO_FLASH SHA1Final(unsigned char digest[20], SHA1_CTX *context);

#endif
