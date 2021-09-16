#pragma once
#pragma once
#include "constants.h"
#include "geometry.h"
#include "material.h"

class Camera {
public:
	Camera(Point3 lookfrom, Point3 lookat, Vec3 up, int w, int h, float fov)
		: origin(lookfrom), width(w), height(h), fovy(fov*constants::deg_to_rad) {
		generateOrthoBasis(lookfrom-lookat, up);
		aspect_ratio = float(width) / height;
	}
	Vec3 u{}, v{}, w{};
	Point3 origin{};
	float fovy{};
	int width{}, height{};
	float aspect_ratio{};

	void generateOrthoBasis(Vec3 a, Vec3 b) {
		w = normalize(a);
		Vec3 temp = cross(b, w);
		u = normalize(temp);
		v = cross(w, u);
	}

	Ray rayThroughPixel(float i, float j) {
		i += randf();
		j += randf();
		float alpha = tan(fovy*0.5f) * ((i - width*0.5f) / (width*0.5f) * aspect_ratio);
		float beta = tan(fovy*0.5f)  * ((height*0.5f - j) / (height*0.5f));
		Vec3 direction = alpha * u + (-beta * v) - w;
		return Ray(origin, direction);
	}


};