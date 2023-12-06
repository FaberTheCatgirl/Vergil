#pragma once

//
// Common Padding Definitions
//

#define PAD8 char : 8;

#define PAD16 short : 16;

#define PAD24 char : 8; short : 16;

#define PAD32 int : 32;

#define PAD64 int : 64; //64 Bit MCC Padding; Todo: Check if the 64 bit padding can be translated into HO's 32 bit EXE