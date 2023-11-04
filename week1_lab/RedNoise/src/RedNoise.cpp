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
// Global camera position and rotation angles
glm::vec3 cameraPosition(0.0, 0.0, 4.0);
glm::mat3 cameraOrientation = glm::mat3(1.0f);  // identity matrix
float orbitAngle = 0.0f;
bool isOrbiting = false;

//std::vector<std::vector<float>> depthBuffer(WIDTH, std::vector<float>(HEIGHT, 0.0f));
std::vector<std::vector<double>> depthBuffer(WIDTH, std::vector<double>(HEIGHT, 0.0));

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
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            if (depth1 > depthBuffer[x1][y1]) {
                depthBuffer[x1][y1] = depth1;
                window.setPixelColour(x1, y1, packedColour);
            }
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


void drawFilledTriangle(DrawingWindow &window, CanvasTriangle triangle, Colour colour) {
    if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());
    if (triangle.v0().y > triangle.v2().y) std::swap(triangle.v0(), triangle.v2());
    if (triangle.v1().y > triangle.v2().y) std::swap(triangle.v1(), triangle.v2());

    // Calculate the slopes
    float slope1 = (triangle.v2().x - triangle.v0().x) / (triangle.v2().y - triangle.v0().y);
    float slope2 = (triangle.v1().x - triangle.v0().x) / (triangle.v1().y - triangle.v0().y);
    float slope3 = (triangle.v2().x - triangle.v1().x) / (triangle.v2().y - triangle.v1().y);

    // Draw upper half of the triangle
    for (float y = std::max(triangle.v0().y, 0.0f); y <= std::min(triangle.v1().y, static_cast<float>(HEIGHT - 1)); y++) {
        float proportion0 = (y - triangle.v0().y) / (triangle.v2().y - triangle.v0().y);
        float proportion1 = (y - triangle.v0().y) / (triangle.v1().y - triangle.v0().y);

        float x1 = triangle.v0().x + slope1 * (y - triangle.v0().y);
        float depth1 = (1 - proportion0) * triangle.v0().depth + proportion0 * triangle.v2().depth;

        float x2 = triangle.v0().x + slope2 * (y - triangle.v0().y);
        float depth2 = (1 - proportion1) * triangle.v0().depth + proportion1 * triangle.v1().depth;

        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(depth1, depth2);
        }

        CanvasPoint start(round(x1), y, depth1);
        CanvasPoint end(round(x2), y, depth2);
        drawLine(window, start, end, colour);
    }

    // Draw lower half of the triangle
    for (float y = std::max(triangle.v1().y, 0.0f); y <= std::min(triangle.v2().y, static_cast<float>(HEIGHT - 1)); y++) {
        float proportion0 = (y - triangle.v1().y) / (triangle.v2().y - triangle.v1().y);
        float proportion1 = (y - triangle.v0().y) / (triangle.v2().y - triangle.v0().y);

        float x1 = triangle.v1().x + slope3 * (y - triangle.v1().y);
        float depth1 = (1 - proportion0) * triangle.v1().depth + proportion0 * triangle.v2().depth;

        float x2 = triangle.v0().x + slope1 * (y - triangle.v0().y);
        float depth2 = (1 - proportion1) * triangle.v0().depth + proportion1 * triangle.v2().depth;

        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(depth1, depth2);
        }

        CanvasPoint start(round(x1), y, depth1);
        CanvasPoint end(round(x2), y, depth2);
        drawLine(window, start, end, colour);
    }
}


