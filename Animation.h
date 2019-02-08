#ifndef INCLUDED_ANIMATION_H_
#define INCLUDED_ANIMATION_H_

//--------------------
//
//		Animation.h
//
//--------------------

class Anim
{
public:
	int		startIdx;	// �J�n�ԍ�
	int		endIdx;		// �I���ԍ�
	int		cnt;		// �J�E���^�[
	int		elem;		// �z��v�f��
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
		int count,		int element, 
		int playSpeed, 
		int *hundle, 
		bool isLoop		= true,
		bool isReverse	= false,
		bool isEnd		= false
	);

	int GetNowHandle() const
	{
		return hnd[elem];
	}
};

#endif //INCLUDED_ANIMATION_H_
