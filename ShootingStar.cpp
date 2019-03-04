#include "DxLib.h"

#include "ShootingStar.h"

namespace ShootingStarImage
{
	static int hStar;

	void Load()
	{
		// Ç∑Ç≈Ç…ílÇ™ì¸Ç¡ÇƒÇ¢ÇΩÇÁÅCì«Ç›çûÇÒÇæÇ‡ÇÃÇ∆Ç›Ç»ÇµÇƒîÚÇŒÇ∑
		if ( 0 != hStar )
		{
			return;
		}
		// else

		hStar = LoadGraph( "./Data/Images/Star/ShootingStar.png" );
	}

	void Release()
	{
		DeleteGraph( hStar );
		hStar = 0;
	}

	int  GetHandle()
	{
		return hStar;
	}
}

void VisionStar::Init( int imageHandle, Vector2 centerPos )
{
	handle	= imageHandle;
	pos		= centerPos;
}

void VisionStar::Uninit()
{

}

void VisionStar::Update()
{
	timer++;
}

void VisionStar::Draw( Vector2 shake ) const
{
	// SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	DrawRotaGraph
	(
		scast<int>( pos.x ),
		scast<int>( pos.y ),
		1.0,
		ToRadian( angle ),
		handle,
		TRUE
	);
	SetDrawBlendMode( DX_BLENDMODE_ADD, 255 - timer );
	DrawRotaGraph
	(
		scast<int>( pos.x ),
		scast<int>( pos.y ),
		1.0,
		ToRadian( angle ),
		handle,
		TRUE
	);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
}

bool VisionStar::IsDisappear() const
{
	constexpr int EXIST_TIME = 30;

	return ( EXIST_TIME <= timer ) ? true : false;
}



void ShootingStar::Init( Vector2 centerPos )
{
	pos = centerPos;

	constexpr float SPD = 10.0f;
	velo.x = -SPD;
	velo.y = SPD;
}

void ShootingStar::Uninit()
{

}

void ShootingStar::Update()
{
	timer++;

	pos += velo;
}

void ShootingStar::Draw( Vector2 shake ) const
{
	// SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	DrawRotaGraph
	(
		scast<int>( pos.x ),
		scast<int>( pos.y ),
		1.0,
		ToRadian( angle ),
		ShootingStarImage::GetHandle(),
		TRUE
	);
	SetDrawBlendMode( DX_BLENDMODE_ADD, 255 - timer );
	DrawRotaGraph
	(
		scast<int>( pos.x ),
		scast<int>( pos.y ),
		1.0,
		ToRadian( angle ),
		ShootingStarImage::GetHandle(),
		TRUE
	);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
}

bool ShootingStar::IsRequestingGenerateVision() const
{
	constexpr int GENERATE_INTERVAL = 2;
	return ( ( timer % GENERATE_INTERVAL ) == 1 ) ? true : false;
}

bool ShootingStar::IsDisappear() const
{
	constexpr int EXIST_TIME = 30;

	return ( EXIST_TIME <= timer ) ? true : false;
}

int ShootingStar::GetHandle() const
{
	return ShootingStarImage::GetHandle();
}



void ShootingStarMng::Init()
{
	stars.clear();
}

void ShootingStarMng::Uninit()
{
	stars.clear();
}

void ShootingStarMng::Update()
{
	for (
			std::vector<ShootingStar>::iterator &it = stars.begin();
			it != stars.end();
		)
	{
		it->Update();

		if ( it->IsRequestingGenerateVision() )
		{
			GenerateVision( it->GetHandle(), it->GetPos() );
		}

		if ( it->IsDisappear() )
		{
			it = stars.erase( it );
			continue;
		}
		// else

		it++;
	}

	for (
			std::vector<VisionStar>::iterator &it = visions.begin();
			it != visions.end();
		)
	{
		it->Update();

		if ( it->IsDisappear() )
		{
			it = visions.erase( it );
			continue;
		}
		// else

		it++;
	}
}

void ShootingStarMng::Draw( Vector2 shake ) const
{
	for ( const VisionStar &it : visions )
	{
		it.Draw( shake );
	}

	for ( const ShootingStar &it : stars )
	{
		it.Draw( shake );
	}
}

void ShootingStarMng::GenerateStar( Vector2 centerPos )
{
	stars.push_back( ShootingStar() );
	stars.back().Init( centerPos );
}

void ShootingStarMng::GenerateVision( int imageHandle, Vector2 centerPos )
{
	visions.push_back( VisionStar() );
	visions.back().Init( imageHandle, centerPos );
}