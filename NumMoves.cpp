#include <string>

#include "NumMoves.h"

void NumMoves::Init( std::vector<int> data )
{
	moves.swap( data );
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
	int i = 1;
	for ( ; i <= scast<int>( moves.size() ); i++ )
	{
		// äÓèÄÅuà»â∫ÅvÇ»ÇÁÇnÇj
		if ( numMoves <= moves.at( i - 1 ) )
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

	ImGui::BeginChild( ImGui::GetID( (void*)0 ), ImVec2( 250, 100 ), ImGuiWindowFlags_NoTitleBar );
	int end = scast<int>( moves.size() );
	for ( int i = 0; i < end; i++ )
	{
		std::string name = "Rank " + std::to_string( i );

		int before = ( i == 0   ) ? -1 : i - 1;
		int after  = ( i == end ) ? -1 : i + 1;
		int lower  = ( before == -1 ) ? 1 : moves.at( before ) + 1;
		int upper  = ( after == -1 ) ? 99 : moves.at( after );
		ImGui::SliderInt( name.c_str(), &moves.at( i ), lower, upper );
	}
	ImGui::EndChild();

	ImGui::End();
}

#endif // USE_IMGUI