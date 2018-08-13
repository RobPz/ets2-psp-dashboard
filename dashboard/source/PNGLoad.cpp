#include "PNGLoad.h"

OSL_IMAGE* loadPNG(const char* filename, bool swizzle)
{
	FILE* fp;
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)
		return NULL;

	png_infop info = png_create_info_struct(png);
	if (!info)
	{
		png_destroy_read_struct(&png, (png_infopp)NULL, (png_infopp)NULL);
		return NULL;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_read_struct(&png, &info, (png_infopp)NULL);
		return NULL;
	}

	png_init_io(png, fp);

	png_read_info(png, info);

	png_uint_32 width = png_get_image_width(png, info);
	png_uint_32 height = png_get_image_height(png, info);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, RGBA format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (png_uint_32 y = 0; y < height; y++)
	{
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, row_pointers);

	fclose(fp);

	OSL_IMAGE* result = oslCreateImage(width, height, OSL_IN_RAM, OSL_PF_8888);
	oslLockImage(result);
	for (png_uint_32 y = 0; y < height; y++)
	{
		png_bytep pngLine = row_pointers[y];
		u32* imageLine = (u32*)oslGetImageLine(result, y);
		memcpy(imageLine, pngLine, width * sizeof(u32));
	}
	oslUnlockImage(result);

	png_destroy_read_struct(&png, &info, (png_infopp)NULL);
	for (size_t y = 0; y < height; y++)
		free(row_pointers[y]);
	free(row_pointers);

	if (swizzle)
		oslSwizzleImage(result);

	return result;
}