
#ifndef QUATERNION_HPP
#define QUATERNION_HPP

class Quaternion{
public:
	float x, y, z, w;
	Quaternion(){ x = y = z = 0; }
	Quaternion(float xval, float yval, float zval, float wval){
		x = xval; y = yval; z = zval; w = wval;
	}
	Quaternion& mul(Quaternion& b){
		w = w*b.w - x*b.x - y*b.y - z*b.z;
		x = x*b.w + w*b.x + y*b.z - z*b.y;
		y = y*b.w + w*b.y + z*b.x - x*b.z;
	  z = z*b.w + w*b.z + x*b.y - y*b.x;
		return *this;
	}
	static Quaternion mul(Quaternion& a, Quaternion& b){
		Quaternion quaternion;
		quaternion.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
		quaternion.x = a.x*b.w + a.w*b.x + a.y*b.z - a.z*b.y;
		quaternion.y = a.y*b.w + a.w*b.y + a.z*b.x - a.x*b.z;
		quaternion.z = a.z*b.w + a.w*b.z + a.x*b.y - a.y*b.x;
		return quaternion;
	}
	Quaternion& mul(Vector3& b){
		w = -x*b.x - y*b.y - z*b.z;
		x =  w*b.x + y*b.z - z*b.y;
		y =  w*b.y + z*b.x - x*b.z;
		z =  w*b.z + x*b.y - y*b.x;
		return *this;
	}
	static Quaternion mul(Quaternion& a, Vector3& b){
		Quaternion quaternion;
		quaternion.w = -a.x*b.x - a.y*b.y - a.z*b.z;
		quaternion.x =  a.w*b.x + a.y*b.z - a.z*b.y;
		quaternion.y =  a.w*b.y + a.z*b.x - a.x*b.z;
		quaternion.z =  a.w*b.z + a.x*b.y - a.y*b.x;
		return quaternion;
	}
	Quaternion conjugate(){
		return Quaternion(-x, -y, -z, w);
	}
};

typedef Quaternion Quat4;
typedef Quaternion Quat4f;

#endif
