#pragma once
#include "primitives.h"
#include "camera.h"
#include <memory>
#include <vector>

using std::shared_ptr; //smart pointer to allocated type <t>
using std::make_shared; //allocates new instance

class Scene {
public:
	
	Scene() {}
	std::vector<shared_ptr<Primitive>> primitives;
	void add(shared_ptr<Primitive> prim) { primitives.push_back(prim); }
	
	bool intersect(Ray& r, float& t, shared_ptr<Primitive>& p) {
		t = constants::inf;
		for (auto& prim : primitives)
		{
			float t_prime = prim->intersect(r);
			if (t_prime > constants::epsilon && t_prime < t) {
				t = t_prime;
				p = prim;
			}
		}
		return (t < constants::inf);
	}
};