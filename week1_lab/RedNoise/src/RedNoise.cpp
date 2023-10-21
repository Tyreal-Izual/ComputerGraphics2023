#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <Colour.h>
#include <CanvasPoint.h>
#include <TextureMap.h>
#include <ModelTriangle.h>
#include <unordered_map>
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

std::vector<std::vector<float>> depthBuffer(WIDTH, std::vector<float>(HEIGHT, 0.0f));

// Line Drawing using Bresenham's algorithm:
// here, we have window, start and end point, and color of the line.
void drawLine(DrawingWindow &window, const CanvasPoint &start, const CanvasPoint &end, const Colour &colour) {
    // x1,y1,x2,y2 for start and end x,y.
    int x1 = start.x;
    int y1 = start.y;
    float depth1 = start.depth;

    int x2 = end.x;
    int y2 = end.y;
    float depth2 = end.depth;

    // dx and dy for absolute difference in x and y.
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    // sx and sy is the direction of line, if the line is going to the right or left, up or down.
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    float depthSlope = (depth2 - depth1) / std::max(dx, dy);

    uint32_t packedColour = (255 << 24) + (colour.red << 16) + (colour.green << 8) + colour.blue;

    while (true) {
        if (depth1 > depthBuffer[x1][y1]) {
            depthBuffer[x1][y1] = depth1;
            window.setPixelColour(x1, y1, packedColour);
        }

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
            depth1 += depthSlope;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
            depth1 += depthSlope;
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



void drawFilledTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour) {
    // Sort the vertices by y-coordinates
    if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());
    if (triangle.v0().y > triangle.v2().y) std::swap(triangle.v0(), triangle.v2());
    if (triangle.v1().y > triangle.v2().y) std::swap(triangle.v1(), triangle.v2());

    // Calculate the slopes
    float slope1 = (triangle.v2().x - triangle.v0().x) / (triangle.v2().y - triangle.v0().y);
    float slope2 = (triangle.v1().x - triangle.v0().x) / (triangle.v1().y - triangle.v0().y);
    float slope3 = (triangle.v2().x - triangle.v1().x) / (triangle.v2().y - triangle.v1().y);

    // Draw horizontal lines
    for (float y = triangle.v0().y; y <= triangle.v1().y; y++) {
        float proportion = (y - triangle.v0().y) / (triangle.v1().y - triangle.v0().y);
        float x1 = triangle.v0().x + slope1 * (y - triangle.v0().y);
        float depth1 = triangle.v0().depth + proportion * (triangle.v1().depth - triangle.v0().depth);

        float x2 = triangle.v0().x + slope2 * (y - triangle.v0().y);
        float depth2 = triangle.v0().depth;  // Since it's the same vertex

//        if (x1 > x2) std::swap(x1, x2);
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(depth1, depth2);
        }
        CanvasPoint start(round(x1), y, depth1);
        CanvasPoint end(round(x2), y, depth2);

//        CanvasPoint start(round(x1), y);
//        CanvasPoint end(round(x2), y);
        drawLine(window, start, end, colour);
    }

    for (float y = triangle.v1().y; y <= triangle.v2().y; y++) {
        float proportion = (y - triangle.v1().y) / (triangle.v2().y - triangle.v1().y);
        float x1 = triangle.v0().x + slope1 * (y - triangle.v0().y);
        float depth1 = triangle.v0().depth;  // Same vertex

        float x2 = triangle.v1().x + slope3 * (y - triangle.v1().y);
        float depth2 = triangle.v1().depth + proportion * (triangle.v2().depth - triangle.v1().depth);

//        if (x1 > x2) std::swap(x1, x2);
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(depth1, depth2);
        }
        CanvasPoint start(round(x1), y, depth1);
        CanvasPoint end(round(x2), y, depth2);
//        CanvasPoint start(round(x1), y);
//        CanvasPoint end(round(x2), y);
        drawLine(window, start, end, colour);
    }

    // Drawing white stroke over the filled triangle
    Colour whiteColour(255, 255, 255);
    drawTriangle(window, triangle, whiteColour);
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


