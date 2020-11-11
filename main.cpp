#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
//#include "helpFile.hpp"
#include <time.h>
#include <SFML/System.hpp>
#define GL_CLAMP_TO_EDGE 0x812F
#include <gl\glu.h>


using namespace sf;
float size = 20.f;
const float PI = 3.141592653;
extern bool mass[1000][1000][1000];
float angleX, angleY; // Углы поворота камеры
float x = 0, y = 0, z = 0;
float speed = 1;

GLuint LoadTexture(sf::String name)
{
    sf::Image image;
    if (!image.loadFromFile(name))
        return EXIT_FAILURE;

    image.flipVertically();

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return texture;
}


void createBox(GLuint skybox[], float size)
{
    glBindTexture(GL_TEXTURE_2D, skybox[0]);
    glBegin(GL_QUADS);
    //front
    glTexCoord2f(0, 0);   glVertex3f(-size, -size, -size);
    glTexCoord2f(1, 0);   glVertex3f(size, -size, -size);
    glTexCoord2f(1, 1);   glVertex3f(size, size, -size);
    glTexCoord2f(0, 1);   glVertex3f(-size, size, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skybox[1]);
    glBegin(GL_QUADS);
    //back
    glTexCoord2f(0, 0); glVertex3f(size, -size, size);
    glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
    glTexCoord2f(1, 1); glVertex3f(-size, size, size);
    glTexCoord2f(0, 1); glVertex3f(size, size, size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glBegin(GL_QUADS);
    //left
    glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
    glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
    glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
    glTexCoord2f(0, 1); glVertex3f(-size, size, size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skybox[3]);
    glBegin(GL_QUADS);
    //right
    glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
    glTexCoord2f(1, 0); glVertex3f(size, -size, size);
    glTexCoord2f(1, 1); glVertex3f(size, size, size);
    glTexCoord2f(0, 1); glVertex3f(size, size, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skybox[4]);
    glBegin(GL_QUADS);
    //bottom
    glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
    glTexCoord2f(1, 0); glVertex3f(size, -size, size);
    glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
    glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skybox[5]);
    glBegin(GL_QUADS);
    //top  		
    glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
    glTexCoord2f(1, 0); glVertex3f(size, size, -size);
    glTexCoord2f(1, 1); glVertex3f(size, size, size);
    glTexCoord2f(0, 1); glVertex3f(-size, size, size);
    glEnd();
}


bool check(int x, int y, int z)
{
    if ((x < 0) || (x >= 1000) ||
        (y < 0) || (y >= 1000) ||
        (z < 0) || (z >= 1000)) return false;

    return mass[x][y][z];
}


int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(1024, 768), "Minecraft C++");
    Texture t;
    t.loadFromFile("resources/background.jpg");
    Sprite background(t);

    // Настройки OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    //glCullFace(GL_FRONT);

    // Загружаем текстуры кубика
    GLuint box[6];
    box[0] = LoadTexture("resources/grassBox/side.jpg");
    box[1] = LoadTexture("resources/grassBox/side.jpg");
    box[2] = LoadTexture("resources/grassBox/side.jpg");
    box[3] = LoadTexture("resources/grassBox/side.jpg");
    box[4] = LoadTexture("resources/grassBox/bottom.jpg");
    box[5] = LoadTexture("resources/grassBox/top.jpg");

    Clock clock;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event));
        {
            if (event.type == Event::Closed)
                window.close();
        }

        float dx = 0, dy = 0, dz = 0;
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            //смещаемься в данном направлении со скоросью speed
            dx = -sin(angleX / 180 * PI) * speed;
            dz = -cos(angleX / 180 * PI) * speed;
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            dx = sin(angleX / 180 * PI) * speed;
            dz = cos(angleX / 180 * PI) * speed;
        }

        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            dx = sin((angleX + 90) / 180 * PI) * speed;
            dz = cos((angleX + 90) / 180 * PI) * speed;
        }

        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            dx = sin((angleX - 90) / 180 * PI) * speed;
            dz = cos((angleX - 90) / 180 * PI) * speed;
        }

        x += dx;
        y += dy;
        z += dz;

        //float time = clock.getElapsedTime().asSeconds() * 100;
        //float size = 20.f;

        // Drow the background
        window.pushGLStates();
        window.draw(background);
        window.popGLStates();

        // Clear the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        ////-----------------------
        POINT mousexy;
        GetCursorPos(&mousexy);
        int xt = window.getPosition().x + 512;
        int yt = window.getPosition().y + 384;

        angleX += (xt - mousexy.x) / 4; //4 — чувствительность 
        angleY += (yt - mousexy.y) / 4;

        if (angleY < -89.0) { angleY = -89.0; }
        if (angleY > 89.0) { angleY = 89.0; }

        SetCursorPos(xt, yt);
        ////----------------------

        // Apply some transformations (смещаем кубик на 100 единиц)
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, -100);
        gluLookAt(x, y, z, x - sin(angleX / 180 * PI), y  + (tan(angleY / 180 * PI)), z - cos(angleX / 180 * PI), 0, 1, 0);

        // Draw a cube
        createBox(box, size);

        // Display all
        window.display();
        //если эскейп, то выходим из игры
        if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }
    }


    return 0;
}
