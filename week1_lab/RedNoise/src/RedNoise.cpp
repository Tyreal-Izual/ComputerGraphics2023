#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#define WIDTH 320
#define HEIGHT 240

//void draw(DrawingWindow &window) {
//	window.clearPixels();
//	for (size_t y = 0; y < window.height; y++) {
//		for (size_t x = 0; x < window.width; x++) {
//			float red = rand() % 256;
//			float green = 0.0;
//			float blue = 0.0;
//			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
//			window.setPixelColour(x, y, colour);
//		}
//	}
//}

std::vector<float> interpolateSingleFloats(double from, double to, int numberOfValues) {
    std::vector<float> values;
    double step = (to - from)/(numberOfValues - 1);
    for(int j = 0; j < numberOfValues; j++) {
        values.push_back(from + j * step);
    }
    return values;
}

//gray drawing:
//void draw(DrawingWindow &window) {
//    window.clearPixels();
//    // Generate the list of grayscale values.
//    std::vector<float> grayscales = interpolateSingleFloats(255, 0, window.width);
//
//    for (size_t y=0; y < window.height; y++) {
//        for (size_t x=0; x < window.width; x++){
//            // Obtain grayscale value from the pre-computed list.
//            float gray = grayscales[x];
//            uint32_t packed = (255 << 24) + (int(gray) << 16) + (int(gray) << 8) + int(gray);
//
//            window.setPixelColour(x, y, packed);
//        }
//    }
//
//}

std::vector <glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues) {
    std::vector<glm::vec3> values;

    std::vector<float> xs = interpolateSingleFloats(from.x, to.x, numberOfValues);
    std::vector<float> ys = interpolateSingleFloats(from.y, to.y, numberOfValues);
    std::vector<float> zs = interpolateSingleFloats(from.z, to.z, numberOfValues);

    for (int i = 0; i < numberOfValues; i++) {
        values.push_back(glm::vec3(xs[i], ys[i], zs[i]));
    }
    return values;
}

//RGB drawing:
void draw(DrawingWindow &window) {
    window.clearPixels();
// Given:
    glm::vec3 topLeft(255, 0, 0);        // red
    glm::vec3 topRight(0, 0, 255);       // blue
    glm::vec3 bottomRight(0, 255, 0);    // green
    glm::vec3 bottomLeft(255, 255, 0);   // yellow

    std::vector<glm::vec3> leftColors = interpolateThreeElementValues(topLeft, bottomLeft, window.height);
    std::vector<glm::vec3> rightColors = interpolateThreeElementValues(topRight, bottomRight, window.height);

    for(int y = 0; y < window.height; y++){
        std::vector<glm::vec3> rowColors = interpolateThreeElementValues(leftColors[y], rightColors[y], window.width);

        for (int x = 0; x < window.width; x++) {
            glm::vec3 color = rowColors[x];

            uint32_t packedRGB = (255 << 24) + ((int)color.r << 16) + ((int)color.g << 8) + (int)color.b;
            window.setPixelColour(x, y, packedRGB);
        }
    }
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}



int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;

    // test function for interpolateSingleFloats
    std::vector<float> result;
    result = interpolateSingleFloats(2.2, 8.5, 7);
    for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
    std::cout << std::endl;

    // test function for interpolateThreeElementValues
    glm::vec3 from(1.0, 4.0, 9.2);
    glm::vec3 to(4.0, 1.0, 9.8);
    std::vector<glm::vec3> results = interpolateThreeElementValues(from, to, 4);
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << "(" << results[i].x << ", " << results[i].y << ", " << results[i].z << ")" << std::endl;
    }

        while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
