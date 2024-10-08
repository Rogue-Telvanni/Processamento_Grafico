#include "color.h"
#include "ray.h"

#include <iostream>
#include <fstream>

inline glm::vec3 operator/(const glm::vec3 &v, float t)
{
    return (1/t) * v;
} 

inline glm::vec3 operator*(double t, const glm::vec3 &v)
{
     return glm::vec3(t*v.x, t*v.y, t*v.z);
}

inline glm::vec3 operator*(const glm::vec3 &v, double t)
{
     return t * v;
} 

inline glm::vec3 unit_vector(const glm::vec3 &v){
    return v / v.length();
}

double hit_sphere(const glm::vec3& center, double radius, const ray& r) {
    glm::vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto h = dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = h*h - a*c;

    if (discriminant < 0)
        return -1.0;
    else
        return (h - std::sqrt(discriminant)) / a;
}

color ray_color(const ray& r) {
    double t = hit_sphere(glm::vec3(0,0,-1), 0.5, r);
    if(t > 0.0)
    {
        glm::vec3 normal = unit_vector(r.at(t) - glm::vec3(0, 0, -1));
        return 0.5 * color(normal.x+1, normal.y+1, normal.z+1);
    }

    glm::vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}


int main() {

    // clear file data
    std::ofstream file;
    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = glm::vec3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = glm::vec3(viewport_width, 0, 0);
    auto viewport_v = glm::vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - glm::vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render

    file.open("test.ppm", std::ofstream::out | std::ofstream::trunc);
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(file, pixel_color);
        }
    }
    file.close();

    std::clog << "\rDone.                 \n";
}