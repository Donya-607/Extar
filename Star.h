#ifndef INCLUDED_STAR_H_
#define INCLUDED_STAR_H_

#include <vector>

#include "Common.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

#include "Animation.h"
#include "Collision.h"
#include "Vector2.h"

#include "Grid.h"

namespace StarImage
{
	constexpr int SIZE = 96;

	void Load();
	void Release();

	int  GetHandle( int level, int animIndex );
	int  GetGradeHandle();
}

class Star
{
public:
	static constexpr int MAX_LEVEL = 6;
private:
	int row;	// �s, 0�n�܂�
	int column;	// ��, 0�n�܂�

	int width;	// �}�X�ڎ��̑傫��, 1�n�܂�
	int height;	// �}�X�ڎ��̑傫��, 1�n�܂�

	int level;	// 1 ~ 6

	float angle;// Degree, ClockWise

	Anim anim;
private:	// ���o�̉�]�Ɏg���ϐ��Q
	float radian;
	float easeFactor;	// 0.0f ~ 1.0f
	bool  nowRotate;	// ��]���ɉ�]��������̂�h������
private:	// ���o�f�ɂ����
	int val;
	int val_state;
	int val_timer;
public:
	Star() : row( 0 ), column( 0 ), width( 1 ), height( 1 ),
		level( 6 ),
		angle( 0 ),
		anim(),

		radian( 0 ), easeFactor( 0 ), nowRotate( false ),

		val( 0 ),
		val_state( 0 ),
		val_timer( 0 )

	{}
	~Star() {}

	void Init( int row, int column, int width, int height, int level, bool flagSetAnimeOnly = false );
	void Uninit();

	void Update();

	void EffectRelatedUpdate();

	// �`�掞�̐��̊p�x�����x���ɂ��v�Z����iangle�ϐ���ύX�j
	void CalcRotate();

	void Draw( Vector2 shake ) const;
public:
	Box  FetchColWorldPos() const
	{
		Vector2 halfSize;
		halfSize.x = ( ( width  * Grid::GetSize().x ) * 0.5f );
		halfSize.y = ( ( height * Grid::GetSize().y ) * 0.5f );

		Box tmp =
		{
			scast<float>( FRAME_POS_X + ( row		* Grid::GetSize().x ) ) + halfSize.x,
			scast<float>( FRAME_POS_Y + ( column	* Grid::GetSize().y ) ) + halfSize.y,
			halfSize.x - 1/* �Ӑ}���Ȃ��d�Ȃ��h������*/,
			halfSize.y - 1/* �Ӑ}���Ȃ��d�Ȃ��h������*/,
			true
		};

		return tmp;
	}
	int  GetLevel() const
	{
		return level;
	}

	void BeExposed();

	// ������]������iradian�ϐ���ύX�j
	void Rotate();

	void AcquireData( int *Row, int *Column, int *Width, int *Height, int *Level ) const
	{
		if ( nullptr != Row		)	{ *Row		= row;		}
		if ( nullptr != Column	)	{ *Column	= column;	}
		if ( nullptr != Width	)	{ *Width	= width;	}
		if ( nullptr != Height	)	{ *Height	= height;	}
		if ( nullptr != Level	)	{ *Level	= level;	}
	}
	void SetData( int Row, int Column, int Width, int Height, int Level )
	{
		if ( NULL != Row	) { row		= Row;		}
		if ( NULL != Column	) { column	= Column;	}
		if ( NULL != Width	) { width	= Width;	}
		if ( NULL != Height	) { height	= Height;	}
		if ( NULL != Level	) { level	= Level;	}
	}
private:
	void RotateUpdate();

#if DEBUG_MODE
public:
	void SetLevelDebug( int newLevel )
	{
		level = newLevel;
		CalcRotate();
	}
#endif // DEBUG_MODE
};

class StarMng
{
private:
	std::vector<Star> stars;

	std::vector<std::vector<int>> levelStorage;	// Ctrl+Z�ɂĎg�p

#if USE_IMGUI

	int choiseRow;		// 0�n�܂�
	int choiseColumn;	// 0�n�܂�
	int width;			// 1�n�܂�
	int height;			// 1�n�܂�
	int level;			// 1�n�܂�

#endif // USE_IMGUI

public:
	StarMng() : stars(), levelStorage(){}
	~StarMng()
	{
		std::vector<Star>().swap( stars );
		std::vector<std::vector<int>>().swap( levelStorage );
	}

	void Init( int stageNumber );
	void Uninit();

	void Update();
	void ClearUpdate();

	void Draw( Vector2 shake ) const;
public:
	int  GetArraySize() const
	{
		return scast<int>( stars.size() );
	}
	Box  FetchColWorldPos( int index ) const
	{
		assert( index < scast<int>( stars.size() ) );

		return stars.at( index ).FetchColWorldPos();
	}
	int  FetchLevel( int index ) const
	{
		assert( index < scast<int>( stars.size() ) );

		return stars.at( index ).GetLevel();
	}
	void Rotate( int index )
	{
		assert( index < scast<int>( stars.size() ) );

		stars.at( index ).Rotate();
	}

	void SaveLog();
	bool Undo();	// ����������TRUE

	void Expose( int index )
	{
		assert( index < scast<int>( stars.size() ) );

		stars.at( index ).BeExposed();
	}

	bool IsEqualLevels() const;
private:
	bool CanSaveLog() const
	{
		// �Z�[�u�ł��Ȃ��󋵂�����Ǝv���Ă������C�����ł��Ȃ������H
		if ( scast<int>( stars.size() ) == scast<int>( levelStorage.size() ) )
		{
			// return false;
		}
		// else
		return true;
	}
	bool CanUndo() const
	{
		if ( scast<int>( levelStorage.size() ) )
		{
			return true;
		}
		// else
		return false;
	}
#if USE_IMGUI

	void ChangeParametersByImGui();

	void SetStar();
	void RemoveStar();

	void DrawUI() const;
public:
	void SaveData();

#endif // USE_IMGUI

#if DEBUG_MODE
public:
	void AlignLevelsDebug( int newLevel = 1 )
	{
		for ( auto &it : stars )
		{
			it.SetLevelDebug( newLevel );
		}
	}
#endif // DEBUG_MODE
};

#endif //INCLUDED_STAR_H_
