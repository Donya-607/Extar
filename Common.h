#ifndef INCLUDED_COMMON_H_
#define INCLUDED_COMMON_H_

//--------------------
//
//		Common.h
//
//--------------------

#include	<assert.h>

#define		scast				static_cast
constexpr	float PI		=	3.14159265359f;
constexpr	float EPSILON	=	1.192092896e-07F;	// smallest such that 1.0+FLT_EPSILON != 1.0

#define		ArraySize( x )		( sizeof( x ) / ( sizeof( x[0] ) ) )
#define		ToRadian( x )		( x * ( PI / 180.0f ) )
#define		ToDegree( x )		( x * ( 180.0f / PI ) )
#define		ZeroEqual( x )		( -EPSILON < x && x < EPSILON )

#define		DEBUG_MODE			( 1 )	/* DEBUG:デバッグモード */
#define		USE_IMGUI			( 1 )	/* DEBUG:ImGuiを使うなら，オンにする */
#define		USE_DIRECT_INPUT	( 0 )	/* DEBUG:DirectInputコントローラを使うなら，オンにする */

extern const int SCREEN_WIDTH;			// 画面の大きさ X 1920
extern const int SCREEN_HEIGHT;			// 画面の大きさ Y 1080

extern const int FRAME_WIDTH;			// 使用枠の大きさ X 1536
extern const int FRAME_HEIGHT;			// 使用枠の大きさ Y 768

extern const int FRAME_POS_X;			// 使用枠の左上座標 X 192 描画でのみ使用すればよい
extern const int FRAME_POS_Y;			// 使用枠の左上座標 Y 64  描画でのみ使用すればよい

#endif //INCLUDED_COMMON_H_
