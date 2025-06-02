#ifndef MODE7_TABLES_H_
# define MODE7_TABLES_H_               202505L

# include <stdint.h>

# if defined(__cplusplus)
extern "C" {
# endif

extern const uint8_t MODE7_ReverseByteTab[256];
extern const uint8_t MODE7_OddParityTable[128];
extern const uint8_t MODE7_Hamming8EncodeTable[16];
extern const uint8_t MODE7_Hamming8DecodeTable[256];
extern const uint8_t MODE7_Hamming24EncodeTable0[256];
extern const uint8_t MODE7_Hamming24EncodeTable1[256];
extern const uint8_t MODE7_Hamming24EncodeTable2[4];
extern const uint8_t MODE7_Hamming24ParityTable[3][256];

# if defined(__cplusplus)
};
# endif

#endif /*!MODE7_TABLES_H_*/
