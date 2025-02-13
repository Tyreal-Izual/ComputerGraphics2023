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
#include <RayTriangleIntersection.h>
#include <algorithm>
#include <random>
#define WIDTH 320
#define HEIGHT 240
// Global camera position and rotation angles
glm::vec3 cameraPosition(0.0, 0.0, 4.0);
glm::mat3 cameraOrientation = glm::mat3(1.0f);  // identity matrix
glm::vec3 lightPosition = glm::vec3(0.0f, 0.5f, 0.5f);  // x, y, and z with the actual coordinates
float orbitAngle = 0.0f;
bool isOrbiting = false;

std::vector<std::vector<double>> depthBuffer(WIDTH, std::vector<double>(HEIGHT, 0.0));

enum class RenderMode {
    Wireframe,
    Rasterisation,
    RayTracing
};

RenderMode currentMode = RenderMode::Rasterisation;

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
    // Correctly order the triangle vertices by y-coordinate
    CanvasPoint temp;
    if (triangle.v0().y > triangle.v1().y) std::swap(triangle.v0(), triangle.v1());
    if (triangle.v0().y > triangle.v2().y) std::swap(triangle.v0(), triangle.v2());
    if (triangle.v1().y > triangle.v2().y) std::swap(triangle.v1(), triangle.v2());

    // Calculate slopes, checking for division by zero
    float dy1 = triangle.v2().y - triangle.v0().y;
    float dy2 = triangle.v1().y - triangle.v0().y;
    float dy3 = triangle.v2().y - triangle.v1().y;

    float slope1 = dy1 == 0 ? 0 : (triangle.v2().x - triangle.v0().x) / dy1;
    float slope2 = dy2 == 0 ? 0 : (triangle.v1().x - triangle.v0().x) / dy2;
    float slope3 = dy3 == 0 ? 0 : (triangle.v2().x - triangle.v1().x) / dy3;


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

                // Draw the texel onto the canvas.
                window.setPixelColour(x, y, texel);
            }
        }
    }
}



// keypress
void handleEvent(SDL_Event event, DrawingWindow &window) {
    float translationSpeed = 0.1f; //
    float rotationSpeed = 0.05f;

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

        else if (event.key.keysym.sym == SDLK_1) { // Switch to wireframe mode
            currentMode = RenderMode::Wireframe;
            std::cout << "Switched to wireframe mode." << std::endl;
        } else if (event.key.keysym.sym == SDLK_2) { // Switch to rasterisation mode
            currentMode = RenderMode::Rasterisation;
            std::cout << "Switched to rasterisation mode." << std::endl;
        } else if (event.key.keysym.sym == SDLK_3) { // Switch to ray tracing mode
            currentMode = RenderMode::RayTracing;
            std::cout << "Switched to ray tracing mode." << std::endl;
        } else if (event.key.keysym.sym == SDLK_5) {
            lightPosition.y += 0.1f;  // Move light up
        } else if(event.key.keysym.sym == SDLK_6){
            lightPosition.y -= 0.1f;  // Move light down

        } else if(event.key.keysym.sym == SDLK_7){
            lightPosition.x -= 0.1f;  // Move light left

        } else if(event.key.keysym.sym == SDLK_8){
            lightPosition.x += 0.1f;  // Move light right
        }

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
        } else if (tokens[0] == "usemtl") {
            currentColour = palette[tokens[1]];
        } else if (tokens[0] == "f") {
            ModelTriangle triangle(vertices[stoi(tokens[1]) - 1], vertices[stoi(tokens[2]) - 1], vertices[stoi(tokens[3]) - 1], currentColour);

            // Calculate the normal for the triangle
            glm::vec3 edge1 = triangle.vertices[1] - triangle.vertices[0];
            glm::vec3 edge2 = triangle.vertices[2] - triangle.vertices[0];
            triangle.normal = glm::normalize(glm::cross(edge1, edge2));

            triangles.push_back(triangle);
        }
    }

    objFile.close();
    return triangles;
}

