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

#define		DEBUG_MODE			( 1 )	/* DEBUG:�f�o�b�O���[�h */
#define		USE_IMGUI			( 1 )	/* DEBUG:ImGui���g���Ȃ�C�I���ɂ��� */
#define		USE_DIRECT_INPUT	( 0 )	/* DEBUG:DirectInput�R���g���[�����g���Ȃ�C�I���ɂ��� */

extern const int SCREEN_WIDTH;			// ��ʂ̑傫�� X 1920
extern const int SCREEN_HEIGHT;			// ��ʂ̑傫�� Y 1080

#endif //INCLUDED_COMMON_H_
