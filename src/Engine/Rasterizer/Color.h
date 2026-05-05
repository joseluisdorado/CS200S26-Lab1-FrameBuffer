#ifndef CS200_COLOR_H_
#define CS200_COLOR_H_

namespace Rasterizer
{
//#pragma warning (disable:4201) // nameless struct warning
	struct Color
	{
		union
		{
			struct
			{
				float r, g, b, a; // normalized values [0-1]
			};
			float v[4];
		};

		Color();
		Color(float rr, float gg, float bb, float aa = 1.0f);
		
		Color	operator *(const float & sc) const;
		Color&	operator *=(const float & sc);
		Color	operator /(const float& sc) const;
		Color& operator /=(const float& sc);
		Color	operator +(const Color& rhs) const;
		Color&	operator += (const Color& rhs);
		Color	operator -(const Color& rhs) const;
		Color& operator -= (const Color& rhs);
		Color operator-()const;
		Color	operator *(const Color& rhs) const;
		Color&	operator *= (const Color& rhs);
		Color& FromU32(unsigned int colorU32);
		unsigned int    ToU32()const;
		AEVec3 ToHSV()const;
		void	FromHSV(const AEVec3& hsv);
	};
//#pragma warning (default:4201) // nameless struct warning
}

#endif