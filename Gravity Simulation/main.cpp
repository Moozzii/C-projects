#include <GL/gl.h>
#include <GL/glu.h>
#include "glfw3.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>

struct Vec3 {
    double x{}, y{}, z{};
    Vec3() = default;
    Vec3(double X, double Y, double Z=0.0) : x(X), y(Y), z(Z) {}
    Vec3 operator+(const Vec3& o) const { return Vec3(x+o.x, y+o.y, z+o.z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(x-o.x, y-o.y, z-o.z); }
    Vec3 operator*(double s) const { return Vec3(x*s, y*s, z*s); }
    Vec3 operator/(double s) const { return Vec3(x/s, y/s, z/s); }
    Vec3& operator+=(const Vec3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    double mag() const { return std::sqrt(x*x + y*y + z*z); }
    Vec3 norm() const { double m = mag(); return (m==0) ? Vec3(0,0,0) : (*this)/m; }
};

struct Body {
    Vec3 pos;
    Vec3 vel;
    Vec3 force;
    double mass;
    double visualRadius;  
    float color[3];
    Body(const Vec3& p, double m, double r, float cr, float cg, float cb)
        : pos(p), vel(0,0,0), force(0,0,0), mass(m), visualRadius(r)
    { color[0]=cr; color[1]=cg; color[2]=cb; }
};


static double G = 0.06;       
static double physics_dt = 0.09;
static int physics_substeps = 3;  


static double camYaw = 20.0;     
static double camPitch = -20.0;  
static double camDist = 220.0;
static bool rightMouseDown = false;
static double lastMouseX = 0.0, lastMouseY = 0.0;
static GLFWwindow* gWindow = nullptr;

void cursor_position_callback(GLFWwindow* , double xpos, double ypos) {
    if (!rightMouseDown) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        return;
    }
    double dx = xpos - lastMouseX;
    double dy = ypos - lastMouseY;
   
    camYaw += dx * 0.3;
    camPitch += dy * 0.3;
    if (camPitch > 89.0) camPitch = 89.0;
    if (camPitch < -89.0) camPitch = -89.0;
    lastMouseX = xpos;
    lastMouseY = ypos;
}

void mouse_button_callback(GLFWwindow* w, int button, int action, int mods) {
    (void)w;
    (void)mods;
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMouseDown = true;
            double x, y;
            glfwGetCursorPos(gWindow, &x, &y);
            lastMouseX = x; lastMouseY = y;
        } else if (action == GLFW_RELEASE) {
            rightMouseDown = false;
        }
    }
}

void scroll_callback(GLFWwindow* /*w*/, double /*xoff*/, double yoff) {

    camDist *= (1.0 - yoff*0.12);
    if (camDist < 30.0) camDist = 30.0;
    if (camDist > 1200.0) camDist = 1200.0;
}


GLUquadric* gQuad = nullptr;

void drawSphere(const Vec3& p, double radius, const float color[3]) {
    glPushMatrix();
    glTranslatef((float)p.x, (float)p.y, (float)p.z);
    GLfloat mat_diff[4] = { color[0], color[1], color[2], 1.0f };
    GLfloat mat_spec[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 16.0f);
    gluSphere(gQuad, (GLdouble)radius, 28, 18);
    glPopMatrix();
}


void zeroForces(std::vector<Body>& bodies) {
    for (auto &b : bodies) b.force = Vec3(0,0,0);
}

void accumulateForces(std::vector<Body>& bodies) {
    int n = (int)bodies.size();
    // pairwise forces (naive O(n^2), fine for few bodies)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            Vec3 r = bodies[j].pos - bodies[i].pos;
            double d = r.mag();
            if (d < 0.1) continue;
            Vec3 f = r.norm() * (G * bodies[i].mass * bodies[j].mass / (d * d));
            bodies[i].force += f;
        }
    }
}

void integrate(std::vector<Body>& bodies, double dt) {
    for (auto &b : bodies) {
        // v += a dt
        Vec3 acc = b.force / b.mass;
        b.vel += acc * dt;
        // x += v dt
        b.pos += b.vel * dt;
    }
}

// a single stable physics step (with center-of-mass optional fixes, but not needed)
void physicsStep(std::vector<Body>& bodies, double dt) {
    zeroForces(bodies);
    accumulateForces(bodies);
    integrate(bodies, dt);
}

