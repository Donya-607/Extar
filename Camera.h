#ifndef INCLUDED_CAMERA_H_
#define INCLUDED_CAMERA_H_

#if USE_IMGUI

#include <imgui.h>
#include <imgui_impl_dxlib.h>

#endif // USE_IMGUI

#include "Vector2.h"
#include "Collision.h"

namespace CameraImage
{
	void Load();
	void Release();

	int  GetHandle();
}

class Camera
{
private:
	int		row;	// 今いる行, 0始まり
	int		column;	// 今いる列, 0始まり

	int		width;	// 覆うマスのサイズ, 1始まり
	int		height;	// 覆うマスのサイズ, 1始まり

	int		moveAmount;	// 一度に動くマスの数, 1始まり

	Vector2 pos;	// LeftTop Position
	Vector2 velo;	// Velocity
	Vector2 size;	// 全体サイズ
public:
	Camera() : row( 0 ), column( 0 ), width( 1 ), height( 1 ),
		moveAmount( 1 ),
		pos(), velo(), size() {}
	~Camera() {}

	void Init( int sizeX, int sizeY, int movementAmount );
	void Uninit();

	void Update();

	void Move();
	void ClampPos();
	void ClampMatrix();

	void Draw( Vector2 shake );

	Box FetchColWorldPos() const
	{
		Vector2 halfSize{ size.x * 0.5f, size.y * 0.5f };

		Box tmp =
		{
			pos.x + halfSize.x,
			pos.y + halfSize.y,
			halfSize.x,
			halfSize.y,
			true
		};

		return tmp;
	}
private:
#if USE_IMGUI

	void ChangeParametersByImGui();

#endif // USE_IMGUI
};

#endif //INCLUDED_CAMERA_H_
