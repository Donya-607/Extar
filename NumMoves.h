#ifndef INCLUDED_NUM_MOVES_H_
#define INCLUDED_NUM_MOVES_H_

#include <vector>

#include "Common.h"

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

class NumMoves
{
private:
	std::vector<int> moves;
public:
	NumMoves() : moves() {}
	~NumMoves()
	{
		std::vector<int>().swap( moves );
	}

	void Init( int stageNumber );
	void Uninit();

	void Update();
public:
	int  CalcRank( int numMoves );
	std::vector<int> GetData() const
	{
		return moves;
	}

#if USE_IMGUI

private:
	void ChangeParametersByImGui();
public:
	void SaveData();

#endif // USE_IMGUI

};

#endif //INCLUDED_NUM_MOVES_H_
