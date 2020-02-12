#pragma once

#include <vector>

// �i�s�x�̕ۊǂ��s���B�i�s�`�F�b�N���͍̂s��Ȃ��B
class ProgressStorage
{
public:
	// �r�b�g���Z�ō��̂ł��܂�
	enum Conditions
	{
		InputMove		= 0b0001,		// �ړ����͂�����܂�
		InputExposure	= 0b0010,		// �I�����͂�����܂�
		InputToggle		= 0b0100,		// �J�����̉�]���͂�����܂�
		WaitingTime		= 0b1000,		// ��莞�ԑ҂܂�
		
		IMPOSSIBLE		= 0b10000000	// �i�߂����Ȃ��Ƃ��Ɏg�p
	};
	// �r�b�g���Z�ō��̂ł��܂�
	enum BitInput
	{
		NIL				= 0b0000,		// ���������Ȃ�
		Move			= 0b0001,		// �ړ�������
		Exposure		= 0b0010,		// �I��������
		Toggle			= 0b0100		// �J�����̉�]������
	};
private:
	int								nowProgress;		// 0�n�܂�C�e�L�X�g�ԍ��i���͂̕��я��j�ɑΉ��B
	Conditions						nowConditions;		// �z�񂩂甲���o���đ�����C�B�������r�b�g����~�낵�Ă����B
	size_t							elementCount;		// �v�f���̕ۊǁB
	const std::vector<Conditions>	inputConditions;	// ���͂��ꂽ�C�i�s�����̔z��B
	const std::vector<BitInput>		inputPermitTimings;	// ���͂��ꂽ�C�s����������i�s�x���������z��BinputConditions�ƑΉ��B
public:
	// �Q�̔z��̗v�f���͑����Ă���K�v������܂�
	// conditions �ɂ́C���ꂼ��̗v�f�ɂ�����C�i�s���������Ă�������
	// permitTimings �ɂ́C���ꂼ��̗v�f�ɂ�����C������s�������Ă�������
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
