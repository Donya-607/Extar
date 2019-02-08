#ifndef INCLUDED_ANIMATION_H_
#define INCLUDED_ANIMATION_H_

//--------------------
//
//		Animation.h
//
//--------------------

/// <summary>
/// int startIdx, endIdx : �A�j���[�V�����͈�<para></para>
/// int cnt, index, spd : �Đ��Ɏg�p<para></para>
/// int *hnd : �����o�ݒ莞�ɃA�h���X��n���Ă����΁CGet�֐����g����<para></para>
/// bool loop : �Ō�̃A�j���ԍ��̍Đ����I������Ƃ��ɁC�ŏ��̃A�j���ɖ߂����ǂ���<para></para>
/// bool reverse : ��������擪�֍Đ�����C�Đ��I���t���O�����̂͐擪�̃A�j���ԍ��̍Đ����I������u��<para></para>
/// bool end : �Đ����I��������ƁC���ɍX�V���Ă΂��܂Ő^�ɂȂ�Cloop���I�t�̂Ƃ��̂ݓ��삷��
/// </summary>
class Anim
{
public:
	int		startIdx;	// �J�n�ԍ�
	int		endIdx;		// �I���ԍ�
	int		cnt;		// �J�E���^�[
	int		index;		// �z��v�f��
	int		spd;		// �Đ����x
	int		*hnd;		// �摜�n���h��
	bool	loop;		// ���[�v�̎g�p�t���O
	bool	reverse;	// �t�Đ��̎g�p�t���O
	bool	end;		// �Đ��I���t���O
public:
	void Update();
private:
	void ReverseUpdate();
public:
	void Set
	(
		int startIndex,	int endIndex,
		int nowIndex, 
		int playSpeed, 
		int *hundle, 
		bool isLoop		= true,
		bool isReverse	= false
	);

	int GetNowHandle() const
	{
		return hnd[index];
	}
};

#endif //INCLUDED_ANIMATION_H_
