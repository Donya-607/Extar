#ifndef INCLUDED_PARTICLE_H_
#define INCLUDED_PARTICLE_H_

#include <vector>

#include "Common.h"	// scast‚ªŽg‚¢‚½‚¢
#include "Vector2.h"

namespace ParticleImage
{
	constexpr int TYPES = 3;
	constexpr int SIZE  = 16;

	void Load();
	void Release();

	int  GetHandle( int type );
}

class Particle
{
private:
	int		type;	// 0 ~ ParticleImage::TYPES

	float	angle;	// Degree, ClockWise
	float	alpha;	// 0.0 ~ 1.0f
	float	scale;	// 0.0 ~ 1.0f

	Vector2	pos;	// Center
	Vector2 velo;	// Velocity
	Vector2 destPos;
public:
	Particle() :
		type( 0 ),
		angle( 0 ), alpha( 1.0f ), scale( 1.0f ),
		pos(), velo(), destPos()
	{}
	~Particle() {}

	void Init( Vector2 centerPos, Vector2 velocity, bool isBig );
	void Uninit();

	void Update();
	void Draw( Vector2 shake ) const;
public:
	bool IsDisappear() const;
};

class ParticleMng
{
private:
	std::vector<Particle> particles;
public:
	ParticleMng() : particles() {}
	~ParticleMng()
	{
		std::vector<Particle>().swap( particles );
	}

	void Init();
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
public:
	void Generate( int num, Vector2 centerPos, bool isBig );
public:
	int  GetArraySize() const
	{
		return scast<int>( particles.size() );
	}
};

#endif //INCLUDED_PARTICLE_H_
