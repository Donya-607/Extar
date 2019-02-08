#ifndef INCLUDED_ANIMATION_H_
#define INCLUDED_ANIMATION_H_

//--------------------
//
//		Animation.h
//
//--------------------

/// <summary>
/// int startIdx, endIdx : アニメーション範囲<para></para>
/// int cnt, index, spd : 再生に使用<para></para>
/// int *hnd : メンバ設定時にアドレスを渡しておけば，Get関数が使える<para></para>
/// bool loop : 最後のアニメ番号の再生が終わったときに，最初のアニメに戻すかどうか<para></para>
/// bool reverse : 末尾から先頭へ再生する，再生終了フラグが立つのは先頭のアニメ番号の再生が終わった瞬間<para></para>
/// bool end : 再生が終わったあと，次に更新が呼ばれるまで真になる，loopがオフのときのみ動作する
/// </summary>
class Anim
{
public:
	int		startIdx;	// 開始番号
	int		endIdx;		// 終了番号
	int		cnt;		// カウンター
	int		index;		// 配列要素数
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
