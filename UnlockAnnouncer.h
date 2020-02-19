#pragma once

#include "Vector2.h"

class UnlockAnnouncer
{
public:
	static void LoadImages();
	static void ReleaseImages();
private:
	int		timer  = 0;
	int		alpha  = 0; // 0 ~ 255
	int		bright = 0; // 0 ~ 255
	Vector2	pos{};		// Left-Top.
	bool	doIncrement = true;
public:
	void Update();

	void Draw();
public:
	bool ShouldRemove() const;
	int  GetDrawAlpha()  const;
	int  GetDrawBright() const;
private:
	void AssignAlpha();
	void AssignBright();
	void AssignImagePos();
private:
	void UseImGui();
};
