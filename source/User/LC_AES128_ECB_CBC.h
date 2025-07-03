/**
 *	@file		LC_AES128_ECB_CBC.h
 *	@author		Qi Yang
 *	@date		11/23/2020
 *	@version	1.0.0
 */
 
/*!
 *  @defgroup	 LC_AES128_ECB_CBC
 *  @brief
 *  @{*/
#ifndef		LC_AES128_ECB_CBC_H
#define		LC_AES128_ECB_CBC_H
/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef	__cplusplus
	extern "C"	{
#endif
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>		// CBC mode, for memset
#include "LC_Common.h"
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
#define		AES128_KEY_LEN		16

// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// CBC enables AES128 encryption in CBC-mode of operation and handles 0-padding.
// ECB enables the basic ECB 16-byte block algorithm. Both can be enabled simultaneously.

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#ifndef CBC
	#define CBC 1
#endif

#ifndef ECB
	#define ECB 1
#endif

/*------------------------------------------------------------------*/
/*						Data structures								*/
/*------------------------------------------------------------------*/


/*------------------------------------------------------------------*/
/*						external variables							*/
/*------------------------------------------------------------------*/
extern uint8 AES128_MiKey[AES128_KEY_LEN];

/*------------------------------------------------------------------*/
/*						User function prototypes					*/
/*------------------------------------------------------------------*/
#if defined(ECB) && ECB
extern	void		AES128_ECB_encrypt				(uint8_t* input, const uint8_t* key, uint8_t *output);
extern	void		AES128_ECB_decrypt				(uint8_t* input, const uint8_t* key, uint8_t *output);
#endif // #if defined(ECB) && ECB

#if defined(CBC) && CBC
extern	void		AES128_CBC_encrypt_buffer	(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
extern	void		AES128_CBC_decrypt_buffer	(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
#endif // #if defined(CBC) && CBC


#ifdef	__cplusplus
	}
#endif
#endif	/**	LC_AES128_ECB_CBC.h **/
/** @}*/
 

