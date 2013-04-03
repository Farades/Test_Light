#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdio.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <iostream>

#define TEX_IMAGE_NAME "D:/workspace/Test_Light/Glass.bmp"

BOOL light = true;      // Свет ВКЛ / ВЫКЛ

GLfloat x_rotation = 50;         // X вращение
GLfloat y_rotation = 50;         // Y вращение
GLfloat xspeed;       // X скорость вращения
GLfloat yspeed;       // Y скорость вращения

GLfloat z=-5.0f;      // Сдвиг вглубь экрана
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };      // Значения фонового света
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };      // Значения диффузного света
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };     // Позиция света

GLuint filter;         // Какой фильтр использовать
GLuint texture[3];     // Место для хранения 3 текстур

float WinWidth       = 700.0;
float WinHeight      = 700.0;

//-----------------------------------------------------------------------------
//Таймер вращения
static void timer_rotation(int value = 0)
{
    x_rotation += xspeed;
    y_rotation += yspeed;

  /* send redisplay event */
  glutPostRedisplay();

  /* call this function again in 30 milliseconds */
  glutTimerFunc(30, timer_rotation, 0);
}

//-----------------------------------------------------------------------------
//Функция загрузки изображения текстуры
void Load_Tex_Image()
{
    int width, height, bpp;

    ilLoad(IL_BMP, reinterpret_cast<const ILstring>(TEX_IMAGE_NAME));
    int err = ilGetError();                          // Считывание кода ошибки
    if (err != IL_NO_ERROR)
    {
        const char* strError = iluErrorString(err);  // Считываем строку ошибки
        std::cout << "Error load texture image: " << strError << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Load texture image completed!" << std::endl;
        width  = ilGetInteger(IL_IMAGE_WIDTH);
        height = ilGetInteger(IL_IMAGE_HEIGHT);
        bpp    = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
        std::cout << "width:  "<< width << std::endl << "height: "
                  << height << std::endl << "bpp:    " << bpp << std::endl;
    }

    unsigned char* data = ilGetData();
    unsigned int type;

    switch (bpp) {
    case 1:
      type  = GL_RGB8;
      break;
    case 3:
      type = GL_RGB;
      break;
    case 4:
      type = GL_RGBA;
      break;
    }
    glGenTextures(3, &texture[0]);
        //1-я текстура
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
    GL_RGB, GL_UNSIGNED_BYTE, data);
        //2-я текстура
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
    GL_RGB, GL_UNSIGNED_BYTE, data);
        //3-я текстура
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
    GL_RGB, GL_UNSIGNED_BYTE, data);
}


//-----------------------------------------------------------------------------
//Функция инициализации
void init()
{
    //Инициализация DevIL
    ilInit();
    iluInit();
    ilutInit();

    Load_Tex_Image();

    glColor4f(1.0f,1.0f,1.0f,0.5f);   // Полная яркость, 50% альфа (НОВОЕ)
    glBlendFunc(GL_SRC_ALPHA,GL_ONE); // Функция смешивания для непрозрачности,
                                      // базирующаяся на значении альфы(НОВОЕ)

    glEnable(GL_TEXTURE_2D);      // Разрешить наложение текстуры
    glViewport( 0, 0, WinWidth, WinHeight );
    glClearColor(0.6f, 0.8f, 0.8f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0f, (GLfloat)WinWidth/(GLfloat)WinHeight, 0.1f, 100.0f );
    glMatrixMode(GL_MODELVIEW);

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // Установка Фонового Света
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);    // Установка Диффузного Света
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   // Позиция света
    glEnable(GL_LIGHT1); // Разрешение источника света номер один

    glLoadIdentity();
}


void KeyboardEvent(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q' :
            exit(0);
            break;

        case 'a' :
            if (!light)               // Если не свет
            {
                light = true;
                glDisable(GL_LIGHTING);  // Запрет освещения
            }
            glutPostRedisplay();
            break;
        case 'd' :
            if (light)
            {
            light = false;
            glEnable(GL_LIGHTING);
            }
            glutPostRedisplay();
            break;
        case 'f' :
            filter += 1;           // значение filter увеличивается на один
            if (filter > 2)        // Значение больше чем 2?
                filter = 0;
            glutPostRedisplay();
            break;
        case 'w' :
            z+=0.2f;
            glutPostRedisplay();
            break;
        case 's' :
            z-=0.2f;
            glutPostRedisplay();
            break;

    }
}

void processSpecialKeys(int key, int x, int y)
{
      switch(key) {
            case GLUT_KEY_UP :
                  xspeed += 0.2f;
                  break;
            case GLUT_KEY_DOWN:
                  xspeed -= 0.2f;
                  break;
            case GLUT_KEY_LEFT:
                  yspeed -= 0.2f;
                  break;
            case GLUT_KEY_RIGHT:
                  yspeed += 0.2f;
                  break;
            case GLUT_KEY_F1:
                  yspeed = 0;
                  xspeed = 0;
                  break;
      }
}
//-----------------------------------------------------------------------------
//Функция рисования сцены
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,z);      // Перенос В/Вне экрана по z
    glRotatef(x_rotation,1.0f,0.0f,0.0f); // Вращение по оси X на xrot
    glRotatef(y_rotation,0.0f,1.0f,0.0f); // Вращение по оси Y по yrot
    glBindTexture(GL_TEXTURE_2D, texture[filter]);    // Выбор текстуры основываясь на filter
    glBegin(GL_QUADS);       // Начало рисования четырехугольников
    // Передняя грань
      glNormal3f( 0.0f, 0.0f, 1.0f);     // Нормаль указывает на наблюдателя
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); // Точка 1 (Перед)
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); // Точка 2 (Перед)
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); // Точка 3 (Перед)
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); // Точка 4 (Перед)
      // Задняя грань
      glNormal3f( 0.0f, 0.0f,-1.0f);     // Нормаль указывает от наблюдателя
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Зад)
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); // Точка 2 (Зад)
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); // Точка 3 (Зад)
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Точка 4 (Зад)
      // Верхняя грань
      glNormal3f( 0.0f, 1.0f, 0.0f);     // Нормаль указывает вверх
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); // Точка 1 (Верх)
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f); // Точка 2 (Верх)
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f); // Точка 3 (Верх)
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); // Точка 4 (Верх)
      // Нижняя грань
      glNormal3f( 0.0f,-1.0f, 0.0f);     // Нормаль указывает вниз
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Низ)
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Точка 2 (Низ)
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); // Точка 3 (Низ)
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); // Точка 4 (Низ)
      // Правая грань
      glNormal3f( 1.0f, 0.0f, 0.0f);     // Нормаль указывает вправо
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // Точка 1 (Право)
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); // Точка 2 (Право)
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); // Точка 3 (Право)
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); // Точка 4 (Право)
      // Левая грань
      glNormal3f(-1.0f, 0.0f, 0.0f);     // Нормаль указывает влево
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Лево)
      glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); // Точка 2 (Лево)
      glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); // Точка 3 (Лево)
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); // Точка 4 (Лево)
    glEnd();        // Кончили рисовать четырехугольник
    glFlush();
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WinWidth, WinHeight);
    glutInitWindowPosition(100, 70);
    glutCreateWindow("Glut Window");
    glutDisplayFunc(display);
    glutKeyboardFunc(KeyboardEvent);
    glutSpecialFunc( processSpecialKeys );
    init();
    timer_rotation();
    glutMainLoop();
}

