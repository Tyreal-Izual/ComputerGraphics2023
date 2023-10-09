#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
// for line drawing:
#include <Colour.h>
#include <CanvasPoint.h>
// width and height
#define WIDTH 320
#define HEIGHT 240
//#define WIDTH 1920
//#define HEIGHT 1080


// RedNoise
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

//gray
std::vector<float> interpolateSingleFloats(double from, double to, int numberOfValues) {
    std::vector<float> values;
    double step = (to - from)/(numberOfValues - 1);
    for(int j = 0; j < numberOfValues; j++) {
        values.push_back(from + j * step);
    }
    return values;
}

//// gray drawing:
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

// RGB
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

////RGB drawing:
//void draw(DrawingWindow &window) {
//    window.clearPixels();
//// Given:
//    glm::vec3 topLeft(255, 0, 0);        // red
//    glm::vec3 topRight(0, 0, 255);       // blue
//    glm::vec3 bottomRight(0, 255, 0);    // green
//    glm::vec3 bottomLeft(255, 255, 0);   // yellow
//
//    std::vector<glm::vec3> leftColors = interpolateThreeElementValues(topLeft, bottomLeft, window.height);
//    std::vector<glm::vec3> rightColors = interpolateThreeElementValues(topRight, bottomRight, window.height);
//
//    for(int y = 0; y < window.height; y++){
//        std::vector<glm::vec3> rowColors = interpolateThreeElementValues(leftColors[y], rightColors[y], window.width);
//
//        for (int x = 0; x < window.width; x++) {
//            glm::vec3 color = rowColors[x];
//
//            uint32_t packedRGB = (255 << 24) + ((int)color.r << 16) + ((int)color.g << 8) + (int)color.b;
//            window.setPixelColour(x, y, packedRGB);
//        }
//    }
//}

// Line Drawing using Bresenham's algorithm:
// here, we have window, start and end point, and color of the line.
void drawLine(DrawingWindow &window, const CanvasPoint &start, const CanvasPoint &end, const Colour &colour) {
    // x1,y1,x2,y2 for start and end x,y.
    int x1 = start.x;
    int y1 = start.y;
    int x2 = end.x;
    int y2 = end.y;

    // dx and dy for absolute difference in x and y.
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    // sx and sy is the direction of line, if the line is going to the right or left, up or down.
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    uint32_t packedColour = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;

    while (true) {
        window.setPixelColour(x1, y1, packedColour);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

//void draw(DrawingWindow &window) {
//    window.clearPixels();
//
//    Colour white(255, 255, 255);
//
//    CanvasPoint top_left(0, 0);
//    CanvasPoint center(WIDTH/2, HEIGHT/2);
//    CanvasPoint top_right(WIDTH-1, 0);
//
//    drawLine(window, top_left, center, white);
//    drawLine(window, top_right, center, white);
//    drawLine(window, CanvasPoint(WIDTH/2, 0), CanvasPoint(WIDTH/2, HEIGHT-1), white);
//    drawLine(window, CanvasPoint(WIDTH/3, HEIGHT/2), CanvasPoint(2*WIDTH/3, HEIGHT/2), white);
//}

// random canvas triangle:
void drawTriangle(DrawingWindow &window,CanvasTriangle triangle, Colour colour) {
    drawLine(window, triangle.v0(), triangle.v1(), colour);
    drawLine(window, triangle.v1(), triangle.v2(), colour);
    drawLine(window, triangle.v0(), triangle.v2(), colour);
}

CanvasTriangle generateRandomTriangle(int canvasWidth, int canvasHeight) {
    CanvasPoint v0(rand() % canvasWidth, rand() % canvasHeight);
    CanvasPoint v1(rand() % canvasWidth, rand() % canvasHeight);
    CanvasPoint v2(rand() % canvasWidth, rand() % canvasHeight);
    return CanvasTriangle(v0, v1, v2);
}

Colour generateRandomColour() {
    uint8_t r = rand() % 256;
    uint8_t g = rand() % 256;
    uint8_t b = rand() % 256;
    return Colour(r, g, b);
}

// keypress
void handleEvent(SDL_Event event, DrawingWindow &window) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
        else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
        else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
        else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
        else if (event.key.keysym.sym == SDLK_u) {
            CanvasTriangle randomTriangle = generateRandomTriangle(WIDTH, HEIGHT);
            Colour randomColour = generateRandomColour();
            drawTriangle(window, randomTriangle, randomColour);
            std::cout << "DRAW TRIANGLE" << std::endl;
        }
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

//    CanvasTriangle randomTriangle = generateRandomTriangle(WIDTH, HEIGHT);
//    Colour randomColour = generateRandomColour();
//    drawTriangle(window, randomTriangle, randomColour);

    while (true) {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event, window);
//        draw(window);


        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
    }
}

