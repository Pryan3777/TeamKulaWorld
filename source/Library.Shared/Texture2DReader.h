#pragma once

#include "ContentTypeReader.h"
#include "Texture2D.h"

namespace FieaGameEngine
{
	class Texture2DReader : public ContentTypeReader<Texture2D>
	{
		RTTI_DECLARATIONS(Texture2DReader, AbstractContentTypeReader)

	public:
		Texture2DReader(Game& game);
		Texture2DReader(const Texture2DReader&) = default;
		Texture2DReader& operator=(const Texture2DReader&) = default;
		Texture2DReader(Texture2DReader&&) = default;
		Texture2DReader& operator=(Texture2DReader&&) = default;
		~Texture2DReader() = default;

	protected:
		virtual std::shared_ptr<Texture2D> _Read(const std::wstring& assetName) override;
	};
}