#pragma once
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool load(const std::string& filename);
	void unload();

	void setActive();
	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }

private:
	unsigned int mTextureID;
	int mWidth;
	int mHeight;
};

