#ifndef MD55_H
#define MD55_H

#include <map>
#include <fstream>

/* Type define */
typedef unsigned char byte;

using std::string;
using std::ifstream;

/* MD55 declaration. */
class MD55 {
public:
	MD55();
	MD55(const void* input, size_t length);
	MD55(const string& str);
	MD55(ifstream& in);
	void update(const void* input, size_t length);
	void update(const string& str);
	void update(ifstream& in);
	const byte* digest();
	string toString();
//	CString GetStrMD55(CString str);
	void reset();

private:
	void update(const byte* input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const unsigned int* input, byte* output, size_t length);
	void decode(const byte* input, unsigned int* output, size_t length);
	string bytesToHexString(const byte* input, size_t length);

	/* class uncopyable */
	MD55(const MD55&);
	MD55& operator=(const MD55&);

private:
	unsigned int _state[4];	/* state (ABCD) */
	unsigned int _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	byte _buffer[64];	/* input buffer */
	byte _digest[16];	/* message digest */
	bool _finished;		/* calculate finished ? */

	static const byte PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	enum { BUFFER_SIZE = 1024 };
};

#endif /*MD55_H*/
