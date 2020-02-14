#pragma once

#include <vector>

// 進行度の保管を行う。進行チェック自体は行わない。
class ProgressStorage
{
public:
	// ビット演算で合体できます
	enum Conditions
	{
		InputMove		= 0b0001,		// 移動入力があるまで
		InputExposure	= 0b0010,		// 露光入力があるまで
		InputToggle		= 0b0100,		// カメラの回転入力があるまで
		WaitingTime		= 0b1000,		// 一定時間待つまで
		
		IMPOSSIBLE		= 0b10000000	// 進めたくないときに使用
	};
	// ビット演算で合体できます
	enum BitInput
	{
		NIL				= 0b0000,		// 何も許さない
		Move			= 0b0001,		// 移動を許す
		Exposure		= 0b0010,		// 露光を許す
		Toggle			= 0b0100		// カメラの回転を許す
	};
private:
	int								nowProgress;		// 0始まり，テキスト番号（文章の並び順）に対応。
	Conditions						nowConditions;		// 配列から抜き出して代入し，達成したビットから降ろしていく。
	size_t							elementCount;		// 要素数の保管。
	const std::vector<Conditions>	inputConditions;	// 入力された，進行条件の配列。
	const std::vector<BitInput>		inputPermitTimings;	// 入力された，行動を許可する進行度を示した配列。inputConditionsと対応。
public:
	// ２つの配列の要素数は揃えてある必要があります
	// conditions には，それぞれの要素における，進行条件を入れてください
	// permitTimings には，それぞれの要素における，許可する行動を入れてください
	ProgressStorage( const std::vector<Conditions> &conditions, const std::vector<BitInput> &permitTimings );
public:
	bool IsAllowInput( BitInput input );
	bool IsCompleteNowConditions();

	void DoneConditions( Conditions achieve );
	void AdvanceProgress();
	void ResetProgress();
public:
	void AssignConditions();
};

static constexpr ProgressStorage::Conditions operator | ( const ProgressStorage::Conditions &lhs, const ProgressStorage::Conditions &rhs )
{
	return static_cast<ProgressStorage::Conditions>( static_cast<int>( lhs ) | static_cast<int>( rhs ) );
}
static constexpr ProgressStorage::Conditions operator & ( const ProgressStorage::Conditions &lhs, const ProgressStorage::Conditions &rhs )
{
	return static_cast<ProgressStorage::Conditions>( static_cast<int>( lhs ) & static_cast<int>( rhs ) );
}
static constexpr ProgressStorage::BitInput operator | ( const ProgressStorage::BitInput &lhs, const ProgressStorage::BitInput &rhs )
{
	return static_cast<ProgressStorage::BitInput>( static_cast<int>( lhs ) | static_cast<int>( rhs ) );
}
static constexpr ProgressStorage::BitInput operator & ( const ProgressStorage::BitInput &lhs, const ProgressStorage::BitInput &rhs )
{
	return static_cast<ProgressStorage::BitInput>( static_cast<int>( lhs ) & static_cast<int>( rhs ) );
}
