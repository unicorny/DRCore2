#ifndef __DR_CORE2_TYPES_H
#define __DR_CORE2_TYPES_H

#include <stdint.h>

/*
* \author einhornimmond
* \date 21.04.2024
* \brief DRCore2 specific type definitions
*/

// typedef to able to increase lib accuracy by swap out float with double later
typedef float DRReal;

// default return types used from many of the functions
enum DRReturn
{
	DR_OK = 0,
	DR_ERROR = 1,
	DR_ZERO_POINTER = 2,
	DR_NOT_ERROR = 3,
	DR_RENDER = 4
};

//Makros um etwas schneller zu löschen
#define DR_SAVE_DELETE(x) {if(x){delete x; x = NULL;}}
#define DR_SAVE_DELETE_ARRAY(x) {if(x){delete [] x; x = NULL;}}

#ifndef u8
#define u8 uint8_t
#endif
#ifndef s8
#define s8 int8_t
#endif
#ifndef u16
#define u16 uint16_t
#endif
#ifndef s16
#define s16 int16_t
#endif
#ifndef u32
#define u32 uint32_t
#endif
#ifndef s32
#define s32 int32_t
#endif
#ifndef u64
#define u64 uint64_t
#endif
#ifndef s64
#define s64 int64_t
#endif

typedef unsigned int HASH;
typedef unsigned long DHASH;

#endif //__DR_CORE2_TYPES_H