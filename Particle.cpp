#include "DxLib.h"

#include "Particle.h"

namespace
{
	constexpr int EXIST_TIME = 20;
}

namespace ParticleImage
{
	static int hParticles[TYPES];

	void Load()
	{
		// すでに値が入っていたら，読み込んだものとみなして飛ばす
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

void Particle::Init( Vector2 centerPos, Vector2 velocity, bool isBig )
{
	type  = rand() % ParticleImage::TYPES;

	angle = scast<float>( rand() % 360 );

	scale = ( isBig ) ? 8.0f : 4.0f;

	pos   = centerPos;
	velo  = ( isBig ) ? velocity * 2.0f : velocity;

	destPos = pos + ( velo * scast<float>( EXIST_TIME >> 1 ) );
}

void Particle::Uninit()
{

}

void Particle::Update()
{
	// Angle
	{
		constexpr float ROTATE_SPD = 10.0f;
		constexpr int	RAND_RANGE = 10;

		angle += ROTATE_SPD + scast<float>( rand() % RAND_RANGE );
	}

	if ( 0 <= scale )
	{
		constexpr float SHRINK_SPD	= 0.05f;
		constexpr int	DIGIT		= 100;	// 小数点以下の桁数
		constexpr int	RANGE		= 3;

		// 0.0 ~ 0.3
		float randShrink = ( rand() % DIGIT ) / scast<float>( RANGE * DIGIT );
		randShrink *= 0.1f;

		scale -= SHRINK_SPD + randShrink;
	}

	if ( 0 <= alpha )
	{
		constexpr float DECREASE = 1.0f / scast<float>( EXIST_TIME );
		alpha -= DECREASE;
	}

	// Position
	{
		constexpr float RESISTANCE = 0.1f;

		velo = ( destPos - pos ) * RESISTANCE;

		pos += velo;
	}
}

void Particle::Draw( Vector2 shake ) const
{
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, scast<int>( alpha * 255.0f ) );

	DrawRotaGraph
	(
		scast<int>( pos.x - shake.x ),
		scast<int>( pos.y - shake.y ),
		scale,
		ToRadian( angle ),
		ParticleImage::GetHandle( type ),
		TRUE
	);
}

bool Particle::IsDisappear() const
{
	return ( alpha <= 0 ) ? true : false;
}



void ParticleMng::Init()
{
	particles.clear();
}

void ParticleMng::Uninit()
{
	for ( Particle &it : particles )
	{
		it.Uninit();
	}

	particles.clear();
}

void ParticleMng::Update()
{
	for (
			std::vector<Particle>::iterator it = particles.begin();
			it != particles.end();
		)
	{
		it->Update();

		if ( it->IsDisappear() )
		{
			it = particles.erase( it );
			continue;
		}
		// else

		it++;
	}
}

void ParticleMng::Draw( Vector2 shake ) const
{
	for ( const Particle &it : particles )
	{
		it.Draw( shake );
	}

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
}

void ParticleMng::Generate( int num, Vector2 centerPos, bool isBig )
{
	constexpr int	RAND_DIGIT	= 100;	// 小数点以下の桁数

	constexpr float BASE_SPD	= 20.0f;
	constexpr int	RANGE_SPD	= 8;	// 0 ~ 8

	const float	BASE_ANGLE		= 360.0f / num;
	const int	RANGE_ANGLE		= 20;	// 0 ~ 20

	for ( int i = 0; i < num; i++ )
	{
		// 0.0 ~ 0. RANGE f
		float randSpd	= ( rand() % RAND_DIGIT ) / scast<float>( RANGE_SPD		* RAND_DIGIT );
		float randAngle	= ( rand() % RAND_DIGIT ) / scast<float>( RANGE_ANGLE	* RAND_DIGIT );
		randSpd		*= 10;
		randAngle	*= 1000;

		Vector2 velocity = { BASE_SPD, 0 };
		velocity.x += randSpd;
		velocity.Rotate( ( i * BASE_ANGLE ) + randAngle );

		particles.push_back( Particle() );
		particles.back().Init( centerPos, velocity, isBig );
	}
}