//// week4 Task1
//std::vector<ModelTriangle> loadOBJ(const std::string& filename, float scalingFactor) {
//    std::ifstream infile(filename);
//    if (!infile.is_open()) {
//        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
//        return {};
//    }
//
//    std::string line;
//    std::vector<glm::vec3> vertices;
//    std::vector<ModelTriangle> triangles;
//
//    while (std::getline(infile, line)) {
//        std::vector<std::string> tokens = split(line, ' ');
//        if (tokens.empty()) continue;
//
//        if (tokens[0] == "v") {
//            // Parse vertex
//            glm::vec3 vertex(std::stof(tokens[1]) * scalingFactor,
//                             std::stof(tokens[2]) * scalingFactor,
//                             std::stof(tokens[3]) * scalingFactor);
//            vertices.push_back(vertex);
//        } else if (tokens[0] == "f") {
//            // Parse face
//            int v1 = std::stoi(tokens[1]) - 1;  // Convert 1-based to 0-based index
//            int v2 = std::stoi(tokens[2]) - 1;
//            int v3 = std::stoi(tokens[3]) - 1;
//            ModelTriangle triangle(vertices[v1], vertices[v2], vertices[v3], Colour(255,255,255)); // Default colour
//            triangles.push_back(triangle);
//        }
//    }
//
//    infile.close();
//    return triangles;
//}


std::unordered_map<std::string, Colour> loadMaterials(const std::string& filename) {
    std::unordered_map<std::string, Colour> palette;
    std::ifstream mtlFile(filename);

    if (!mtlFile.is_open()) {
        std::cerr << "Failed to open materials file: " << filename << std::endl;
        return palette;
    }

    std::string line;
    std::string currentMaterialName;
    while (getline(mtlFile, line)) {
        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "newmtl") {
            ss >> currentMaterialName;
        } else if (keyword == "Kd") {
            float r, g, b;
            ss >> r >> g >> b;
            r *= 255;
            g *= 255;
            b *= 255;
            palette[currentMaterialName] = Colour(currentMaterialName, static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
        }
    }

    mtlFile.close();
    return palette;
}


// Assuming utils.cpp provides the split function
std::vector<ModelTriangle> loadOBJWithMaterials(const std::string& objFilename, const std::string& mtlFilename, float scale) {
    std::unordered_map<std::string, Colour> palette = loadMaterials(mtlFilename);
    std::vector<ModelTriangle> triangles;
    std::ifstream objFile(objFilename);

    if (!objFile.is_open()) {
        std::cerr << "Failed to open geometry file: " << objFilename << std::endl;
        return triangles;
    }

    std::vector<glm::vec3> vertices;
    Colour currentColour; // This will hold the current colour being used by the triangles

    std::string line;
    while (getline(objFile, line)) {
        std::vector<std::string> tokens = split(line, ' ');
        if (tokens.empty()) continue;

        if (tokens[0] == "v") {
            glm::vec3 vertex(stof(tokens[1])* scale , stof(tokens[2])* scale , stof(tokens[3])* scale );
            vertices.push_back(vertex);
            // Print out the first 5 vertices for checking
            if (vertices.size() <= 100) {
                std::cout << std::setprecision(10) << "Loaded vertex: (" << tokens[1] << ", " << tokens[2] << ", " << tokens[3] << ")" << std::endl;
                std::cout << std::setprecision(10) << "Loaded vertex: (" << (stof(tokens[1])* scale , stof(tokens[2])* scale , stof(tokens[3])* scale ) << ")" << std::endl;
                std::cout << std::setprecision(10) << "vertex: (" << vertex.x <<"," << vertex.y << ","<< vertex.z << ")" << std::endl;

            }
        } else if (tokens[0] == "usemtl") {
            currentColour = palette[tokens[1]];
        } else if (tokens[0] == "f") {
            ModelTriangle triangle(vertices[stoi(tokens[1]) - 1], vertices[stoi(tokens[2]) - 1], vertices[stoi(tokens[3]) - 1], currentColour);
            triangles.push_back(triangle);
            if (triangles.size() <= 100) {
                std::cout << "Loaded triangle with vertices: \n";
                for (const auto& vertex : triangle.vertices) {
                    std::cout << "\t(" << tokens[1] << ", " << tokens[2] << ", " << tokens[3] << ")\n";
                }}
        }
    }

    objFile.close();
    return triangles;
}

const float IMAGE_PLANE_SCALING = 240.0f;  // The scaling factor

CanvasPoint getCanvasIntersectionPoint(const glm::vec3 &cameraPosition, const glm::vec3 &vertexPosition, float focalLength) {
    float xi = vertexPosition.x - cameraPosition.x;
    float yi = vertexPosition.y - cameraPosition.y;
    float zi = cameraPosition.z - vertexPosition.z;

    float ui = focalLength * (xi / zi) * IMAGE_PLANE_SCALING + WIDTH / 2 ;
    float vi = focalLength * (yi / zi) * IMAGE_PLANE_SCALING + HEIGHT / 2 ;
    std::cout << "ui,vi point: (" << ui << ", " << vi << ")" << std::endl;

    return CanvasPoint(ui, vi, 1/zi);
}



