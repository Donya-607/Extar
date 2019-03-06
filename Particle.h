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
	int		timer;
	float	angle;	// Degree, ClockWise

	Vector2	pos;	// Center
	Vector2 velo;	// Velocity
public:
	Particle() :
		timer( 0 ),
		angle( 0 ),
		pos(), velo()
	{}
	~Particle() {}

	void Init( Vector2 centerPos );
	void Uninit();

	void Update();
	void Draw( Vector2 shake ) const;
public:
	bool IsDisappear() const;
};

class ParticleMng
{
private:
	std::vector<Particle> stars;
public:
	ParticleMng() : stars() {}
	~ParticleMng()
	{
		std::vector<Particle>().swap( stars );
	}

	void Init();
	void Uninit();

	void Update();

	void Draw( Vector2 shake ) const;
public:
	void Generate( Vector2 centerPos );
public:
	int  GetArraySize() const
	{
		return scast<int>( stars.size() );
	}
};

#endif //INCLUDED_PARTICLE_H_