std::vector<ModelTriangle> loadSphereOBJ(const std::string& objFilename, float scale) {
    std::vector<ModelTriangle> triangles;
    std::ifstream objFile(objFilename);

    if (!objFile.is_open()) {
        std::cerr << "Failed to open geometry file: " << objFilename << std::endl;
        return triangles;
    }

    std::vector<glm::vec3> vertices;
    Colour sphereColour(150, 75, 0); // brown color for the sphere

    std::string line;
    while (getline(objFile, line)) {
        std::vector<std::string> tokens = split(line, ' ');
        if (tokens.empty()) continue;

        if (tokens[0] == "v") {
            glm::vec3 vertex(stof(tokens[1]) * scale+0.5f, stof(tokens[2]) * scale, stof(tokens[3]) * scale -0.9f);
            vertices.push_back(vertex);
        } else if (tokens[0] == "f") {
            ModelTriangle triangle(vertices[stoi(tokens[1]) - 1], vertices[stoi(tokens[2]) - 1], vertices[stoi(tokens[3]) - 1], sphereColour);

            // Calculate the normal for the triangle
            glm::vec3 edge1 = triangle.vertices[1] - triangle.vertices[0];
            glm::vec3 edge2 = triangle.vertices[2] - triangle.vertices[0];
            triangle.normal = glm::normalize(glm::cross(edge1, edge2));

            triangles.push_back(triangle);
        }
    }

    objFile.close();
    return triangles;
}


const float IMAGE_PLANE_SCALING = 240.0f;  // The scaling factor

CanvasPoint getCanvasIntersectionPoint(const glm::vec3 &cameraPosition, const glm::vec3 &vertexPosition, float focalLength) {

    glm::vec3 direction = cameraOrientation * (vertexPosition - cameraPosition);
    float xi = direction.x;
    float yi = direction.y;
    float zi = -direction.z;

    float ui = focalLength * (xi / zi) * IMAGE_PLANE_SCALING + WIDTH / 2;
    float vi = focalLength * (yi / zi) * IMAGE_PLANE_SCALING + HEIGHT / 2;

    return CanvasPoint(ui, vi, 1/zi);
}

void lookAt(const glm::vec3 &point) {
    glm::vec3 forward = glm::normalize(cameraPosition - point );  // The direction from the point to the camera

    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0,1,0), -forward));  // Compute the right direction
    glm::vec3 up = glm::normalize(glm::cross(right, forward));  // Compute the up direction

    // Set the camera orientation
    cameraOrientation[0] = right;
    cameraOrientation[1] = up;
    cameraOrientation[2] = forward;

    std::cout << "Right: (" << right.x << ", " << right.y << ", " << right.z << ")" << std::endl;
    std::cout << "Up: (" << up.x << ", " << up.y << ", " << up.z << ")" << std::endl;
    std::cout << "Forward: (" << -forward.x << ", " << -forward.y << ", " << -forward.z << ")" << std::endl;

}

RayTriangleIntersection getClosestIntersection(const glm::vec3 &rayDirection, const std::vector<ModelTriangle> &triangles, const glm::vec3 &rayOrigin, float maxDistance = std::numeric_limits<float>::max()) {
    RayTriangleIntersection closestIntersection;
    float closestDistance = maxDistance; // Set closestDistance to maxDistance initially

    for (size_t i = 0; i < triangles.size(); i++) {
        const auto &triangle = triangles[i];
        glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
        glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
        glm::vec3 SPVector = rayOrigin - triangle.vertices[0]; // Use rayOrigin instead of cameraPosition
        glm::mat3 DEMatrix(-rayDirection, e0, e1);
        glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

        float t = possibleSolution.x, u = possibleSolution.y, v = possibleSolution.z;
        if (t > 0 && u >= 0.0 && u <= 1.0 && v >= 0.0 && v <= 1.0 && (u + v) <= 1.0) {
            if (t < closestDistance) { // This condition ensures we're also checking against maxDistance
                closestDistance = t;
                glm::vec3 intersectionPoint = cameraPosition + t * rayDirection;
                closestIntersection = RayTriangleIntersection(intersectionPoint, t, triangle, i);
            }
        }
    }

    if (closestDistance == maxDistance) { // Change this to check if no intersection was found
        closestIntersection.distanceFromCamera = -1;
    }

    return closestIntersection;
}

