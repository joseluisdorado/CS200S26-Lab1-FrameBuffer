#ifndef CS200_FRAME_BUFFER_H_
#define CS200_FRAME_BUFFER_H_

namespace Rasterizer
{
	struct Color; // forward declare the color structure

	static class FrameBuffer
	{
	public:
		// Initialize
		static bool Allocate(unsigned int width, unsigned int height);
		static void Delete();

		// Getters
		static unsigned char*	GetBufferData();
		static unsigned int		GetWidth();
		static unsigned int		GetHeight();

		// FrameBuffer Operations
		static void Clear(const Color & c);
		static void Clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		static void SetPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		static void SetPixel(unsigned int x, unsigned int y, const Color & c);
		static Color GetPixel(unsigned int x, unsigned int y);
		static void Present();

		// Debug
		static void SaveToFile(const char *filename);
		static void LoadFromFile(const char * filename);

		// Debug
		static void SaveToImageFile(const char * filename);

		// Extra Challenges
		static void ClearCheckerboard(Color colors[2], unsigned int size);
		static void LoadFromImageFile(const char* filename);
		static void CheckerboardImage(const char* filename, unsigned int size);

		// Private Variables
	private:
		static unsigned char *		frameBuffer;
		static unsigned int		frameBufferWidth;
		static unsigned int		frameBufferHeight;
	};
}

#endif