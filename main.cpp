#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

struct ImageResolution {
	int x;
	int y;
};

int main() {
	ImageResolution res;
	res.x = 400;

	// Aspect ratio
	auto aspectRatio = 16.0 / 9.0;

	// Calculate height and ensure its at least 1
	res.y = int(res.x / aspectRatio);
	res.y = (res.y < 1) ? 1 : res.y;

	// Viewport widths less than one are ok since they're real valued
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(res.x) / res.y);

	// PPM header
	std::cout << "P3\n" << res.x << ' ' << res.y << "\n255\n";

	// Rendering
	for (int y = 0; y < res.y; y++) {
		std::clog << "\rScanlines " << y << "/" << res.y << " complete. " << std::flush;
		for (int x = 0; x < res.x; x++) {
			auto pixelColor = color(double(x) / (res.x - 1), double(y) / (res.y - 1), 0);

			write_color(std::cout, pixelColor);
		}
	}

	std::clog << "\rDone!                      \n";

	return 0;
}