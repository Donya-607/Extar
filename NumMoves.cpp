#include <string>

#include "Music.h"
#include "NumMoves.h"

#include "FileIO.h"

void NumMoves::Init( int stageNumber )
{
	moves.swap( FileIO::FetchNumMovesInfo( stageNumber ) );
}
void NumMoves::Uninit()
{
	moves.clear();
}

void NumMoves::Update()
{
	ChangeParametersByImGui();
}

int  NumMoves::CalcRank( int numMoves )
{
	int i = 0;
	for ( ; i <= scast<int>( moves.size() ); i++ )
	{
		// 基準「以下」ならＯＫ
		if ( numMoves <= moves.at( i ) )
		{
			return i;
		}
	}

	return i;
}

#if USE_IMGUI

void NumMoves::ChangeParametersByImGui()
{
	ImGui::Begin( "Number_of_Moves" );

	ImGui::Text( "Now_Number_of_Ranks : %d", scast<int>( moves.size() ) );

	if ( ImGui::Button( "Add_Ranks" ) )
	{
		moves.push_back( 99 );
	}
	if ( 1 < scast<int>( moves.size() ) )
	{
		if ( ImGui::Button( "Remove_Ranks" ) )
		{
			moves.pop_back();
		}
	}

	ImGui::BeginChild( ImGui::GetID( (void*)0 ), ImVec2( 420, 100 ), ImGuiWindowFlags_NoTitleBar );
	int end = scast<int>( moves.size() );
	for ( int i = 0; i < end; i++ )
	{
		std::string name = "Rank " + std::to_string( i );

		int before = ( i == 0   ) ? -1 : i - 1;
		int after  = ( i == end - 1 ) ? -1 : i + 1;
		int lower  = ( before == -1 ) ? 1  : moves.at( before ) + 1;
		int upper  = ( after  == -1 ) ? 99 : moves.at( after  );
		ImGui::SliderInt( name.c_str(), &moves.at( i ), lower, upper );
	}
	ImGui::EndChild();

	ImGui::Text( "" );	// Space

	if ( FileIO::GetNowStageNumber() <= FileIO::GetMaxStageNumber() )
	{
		if ( ImGui::Button( "Save" ) )
		{
			SaveData();

			// ファイルに保存するだけで適用しないため，ついでにまとめて読み込みなおす
			FileIO::ReadAllCamera();
			FileIO::ReadAllStars();
			FileIO::ReadAllNumMoves();

			PlaySE( M_E_NEXT );
		}
		if ( ImGui::Button( "Load" ) )
		{
			moves = FileIO::FetchNumMovesInfo( FileIO::GetNowStageNumber() );

			PlaySE( M_E_BACK );
		}
	}

	ImGui::End();
}

void NumMoves::SaveData()
{
	FileIO::WriteNumMoves( FileIO::GetNowStageNumber(), &moves );
}

#endif // USE_IMGUI