glm::vec3 getRayDirection(int pixelX, int pixelY) {
    float focalLength = 1.5f;
    // Convert canvas coordinates back to camera space
    float ndcX = (pixelX - WIDTH / 2.0f) / IMAGE_PLANE_SCALING;
    float ndcY = ( HEIGHT / 2.0f - pixelY) / IMAGE_PLANE_SCALING;

    // Generate a point on the image plane in camera space
    glm::vec3 imagePlanePoint = glm::vec3(ndcX, ndcY, -focalLength);

    // Transform the point by the camera orientation to get the direction in world space
    glm::vec3 rayDirection = cameraOrientation * imagePlanePoint;

    // Normalize the direction
    rayDirection = glm::normalize(rayDirection);

    return rayDirection;
}


float calculateShadowContribution(float shadowDistance, float lightDistance) {
    // Example calculation; adjust based on your scene and preferences
    float softnessFactor = 0.5f; // Adjust this for softer/harder shadow edges
    return glm::smoothstep(0.0f, lightDistance * softnessFactor, shadowDistance);
}


float calculateShadowIntensity(const glm::vec3 &point, const std::vector<glm::vec3> &lightPositions, const std::vector<ModelTriangle> &triangles, float bias = 0.001f) {
    float shadowIntensity = 0.0f;

    for (const auto& lightPosition : lightPositions) {
        glm::vec3 toLight = lightPosition - point;
        float distanceToLight = glm::length(toLight);
        glm::vec3 shadowRayDirection = glm::normalize(toLight);

        glm::vec3 shadowRayOrigin = point + bias * shadowRayDirection;
        RayTriangleIntersection shadowIntersection = getClosestIntersection(shadowRayDirection, triangles, shadowRayOrigin, distanceToLight - bias);

        if (shadowIntersection.distanceFromCamera > 0 && shadowIntersection.distanceFromCamera < distanceToLight - bias) {
            // Soft shadow calculation: scale shadow contribution by some factor (e.g., distance)
            shadowIntensity += calculateShadowContribution(shadowIntersection.distanceFromCamera, distanceToLight);
        } else {
            shadowIntensity += 1.0f; // Full light contribution if not in shadow
        }
    }

    shadowIntensity /= static_cast<float>(lightPositions.size());
    return glm::clamp(shadowIntensity, 0.0f, 1.0f);
}


struct Photon {
    glm::vec3 position;
    glm::vec3 direction;
    Colour power;

    Photon(const glm::vec3& pos, const glm::vec3& dir, const Colour& pow)
            : position(pos), direction(dir), power(pow) {}
};

class PhotonMap {
    std::vector<Photon> photons;

public:
    void store(const Photon& photon) {
        photons.push_back(photon);
    }

    Colour gatherLight(const glm::vec3& position, const glm::vec3& normal, size_t numPhotons, float radius) const {
        Colour accumulatedColor(0, 0, 0);
        size_t found = 0;
        for (const auto& photon : photons) {
            if (glm::distance(photon.position, position) < radius) {
                // Optionally, use the angle between photon direction and normal for weighting
                accumulatedColor.red += photon.power.red;
                accumulatedColor.green += photon.power.green;
                accumulatedColor.blue += photon.power.blue;
                if (++found >= numPhotons) break;
            }
        }
        if (found > 0) {
            accumulatedColor.red /= found;
            accumulatedColor.green /= found;
            accumulatedColor.blue /= found;
        }
        return accumulatedColor;
    }
};


