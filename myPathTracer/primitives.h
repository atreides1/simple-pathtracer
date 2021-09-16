#pragma once
#include "geometry.h"
#include "material.h"

class Primitive
{
public:
	shared_ptr<Material> mat;
	virtual float intersect(Ray& r) { return 0.0f; }
	virtual Vec3 normalAt(Point3& hp) { return Vec3(); }
	virtual shared_ptr<Material> getMaterial() { return make_shared<Material>(); }
};


// class Triangle {
// };

class Sphere : public Primitive {
public:
	Sphere() : center(Point3(0,0,0)), radius(0.25), mat(make_shared<Material>()) {}
	Sphere(Point3 origin, float r, shared_ptr<Material> m) : center(origin), radius(r), mat(m) {}
	Point3 center;
	float radius;
	shared_ptr<Material> mat;

	float intersect(Ray& r) override {
		Vec3 v = r.origin - center;
		Vec3 d = r.direction;

		// float a = d.dot(d); // we can get rid of a, since ||d||^2 = 1
		float b = 2.0f * d.dot(v);
		float c = v.dot(v) - radius * radius;
		float discriminant = b * b - 4 * c;
		if (discriminant < 0.0f) {
			return -1.0f;
		} else {
			discriminant = sqrt(discriminant);
			// check the smaller (closest) solution first
			float t = -b - discriminant;
			if (t > 0.0f) {
				return t / 2.0f;
			} else {
				// check the larger sol
				t = -b + discriminant;
				if (t > 0.0f) {
					return t / 2.0f;
				}
				return 0.0f;
			}
		}
	}
	Vec3 normalAt(Point3& hp) override {
		return (hp - center) / radius;
	}

	shared_ptr<Material> getMaterial() override {
		return mat;
	}
};