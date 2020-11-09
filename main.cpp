#pragma comment(lib,"glu32.lib")
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
//#include "helpFile.hpp"
#include <gl\glu.h>


using namespace sf;

int main()
{
    RenderWindow window(VideoMode(800, 600), "Minecraft C++");
    Texture t;
    t.loadFromFile("resources/background.jpg");
    Sprite background(t);

    GLuint texture = 0;
    {
        Image image;
        image.loadFromFile("resources/kirp.jpg");

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);
    glEnable(GL_TEXTURE_2D);

    Clock clock;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event));
        {
            if (event.type == Event::Closed)
                window.close();
        }

        float time = clock.getElapsedTime().asSeconds() * 100;
        float size = 20.f;

        // Drow the background
        window.pushGLStates();
        window.draw(background);
        window.popGLStates();

        // Clear the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        // Apply some transformations (смещаем кубик на 100 единиц)
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, -100);
        glRotatef(time, 50, 50, 0);

        // Draw a cube

        // Front
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(size, -size, size);
        glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
        glTexCoord2f(1, 1); glVertex3f(-size, size, size);
        glTexCoord2f(0, 1); glVertex3f(size, size, size);
        glEnd();

        // Back
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1, 0); glVertex3f(size, -size, -size);
        glTexCoord2f(1, 1); glVertex3f(size, size, -size);
        glTexCoord2f(0, 1); glVertex3f(-size, size, -size);
        glEnd();
 
        // Left
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
        glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
        glTexCoord2f(0, 1); glVertex3f(-size, size, size);
        glEnd();

        // Right
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
        glTexCoord2f(1, 0); glVertex3f(size, -size, size);
        glTexCoord2f(1, 1); glVertex3f(size, size, size);
        glTexCoord2f(0, 1); glVertex3f(size, size, -size);
        glEnd();

        // Bottom
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
        glTexCoord2f(1, 0); glVertex3f(size, -size, size);
        glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
        glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
        glEnd();

        // Top
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
        glTexCoord2f(1, 0); glVertex3f(size, size, -size);
        glTexCoord2f(1, 1); glVertex3f(size, size, size);
        glTexCoord2f(0, 1); glVertex3f(-size, size, size);

        glEnd();

        glFlush();

        // Display all
        window.display();
    }

    glDeleteTextures(1, &texture);

    return 0;
}
