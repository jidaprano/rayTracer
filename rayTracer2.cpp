#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "bvh.h"
#include "texture.h"
#include "aarect.h"
#include "box.h"

#include <iostream>

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    if(depth <= 0) {
        return color(0,0,0);
    }

    if (!world.hit(r, 0.001, infinity, rec)) {
        return background;
    }

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr-> emitted(rec.u, rec.v, rec.p);

    if(!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }
    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);

}

hittable_list scene1() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9,0.9,0.9));
    auto perltex = make_shared<noise_texture>(20);
    auto earthtex = make_shared<image_texture>("earthmap.jpg");
    auto ground_material = make_shared<lambertian>(color(0.4, 0.5, 0.8));
    auto ball_material = make_shared<lambertian>(perltex);
    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    auto white = make_shared<lambertian>(color(.73, .73, .73));

    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, ball_material)); 
    objects.add(make_shared<sphere>(point3(0,1.3,0), 1.3, ball_material)); 
    objects.add(make_shared<xy_rect>(3,5,1,3,-2, white));
    objects.add(make_shared<xz_rect>(0, 5.55, 0, 5.55, 5.55, difflight));
    //objects.add(make_shared<xy_rect>(3,5,1,3,-2, difflight));

    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 5.55, 0, 5.55, 5.55, green));
    objects.add(make_shared<yz_rect>(0, 5.55, 0, 5.55, 0, red));
    objects.add(make_shared<xz_rect>(2.13, 3.43, 2.27, 3.32, 5.54, light));
    objects.add(make_shared<xz_rect>(0, 5.55, 0, 5.55, 0, white));
    objects.add(make_shared<xz_rect>(0, 5.55, 0, 5.55, 5.55, white));
    objects.add(make_shared<xy_rect>(0, 5.55, 0, 5.55, 5.55, white));

    return objects;
}

hittable_list cornell_box_cubes() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    //objects.add(make_shared<xy_rect>(100, 400, 100, 400, 500, white));

    objects.add(make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), red));

    return objects;
}

hittable_list scene2 () {
    hittable_list objects;

    auto perltex = make_shared<noise_texture>(20);
    auto perlmat = make_shared<lambertian>(perltex);
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto red   = make_shared<lambertian>(color(.65, .05, .05));


    objects.add(make_shared<xz_rect>(-50, 50, -50, 50, 0, perlmat));
    objects.add(make_shared<xz_rect>(-3, 3, -3, 3, 6, light));
    shared_ptr<hittable> box1 = make_shared<box>(point3(-1,0,-1), point3(1,2,1), red);
    //box1 = make_shared<translate>(box1, vec3(0,1,0));
    box1 = make_shared<rotate_y>(box1, 45);
    box1 = make_shared<rotate_z>(box1, 45);
    objects.add(box1);

    return objects;
}

hittable_list scene3 () {
    hittable_list objects;

    auto perltex = make_shared<noise_texture>(20);
    auto perlmat = make_shared<lambertian>(perltex);
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
    auto red   = make_shared<lambertian>(color(.65, .05, .05));


    objects.add(make_shared<xz_rect>(-50, 50, -50, 50, 0, perlmat));
    objects.add(make_shared<xz_rect>(-3, 3, -3, 3, 6, light));
    
    // std::vector<Vertex>* vertices;
    // vertices->push_back(Vertex(vec3(0,2,0), vec3(0,0,0)));
    // vertices->push_back(Vertex(vec3(2,2,0), vec3(0,0,0)));
    // vertices->push_back(Vertex(vec3(1,4,0), vec3(0,0,0)));

    //objects.add(make_shared<triangle>(0, 1, 2, vertices, red));

    return objects;
}

int main() {

    // Image

    double aspect_ratio;
    double image_width;
    int image_height;
    int samples_per_pixel;
    const int max_depth = 50;

    // World
    hittable_list world;

    // Camera
    point3 lookfrom;
    point3 lookat;
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0;
    color background;
    double vfov;

    //Scenes
    switch(0) {
        
        case 1:
            world = scene1();
            background = color(0,0,0);
            aspect_ratio = 16.0 / 9.0;
            image_width = 400.0;
            image_height = static_cast<int>(image_width / aspect_ratio);
            samples_per_pixel = 200;
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;
        
        case 2:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600.0;
            image_height = static_cast<int>(image_width / aspect_ratio);
            samples_per_pixel = 200;
            background = color(0,0,0);
            lookfrom = point3(2.78, 2.78, -8.00);
            lookat = point3(2.78, 2.78, 0);
            vfov = 40.0;
            break;
        //default:
        case 3:
            world = cornell_box_cubes();
            aspect_ratio = 1.0;
            image_width = 600.0;
            image_height = static_cast<int>(image_width / aspect_ratio);
            samples_per_pixel = 200;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
        default:
        case 4:
            world = scene2();
            aspect_ratio = 1.0;
            image_width = 600.0;
            image_height = static_cast<int>(image_width / aspect_ratio);
            samples_per_pixel = 100;
            background = color(0,0,0);
            lookfrom = point3(13,4,3);
            lookat = point3(0,0,0);
            vfov = 30.0;
            break;
    }

   camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);


    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for(int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u,v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}