std::vector<Photon> emitPhotons(int numPhotons, const glm::vec3& lightPosition, const Colour& lightColour) {
    std::vector<Photon> photons;
    photons.reserve(numPhotons);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    for (int i = 0; i < numPhotons; ++i) {
        float theta = acos(sqrt(1.0 - distribution(generator))); // Cosine-weighted distribution
        float phi = 2.0 * M_PI * distribution(generator);

        glm::vec3 direction(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

        Photon photon(lightPosition, direction, lightColour);


        photons.push_back(photon);
    }

    return photons;
}


Colour calculateIndirectIllumination(const glm::vec3& position, const glm::vec3& normal, const PhotonMap& photonMap, float searchRadius, size_t numPhotons) {
    // Use gatherLight method to calculate indirect illumination
    return photonMap.gatherLight(position, normal, numPhotons, searchRadius);
}


TextureMap environmentMap("environment map.ppm");

glm::vec2 mapDirectionToUV(const glm::vec3 &direction) {
    // Convert direction vector into spherical coordinates
    float theta = atan2(direction.z, direction.x);
    float phi = acos(direction.y / glm::length(direction));

    // Map spherical coordinates to UV space
    float u = (theta + M_PI) / (2.0f * M_PI);
    float v = phi / M_PI;

    return glm::vec2(u, v);
}


Colour sampleEnvironmentMap(const glm::vec2 &uv, const TextureMap &environmentMap) {
    // Manually clamp UV coordinates between 0 and 1
    float clampedU = uv.x < 0.0f ? 0.0f : (uv.x > 1.0f ? 1.0f : uv.x);
    float clampedV = uv.y < 0.0f ? 0.0f : (uv.y > 1.0f ? 1.0f : uv.y);

    // Convert UV coordinates to pixel coordinates
    int x = static_cast<int>(clampedU * (environmentMap.width - 1));
    int y = static_cast<int>((1.0f - clampedV) * (environmentMap.height - 1)); // Invert v-coordinate if necessary

    // Manually ensure x and y are within the bounds of the texture dimensions
    x = x < 0 ? 0 : (x > environmentMap.width - 1 ? environmentMap.width - 1 : x);
    y = y < 0 ? 0 : (y > environmentMap.height - 1 ? environmentMap.height - 1 : y);

    // Fetch the pixel color
    uint32_t pixel = environmentMap.pixels[y * environmentMap.width + x];
    int red = (pixel >> 16) & 0xFF;
    int green = (pixel >> 8) & 0xFF;
    int blue = pixel & 0xFF;
    return Colour(red, green, blue);
}


// prepare for reflections:
glm::vec3 calculateRefractionDirection(const glm::vec3& normal, const glm::vec3& incident, float indexOfRefraction) {
    float cosi = glm::clamp(glm::dot(incident, normal), -1.0f, 1.0f);
    float etai = 1.0f; // Refractive index of air
    float etat = indexOfRefraction; // Refractive index of the material

    glm::vec3 n = normal;
    if (cosi < 0) {
        cosi = -cosi;
    } else {
        std::swap(etai, etat);
        n = -normal;
    }

    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? glm::vec3(0.0f) : eta * incident + (eta * cosi - sqrtf(k)) * n;
}


float calculateFresnelEffect(const glm::vec3& incident, const glm::vec3& normal, float indexOfRefraction) {
    float cosi = glm::clamp(glm::dot(incident, normal), -1.0f, 1.0f);
    float etai = 1.0f, etat = indexOfRefraction;
    if (cosi > 0) { std::swap(etai, etat); }

    float sint = etai / etat * sqrtf(std::max(0.0f, 1.0f - cosi * cosi));
    if (sint >= 1.0) {
        return 1.0f; // Total internal reflection
    }

    float cost = sqrtf(std::max(0.0f, 1.0f - sint * sint));
    cosi = fabsf(cosi);
    float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
    float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
    return (Rs * Rs + Rp * Rp) / 2;
}


Colour mix(const Colour& c1, const Colour& c2, float factor) {
    return Colour(
            c1.red * (1 - factor) + c2.red * factor,
            c1.green * (1 - factor) + c2.green * factor,
            c1.blue * (1 - factor) + c2.blue * factor
    );
}


// Helper refraction to check if two colors are equal
bool isColorEqual(const Colour& c1, const Colour& c2) {
    return c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue;
}
// Check if the material is reflective
bool isReflective(const Colour& c) {
    return c.red == 0 && c.green == 0 && c.blue == 255; // Blue color for reflective surfaces
}


// Check if the material is refractive
bool isRefractive(const Colour& c) {
    return c.red == 255 && c.green == 0 && c.blue == 0; // Red color for refractive surfaces
}

bool isDiffuse(const Colour &colour) {
    // Example heuristic: a material is considered diffuse if it's not purely reflective or refractive
    return !(isReflective(colour) || isRefractive(colour));
}

glm::vec3 randomInHemisphere(const glm::vec3 &normal) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    glm::vec3 inUnitSphere;
    do {
        inUnitSphere = 2.0f * glm::vec3(dis(gen), dis(gen), dis(gen)) - glm::vec3(1, 1, 1);
    } while (glm::dot(inUnitSphere, inUnitSphere) >= 1.0f);

    if (glm::dot(inUnitSphere, normal) > 0.0f)
        return inUnitSphere;
    else
        return -inUnitSphere;
}


