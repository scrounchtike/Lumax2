
#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>

class Vector2{
public:
	float x, y;
	Vector2(){ x = y = 0; }
	Vector2(float x_val, float y_val){ x = x_val; y = y_val; }
	float& operator[](int index){
		if(index == 0)
			return x;
		else
			return y;
	}

	Vector2 operator+(Vector2 v2){Vector2 result; result.x = x + v2.x; result.y = y + v2.y; return result;}
	Vector2 operator-(Vector2 v2){Vector2 result; result.x = x - v2.x; result.y = y - v2.y; return result;}
	Vector2& operator+=(Vector2 v2){ x += v2.x; y += v2.y; return *this; }
	Vector2& operator-=(Vector2 v2){ x -= v2.x; y -= v2.y; return *this; }

	Vector2 operator+(float s){ Vector2 result; result.x = x + s; result.y = y + s; return result;}
	Vector2 operator-(float s){ Vector2 result; result.x = x - s; result.y = y - s; return result;}
	Vector2 operator*(float s){ Vector2 result; result.x = x * s; result.y = y * s; return result;}
	Vector2 operator/(float s){ Vector2 result; result.x = x / s; result.y = y / s; return result;}
	Vector2& operator+=(float s){ x += s; y += s; return *this; }
	Vector2& operator-=(float s){ x -= s; y -= s; return *this; }
	Vector2& operator*=(float s){ x *= s; y *= s; return *this; }
	Vector2& operator/=(float s){ x /= s; y /= s; return *this; }

	Vector2 operator-(){ Vector2 result; result.x = -x; result.y = -y; return result; }

	float dot(Vector2 v2){
		return x*v2.x + y*v2.y;
	}
	float getLengthSquared(){
		return x*x + y*y;
	}
	float getLength(){
		return sqrt(getLengthSquared());
	}
	Vector2& normalize(){
		float length = getLength();
		x /= length;
		y /= length;
		return *this;
	}
};

typedef Vector2 Vec2;

#endif
