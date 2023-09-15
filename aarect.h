#ifndef AARECT_H
#define AARECT_H

#include "rtweekend.h"
#include "hittable.h"
#include <vector>

//Vertex struct
struct Vertex {
    Vertex() {}
    Vertex(vec3 pos, vec3 norm) : position(pos), normal(norm) {}
    vec3 position;
    vec3 normal;
};

//Rect classes and hit functions
class xy_rect : public hittable {
    public:
        double x0, x1, y0, y1, k;
        shared_ptr<material> mp;

    public:
        xy_rect() {}

        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k,
            shared_ptr<material> mat) 
            : x0(_x0), x1(_x1), y1(_y1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            
            // The bounding box must have non-zero width in each dimension, so pad the X
            // dimension a small amount.
            output_box = aabb(point3(x0, y0, k-0.0001), point3(x1, y1, k+0.0001));
            return true;
        }
};

class xz_rect : public hittable {
    public:
        xz_rect() {}

        xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Y
            // dimension a small amount.
            output_box = aabb(point3(x0,k-0.0001,z0), point3(x1, k+0.0001, z1));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, z0, z1, k;
};

class yz_rect : public hittable {
    public:
        yz_rect() {}

        yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
            shared_ptr<material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the X
            // dimension a small amount.
            output_box = aabb(point3(k-0.0001, y0, z0), point3(k+0.0001, y1, z1));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double y0, y1, z0, z1, k;
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}
bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k-r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;
    auto y = r.origin().y() + t*r.direction().y();
    auto z = r.origin().z() + t*r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

// //Triangle classes and hit functions
// class triangle : public hittable {
//     public:
//         triangle(int a, int b, int c, std::vector<Vertex>* vertices, material* mat);
//         virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        
//         bool triangle::bounding_box(double time0, double time1, aabb& output_box) const {
//                 // The bounding box must have non-zero width in each dimension, so pad the Y
//                 // dimension a small amount.
//                 output_box = aabb(point3(x0,k-0.0001,z0), point3(x1, k+0.0001, z1));
//                 return true;
//             }
    
//     private:
//         std::vector<Vertex>* vertices;
//         int a, b, c;
//         std::shared_ptr<material> mat_ptr;
// };

// bool triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec)const
//     {
//         const float EPSILON = 0.0000001;
 
//         vec3 edge1, edge2, h, s, q, ca, cb, cc;
//         float a, f, u, v;
 
//         ca = vertices->at(a).position;
//         cb = vertices->at(b).position;
//         cc = vertices->at(c).position;
 
//         edge1 = cb - ca;
//         edge2 = cc - ca;
         
//         h = cross(r.direction(), edge2);
//         a = dot(edge1, h);
//         if (a > -EPSILON && a < EPSILON)
//             return false;    // This ray is parallel to this triangle.
 
//         f = 1.0 / a;
//         s = r.origin() - ca;
//         u = f * dot(s, h);
//         if (u < 0.0 || u > 1.0)
//             return false;
 
//         q = cross(s, edge1);
//         v = f * dot(r.direction(), q);
//         if (v < 0.0 || u + v > 1.0)
//             return false;
 
//         // At this stage we can compute t to find out where the intersection point is on the line.
//         float t = f * dot(edge2, q);
//         if (t > EPSILON && t < 1 / EPSILON) // ray intersection
//         {
//             if (t < t_max && t > t_min) //object closer than previous
//             {
//                 rec.t = t;
//                 rec.p = r.origin() + r.direction() * t;
//                 rec.normal = h;
//                 rec.mat_ptr = mat_ptr;
//                 return true;
//             }
//             else
//                 return false; //this means there is a ray intersection, but it is behind a differend object
//         }
//         else // This means that there is a line intersection but not a ray intersection.
//             return false;
//     }

 #endif