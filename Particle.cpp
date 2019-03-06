#include "DxLib.h"

#include "Particle.h"

namespace ParticleImage
{
	static int hParticles[TYPES];

	void Load()
	{
		// ‚·‚Å‚É’l‚ª“ü‚Á‚Ä‚¢‚½‚çC“Ç‚İ‚ñ‚¾‚à‚Ì‚Æ‚İ‚È‚µ‚Ä”ò‚Î‚·
		if ( 0 != hParticles[0] )
		{
			return;
		}
		// else

		LoadDivGraph
		(
			"./Data/Images/Particle/Particle.png",
			TYPES,
			1, TYPES,
			SIZE, SIZE,
			hParticles
		);
	}

	void Release()
	{
		for ( int i = 0; i < TYPES; i++ )
		{
			DeleteGraph( hParticles[i] );
			hParticles[i] = 0;
		}
	}

	int  GetHandle( int type )
	{
		assert( 0 <= type && type < TYPES );

		return hParticles[type];
	}
}

void Particle::Init( Vector2 centerPos )
{

}

void Particle::Uninit()
{

}

void Particle::Update()
{

}

void Particle::Draw( Vector2 shake ) const
{

}



void ParticleMng::Init()
{

}

void ParticleMng::Uninit()
{

}

void ParticleMng::Update()
{

}

void ParticleMng::Draw( Vector2 shake ) const
{

}

void ParticleMng::Generate( Vector2 centerPos )
{

}