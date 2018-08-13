#pragma once

#include <oslib/oslib.h>
#include <math.h>

#define SEGMENT_COUNT 7

class SegmentDisplay
{
private:
	struct TSegment
	{
		unsigned int x, y;
		OSL_IMAGE* texture;
		TSegment();
		TSegment(unsigned int x_, unsigned int y_, OSL_IMAGE* texture_);
		TSegment(const TSegment &obj);
	};
private:
	unsigned char mDigits, mPadding;
	int mLeft, mTop;
	unsigned int mWidth, mHeight;
	int mValue;
	unsigned int mSingleDigitWidth, mSingleDigitHeight;
	TSegment mSegments[SEGMENT_COUNT];
	void mRenderSegment(unsigned char segment, int digitOffsetX, int digitOffsetY);
public:
	SegmentDisplay(unsigned char digits, OSL_IMAGE* horizontalNumberBar, OSL_IMAGE* verticalNumberBar, 
		unsigned char padding, unsigned char diagonalOffset);

	int getLeft();
	void setLeft(int value);
	int getTop();
	void setTop(int value);

	unsigned int getWidth();
	unsigned int getHeight();

	unsigned int getSingleDigitWidth();
	unsigned int getSingleDigitHeight();
	unsigned char getDigitPadding();

	int getValue();
	void setValue(int value);

	void render();
};