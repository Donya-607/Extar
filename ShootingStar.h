#ifndef INCLUDED_SHOOTING_STAR_H_
#define INCLUDED_SHOOTING_STAR_H_

#include <vector>

#include "Common.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

#include "Vector2.h"

namespace ShootingStarImage
{
	void Load();
	void Release();

	int  GetHandle();
}

class VisionStar
{
private:
	int		timer;
	int		handle;
	float	angle;	// Degree, ClockWise

	Vector2	pos;	// Center
public:
	VisionStar() : timer( 0 ), handle( 0 ), angle( 0 ), pos() {}
	~VisionStar() {}

	void Init( int imageHandle, Vector2 pos );
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
public:
	bool IsDisappear() const;
};

class ShootingStar
{
private:
	int		timer;
	float	angle;	// Degree, ClockWise

	Vector2	pos;	// Center
	Vector2 velo;	// Velocity
public:
	ShootingStar() :
		timer( 0 ),
		angle( 0 ),
		pos(), velo()
	{}
	~ShootingStar() {}

	void Init( Vector2 centerPos );
	void Uninit();

	void Update();
	void Draw( Vector2 shake ) const;
public:
	bool IsRequestingGenerateVision() const;
	bool IsDisappear() const;
public:
	int GetHandle() const;
	Vector2 GetPos() const
	{
		return pos;
	}
};

class ShootingStarMng
{
private:
	std::vector<ShootingStar> stars;
	std::vector<VisionStar> visions;

#if USE_IMGUI

#endif // USE_IMGUI

public:
	ShootingStarMng() : stars(), visions() {}
	~ShootingStarMng()
	{
		std::vector<ShootingStar>().swap( stars );
		std::vector<VisionStar>().swap( visions );
	}

	void Init();
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
public:
	void GenerateStar( Vector2 centerPos );
private:
	void GenerateVision( int imageHandle, Vector2 centerPos );
public:
	int  GetArraySize() const
	{
		return scast<int>( stars.size() );
	}
#if USE_IMGUI

#endif // USE_IMGUI
};

#endif //INCLUDED_SHOOTING_STAR_H_
