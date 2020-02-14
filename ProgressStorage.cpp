#include "ProgressStorage.h"

#include "Common.h"	// assert()�̎g�p�̂���

ProgressStorage::ProgressStorage( const std::vector<Conditions> &conditions, const std::vector<BitInput> &permitTimings ) :
	nowProgress( 0 ), nowConditions(), elementCount( NULL ),
	inputConditions( conditions ), inputPermitTimings( permitTimings )
{
	assert( conditions.size() == permitTimings.size() );
	elementCount = conditions.size();

	AssignConditions();
}

bool ProgressStorage::IsAllowInput( BitInput input )
{
	// �R���X�g���N�^�ł� assert() ���ʂ��Ă���Ζ��Ȃ��͂��ł͂��邪�C�˂�̂���
	assert( elementCount == inputPermitTimings.size() );

	BitInput currentPermission = inputPermitTimings[nowProgress];
	return ( currentPermission & input ) ? true : false;
}

bool ProgressStorage::IsCompleteNowConditions()
{
	return ( nowConditions == 0 ) ? true : false;
}

void ProgressStorage::DoneConditions( Conditions achieve )
{
	// enum�^���ƃr�b�g���Z���ł��Ȃ����߁C��������int�^�Ōv�Z���Ă���������
	int intCondition	= scast<int>( nowConditions );
	int intAchieve		= scast<int>( achieve );
	int calculated		= intCondition & ( ~intAchieve );

	nowConditions = scast<Conditions>( calculated );
}

void ProgressStorage::AdvanceProgress()
{
	nowProgress++;
	AssignConditions();
}

void ProgressStorage::ResetProgress()
{
	nowProgress = 0;
	AssignConditions();
}

void ProgressStorage::AssignConditions()
{
	if ( elementCount <= scast<size_t>( nowProgress ) ) { return; }
	// else

	nowConditions = inputConditions[nowProgress];
}
