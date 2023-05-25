#pragma once
#include "DirectXMath.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class BaseDrawableGameobject
	{
	public:
		virtual DirectX::XMVECTOR GetPositionVector() = 0;
		virtual void DrawableUpdate(const GameTime&) = 0;
		virtual void DrawableDraw(const GameTime&) = 0;
	};
}
