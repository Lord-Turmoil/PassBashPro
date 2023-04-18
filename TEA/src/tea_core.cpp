/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : TEA                                       *
 *                                                                            *
 *                      File Name : tea_core.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : January 16, 2023                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Core algorithm for TEA algorithm.                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community Preview                                     *
 ******************************************************************************/

#include "../inc/tea_core.h"
#include "../inc/tea_util.h"


/********************************************************************
** Here, a strengthened version of TEA called XTEA is implemented.
*/

_TEA_BEGIN

static const DATA TEA_DELTA = 0x9E3779B9;
static const DATA TEA_SUM   = 0xC6EF3720;

void encipher(const DATA* const v, DATA* const w, const DATA* const k)
{
	static_assert(sizeof(DATA) == 4, "Size of DATA wrong for TEA");

	DATA y = v[0];
	DATA z = v[1];
	DATA sum = 0;

	for (DATA n = 32; n > 0; n--)
	{
		y += ((z << 4) ^ (z >> 5)) + z ^ sum + k[sum & 3];
		sum += TEA_DELTA;
		z += ((y << 4) ^ (y >> 5)) + y ^ sum + k[(sum >> 11) & 3];
	}
	w[0] = y;
	w[1] = z;
}

void decipher(const DATA* const v, DATA* const w, const DATA* const k)
{
	static_assert(sizeof(DATA) == 4, "Size of DATA wrong for TEA");

	DATA y = v[0];
	DATA z = v[1];
	DATA sum = TEA_SUM;

	for (DATA n = 32; n > 0; n--)
	{
		z -= ((y << 4) ^ (y >> 5)) + y ^ sum + k[(sum >> 11) & 3];
		sum -= TEA_DELTA;
		y -= ((z << 4) ^ (z >> 5)) + z ^ sum + k[sum & 3];
	}
	w[0] = y;
	w[1] = z;
}


void encode(TEAReader* input, TEAWriter* output, const char* key)
{
	DATA outptr[2];
	char inbuf[NCHAR];
	DATA* inptr = reinterpret_cast<DATA*>(inbuf);
	const DATA* k = reinterpret_cast<const DATA*>(key);

	while (input->Read(inbuf, NCHAR))
	{
		encipher(inptr, outptr, k);
		output->Write((char*)outptr, KCHAR);
	}
}

void decode(TEAReader* input, TEAWriter* output, const char* key)
{
	DATA inptr[2];
	char outbuf[NCHAR + 1];
	DATA* outptr = reinterpret_cast<DATA*>(outbuf);
	const DATA* k = reinterpret_cast<const DATA*>(key);

	outbuf[NCHAR] = '\0';
	while (input->Read((char*)inptr, KCHAR))
	{
		decipher(inptr, outptr, k);
		output->Write((char*)outptr, NCHAR);
	}
}

_TEA_END