// task 5
void drawTexturedTriangle(DrawingWindow &window, CanvasTriangle triangle, TextureMap &texture) {
    //  Sort the triangle vertices by their y-values.
//    printf("Triangle vertices:\n");
//    printf("V0: (%f, %f), V1: (%f, %f), V2: (%f, %f)\n",
//           triangle.v0().x, triangle.v0().y,
//           triangle.v1().x, triangle.v1().y,
//           triangle.v2().x, triangle.v2().y);

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
//        printf("y: %d, xFull: %d, xPart: %d\n", y, xFull, xPart);

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



// keypress
void handleEvent(SDL_Event event, DrawingWindow &window) {
    float translationSpeed = 0.1f; //
//    float rotationSpeed = glm::radians(5.0f);
    float rotationSpeed = 0.005f;

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) { // go left
            cameraPosition.x -= translationSpeed;
            std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;
        } else if (event.key.keysym.sym == SDLK_RIGHT) { // go right
            cameraPosition.x += translationSpeed;
            std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;
        } else if (event.key.keysym.sym == SDLK_UP) { // go up
            cameraPosition.y += translationSpeed;
            std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;
        } else if (event.key.keysym.sym == SDLK_DOWN) { // go down
            cameraPosition.y -= translationSpeed;
            std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;
        } else if (event.key.keysym.sym == SDLK_w) { // move closer
            cameraPosition.z -= translationSpeed;
            std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;
        } else if (event.key.keysym.sym == SDLK_s) { // move far
            cameraPosition.z += translationSpeed;
            std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;

        } else if (event.key.keysym.sym == SDLK_a) { // rotate y axis

            glm::mat3 rotationMatrix = glm::mat3(
                    glm::cos(rotationSpeed), 0.0f, glm::sin(rotationSpeed),
                    0.0f, 1.0f, 0.0f,
                    -1*glm::sin(rotationSpeed), 0.0f, glm::cos(rotationSpeed)
            );
            cameraPosition = rotationMatrix * cameraPosition;
            std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;

        } else if (event.key.keysym.sym == SDLK_d) { // rotate y axis
            glm::mat3 rotationMatrix = glm::mat3(
                    cos(-rotationSpeed), 0.0f, sin(-rotationSpeed),
                    0.0f, 1.0f, 0.0f,
                    -sin(-rotationSpeed), 0.0f, cos(-rotationSpeed)
            );
            cameraPosition = rotationMatrix * cameraPosition;
            std::cout << "Camera Position: (" << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << ")" << std::endl;
        } else if (event.key.keysym.sym == SDLK_q) { // rotate x axis
            glm::mat3 rotationMatrix = glm::mat3(
                    1.0f, 0.0f, 0.0f,
                    0.0f, cos(rotationSpeed), -sin(rotationSpeed),
                    0.0f, sin(rotationSpeed), cos(rotationSpeed)
            );
            cameraPosition = rotationMatrix * cameraPosition;
        } else if (event.key.keysym.sym == SDLK_e) { // rotate x axis
            glm::mat3 rotationMatrix = glm::mat3(
                    1.0f, 0.0f, 0.0f,
                    0.0f, cos(-rotationSpeed), -sin(-rotationSpeed),
                    0.0f, sin(-rotationSpeed), cos(-rotationSpeed)
            );
            cameraPosition = rotationMatrix * cameraPosition;
        }
        else if (event.key.keysym.sym == SDLK_j) {  // Pan left
            glm::mat3 rotationMatrix = glm::mat3(
                    cos(rotationSpeed), 0.0f, sin(rotationSpeed),
                    0.0f, 1.0f, 0.0f,
                    -sin(rotationSpeed), 0.0f, cos(rotationSpeed)
            );
            cameraOrientation = rotationMatrix * cameraOrientation;
        } else if (event.key.keysym.sym == SDLK_l) {  // Pan right
            glm::mat3 rotationMatrix = glm::mat3(
                    cos(-rotationSpeed), 0.0f, sin(-rotationSpeed),
                    0.0f, 1.0f, 0.0f,
                    -sin(-rotationSpeed), 0.0f, cos(-rotationSpeed)
            );
            cameraOrientation = rotationMatrix * cameraOrientation;
        } else if (event.key.keysym.sym == SDLK_i) {  // Tilt up
            glm::mat3 rotationMatrix = glm::mat3(
                    1.0f, 0.0f, 0.0f,
                    0.0f, cos(rotationSpeed), -sin(rotationSpeed),
                    0.0f, sin(rotationSpeed), cos(rotationSpeed)
            );
            cameraOrientation = rotationMatrix * cameraOrientation;
        } else if (event.key.keysym.sym == SDLK_k) {  // Tilt down
            glm::mat3 rotationMatrix = glm::mat3(
                    1.0f, 0.0f, 0.0f,
                    0.0f, cos(-rotationSpeed), -sin(-rotationSpeed),
                    0.0f, sin(-rotationSpeed), cos(-rotationSpeed)
            );
            cameraOrientation = rotationMatrix * cameraOrientation;
        }else if (event.key.keysym.sym == SDLK_g) {
            isOrbiting = !isOrbiting;
        }


//        else if (event.key.keysym.sym == SDLK_u) {
//            CanvasTriangle randomTriangle = generateRandomTriangle(WIDTH, HEIGHT);
//            Colour randomColour = generateRandomColour();
//            drawTriangle(window, randomTriangle, randomColour);
//            std::cout << "DRAW TRIANGLE" << std::endl;
//        }
//        else if (event.key.keysym.sym == SDLK_f){
//            CanvasTriangle randomTriangle = generateRandomTriangle(WIDTH, HEIGHT);
//            Colour randomColour = generateRandomColour();
//            drawFilledTriangle(window, randomTriangle, randomColour);
//            std::cout << "DRAW FILLED TRIANGLE" << std::endl;
//        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}




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
//                std::cout << std::setprecision(10) << "Loaded vertex: (" << tokens[1] << ", " << tokens[2] << ", " << tokens[3] << ")" << std::endl;
//                std::cout << std::setprecision(10) << "Loaded vertex: (" << (stof(tokens[1])* scale , stof(tokens[2])* scale , stof(tokens[3])* scale ) << ")" << std::endl;
//                std::cout << std::setprecision(10) << "vertex: (" << vertex.x <<"," << vertex.y << ","<< vertex.z << ")" << std::endl;

            }
        } else if (tokens[0] == "usemtl") {
            currentColour = palette[tokens[1]];
        } else if (tokens[0] == "f") {
            ModelTriangle triangle(vertices[stoi(tokens[1]) - 1], vertices[stoi(tokens[2]) - 1], vertices[stoi(tokens[3]) - 1], currentColour);
            triangles.push_back(triangle);
            if (triangles.size() <= 100) {
//                std::cout << "Loaded triangle with vertices: \n";
                for (const auto& vertex : triangle.vertices) {
//                    std::cout << "\t(" << tokens[1] << ", " << tokens[2] << ", " << tokens[3] << ")\n";
                }}
        }
    }

    objFile.close();
    return triangles;
}

