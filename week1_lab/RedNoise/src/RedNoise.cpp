#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <Colour.h>
#include <CanvasPoint.h>
#include <TextureMap.h>
#define WIDTH 320
#define HEIGHT 240


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
void drawTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour) {
    drawLine(window, triangle.v0(), triangle.v1(), colour);
    drawLine(window, triangle.v1(), triangle.v2(), colour);
    drawLine(window, triangle.v0(), triangle.v2(), colour);
}

// filled triangle:
void drawFilledTriangle(DrawingWindow &window, CanvasTriangle &triangle, const Colour &colour) {
    // Sort the vertices by y-coordinate
    std::vector<CanvasPoint> vertices = {triangle.v0(), triangle.v1(), triangle.v2()};
    std::sort(vertices.begin(), vertices.end(), [](const CanvasPoint &a, const CanvasPoint &b) {
        return a.y < b.y;
    });

    // slopes of the edges
    float slope1 = (vertices[1].x - vertices[0].x) / (vertices[1].y - vertices[0].y);
    float slope2 = (vertices[2].x - vertices[0].x) / (vertices[2].y - vertices[0].y);
    float slope3 = (vertices[2].x - vertices[1].x) / (vertices[2].y - vertices[1].y);

    // Iterate over the y-values of the triangle
    for (int y = (int)vertices[0].y; y < (int)vertices[1].y; y++) {
        int x1 = vertices[0].x + slope1 * (y - vertices[0].y);
        int x2 = vertices[0].x + slope2 * (y - vertices[0].y);
        for (int x = x1; x < x2; x++) {
            window.setPixelColour(x, y, (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue);
        }
    }

    for (int y = (int)vertices[1].y; y < (int)vertices[2].y; y++) {
        int x1 = vertices[1].x + slope3 * (y - vertices[1].y);
        int x2 = vertices[0].x + slope2 * (y - vertices[0].y);
        for (int x = x1; x < x2; x++) {
            window.setPixelColour(x, y, (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue);
        }
    }
    drawTriangle(window, triangle, Colour(255, 255, 255)); // white outline
}

// task 5
void drawTexturedTriangle(DrawingWindow &window, CanvasTriangle triangle, TextureMap &texture) {
    //  Sort the triangle vertices by their y-values.
    printf("Triangle vertices:\n");
    printf("V0: (%f, %f), V1: (%f, %f), V2: (%f, %f)\n",
           triangle.v0().x, triangle.v0().y,
           triangle.v1().x, triangle.v1().y,
           triangle.v2().x, triangle.v2().y);

    std::vector<CanvasPoint> vertices = {triangle.v0(), triangle.v1(), triangle.v2()};
    std::sort(vertices.begin(), vertices.end(), [](const CanvasPoint &a, const CanvasPoint &b) {
        return a.y < b.y;
    });

    CanvasPoint top = vertices[0];
    CanvasPoint mid = vertices[1];
    CanvasPoint bot = vertices[2];
    bool flatBottom = false;

    // Split triangle if necessary (just like before).
    if (top.y != mid.y && mid.y != bot.y) {
        float alpha = (mid.y - top.y) / (bot.y - top.y);
        float newX = top.x + alpha * (bot.x - top.x);

        CanvasPoint newVert(newX, mid.y);
        newVert.texturePoint.x = top.texturePoint.x + alpha * (bot.texturePoint.x - top.texturePoint.x);
        newVert.texturePoint.y = top.texturePoint.y + alpha * (bot.texturePoint.y - top.texturePoint.y);

        drawTexturedTriangle(window, CanvasTriangle(top, mid, newVert), texture);
        drawTexturedTriangle(window, CanvasTriangle(bot, mid, newVert), texture);
        return;

    }else if (top.y == mid.y) {
        // Determine if we're drawing a flat-bottom or flat-top triangle.
        flatBottom = true;
    }


    for (int y = (int)top.y; y <= (int)bot.y; y++) {
        float alphaFull = (bot.y == top.y) ? 0 : (y - top.y) / (bot.y - top.y);
        float alphaPart;
        if (y <= mid.y) {
            // This is the top triangle
            alphaPart = (mid.y == top.y) ? 0 : (y - top.y) / (mid.y - top.y);
        } else {
            // This is the bottom triangle
            alphaPart = (mid.y == bot.y) ? 0 : (y - mid.y) / (bot.y - mid.y);
        }


        int xFull = top.x + alphaFull * (bot.x - top.x);
        int xPart = flatBottom ? mid.x + alphaPart * (bot.x - mid.x) : top.x + alphaPart * (mid.x - top.x);
        printf("y: %d, xFull: %d, xPart: %d\n", y, xFull, xPart);

        float texXFull = top.texturePoint.x + alphaFull * (bot.texturePoint.x - top.texturePoint.x);
        float texXPart = flatBottom ? mid.texturePoint.x + alphaPart * (bot.texturePoint.x - mid.texturePoint.x) : top.texturePoint.x + alphaPart * (mid.texturePoint.x - top.texturePoint.x);

        float texYFull = top.texturePoint.y + alphaFull * (bot.texturePoint.y - top.texturePoint.y);
        float texYPart = flatBottom ? mid.texturePoint.y + alphaPart * (bot.texturePoint.y - mid.texturePoint.y) : top.texturePoint.y + alphaPart * (mid.texturePoint.y - top.texturePoint.y);

        if (xFull > xPart) {
            std::swap(xFull, xPart);
            std::swap(texXFull, texXPart);
            std::swap(texYFull, texYPart);

        }

        for (int x = xFull; x <= xPart; x++) {
            float alphaSpan = (x - xFull) / (float)(xPart - xFull);
            float texX = texXFull + alphaSpan * (texXPart - texXFull);
            float texY = texYFull + alphaSpan * (texYPart - texYFull);

            int texturePixelIndex = ((int)texY * texture.width) + (int)texX;
            if (texturePixelIndex >= 0 && texturePixelIndex < texture.width * texture.height) {
                uint32_t texel = texture.pixels[texturePixelIndex];

                // Draw the texel onto the canvas (replace with your actual drawing method).
                window.setPixelColour(x, y, texel);
            }
        }
    }
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
        else if (event.key.keysym.sym == SDLK_f){
            CanvasTriangle randomTriangle = generateRandomTriangle(WIDTH, HEIGHT);
            Colour randomColour = generateRandomColour();
            drawFilledTriangle(window, randomTriangle, randomColour);
            std::cout << "DRAW FILLED TRIANGLE" << std::endl;
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}


int main(int argc, char *argv[]) {
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;

    // path to the ppm file:
    TextureMap texture("texture.ppm");

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
//// random triangle
//    CanvasTriangle randomTriangle = generateRandomTriangle(WIDTH, HEIGHT);
//    Colour randomColour = generateRandomColour();
//    drawTriangle(window, randomTriangle, randomColour);

//    while (true) {
//        // We MUST poll for events - otherwise the window will freeze !
//        if (window.pollForInputEvents(event)) handleEvent(event, window);
////        draw(window);
//
//        CanvasPoint v0(160, 10);
//        v0.texturePoint = TexturePoint(195, 5);
//        CanvasPoint v1(300, 230);
//        v1.texturePoint = TexturePoint(395, 380);
//        CanvasPoint v2(10, 150);
//        v2.texturePoint = TexturePoint(65, 330);
//
//        CanvasTriangle triangle(v0, v1, v2);
//        drawTexturedTriangle(window, triangle, texture);
//
//        // Need to render the frame at the end, or nothing actually gets shown on the screen !
//        window.renderFrame();
//    }
    bool hasRendered = false;

    while (true) {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) {
            handleEvent(event, window);
        }

        // Only render once.
        if (!hasRendered) {
            CanvasPoint v0(160, 10);
            v0.texturePoint = TexturePoint(195, 5);
            CanvasPoint v1(300, 230);
            v1.texturePoint = TexturePoint(395, 380);
            CanvasPoint v2(10, 150);
            v2.texturePoint = TexturePoint(65, 330);

            CanvasTriangle triangle(v0, v1, v2);
            drawTexturedTriangle(window, triangle, texture);

            // Need to render the frame at the end, or nothing actually gets shown on the screen !
            window.renderFrame();

            hasRendered = true;
        }
    }

}