Colour reflectiveMaterialColor = Colour(0, 0, 255); // Red color for reflective surfaces
Colour refractiveMaterialColor = Colour(255, 0, 0); // brown color for refractive surfaces

Colour traceRay(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const std::vector<ModelTriangle> &triangles, int depth) {
    if (depth <= 0) {
        return Colour(0, 0, 0); // Base case for recursion, return black or background color
    }

    RayTriangleIntersection intersection = getClosestIntersection(rayDirection, triangles, rayOrigin);
    if (intersection.distanceFromCamera >= 0) {

        Colour color = intersection.intersectedTriangle.colour;

        if (isReflective(color)) {
            // Handle reflective surface (mirror)
            glm::vec3 reflectedDirection = glm::reflect(rayDirection, intersection.intersectedTriangle.normal);
            glm::vec3 reflectedRayOrigin = intersection.intersectionPoint + reflectedDirection * 0.001f;
            Colour reflectedColor = traceRay(reflectedRayOrigin, reflectedDirection, triangles, depth - 1);
            return reflectedColor;
        } else if (isRefractive(color)) {
            float refractiveIndex = 1.5f; // Example for glass
            glm::vec3 refractedDirection = calculateRefractionDirection(-intersection.intersectedTriangle.normal, rayDirection, refractiveIndex);
            glm::vec3 refractedRayOrigin = intersection.intersectionPoint + refractedDirection * 0.001f; // Bias to avoid self-intersection
            return traceRay(refractedRayOrigin, refractedDirection, triangles, depth - 1);
        } else if (isDiffuse(color)) {
            // Handle diffuse surface (indirect lighting)
            glm::vec3 bounceDirection = randomInHemisphere(intersection.intersectedTriangle.normal);
            glm::vec3 bounceRayOrigin = intersection.intersectionPoint + 0.001f * bounceDirection;
            Colour indirectColor = traceRay(bounceRayOrigin, bounceDirection, triangles, depth - 1);

            // Mix direct color (from material) and indirect color (from bounce)
            float mixRatio = 0.2; // Adjust this ratio as needed
            return mix(color, indirectColor, mixRatio);
        }

        // Direct color for non-reflective, non-refractive, and non-diffuse materials
        return color;
    } else {
        // When no intersection is found, sample the environment map
        glm::vec2 uv = mapDirectionToUV(rayDirection);

        return sampleEnvironmentMap(uv, environmentMap);
    }

    return Colour(0, 0, 0); // Return black or background color if no intersection
}

Colour combineIllumination(const Colour& direct, const Colour& indirect) {
    // Cast color components to float before addition and use std::min
    return Colour(
            static_cast<int>(std::min(255.0f, static_cast<float>(direct.red) + static_cast<float>(indirect.red))),
            static_cast<int>(std::min(255.0f, static_cast<float>(direct.green) + static_cast<float>(indirect.green))),
            static_cast<int>(std::min(255.0f, static_cast<float>(direct.blue) + static_cast<float>(indirect.blue)))
    );
}