const float IMAGE_PLANE_SCALING = 240.0f;  // The scaling factor

CanvasPoint getCanvasIntersectionPoint(const glm::vec3 &cameraPosition, const glm::vec3 &vertexPosition, float focalLength) {
//    float xi = vertexPosition.x - cameraPosition.x;
//    float yi = vertexPosition.y - cameraPosition.y;
//    float zi = cameraPosition.z - vertexPosition.z;
//
//    float ui = focalLength * (xi / zi) * IMAGE_PLANE_SCALING + WIDTH / 2 ;
//    float vi = focalLength * (yi / zi) * IMAGE_PLANE_SCALING + HEIGHT / 2 ;
//    std::cout << "ui,vi point: (" << ui << ", " << vi << ")" << std::endl;

    glm::vec3 direction = cameraOrientation * (vertexPosition - cameraPosition);
    float xi = direction.x;
    float yi = direction.y;
    float zi = -direction.z;  // We assume the camera looks into the negative Z direction

    float ui = focalLength * (xi / zi) * IMAGE_PLANE_SCALING + WIDTH / 2;
    float vi = focalLength * (yi / zi) * IMAGE_PLANE_SCALING + HEIGHT / 2;

    return CanvasPoint(ui, vi, 1/zi);
}

void lookAt(const glm::vec3 &point) {
    glm::vec3 forward = glm::normalize(cameraPosition - point );  // The direction from the point to the camera

    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0,1,0), -forward));  // Compute the right direction
    glm::vec3 up = glm::normalize(glm::cross(right, forward));  // Compute the up direction

//    forward.x*=-1;
    // Set the camera orientation
    cameraOrientation[0] = right;
    cameraOrientation[1] = up;
    cameraOrientation[2] = forward;

    std::cout << "Right: (" << right.x << ", " << right.y << ", " << right.z << ")" << std::endl;
    std::cout << "Up: (" << up.x << ", " << up.y << ", " << up.z << ")" << std::endl;
    std::cout << "Forward: (" << -forward.x << ", " << -forward.y << ", " << -forward.z << ")" << std::endl;

}

void draw(DrawingWindow &window) {
    window.clearPixels();
    if (isOrbiting) {
        orbitAngle += 0.005f; // Adjust this value to control the speed of the orbit
        glm::mat3 rotationMatrix = glm::mat3(
                cos(orbitAngle), 0.0f, sin(orbitAngle),
                0.0f, 1.0f, 0.0f,
                -sin(orbitAngle), 0.0f, cos(orbitAngle)
        );

        // Choose a suitable distance from the center of the Cornell Box (adjust as needed)
        glm::vec3 initialCameraPosition(0.0f, 0.0f, 4.0f);
        cameraPosition = rotationMatrix * initialCameraPosition;
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat3 mirrorMatrix = glm::mat3(-1.0f, 0.0f, 0.0f,
                                           0.0f, 1.0f, 0.0f,
                                           0.0f, 0.0f, 1.0f);

        cameraOrientation = mirrorMatrix * cameraOrientation;
    }

    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            depthBuffer[x][y] = 0.0f;

//    glm::vec3 cameraPosition(0, 0, 4.0);
    float focalLength = 1.5;

    std::vector<ModelTriangle> modelTriangles = loadOBJWithMaterials("/Users/frederick_zou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cornell-box.obj", "/Users/frederick_zou/Desktop/ComputerGraphics2023/week1_lab/RedNoise/cornell-box.mtl", 0.35);
//    std::cout << "Number of triangles: " << modelTriangles.size() << std::endl;

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
    while (true) {
        // We MUST poll for events - otherwise the window will freeze!
        if (window.pollForInputEvents(event)) handleEvent(event, window);
        draw(window);
        // Need to render the frame at the end, or nothing actually gets shown on the screen!
        window.renderFrame();
    }
}


