#include <AEEngine.h>
#include "Color.h"
namespace Rasterizer
{
	// ------------------------------------------------------------------------
	/// \fn		Default & custom
	/// \brief	
	Color::Color()
		: r(0.0f)
		, g(0.0f)
		, b(0.0f)
		, a(1.0f)
	{}

	// ------------------------------------------------------------------------
	/// \fn		Default & custom
	/// \brief	
	Color::Color(float rr, float gg, float bb, float aa)
		: r(rr)
		, g(gg)
		, b(bb)
		, a(aa)
	{}

	// ------------------------------------------------------------------------
	/// \fn		FromU32
	/// \brief	Set the color to the unsigned int color in format ARGB hex
	Color & Color::FromU32(unsigned int color) // custom constructor
	{
		// decompose
		unsigned int aa = (color & (0x000000FF << 24)) >> 24;
		unsigned int rr = (color & (0x000000FF << 16)) >> 16;
		unsigned int gg = (color & (0x000000FF << 8)) >> 8;
		unsigned int bb = color & 0x000000FF;

		// float
		r = (float)rr / 255.0f;
		g = (float)gg / 255.0f;
		b = (float)bb / 255.0f;
		a = (float)aa / 255.0f;

		// return the modified color
		return *this;
	}

	// ------------------------------------------------------------------------
	/// \fn		ToU32
	/// \brief	returns unsigned int color from this color.
	unsigned int Color::ToU32()	const // conversion operator
	{
		// convert each element to range [0,255]
		unsigned int aa = (a < 0.0f) ? (0) : ((a > 1.0f) ? (255) : unsigned int(a*255.0f));
		unsigned int rr = (r < 0.0f) ? (0) : ((r > 1.0f) ? (255) : unsigned int(r*255.0f));
		unsigned int gg = (g < 0.0f) ? (0) : ((g > 1.0f) ? (255) : unsigned int(g*255.0f));
		unsigned int bb = (b < 0.0f) ? (0) : ((b > 1.0f) ? (255) : unsigned int(b*255.0f));

		// pack
		unsigned int c = (aa << 24) | (rr << 16) | (gg << 8) | bb;
		return c;
	}

	AEVec3 Color::ToHSV() const
	{
		AEVec3 outHSV;
		// Find the minimum and maximum RGB values
		float maxVal = max(r, max(g, b));
		float minVal = min(r, min(g, b));

		// Compute hue
		if (maxVal == minVal)
		{
			outHSV.x = 0;
		}
		else if (maxVal == r)
		{
			outHSV.x = 60 * ((g - b) / (maxVal - minVal)) + 0;
			if (outHSV.x < 0) outHSV.x += 360;
		}
		else if (maxVal == g)
		{
			outHSV.x = 60 * ((b - r) / (maxVal - minVal)) + 120;
		}
		else if (maxVal == b)
		{
			outHSV.x = 60 * ((r - g) / (maxVal - minVal)) + 240;
		}

		// Compute saturation
		if (maxVal == 0)
		{
			outHSV.y = 0;
		}
		else
		{
			outHSV.y = (maxVal - minVal) / maxVal;
		}

		// Compute value
		outHSV.z = maxVal;

		return outHSV;
	}

	void Color::FromHSV(const AEVec3& hsv)
	{
		float h = hsv.x;
		float s = hsv.y;
		float v = hsv.z;

		// Compute the chroma
		float c = v * s;

		// Compute the intermediate value
		float x = c * (1 - abs(int(h / 60) % 2 - 1));

		// Initialize RGB values
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;

		// Compute RGB values based on the hue
		if (h >= 0 && h < 60)
		{
			r = c;
			g = x;
		}
		else if (h >= 60 && h < 120)
		{
			r = x;
			g = c;
		}
		else if (h >= 120 && h < 180)
		{
			g = c;
			b = x;
		}
		else if (h >= 180 && h < 240)
		{
			g = x;
			b = c;
		}
		else if (h >= 240 && h < 300)
		{
			r = x;
			b = c;
		}
		else if (h >= 300 && h < 360)
		{
			r = c;
			b = x;
		}

		// Compute the minimum value
		float minVal = v - c;

		// Add the minimum value to the RGB values
		r += minVal;
		g += minVal;
		b += minVal;
	}

	Color Color::operator *(const float & sc)const 
	{
		return Color(r*sc, g*sc, b*sc, a*sc); 
	}
	Color& Color::operator *=(const float & sc) 
	{
		return *this = *this * sc; 
	}
	Color Color::operator/(const float& sc) const
	{
		return Color(r/sc, g/sc, b/sc, a/sc);
	}
	Color& Color::operator/=(const float& sc)
	{
		return *this = *this / sc;
	}
	Color Color::operator +(const Color & rhs)const  
	{
		return Color(
			r + rhs.r,
			g + rhs.g,
			b + rhs.b,
			a + rhs.a);
	}
	Color& Color::operator += (const Color & rhs)
	{
		return *this = *this + rhs;
	}
	Color Color::operator-(const Color& rhs) const
	{
		return Color(
			r - rhs.r,
			g - rhs.g,
			b - rhs.b,
			a - rhs.a);
	}
	Color& Color::operator-=(const Color& rhs)
	{
		return *this = *this - rhs;
	}
	Color Color::operator-() const
	{
		return Color(-r,-g,-b,-a);
	}
	Color Color::operator*(const Color& rhs) const
	{
		return Color(
			r * rhs.r,
			g * rhs.g,
			b * rhs.b,
			a * rhs.a);
	}
	Color& Color::operator*=(const Color& rhs)
	{
		return *this = *this * rhs;
	}
}// namespace Rasterizer