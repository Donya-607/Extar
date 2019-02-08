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
	int		startIdx;	// 開始番号
	int		endIdx;		// 終了番号
	int		cnt;		// カウンター
	int		elem;		// 配列要素数
	int		spd;		// 再生速度
	int		*hnd;		// 画像ハンドル
	bool	loop;		// ループの使用フラグ
	bool	reverse;	// 逆再生の使用フラグ
	bool	end;		// 再生終了フラグ
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