// ---------------- Setup scene (sun + planets) ----------------
std::vector<Body> makeSystem() {
    std::vector<Body> bodies;
    // Sun (big mass, visually larger)
    bodies.emplace_back(Vec3(0,0,0), 4200.0, 12.0, 1.0f, 0.9f, 0.2f); // yellow-ish

    // Planets: radius(distance), mass, visual size, color
    const double planetMass = 1.0;
    struct PSpec { double r; double size; float cr, cg, cb; };
    std::vector<PSpec> specs = {
        {30.0, 3.5, 0.6f, 0.7f, 1.0f},   // inner
        {50.0, 4.5, 0.8f, 0.5f, 0.3f},   // orange
        {75.0, 5.0, 0.4f, 0.8f, 0.5f},   // greenish
        {105.0, 6.0, 0.9f, 0.9f, 1.0f}   // pale
    };

    for (auto &s : specs) {
        bodies.emplace_back(Vec3(s.r, 0, 0), planetMass, s.size, s.cr, s.cg, s.cb);
    }

    // Give circular velocities for the planets: v = sqrt(G * M_sun / r)
    double M = bodies[0].mass;
    for (int i = 1; i < (int)bodies.size(); ++i) {
        double r = bodies[i].pos.mag();
        double v = std::sqrt(G * M / r);
        bodies[i].vel = Vec3(0, v, 0);
    }

    return bodies;
}

// ---------------- Utilities ----------------
void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // center light (sun) - position will follow camera/modelview, so we set it each frame
    GLfloat ambient[]  = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glShadeModel(GL_SMOOTH);
}

// ---------------- Main ----------------
int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    gWindow = glfwCreateWindow(1000, 700, "3D Solar Demo", NULL, NULL);
    if (!gWindow) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(gWindow);
    glfwSwapInterval(1); // vsync

    // Callbacks
    glfwSetCursorPosCallback(gWindow, cursor_position_callback);
    glfwSetMouseButtonCallback(gWindow, mouse_button_callback);
    glfwSetScrollCallback(gWindow, scroll_callback);

    // Setup GL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Projection (perspective)
    int w, h;
    glfwGetFramebufferSize(gWindow, &w, &h);
    double aspect = (h>0) ? (double)w / (double)h : 1.0;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 5000.0);
    glMatrixMode(GL_MODELVIEW);

    setupLighting();

    // create quadric once and reuse
    gQuad = gluNewQuadric();
    gluQuadricNormals(gQuad, GLU_SMOOTH);

    // create system
    auto bodies = makeSystem();

    // timing: we run physics substeps per frame for stability & determinism
    using clock = std::chrono::high_resolution_clock;
    auto last = clock::now();

    while (!glfwWindowShouldClose(gWindow)) {
        // compute elapsed (but we will step fixed micro-steps for stability)
        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - last;
        last = now;

        // do a number of fixed tiny physics steps per frame
        for (int s = 0; s < physics_substeps; ++s) {
            physicsStep(bodies, physics_dt);
        }

        // prepare camera & light
        glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // camera transforms (arcball-ish)
        glTranslatef(0.0f, 0.0f, (float)(-camDist));
        glRotatef((float)camPitch, 1.0f, 0.0f, 0.0f);
        glRotatef((float)camYaw,   0.0f, 1.0f, 0.0f);

        // set light position at Sun's world position (so planets are lit from the Sun)
        GLfloat lightPos[4] = { (float)bodies[0].pos.x, (float)bodies[0].pos.y, (float)bodies[0].pos.z, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // Draw Sun and planets
        for (size_t i = 0; i < bodies.size(); ++i) {
            drawSphere(bodies[i].pos, bodies[i].visualRadius, bodies[i].color);
        }

        // optional: draw thin trails (simple, short)
        glDisable(GL_LIGHTING);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        // we will draw a single faint ring for each planet (approximate circular)
        for (size_t i = 1; i < bodies.size(); ++i) {
            double r = bodies[i].pos.mag();
            // small static ring (visual only)
            const int seg = 64;
            for (int k = 0; k < seg; ++k) {
                double a = (2.0*M_PI*k)/(double)seg;
                double x = r * std::cos(a);
                double y = r * std::sin(a);
                glVertex3f((float)x, (float)y, 0.0f);
            }
        }
        glEnd();
        glEnable(GL_LIGHTING);

        glfwSwapBuffers(gWindow);
        glfwPollEvents();
    }

    // cleanup
    gluDeleteQuadric(gQuad);
    glfwDestroyWindow(gWindow);
    glfwTerminate();

    return 0;
}