void tracePhoton(Photon& photon, const std::vector<ModelTriangle>& triangles, PhotonMap& photonMap, int maxBounces) {
    glm::vec3 currentPhotonPosition = photon.position;
    glm::vec3 currentPhotonDirection = photon.direction;
    Colour currentPhotonPower = photon.power;

    for (int bounce = 0; bounce < maxBounces; ++bounce) {
        RayTriangleIntersection intersection = getClosestIntersection(currentPhotonDirection, triangles, currentPhotonPosition);
        if (intersection.distanceFromCamera < 0) break; // Photon escapes the scene

        currentPhotonPosition = intersection.intersectionPoint;

        // Store photon at the intersection point
        photonMap.store(Photon(currentPhotonPosition, currentPhotonDirection, currentPhotonPower));

        // For a diffuse surface, reflect the photon in a random direction
        if (isDiffuse(intersection.intersectedTriangle.colour)) {
            currentPhotonDirection = randomInHemisphere(intersection.intersectedTriangle.normal);
            currentPhotonPosition += currentPhotonDirection * 0.001f; // Offset to prevent self-intersection
            currentPhotonPower.red = static_cast<int>(currentPhotonPower.red * 0.8f);
            currentPhotonPower.green = static_cast<int>(currentPhotonPower.green * 0.8f);
            currentPhotonPower.blue = static_cast<int>(currentPhotonPower.blue * 0.8f);
        } else {
            break; // For now, stop tracing when hitting a non-diffuse surface
        }
    }
}


