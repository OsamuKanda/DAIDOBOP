// MD5.H - CMD5クラス用ヘッダファイル
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 著作権表示
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

// POINTERを汎用ポインタ型と定義する
typedef unsigned char *POINTER;

// UINT2を2バイトワードと定義する
typedef unsigned short int UINT2;

// UINT4を4バイトワードと定義する
typedef unsigned long int UINT4;

// MD5コンテキスト構造体
typedef struct
{
	UINT4	state[4];			// 状態(ABCD)
	UINT4	count[2];			// 2^64を法としたビット数(下位ビットが先)
	unsigned char buffer[64];	// 入力データのバッファ
}MD5_CTX;

// MD5クラス
class CMD5
{
// public member function.
public:
	CMD5(){};
	virtual ~CMD5(){};

	// MD5の初期化
	void Init(void);

	// MD5ブロックは作業を最新のものにする
	void Update(unsigned char *input, unsigned int inputLen);

	// MD5メッセージ要約の作業を終える
	void Final(unsigned char digest[16]);

// protected member function.
protected:
	// MD5基本変換　ブロックに基づいてstateを変換する。
	void Transform(UINT4 state[4], unsigned char block[64]);

	// 入力データ(UNIT4)を出力データ(unsigned char)にエンコードする
	void Encode(unsigned char *output, UINT4 *input, unsigned int len);

	// 入力データ(unsigned char)を出力データ(UNIT4)にデコードする
	void Decode(UINT4 *output, unsigned char *input, unsigned int len);

// protected member variable.
protected:
	MD5_CTX		m_stContext;		// MD5コンテキスト
};
