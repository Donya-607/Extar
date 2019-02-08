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
	if ( elem < endIdx - 1 )
	{
		elem++;
		return;
	}
	// else

	end = true;
	if ( loop )
	{
		elem = startIdx;
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
	if ( startIdx < elem )
	{
		elem--;
		return;
	}
	// else
	
	end = true;
	if ( loop )
	{
		// 0 Žn‚Ü‚è‚Ì‚½‚ßC–‡”‚©‚ç‚P‚Âˆø‚¢‚Ä‚¢‚é
		elem = endIdx - 1;
	}
}

void Anim::Set
(
	int startIndex,	int endIndex,
	int count,		int element,
	int playSpeed,
	int *hundle,
	bool isLoop,
	bool isReverse,
	bool isEnd
)
{
	startIdx	=	startIndex;
	endIdx		=	endIndex;
	cnt			=	count;
	elem		=	element;
	spd			=	playSpeed;
	hnd			=	hundle;
	loop		=	isLoop;
	reverse		=	isReverse;
	end			=	isEnd;
}