void drawRayTracedScene(DrawingWindow &window) {
    float focalLength = 1.5f;

    float ambientLightLevel = 0.5f;  // This is the minimum light level (50% in this case)
    float indexOfRefraction = 1.5f; // Index of refraction for glass, adjust as needed

//    Auto lightPositions:
    std::vector<glm::vec3> lightPositions;
    float offset = 0.3f;  // This is the distance between each light point
    int maxDepth = 3; // Maximum recursion depth for reflections

    // Jittered sampling for area lights
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    const float jitterAmount = 0.05f; // Adjust this value for more or less jitter

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            // Generate jittered offsets
            float jitterX = distribution(generator) * jitterAmount;
            float jitterZ = distribution(generator) * jitterAmount;

            glm::vec3 jitteredLightPoint = lightPosition + glm::vec3(i * offset + jitterX, 0.0f, j * offset + jitterZ);
            lightPositions.push_back(jitteredLightPoint);
        }
    }

    float scale = 0.35f; // this is the scaling factor for the object
    std::vector<ModelTriangle> modelTriangles = loadOBJWithMaterials(
            "cornell-box.obj",
            "cornell-box.mtl",
            scale
    );
    std::vector<ModelTriangle> sphereTriangles = loadSphereOBJ("sphere.obj", 0.35f);

    // Combine both sets of triangles
    std::vector<ModelTriangle> allTriangles;
    allTriangles.insert(allTriangles.end(), modelTriangles.begin(), modelTriangles.end());
    allTriangles.insert(allTriangles.end(), sphereTriangles.begin(), sphereTriangles.end());

    // for photon map:
    PhotonMap photonMap;
    Colour lightColour = Colour(255, 255, 255); // Example light color
    const int NUM_PHOTONS = 10000;
    const int MAX_PHOTON_BOUNCES = 5;
    const float SEARCH_RADIUS = 0.01f;

    // Emit and trace photons for each light source
    for (const auto& lightPosition : lightPositions) {
        std::vector<Photon> emittedPhotons = emitPhotons(NUM_PHOTONS, lightPosition, lightColour);
        for (auto& photon : emittedPhotons) {
            tracePhoton(photon, allTriangles, photonMap, MAX_PHOTON_BOUNCES);
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            glm::vec3 rayDirection = getRayDirection(x, y);
            RayTriangleIntersection intersection = getClosestIntersection(rayDirection, allTriangles, cameraPosition);
            Colour color;


            if (intersection.distanceFromCamera >= 0) {

                // Check if the surface is reflective
                if (isColorEqual(intersection.intersectedTriangle.colour, reflectiveMaterialColor)) {

                    glm::vec3 reflectedDirection = glm::reflect(rayDirection, intersection.intersectedTriangle.normal);

                    glm::vec3 reflectedRayOrigin = intersection.intersectionPoint + reflectedDirection * 0.001f;

                    color = traceRay(reflectedRayOrigin, reflectedDirection, allTriangles, maxDepth);


                } else if (isColorEqual(intersection.intersectedTriangle.colour, refractiveMaterialColor)) {

                    // Calculate refractive direction
                    glm::vec3 refractedDirection = calculateRefractionDirection(intersection.intersectedTriangle.normal, rayDirection, indexOfRefraction);
                    glm::vec3 refractedRayOrigin = intersection.intersectionPoint + refractedDirection * 0.001f;
                    Colour refractedColor = traceRay(refractedRayOrigin, refractedDirection, allTriangles, maxDepth);

                    // Calculate reflected direction
                    glm::vec3 reflectedDirection = glm::reflect(rayDirection, intersection.intersectedTriangle.normal);
                    glm::vec3 reflectedRayOrigin = intersection.intersectionPoint + reflectedDirection * 0.001f;
                    Colour reflectedColor = traceRay(reflectedRayOrigin, reflectedDirection, allTriangles, maxDepth);

                    // Calculate Fresnel effect
                    float fresnelEffect = calculateFresnelEffect(rayDirection, intersection.intersectedTriangle.normal, indexOfRefraction);

                    // Combine reflected and refracted colors based on Fresnel effect
                    color = mix(refractedColor, reflectedColor, fresnelEffect);

                } else {

                    // Non-reflective surface lighting calculations
                    float shadowIntensity = calculateShadowIntensity(intersection.intersectionPoint, lightPositions, allTriangles);

                    glm::vec3 lightDirection = glm::normalize(lightPosition - intersection.intersectionPoint);
                    glm::vec3 viewDirection = glm::normalize(cameraPosition - intersection.intersectionPoint);

                    float angleOfIncidence = glm::dot(intersection.intersectedTriangle.normal, lightDirection);
                    angleOfIncidence = glm::clamp(angleOfIncidence, 0.0f, 1.0f);

                    glm::vec3 reflectionVector = glm::reflect(-lightDirection, intersection.intersectedTriangle.normal);
                    float specularCoefficient = pow(glm::max(glm::dot(reflectionVector, viewDirection), 0.0f), 256);

                    Colour directColor = intersection.intersectedTriangle.colour;
                    float totalLighting = glm::max(angleOfIncidence + specularCoefficient, ambientLightLevel) * shadowIntensity;

                    directColor.red = glm::clamp(directColor.red * totalLighting, 0.0f, 255.0f);
                    directColor.green = glm::clamp(directColor.green * totalLighting, 0.0f, 255.0f);
                    directColor.blue = glm::clamp(directColor.blue * totalLighting, 0.0f, 255.0f);

                    // Calculate indirect illumination using the photon map
                    Colour indirectIllumination = calculateIndirectIllumination(intersection.intersectionPoint, intersection.intersectedTriangle.normal, photonMap, SEARCH_RADIUS, NUM_PHOTONS);

                    // Combine direct and indirect illumination
                    color = combineIllumination(directColor, indirectIllumination);
//                    color = directColor;
                }

            } else {

                // When no intersection is found, sample the environment map
                glm::vec2 uv = mapDirectionToUV(rayDirection);
                color = sampleEnvironmentMap(uv, environmentMap);
            }

            // Convert color to the format required by the window and set pixel color
            uint32_t colourPacked = (255 << 24) + (static_cast<int>(color.red) << 16) + (static_cast<int>(color.green) << 8) + static_cast<int>(color.blue);
            window.setPixelColour(x, y, colourPacked);
        }
    }

    // Render the final frame
    window.renderFrame();
}


