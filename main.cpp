//#include <SFML/Graphics.hpp>
//#include <SFML/OpenGL.hpp>
#include <iostream>
#include "helpFile.hpp"


using namespace sf;
float size = 20.f;

int main()
{
    RenderWindow window(VideoMode(1024, 768), "Minecraft C++");
    Texture t;
    t.loadFromFile("resources/background.jpg");
    Sprite background(t);

    // Настройки OpenGL
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
        createBox(box,20);

        // Display all
        window.display();
    }


    return 0;
}
