#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

struct ImageResolution {
	int x;
	int y;
};

color ray_color(const ray& ray, const hittable& world) {
	hit_record rec;
	if (world.hit(ray, interval(0, infinity), rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}

	vec3 unit_direction = unit_vector(ray.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);

	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // Sky gradient color
}

int main() {
	// Aspect ratio
	auto aspectRatio = 16.0 / 9.0;

	ImageResolution res;
	res.x = 400;

	// Calculate height and ensure its at least 1
	res.y = int(res.x / aspectRatio);
	res.y = (res.y < 1) ? 1 : res.y;

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	// Camera
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(res.x) / res.y); // Viewport widths less than one are ok since they're real valued
	auto camera_center = point3(0, 0, 0);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	auto pixel_delta_u = viewport_u / res.x;
	auto pixel_delta_v = viewport_v / res.y;

	// Calculate the location of the upper left pixel.
	auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
	auto pixel00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// PPM header
	std::cout << "P3\n" << res.x << ' ' << res.y << "\n255\n";

	// Rendering
	for (int y = 0; y < res.y; y++) {
		std::clog << "\rScanlines " << y << "/" << res.y << " complete. " << std::flush;
		for (int x = 0; x < res.x; x++) {
			auto pixel_center = pixel00_location + (x * pixel_delta_u) + (y * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone!                      \n";

	return 0;
}