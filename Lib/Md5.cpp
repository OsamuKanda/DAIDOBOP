// MD5.cpp - CMD5�N���X�p�\�[�X�t�@�C��
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// ���쌠�\��
//-----------------------------------------------------------------------------
/****************************************************************************** 
Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
******************************************************************************/

#include <windows.h>
#include "md5.h"


// MD5Transform���[�`���p�萔
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// F,G,H,I��MD5�̊�{�֐��ł���
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

// ROTATE_LEFT��x������n�r�b�g�z������
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// FF,GG,HH,II�̓��E���h1,2,3,4�p�̕ϊ��֐��ł���
// �z�͍Čv�Z������邽�߉��Z�ƕ�������Ă���
#define FF(a, b, c, d, x, s, ac) {	\
	(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac);	\
	(a) = ROTATE_LEFT ((a), (s));	\
	(a) += (b);	\
	}
#define GG(a, b, c, d, x, s, ac) {	\
	(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac);	\
	(a) = ROTATE_LEFT ((a), (s));	\
	(a) += (b);	\
	}
#define HH(a, b, c, d, x, s, ac) {	\
	(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac);	\
	(a) = ROTATE_LEFT ((a), (s));	\
	(a) += (b);	\
	}
#define II(a, b, c, d, x, s, ac) {	\
	(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac);	\
	(a) = ROTATE_LEFT ((a), (s));	\
	(a) += (b);	\
	}

// MD5�̏������@�V�����R���e�L�X�g����������MD5�̍�Ƃ��n�߂�
void CMD5::Init(void)
{
	m_stContext.count[0] = m_stContext.count[1] = 0;
	
	// �ٗl�Ȗ��͂��������萔����������
	m_stContext.state[0] = 0x67452301;
	m_stContext.state[1] = 0xefcdab89;
	m_stContext.state[2] = 0x98badcfe;
	m_stContext.state[3] = 0x10325476;
}

// MD5�u���b�N�͍�Ƃ��ŐV�̂��̂ɂ���@�ʂ̃��b�Z�[�W�u���b�N��������
// �R���e�L�X�g���ŐV�̂��̂ɂ��Ȃ���MD5���b�Z�[�W�v��A���S���Y���̍�Ƃ𑱂���
void CMD5::Update(unsigned char *input, unsigned int inputLen)
{
	unsigned int i, index, partLen;
	
	// 64��@�Ƃ����o�C�g�����v�Z����
	index = (unsigned int)((m_stContext.count[0] >> 3) & 0x3F);
	
	// �r�b�g�����ŐV�̂��̂ɂ���
	if( ( m_stContext.count[0] += ((UINT4)inputLen << 3) )	< ((UINT4)inputLen << 3) )
		m_stContext.count[1]++;
	m_stContext.count[1] += ((UINT4)inputLen >> 29);
	
	partLen = 64 - index;
	
	// �\�Ȍ��葽���̉񐔁A�ϊ�����
	if( inputLen >= partLen ) 
	{
		memcpy( (POINTER)&m_stContext.buffer[index], (POINTER)input, partLen );
		Transform(m_stContext.state, m_stContext.buffer);
		
		for (i = partLen; i + 63 < inputLen; i += 64)
			Transform(m_stContext.state, &input[i]);
		
		index = 0;
	}
	else
		i = 0;
	
	// ���̓f�[�^��ۂ����o�b�t�@
	memcpy( (POINTER)&m_stContext.buffer[index], (POINTER)&input[i], inputLen-i );
}

// ���b�Z�[�W�v��������R���e�L�X�g���[���ɂ���MD5���b�Z�[�W�v��̍�Ƃ��I����
void CMD5::Final(unsigned char digest[16])
{
	unsigned char bits[8];
	unsigned int index, padLen;
	
	// �r�b�g����ۑ�����
	Encode (bits, m_stContext.count, 8);
	
	// 64��@�Ƃ���56�ɂȂ�悤�ɖ��߂�
	index = (unsigned int)((m_stContext.count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	Update(PADDING, padLen);
	
	// (���ߍ��킹�O��)������t��������
	Update(bits, 8);
	
	// �v��̏�Ԃ�ۑ�����
	Encode (digest, m_stContext.state, 16);
	
	// �f���P�[�g�ȏ��̂Ƃ�����[���ɂ���
	memset( (POINTER)&m_stContext, 0, sizeof(m_stContext) );
}

// MD5��{�ϊ��@�u���b�N�Ɋ�Â���state��ϊ�����B
void CMD5::Transform(UINT4 state[4], unsigned char block[64])
{
	UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	
	Decode (x, block, 64);

	// ���E���h1
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); // 1 
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); // 2 
	FF (c, d, a, b, x[ 2], S13, 0x242070db); // 3 
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); // 4 
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); // 5 
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); // 6 
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); // 7 
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); // 8 
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); // 9 
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); // 10
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); // 11
	FF (b, c, d, a, x[11], S14, 0x895cd7be); // 12
	FF (a, b, c, d, x[12], S11, 0x6b901122); // 13
	FF (d, a, b, c, x[13], S12, 0xfd987193); // 14
	FF (c, d, a, b, x[14], S13, 0xa679438e); // 15
	FF (b, c, d, a, x[15], S14, 0x49b40821); // 16

	// ���E���h2
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); // 17
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); // 18
	GG (c, d, a, b, x[11], S23, 0x265e5a51); // 19
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); // 20
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); // 21
	GG (d, a, b, c, x[10], S22,  0x2441453); // 22
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); // 23
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); // 24
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); // 25
	GG (d, a, b, c, x[14], S22, 0xc33707d6); // 26
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); // 27
	
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); // 28
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); // 29
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); // 30
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); // 31
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); // 32

	// ���E���h3
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); // 33
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); // 34
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); // 35
	HH (b, c, d, a, x[14], S34, 0xfde5380c); // 36
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); // 37
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); // 38
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); // 39
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); // 40
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); // 41
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); // 42
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); // 43
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); // 44
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); // 45
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); // 46
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); // 47
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); // 48

	// ���E���h4
	II (a, b, c, d, x[ 0], S41, 0xf4292244); // 49
	II (d, a, b, c, x[ 7], S42, 0x432aff97); // 50
	II (c, d, a, b, x[14], S43, 0xab9423a7); // 51
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); // 52
	II (a, b, c, d, x[12], S41, 0x655b59c3); // 53
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); // 54
	II (c, d, a, b, x[10], S43, 0xffeff47d); // 55
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); // 56
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); // 57
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); // 58
	II (c, d, a, b, x[ 6], S43, 0xa3014314); // 59
	II (b, c, d, a, x[13], S44, 0x4e0811a1); // 60
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); // 61
	II (d, a, b, c, x[11], S42, 0xbd3af235); // 62
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); // 63
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); // 64

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	// �f���P�[�g�ȏ��̂Ƃ�����[���ɂ���
	memset((POINTER)x, 0, sizeof (x));
}

// ���̓f�[�^(UNIT4)���o�̓f�[�^(unsigned char)�ɃG���R�[�h����
// ������4�̔{���ł���ƌ��Ȃ�
void CMD5::Encode(unsigned char *output, UINT4 *input, unsigned int len)
{
	unsigned int i, j;
	
	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

// ���̓f�[�^(unsigned char)���o�̓f�[�^(UNIT4)�Ƀf�R�[�h����
// ������4�̔{���ł���ƌ��Ȃ�
void CMD5::Decode(UINT4 *output, unsigned char *input, unsigned int len)
{
	unsigned int i, j;
	
	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) |
					(((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24);
	}
}

