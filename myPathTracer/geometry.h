#pragma once
#include <iostream>
#include <cmath>

using std::sqrt;
// contains classes Vec3, Point3, Ray

// Vector3 - print + - dot cross normalize length, += (scalar), * (scalar) and * vector components
class Vec3 {
public:
	Vec3() : x(0.0), y(0.0), z(0.0) {};
	Vec3(float _x) : x(_x), y(_x), z(_x) {};
	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	// copy constructor
	Vec3(const Vec3& v) { x = v.x; y = v.y; z = v.z; }

	float x{}, y{}, z{};

	// class methods 
	Vec3 operator+(const Vec3 &v) {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator-(const Vec3 &v) {
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 operator*(Vec3 v) {
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3& operator+=(const float &f) {
		x += f; y += f; z += f;
		return *this;
	}

	Vec3& operator+=(const Vec3 v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vec3& operator*=(float f) {
		x *= f; y *= f; z *= f;
		return *this;
	}

	Vec3& operator*=(const Vec3 &v) {
		x *= v.x; y *= v.y; z *= v.z;
		return *this;
	}
	
	Vec3& operator/=(const float &f) {
		x /= f; y /= f; z /= f;
		return *this;
	}
	float dot(const Vec3& v) {
		return (x * v.x + y * v.y + z * v.z);
	}

	float length() {
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 operator/(float f) {
		return Vec3(x / f, y / f, z / f);
	}

	friend auto operator<<(std::ostream& os, Vec3 const& v) -> std::ostream& {
		return os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
};

Vec3 operator*(float f, Vec3 v) {
	return Vec3(f * v.x, f * v.y, f * v.z);
}
Vec3 operator*(Vec3& v, float& f) {
	return f * v;
}

Vec3 cross(Vec3& v, Vec3& u) {
	float w_x = v.y * u.z - v.z * u.y;
	float w_y = v.z * u.x - v.x * u.z;
	float w_z = v.x * u.y - v.y * u.x;
	return Vec3(w_x, w_y, w_z);

}

Vec3& normalize(Vec3& v) {
	float length = v.length();
	if (length == 1.0f) {
		return v;
	}
	v = v / length;
	return v;
}

Vec3 clamp(Vec3& v, float min, float max) {
	v.x = (v.x < min) ? min : (v.x > max) ? max : v.x;
	v.y = (v.y < min) ? min : (v.y > max) ? max : v.y;
	v.z = (v.z < min) ? min : (v.z > max) ? max : v.z;
	return v;
}

using Point3 = Vec3;

// Mat4 - multiply, inverse, transpose
// class Mat4 {
// };

// Ray - defined by a Point3 origin, and Vec3 direction
class Ray {
public:
	Ray() {};
	Ray(Point3 o, Vec3 d) : origin(o), direction(normalize(d)) {}
	Point3 origin;
	Vec3 direction;

	Point3 getPointAtDistance(float t) { return origin + t*direction; }

	friend auto operator<<(std::ostream& os, const Ray &r) -> std::ostream& {
		return os << "Ray(" << r.origin << ", " << r.direction << ")";
	}
};