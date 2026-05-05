#include <AEEngine.h> // float, unsigned int, etc...
#include "FrameBuffer.h"
#include "Color.h"

// file io
#include <fstream>
#include <sstream>

#define COLOR_COMP 4

namespace Rasterizer
{
	// ------------------------------------------------------------------------
	// Our framebuffer
	unsigned char *	FrameBuffer::frameBuffer = NULL;
	unsigned int		FrameBuffer::frameBufferWidth = 0;
	unsigned int		FrameBuffer::frameBufferHeight = 0;

	// ---------------------------------------------------------------------------
	// \fn		Allocate
	// \brief	Allocate memory for the frame buffer given by the width and height. 
	bool FrameBuffer::Allocate(unsigned int width, unsigned int height)
	{
		// free any data
		Delete();

		frameBufferWidth = width;
		frameBufferHeight = height;
		frameBuffer = new unsigned char[width * height * COLOR_COMP];
		if (frameBuffer)
		{
			Clear(0, 0, 0);
			return true;
		}
		return false;
	}
	// ---------------------------------------------------------------------------
	// \fn		Delete
	// \brief	Free the memory allocated in the function above. 
	void FrameBuffer::Delete()
	{
		// TODO delete the data

		if (frameBuffer)
			delete[] frameBuffer;
	}