void draw(DrawingWindow &window) {
    window.clearPixels();

    glm::vec3 cameraPosition(0, 0, 4.0);
    float focalLength = 1.5;

    std::vector<ModelTriangle> modelTriangles = loadOBJWithMaterials("/Users/frederick_zou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cornell-box.obj", "/Users/frederick_zou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cornell-box.mtl", 0.35);
    std::cout << "Number of triangles: " << modelTriangles.size() << std::endl;

//    Colour white(255, 255, 255); // For wireframe

    for (const ModelTriangle &modelTriangle : modelTriangles) {
        CanvasPoint v0 = getCanvasIntersectionPoint(cameraPosition, modelTriangle.vertices[0], focalLength);
        CanvasPoint v1 = getCanvasIntersectionPoint(cameraPosition, modelTriangle.vertices[1], focalLength);
        CanvasPoint v2 = getCanvasIntersectionPoint(cameraPosition, modelTriangle.vertices[2], focalLength);

        // Scale, flip, etc. if needed (similar to what you did for individual vertices)
        // Here's the flip as an example:
        v0.y = HEIGHT - v0.y;
        v1.y = HEIGHT - v1.y;
        v2.y = HEIGHT - v2.y;

        CanvasTriangle canvasTriangle(v0, v1, v2);
        drawFilledTriangle(window, canvasTriangle, modelTriangle.colour); // Draw the filled triangle
    }
}


int main(int argc, char *argv[]) {
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;

    // Poll for events
    if (window.pollForInputEvents(event)) {
        handleEvent(event, window);
    }

    // Draw and render the frame
    draw(window);
    window.renderFrame();

    // Pause and wait for user input
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return 0;
}

//int main(int argc, char *argv[]) {
//    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
//    SDL_Event event;
//    while (true) {
//        // We MUST poll for events - otherwise the window will freeze !
//        if (window.pollForInputEvents(event)) handleEvent(event, window);
//        draw(window);
//        // Need to render the frame at the end, or nothing actually gets shown on the screen !
//        window.renderFrame();
//    }
//}



//int main(int argc, char *argv[]) {
//    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
//    SDL_Event event;
//
//    // path to the ppm file:
//    TextureMap texture("texture.ppm");
//
//    // test function for interpolateSingleFloats
//    std::vector<float> result;
//    result = interpolateSingleFloats(2.2, 8.5, 7);
//    for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
//    std::cout << std::endl;
//
//    // test function for interpolateThreeElementValues
//    glm::vec3 from(1.0, 4.0, 9.2);
//    glm::vec3 to(4.0, 1.0, 9.8);
//    std::vector<glm::vec3> results = interpolateThreeElementValues(from, to, 4);
//    for (size_t i = 0; i < results.size(); i++) {
//        std::cout << "(" << results[i].x << ", " << results[i].y << ", " << results[i].z << ")" << std::endl;
//    }
////// random triangle
////    CanvasTriangle randomTriangle = generateRandomTriangle(WIDTH, HEIGHT);
////    Colour randomColour = generateRandomColour();
////    drawTriangle(window, randomTriangle, randomColour);
//
////    while (true) {
////        // We MUST poll for events - otherwise the window will freeze !
////        if (window.pollForInputEvents(event)) handleEvent(event, window);
//////        draw(window);
////
////        CanvasPoint v0(160, 10);
////        v0.texturePoint = TexturePoint(195, 5);
////        CanvasPoint v1(300, 230);
////        v1.texturePoint = TexturePoint(395, 380);
////        CanvasPoint v2(10, 150);
////        v2.texturePoint = TexturePoint(65, 330);
////
////        CanvasTriangle triangle(v0, v1, v2);
////        drawTexturedTriangle(window, triangle, texture);
////
////        // Need to render the frame at the end, or nothing actually gets shown on the screen !
////        window.renderFrame();
////    }
//    bool hasRendered = false;
//
//    while (true) {
//        // We MUST poll for events - otherwise the window will freeze !
//        if (window.pollForInputEvents(event)) {
//            handleEvent(event, window);
//        }
//
//        // Only render once.
//        if (!hasRendered) {
//            CanvasPoint v0(160, 10);
//            v0.texturePoint = TexturePoint(195, 5);
//            CanvasPoint v1(300, 230);
//            v1.texturePoint = TexturePoint(395, 380);
//            CanvasPoint v2(10, 150);
//            v2.texturePoint = TexturePoint(65, 330);
//
//            CanvasTriangle triangle(v0, v1, v2);
//            drawTexturedTriangle(window, triangle, texture);
//
//            // Need to render the frame at the end, or nothing actually gets shown on the screen !
//            window.renderFrame();
//
//            hasRendered = true;
//        }
//    }
//
//}
//
