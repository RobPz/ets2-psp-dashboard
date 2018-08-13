#include "SegmentDisplay.h"

SegmentDisplay::TSegment::TSegment()
{
	x = 0;
	y = 0;
	texture = NULL;
}

SegmentDisplay::TSegment::TSegment(unsigned int x_, unsigned int y_, OSL_IMAGE* texture_)
{
	x = x_;
	y = y_;
	texture = texture_;
}

SegmentDisplay::TSegment::TSegment(const TSegment &obj)
{
	x = obj.x;
	y = obj.y;
	texture = obj.texture;
}

void SegmentDisplay::mRenderSegment(unsigned char segment, int digitOffsetX, int digitOffsetY)
{
	OSL_IMAGE* texture = mSegments[segment].texture;
	texture->x = digitOffsetX + mSegments[segment].x;
	texture->y = digitOffsetY + mSegments[segment].y;
	oslDrawImage(texture);
}

SegmentDisplay::SegmentDisplay(unsigned char digits, OSL_IMAGE* horizontalNumberBar, OSL_IMAGE* verticalNumberBar,
	unsigned char padding, unsigned char diagonalOffset)
{
	mDigits = digits;
	mPadding = padding;
	mValue = 0;

	mLeft = 0;
	mTop = 0;

	// bar side size
	unsigned int width = horizontalNumberBar->sizeY;

	// corners/intersections
	unsigned int half = horizontalNumberBar->sizeY / 2;
	unsigned int centerPiece = width % 2 == 0 ? 2 : 1;
	unsigned int halfShaved = width % 2 == 0 ? half - 1 : half;

	// insides
	unsigned int insideHorizontal = horizontalNumberBar->sizeX - width + diagonalOffset * 2 + centerPiece;
	unsigned int insideVertical = verticalNumberBar->sizeY - width + diagonalOffset * 2 + centerPiece;

	mSegments[0] = TSegment(halfShaved + centerPiece + diagonalOffset, 0, horizontalNumberBar);
	mSegments[1] = TSegment(mSegments[0].y, mSegments[0].x, verticalNumberBar);
	mSegments[2] = TSegment(mSegments[1].x + width + insideHorizontal, mSegments[1].y, verticalNumberBar);
	mSegments[3] = TSegment(mSegments[0].x, mSegments[0].y + width + insideVertical, horizontalNumberBar);
	mSegments[4] = TSegment(mSegments[1].x, mSegments[1].y + verticalNumberBar->sizeY + centerPiece + diagonalOffset * 2, verticalNumberBar);
	mSegments[5] = TSegment(mSegments[2].x, mSegments[4].y, verticalNumberBar);
	mSegments[6] = TSegment(mSegments[0].x, mSegments[3].y + width + insideVertical, horizontalNumberBar);

	mSingleDigitWidth = mSegments[2].x + width;
	mSingleDigitHeight = mSegments[6].y + width;
	mWidth = mSingleDigitWidth * mDigits + mPadding * (mDigits - 1);
	mHeight = mSingleDigitHeight;
}

int SegmentDisplay::getLeft()
{
	return mLeft;
}

void SegmentDisplay::setLeft(int value)
{
	mLeft = value;
}

int SegmentDisplay::getTop()
{
	return mTop;
}

void SegmentDisplay::setTop(int value)
{
	mTop = value;
}

unsigned int SegmentDisplay::getWidth()
{
	return mWidth;
}

unsigned int SegmentDisplay::getHeight()
{
	return mHeight;
}

unsigned int SegmentDisplay::getSingleDigitWidth()
{
	return mSingleDigitWidth;
}

unsigned int SegmentDisplay::getSingleDigitHeight()
{
	return mSingleDigitHeight;
}

unsigned char SegmentDisplay::getDigitPadding()
{
	return mPadding;
}

int SegmentDisplay::getValue()
{
	return mValue;
}

void SegmentDisplay::setValue(int value)
{
	mValue = value;
}

void SegmentDisplay::render()
{
	int rightOffset = mLeft + mWidth;

	unsigned char digit, count = 0;
	int value = mValue;

	do
	{
		digit = value % 10;
		value /= 10;

		int x = rightOffset - mSingleDigitWidth;
		int y = mTop;

		switch (digit)
		{
		default:
			mRenderSegment(3, x, y);
			break;
		case 0:
			mRenderSegment(0, x, y);
			mRenderSegment(1, x, y);
			mRenderSegment(2, x, y);
			mRenderSegment(4, x, y);
			mRenderSegment(5, x, y);
			mRenderSegment(6, x, y);
			break;
		case 1:
			mRenderSegment(2, x, y);
			mRenderSegment(5, x, y);
			break;
		case 2:
			mRenderSegment(0, x, y);
			mRenderSegment(2, x, y);
			mRenderSegment(3, x, y);
			mRenderSegment(4, x, y);
			mRenderSegment(6, x, y);
			break;
		case 3:
			mRenderSegment(0, x, y);
			mRenderSegment(2, x, y);
			mRenderSegment(3, x, y);
			mRenderSegment(5, x, y);
			mRenderSegment(6, x, y);
			break;
		case 4:
			mRenderSegment(1, x, y);
			mRenderSegment(2, x, y);
			mRenderSegment(3, x, y);
			mRenderSegment(5, x, y);
			break;
		case 5:
			mRenderSegment(0, x, y);
			mRenderSegment(1, x, y);
			mRenderSegment(3, x, y);
			mRenderSegment(5, x, y);
			mRenderSegment(6, x, y);
			break;
		case 6:
			mRenderSegment(0, x, y);
			mRenderSegment(1, x, y);
			mRenderSegment(3, x, y);
			mRenderSegment(4, x, y);
			mRenderSegment(5, x, y);
			mRenderSegment(6, x, y);
			break;
		case 7:
			mRenderSegment(0, x, y);
			mRenderSegment(2, x, y);
			mRenderSegment(5, x, y);
			break;
		case 8:
			mRenderSegment(0, x, y);
			mRenderSegment(1, x, y);
			mRenderSegment(2, x, y);
			mRenderSegment(3, x, y);
			mRenderSegment(4, x, y);
			mRenderSegment(5, x, y);
			mRenderSegment(6, x, y);
			break;
		case 9:
			mRenderSegment(0, x, y);
			mRenderSegment(1, x, y);
			mRenderSegment(2, x, y);
			mRenderSegment(3, x, y);
			mRenderSegment(5, x, y);
			mRenderSegment(6, x, y);
			break;
		}

		rightOffset -= mPadding + mSingleDigitWidth;
		count++;
	} while (abs(value) > 0 && count < mDigits);
}