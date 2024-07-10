// MD5.H - CMD5�N���X�p�w�b�_�t�@�C��
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

// POINTER��ėp�|�C���^�^�ƒ�`����
typedef unsigned char *POINTER;

// UINT2��2�o�C�g���[�h�ƒ�`����
typedef unsigned short int UINT2;

// UINT4��4�o�C�g���[�h�ƒ�`����
typedef unsigned long int UINT4;

// MD5�R���e�L�X�g�\����
typedef struct
{
	UINT4	state[4];			// ���(ABCD)
	UINT4	count[2];			// 2^64��@�Ƃ����r�b�g��(���ʃr�b�g����)
	unsigned char buffer[64];	// ���̓f�[�^�̃o�b�t�@
}MD5_CTX;

// MD5�N���X
class CMD5
{
// public member function.
public:
	CMD5(){};
	virtual ~CMD5(){};

	// MD5�̏�����
	void Init(void);

	// MD5�u���b�N�͍�Ƃ��ŐV�̂��̂ɂ���
	void Update(unsigned char *input, unsigned int inputLen);

	// MD5���b�Z�[�W�v��̍�Ƃ��I����
	void Final(unsigned char digest[16]);

// protected member function.
protected:
	// MD5��{�ϊ��@�u���b�N�Ɋ�Â���state��ϊ�����B
	void Transform(UINT4 state[4], unsigned char block[64]);

	// ���̓f�[�^(UNIT4)���o�̓f�[�^(unsigned char)�ɃG���R�[�h����
	void Encode(unsigned char *output, UINT4 *input, unsigned int len);

	// ���̓f�[�^(unsigned char)���o�̓f�[�^(UNIT4)�Ƀf�R�[�h����
	void Decode(UINT4 *output, unsigned char *input, unsigned int len);

// protected member variable.
protected:
	MD5_CTX		m_stContext;		// MD5�R���e�L�X�g
};
