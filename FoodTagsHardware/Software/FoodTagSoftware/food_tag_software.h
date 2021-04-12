//
//  food_tag_software.h
//  FoodTagSoftware
//
//  Created by Erik Coltey on 4/11/21.
//  Copyright 2021 Erik Coltey. All rights reserved.
//

#ifndef FOOD_TAG_SOFTWARE_H_
#define FOOD_TAG_SOFTWARE_H_

#define TAG_TEMP_ILLUM_DEFAULT_DATA      {                                              \
/*NDEF Tag Application Name*/                                                           \
0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01,                                               \
                                                                                        \
/*Capability Container ID*/                                                             \
0xE1, 0x03,                                                                             \
0x00, 0x0F, /* CCLEN */                                                                 \
0x20,       /* Mapping version 2.0 */                                                   \
0x00, 0xF9, /* MLe (49 bytes); Maximum R-APDU data size */                              \
0x00, 0xF6, /* MLc (52 bytes); Maximum C-APDU data size */                              \
0x04,       /* Tag, File Control TLV (4 = NDEF file) */                                 \
0x06,       /* Length, File Control TLV (6 = 6 bytes of data for this tag) */           \
0xE1, 0x04, /* File Identifier */                                                       \
0x0B, 0xDF, /* Max NDEF size (3037 bytes of useable memory) */                          \
0x00,       /* NDEF file read access condition, read access without any security */     \
0x00,       /* NDEF file write access condition; write access without any security */   \
                                                                                        \
/* NDEF File ID */                                                                      \
0xE1, 0x04,                                                                             \
/* (26 bytes) Initial Header End */                                                     \
/* NDEF File for tag temp illum */                                                      \
0x00, 152, /* NLEN: NDEF length (255 byte long message, max. length for RF430CL) */     \
                                                                                        \
/* NDEF Record (refer to NFC Data Exchange Format specifications)*/                     \
0xD1,       /*MB(Message Begin), SR(Short Record) flags set, ME(Message End), IL(ID length field present) flags cleared; TNF(3bits) = 1; */ \
0x01, 148, /*Type Length = 0x01; Payload Length = 251 */                                \
0x54,       /* Type = T (text) */                                                       \
                                                                                        \
/* (32 bytes) Message + Record Header End */                                            \
                                                                                        \
/* NDEF data First 4 bytes = product ID, rest = 24 measurements @ 6 bytes each. */      \
0xFF, 0xFF, 0xFF, 0xFF,                                                                 \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                     \
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF                                                      \
    } /* End of data */                                                                 \

#endif /* FOOD_TAG_SOFTWARE_H_ */