void drawRasterisedScene(DrawingWindow &window) {
    window.clearPixels();
    if (isOrbiting) {
        orbitAngle += 0.005f; // Adjust this value to control the speed of the orbit
        glm::mat3 rotationMatrix = glm::mat3(
                cos(orbitAngle), 0.0f, sin(orbitAngle),
                0.0f, 1.0f, 0.0f,
                -sin(orbitAngle), 0.0f, cos(orbitAngle)
        );

        // Choose a suitable distance from the center of the Cornell Box
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

    float focalLength = 1.5;

    std::vector<ModelTriangle> modelTriangles = loadOBJWithMaterials(
            "cornell-box.obj",
            "cornell-box.mtl",
            0.35);

    std::vector<ModelTriangle> sphereTriangles = loadSphereOBJ("sphere.obj", 0.35f);
    // Combine both sets of triangles
    std::vector<ModelTriangle> allTriangles;
    allTriangles.insert(allTriangles.end(), modelTriangles.begin(), modelTriangles.end());
    allTriangles.insert(allTriangles.end(), sphereTriangles.begin(), sphereTriangles.end());

//    Colour white(255, 255, 255); // For wireframe

    for (const ModelTriangle &modelTriangle : allTriangles) {

        CanvasPoint v0 = getCanvasIntersectionPoint(cameraPosition, modelTriangle.vertices[0], focalLength);
        CanvasPoint v1 = getCanvasIntersectionPoint(cameraPosition, modelTriangle.vertices[1], focalLength);
        CanvasPoint v2 = getCanvasIntersectionPoint(cameraPosition, modelTriangle.vertices[2], focalLength);

        v0.y = HEIGHT - v0.y;
        v1.y = HEIGHT - v1.y;
        v2.y = HEIGHT - v2.y;

        CanvasTriangle canvasTriangle(v0, v1, v2);
        drawFilledTriangle(window, canvasTriangle, modelTriangle.colour); // Draw the filled triangle
    }
}

void drawWireframeScene(DrawingWindow &window) {
    window.clearPixels(); // Clear the window before drawing the new frame
    Colour wireframeColour(255, 255, 255); // White colour for wireframe

    std::vector<ModelTriangle> modelTriangles = loadOBJWithMaterials(
            "cornell-box.obj",
            "cornell-box.mtl",
            0.35f // Adjust scale as needed
    );
    std::vector<ModelTriangle> sphereTriangles = loadSphereOBJ("sphere.obj", 0.35f);
    // Combine both sets of triangles
    std::vector<ModelTriangle> allTriangles;
    allTriangles.insert(allTriangles.end(), modelTriangles.begin(), modelTriangles.end());
    allTriangles.insert(allTriangles.end(), sphereTriangles.begin(), sphereTriangles.end());

    for (const ModelTriangle &triangle : allTriangles) {
        CanvasPoint v0 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[0], 1.5);
        CanvasPoint v1 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[1], 1.5);
        CanvasPoint v2 = getCanvasIntersectionPoint(cameraPosition, triangle.vertices[2], 1.5);

        v0.y = HEIGHT - v0.y;
        v1.y = HEIGHT - v1.y;
        v2.y = HEIGHT - v2.y;

        drawLine(window, v0, v1, wireframeColour); // Draw line between v0 and v1
        drawLine(window, v1, v2, wireframeColour); // Draw line between v1 and v2
        drawLine(window, v2, v0, wireframeColour); // Draw line between v2 and v0
    }

    window.renderFrame(); // Update the window with the wireframe image
}



int main(int argc, char *argv[]) {
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;


    while (true) {
        // We MUST poll for events - otherwise the window will freeze!
        if (window.pollForInputEvents(event)) handleEvent(event, window);
        for (int x = 0; x < WIDTH; x++)
            for (int y = 0; y < HEIGHT; y++)
                depthBuffer[x][y] = 0.0;
        switch (currentMode) {
            case RenderMode::Wireframe:
                drawWireframeScene(window);
                break;
            case RenderMode::Rasterisation:
                drawRasterisedScene(window);
                break;
            case RenderMode::RayTracing:
                drawRayTracedScene(window);
                break;
        }
        // Need to render the frame at the end, or nothing actually gets shown on the screen!
        window.renderFrame();
    }
}