	// ---------------------------------------------------------------------------
	// \fn		Present
	// \brief	Draws the contents of the frame buffer to the screen using Alpha
	//			Engine.
	void FrameBuffer::Present()
	{
		if (frameBuffer && frameBufferWidth != 0 && frameBufferHeight != 0) {
			auto tex = AEGfxTextureLoad(frameBufferWidth, frameBufferHeight, frameBuffer);
			AEGfxTriStart();
			AEGfxTriAdd(
				-0.5f, 0.5f, AE_COLORS_WHITE, 0, 1,
				-0.5f, -0.5f, AE_COLORS_WHITE, 0, 0,
				0.5f, -0.5f, AE_COLORS_WHITE, 1, 0);
			AEGfxTriAdd(
				-0.5f, 0.5f, AE_COLORS_WHITE, 0, 1,
				0.5f, -0.5f, AE_COLORS_WHITE, 1, 0,
				0.5f, 0.5f, AE_COLORS_WHITE, 1, 1);
			auto quad = AEGfxTriEnd();

			AEGfxTextureSet(tex);
			AEMtx33 mtx = AEMtx33::Scale((float)AEGetSysWindowWidth(), (float)AEGetSysWindowHeight());
			AEGfxSetTransform(&mtx);
			AEGfxTriDraw(quad);

			AEGfxTextureUnload(tex);
			AEGfxTriFree(quad);
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		GetBufferData
	// \brief	Returns the pointer to the frame buffer variable
	unsigned char *	FrameBuffer::GetBufferData()
	{
		return frameBuffer;
	}

	// ---------------------------------------------------------------------------
	// \fn		GetWidth
	// \brief	Returns the width of the frame buffer.
	unsigned int		FrameBuffer::GetWidth()
	{
		return frameBufferWidth;
	}

	// ---------------------------------------------------------------------------
	// \fn		GetHeight
	// \brief	Returns the height of the frame buffer.
	unsigned int		FrameBuffer::GetHeight()
	{
		return frameBufferHeight;
	}

	// ---------------------------------------------------------------------------
	// \fn		Clear
	// \brief	Sets the entire frame buffer to the provided color.
	void FrameBuffer::Clear(const Color & c)
	{
		Clear(unsigned char(c.r * 255.0f),
			unsigned char(c.g * 255.0f),
			unsigned char(c.b * 255.0f),
			unsigned char(c.a * 255.0f));
	}

	// ---------------------------------------------------------------------------
	// \fn		Clear
	// \brief	Sets the entire frame buffer to the provided color in rgb format
	void FrameBuffer::Clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		if (frameBuffer)
		{
			unsigned int totalSize = frameBufferWidth * frameBufferHeight * COLOR_COMP;
			for (unsigned int i = 0; i < totalSize; i += COLOR_COMP)
			{
				frameBuffer[i] = r;
				frameBuffer[i + 1] = g;
				frameBuffer[i + 2] = b;
				frameBuffer[i + 3] = a;
			}
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		SetPixel
	// \brief	Sets the pixel at position x, y to the provided color. 
	void FrameBuffer::SetPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		// Sanity check
		if (NULL == frameBuffer || x >= frameBufferWidth || y >= frameBufferHeight)
			return;

		// advance to pixel
		unsigned int startOffset = COLOR_COMP * (y * frameBufferWidth + x);

		// set
		frameBuffer[startOffset] = r;
		frameBuffer[startOffset + 1] = g;
		frameBuffer[startOffset + 2] = b;
		frameBuffer[startOffset + 3] = a;
	}

	// ---------------------------------------------------------------------------
	// \fn		SetPixel
	// \brief	Sets the pixel at position x, y to the provided color. 
	void FrameBuffer::SetPixel(unsigned int x, unsigned int y, const Color& c)
	{
		SetPixel(x, y,
			unsigned char(c.r * 255.0f),
			unsigned char(c.g * 255.0f),
			unsigned char(c.b * 255.0f),
			unsigned char(c.a * 255.0f));
	}

	// ---------------------------------------------------------------------------
	// \fn		GetPixel
	// \brief	Returns the color of the pixel at position x, y.
	Color FrameBuffer::GetPixel(unsigned int x, unsigned int y)
	{
		// Sanity check
		if (NULL == frameBuffer || x >= frameBufferWidth || y >= frameBufferHeight)
			return Color();

		// advance to pixel
		unsigned int startOffset = COLOR_COMP * (y * frameBufferWidth + x);

		// Get the color component
		unsigned char r = frameBuffer[startOffset];
		unsigned char g = frameBuffer[startOffset + 1];
		unsigned char b = frameBuffer[startOffset + 2];
		unsigned char a = frameBuffer[startOffset + 3];

		// Convert to color class
		return Color((float)r*255.0f, (float)g*255.0f, (float)b*255.0f, (float)a*255.0f);
	}

	// ---------------------------------------------------------------------------
	// \fn		SaveToFile
	// \brief	Saves the frame buffer to a binary file. 
	void FrameBuffer::SaveToFile(const char *filename)
	{
		// Sanity Check
		if (!filename)
			return;

		// try to open the file
		std::fstream fp(filename, std::ios::out | std::ios::binary);

		if (fp.is_open() && fp.good())
		{
			// write header information - just width and height
			fp.write(reinterpret_cast<const char*>(&frameBufferWidth), sizeof(unsigned int));
			fp.write(reinterpret_cast<const char*>(&frameBufferHeight), sizeof(unsigned int));

			// write pixel data
			fp.write(reinterpret_cast<const char*>(frameBuffer), frameBufferWidth * frameBufferHeight * COLOR_COMP);

			// close the file
			fp.close();
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		LoadFromFile
	// \brief	Load the frame buffer from a binary file. 
	void FrameBuffer::LoadFromFile(const char * filename)
	{
		// Sanity check
		if (!filename)
			return;

		// try to open the file
		std::fstream fp(filename, std::ios::in | std::ios::binary);

		if (fp.is_open() && fp.good())
		{
			unsigned int fbWidth, fbHeight;
			fp.read(reinterpret_cast<char *>(&fbWidth), sizeof(unsigned int));
			fp.read(reinterpret_cast<char *>(&fbHeight), sizeof(unsigned int));

			// re-allocate the data if necessary
			if (NULL != frameBuffer && (fbWidth * fbHeight) != (frameBufferWidth * frameBufferHeight))
			{
				delete[] frameBuffer;
				frameBuffer = new unsigned char[fbWidth * fbHeight * COLOR_COMP];
			}
			// no frame buffer, allocate one
			else if (NULL == frameBuffer)
			{
				frameBuffer = new unsigned char[fbWidth * fbHeight * COLOR_COMP];
			}

			// store new width and new height
			frameBufferWidth = fbWidth;
			frameBufferHeight = fbHeight;

			// now read the framebuffer data
			fp.read(reinterpret_cast<char *>(frameBuffer), frameBufferWidth * frameBufferHeight * COLOR_COMP);

			// close the file
			fp.close();
		}
	}

	// ---------------------------------------------------------------------------
	// \fn		SaveToImageFile
	// \brief	Save the frame buffer to image file. 
	void FrameBuffer::SaveToImageFile(const char * filename)
	{
		AEGfxSaveImagePNG(filename, frameBuffer, frameBufferWidth, frameBufferHeight);
	}

	// Extra Challenges
	void FrameBuffer::ClearCheckerboard(unsigned int colors[2], unsigned int size)
	{
		unsigned int c = 0;
		unsigned int s = 0;
		for (unsigned int i = 0; i < frameBufferHeight; ++i) {
			if (i % size == 0)
				s = ++s % 2;
			c = s;
			for (unsigned int j = 0; j < frameBufferWidth; ++j)
			{
				if (j % size == 0)
					c = ++c % 2;
				unsigned int p[4];
				AEGfxColorComp(colors[c], p, p + 1, p + 2, p + 3);
				for (unsigned int k = 0; k < 4; ++k)
					frameBuffer[(i * frameBufferWidth + j) * 4 + k] = (unsigned char)p[k];
			}
		}
	}

	void FrameBuffer::LoadFromImageFile(const char* filename) {
		// load the file using the alpha engine
		unsigned char* imgPixels = 0;
		unsigned int imgWidth = 0, imgHeight = 0;
		if (AEGfxLoadImagePNG(filename, imgPixels, imgWidth, imgHeight))
		{
			auto minW = min(imgWidth, frameBufferWidth);
			auto minH = min(imgHeight, frameBufferHeight);

			// center image if not bigger than frame buffer
			auto sX =  minW == frameBufferWidth ? 0 : (frameBufferWidth - imgWidth) / 2;
			auto sY =  minH == frameBufferHeight ? 0 : (frameBufferHeight - imgHeight) / 2;

			// copy data
			for (unsigned int i = 0; i < minH; ++i) {
				for (unsigned int j = 0; j < minW; ++j) {
					for (unsigned int k = 0; k < 4; ++k) {
						unsigned int fbIdx= ((sY+i) * frameBufferWidth + (sX+j)) * 4 + k;
						unsigned int imgIdx = (i * imgWidth + j) * 4 + k;
						frameBuffer[fbIdx] = imgPixels[imgIdx];
					}
				}
			}

			// cleanup
			delete[] imgPixels;
		}
	}


	/*
		\details: This is a tricky one. Why? Because of the image size. 
		here the user is passing us what is supposed to be the cell size as 
		a parameter. 

		This wasn't a problem in the first checker board function, because
		we were setting each "square" in the checker board with one single color. 

		In this function, the user expects the image to be SCALED to the size 
		of the square. But we don't know how to scale an image. Copying the data
		is one thing, modifying it is another. 

		As a first step, we are going to copy the texture data into the size specified. 
		If the image is bigger then we simply copy everything (like in the previous function). 
		If the image is smaller, then we can do the same centering logic as above. 
		The result would be a small buffer that we copy into the framebuffer.

		After this works, then we can tackle the scaling down of the image to fit the size of 
		the rectangle. The simplest thing is to take the average of a group of pixels in the image
		and use it as the color of 1 pixel in the cell. 
	*/
	void FrameBuffer::CheckerboardImage(const char* filename, unsigned int size)
	{
		// load the file using the alpha engine
		unsigned char* imgPixels = 0;
		unsigned int imgWidth = 0, imgHeight = 0;
		if (AEGfxLoadImagePNG(filename, imgPixels, imgWidth, imgHeight))
		{
			
			unsigned char* cellData1 = new unsigned char[size * size * 4];
			unsigned char* cellData2 = new unsigned char[size * size * 4];
			unsigned char* cellSrc[2] = { cellData1, cellData2 };
			
			// we are going to 
			auto minW = min(imgWidth, size);
			auto minH = min(imgHeight, size);

			// center image if not bigger than frame buffer
			auto sX = minW == size ? 0 : (size - imgWidth) / 2;
			auto sY = minH == size ? 0 : (size - imgHeight) / 2;

			// copy data into cell size
			for (unsigned int i = 0; i < minH; ++i) {
				for (unsigned int j = 0; j < minW; ++j) {
					unsigned int sqIdx = ((sY + i) * size + (sX + j)) * 4;
					unsigned int imgIdx = (i * imgWidth + j) * 4;


					// square 1: copy original color 
					// square 2: invert original color 
					for (unsigned int k = 0; k < 4; ++k) {
						cellData1[sqIdx + k] = imgPixels[imgIdx + k];
						// alpha stays the same
						if(k == 3)
							cellData2[sqIdx + k] = imgPixels[imgIdx + k];
						else
							cellData2[sqIdx + k] = 255-imgPixels[imgIdx + k];
					}
				}
			}

			unsigned int c = 0;
			unsigned int s = 0;
			unsigned int sqRowSize = size * 4;
			unsigned int sqRowSize2 = size;
			for (unsigned int i = 0; i < frameBufferHeight; ++i) {
				c = s;
				unsigned int sqIdx = (i%size) * size * 4;
				unsigned int sqIdx2 = (i * size);
				for (unsigned int j = 0; j < frameBufferWidth; j+=size)
				{

					unsigned int fbIdx2 = (i * frameBufferWidth + j);
					unsigned int fbRowSize2 = (frameBufferWidth - j);

					unsigned int fbIdx = (i * frameBufferWidth + j) * 4;
					unsigned int fbRowSize = (frameBufferWidth -j) * 4;
					unsigned int minSize = min(fbRowSize, sqRowSize);

					// copy square data
					//memcpy_s(frameBuffer + fbIdx, fbRowSize, cellSrc[c] + sqIdx, minSize);
					for (unsigned int k = 0; k < minSize; ++k) {
						frameBuffer[fbIdx + k] = cellSrc[c][sqIdx + k];
					}
					

					c = ++c % 2;
				}
				if (i % size == 0)
					s = ++s % 2;
			}


			// cleanup
			delete[] imgPixels;
		}
	}
}
