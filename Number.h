#ifndef INCLUDED_NUMBER_H_
#define INCLUDED_NUMBER_H_

namespace Number
{
	constexpr int SIZE_X = 64;
	constexpr int SIZE_Y = 96;

	void Load();
	void Release();

	int  GetHandle( int num, bool isGlow );
}

#endif //INCLUDED_NUMBER_H_
