#include "Animation.h"

void Anim::Update()
{
	if ( reverse )
	{
		ReverseUpdate();
		return;
	}
	//else

	if ( ++cnt < spd )
	{
		end = false;
		return;
	}
	// else

	cnt = 0;
	if ( index < endIdx - 1 )
	{
		index++;
		return;
	}
	// else

	end = true;
	if ( loop )
	{
		index = startIdx;
	}
}
void Anim::ReverseUpdate()
{
	if ( ++cnt < spd )
	{
		end = false;

		return;
	}
	// else

	cnt = 0;
	if ( startIdx < index )
	{
		index--;
		return;
	}
	// else
	
	end = true;
	if ( loop )
	{
		// 0 Žn‚Ü‚è‚Ì‚½‚ßC–‡”‚©‚ç‚P‚Âˆø‚¢‚Ä‚¢‚é
		index = endIdx - 1;
	}
}

void Anim::Set
(
	int startIndex,	int endIndex,
	int nowIndex,
	int playSpeed,
	int *hundle,
	bool isLoop,
	bool isReverse
)
{
	startIdx	=	startIndex;
	endIdx		=	endIndex;
	index		=	nowIndex;
	spd			=	playSpeed;
	hnd			=	hundle;
	loop		=	isLoop;
	reverse		=	isReverse;
}