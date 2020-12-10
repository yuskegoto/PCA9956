// i2c addresses
#define DEV_ADDRESS1 0x3D
#define DEV_ADDRESS2 0x3F
#define DEV_ADDRESS3 0x40
#define DEV_ADDRESS4 0x41
#define DEV_ADDRESS5 0x42
#define DEV_ADDRESS6 0x43
#define DEV_ADDRESS7 0x44
#define DEV_ADDRESS8 0x45
#define DEV_ADDRESS9 0x46
#define DEV_ADDRESS10 0x47

const uint8_t DEV1_Groupe[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1};
const uint8_t DEV1_ledNo[] =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

const uint8_t DEV2_Groupe[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2, 2,   2, 3, 3, 3, 3, 3, 3};
const uint8_t DEV2_ledNo[] =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 1, 2, 3, 4, 5};

// const uint8_t DEV3_Groupe[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, 5, 5, 5, 5};
// const uint8_t DEV3_ledNo[] =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 1, 2, 3};
// const uint8_t DEV4_Groupe[] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6};
// const uint8_t DEV5_Groupe[] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
// const uint8_t DEV6_Groupe[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
// const uint8_t DEV7_Groupe[] = {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};
// const uint8_t DEV8_Groupe[] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
// const uint8_t DEV9_Groupe[] = {11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11};
// const uint8_t DEV10_Groupe[] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12};

// led control settings
#define LED_BRIGHTNESS 0x0F
