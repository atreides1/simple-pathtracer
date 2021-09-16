#pragma once
#include "geometry.h"
//#include <memory>
#include <random>

static std::random_device rd;  //seed for random number generator
static std::mt19937 rng(rd()); //Mersenne twister is good for Monte Carlo methods
// see https://stackoverflow.com/questions/16536617/random-engine-differences
using std::shared_ptr;
using std::make_shared;

//using std::shared_ptr;
//using std::make_shared;

float randf(float x = 0.0, float y = 1.0) {
    static std::uniform_real_distribution<float> distribution(x, y);
    //static std::mt19937 generator;
    return distribution(rng);
}

// Helpers for random number generation
//std::mt19937 mersenneTwister;
//std::uniform_real_distribution<float> uniform;

//#define RND (2.0*uniform(mersenneTwister)-1.0)
//#define RND2 (uniform(mersenneTwister))


Vec3 unit_sphere()
{
    while (true)
    {
        Vec3 random_vec(randf(), randf(), randf());
        float len = random_vec.length();
        if (len * len >= 1)
            continue; // if length is too long, "reject" and try again
        return random_vec;
    }
}
// returns a random vector in the hemisphere of the surface normal
Vec3 random_in_hemisphere(Vec3 &surface_normal)
{
    Vec3 in_unit_sphere = unit_sphere();
    if (in_unit_sphere.dot(surface_normal) > 0.0) // if the vector faces the same direction as the normal, return it
    {
        return in_unit_sphere;
    }
    else
    {
        return -1.0f * in_unit_sphere; // else, it's pointing in the wrong direction, so mult. by -1
    }
}

enum Type
{
    DIFFUSE,
    SPECULAR,
    REFRACTIVE
};

class Material {
public:
	Material() : albedo(Vec3(1, 0, 0)), emission(Vec3()), roughness(0.0f), materialType(DIFFUSE) {}//diffuseReflectance(1.0f) {};
	Material(Vec3 a, Vec3 e, float r, Type mat) : albedo(a), emission(e), roughness(r), materialType(mat) {}//emittance(e), diffuseReflectance(r) {};
	Vec3 albedo{};
	Vec3 emission{};
    float roughness{};
    Type materialType;
	//float diffuseReflectance{};
	//float specularReflectance{};

    bool scatter(Ray& ray, Point3& hp, Vec3& normal, float& t, Vec3& attenuation) {
        Vec3 diffuse;
        Vec3 specular;
        if (materialType == DIFFUSE) {

            Vec3 color = albedo;
            Point3 target = hp + random_in_hemisphere(normal);
            diffuse = target - hp;
            ray.origin = hp;
            ray.direction = diffuse;//normalize(diffuse);
            const float p = 1 / (2 * constants::pi);
            float cos_theta = diffuse.dot(normal);
            attenuation = (albedo / constants::pi) * cos_theta / p; 
            return true;


        } else if (materialType == SPECULAR) {
            float b = ray.direction.dot(normal);
            Vec3 reflected_dir = ray.direction - normal * (b * 2);
            specular = normalize(reflected_dir);
            diffuse = (hp + random_in_hemisphere(normal)) - hp;
            ray.origin = hp;
            // lerp specular and diffuse based on roughness
            // ray.d = specular;
            ray.direction = (roughness == 1) ? diffuse :
                (roughness == 0) ? specular :
                ((1 - roughness * roughness) * specular + roughness * roughness * diffuse);
            ray.direction = normalize(ray.direction);
            attenuation = albedo;
            return true;

        } else if (materialType == REFRACTIVE) {
                float n = 1.5f; // IOR
                if (normal.dot(ray.direction) > 0)
                {
                    normal = normal * -1;
                    n = 1/n;
                }
                n = 1/n; //do i do this twice?
                float cos1 = -1*(normal.dot(ray.direction));
                float cos2 = 1.0f - n*n*(1.0f-cos1*cos1);
                // Schlick Approximation
                float r0 = (1.0f - n)/(1.0f + n); // from Fresnel eq.
                r0 *= r0;
                float refrac_prob = r0 + (1.0f - r0) * pow(1.0f - cos1, 5.0f);
                if (cos2 > 0 && randf() > refrac_prob)
                {
                    //refract
                    Vec3 dir = ((ray.direction * n) + (normal * (n * cos1 - sqrt(cos2))));
                    ray.direction = normalize(dir);
                } else {
                    Vec3 dir = (ray.direction + normal * (cos1 * 2));
                    ray.direction = normalize(dir);
                }
                attenuation = (albedo.x+albedo.y+albedo.z == 0) ? Vec3(1,1,1) : albedo;
                return true;
            }
        return false;
    }
};

