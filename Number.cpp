#include <assert.h>

#include "DxLib.h"
#include "Number.h"

namespace Number
{
	static int hOriginal = -1;
	static int hNumbers[10];
	static int hDarkNumbers[10];

	void Load()
	{
		// ‚·‚Å‚É’l‚ª“ü‚Á‚Ä‚¢‚½‚çC“Ç‚İ‚ñ‚¾‚à‚Ì‚Æ‚İ‚È‚µ‚Ä”ò‚Î‚·
		if ( 0 != hNumbers[0] ) { return; }
		// else

		hOriginal = LoadGraph( "./Data/Images/UI/Number.png" );

		for ( int i = 0; i < 10; ++i )
		{
			hNumbers[i] = DerivationGraph
			(
				i * SIZE_X, 0,
				SIZE_X, SIZE_Y,
				hOriginal
			);
			hDarkNumbers[i] = DerivationGraph
			(
				i * SIZE_X, SIZE_Y,
				SIZE_X, SIZE_Y,
				hOriginal
			);
		}
	}
	void Release()
	{
		DeleteGraph( hOriginal );
		hOriginal = 0;

		for ( int i = 0; i < 10; i++ )
		{
			DeleteGraph( hNumbers[i]	 );
			DeleteGraph( hDarkNumbers[i] );
			hNumbers[i]		= 0;
			hDarkNumbers[i] = 0;
		}
	}

	int  GetHandle( int num, bool isGlow )
	{
		assert( 0 <= num && num <= 9 );

		if ( isGlow )
		{
			return hNumbers[num];
		}
		// else
		return hDarkNumbers[num];
	}
}