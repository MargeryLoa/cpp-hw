/* GEMS CPP-project
 * FILE: scene.cpp - implementation of match-3 scene logic
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */

#include "glut.h"
#include <cstdlib>
#include <ctime>
#include <cmath>   
#include <algorithm>
#include "scene.h"
#include <cstdio>

Scene& Scene::GetInstance() {
    static Scene instance;
    return instance;
}

void Scene::Init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(m_width, m_height);
    glutCreateWindow("GEMS - Match-3");

    glutDisplayFunc(StaticDisplay);
    glutKeyboardFunc(StaticKeyboard);
    glutReshapeFunc(StaticReshape);
    glutMouseFunc(StaticMouse);
    glutTimerFunc(16, StaticTimer, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glClearColor(0.682f, 0.835f, 0.961f, 1.0f);

    std::vector<t_vec3> colors = {
        {14, 204, 237},  // Spiro Disco Ball
        {2, 94, 196},    // Sapphire
        {4, 55, 128},    // Dark Cerulean
        {2, 7, 100},     // Navy Blue
        {3, 8, 18},       // Rich Black
        {100, 7, 2}      // Unbreking color
    };
    mBlockPalette = colors;
    srand((unsigned)time(nullptr));

    m_game = new Game(m_width, m_height, colors);
    float startX = (m_width - m_game->GRID_W * (m_game->BLOCK_W + m_game->DIST) + m_game->DIST) / 2,//50,
        startY = m_height - (m_game->GRID_H * (m_game->BLOCK_H + m_game->DIST) - m_game->DIST) - 50;

    t_vec2 CarPos = {m_width / 2, m_height / 2 - 280};
    m_game->InitObjects(startX, startY, CarPos, colors, m_width);

    glutMainLoop();
}

void Scene::Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
   

    for (auto* sq : m_game->GetBlocks())
        if (sq->IsToDraw()) sq->Draw();
    for (auto* sq : m_game->GetCars())
        if (sq->IsToDraw()) sq->Draw();
    for (auto* sq : m_game->GetBalls())
        if (sq->IsToDraw()) sq->Draw();
    for (auto* sq : m_game->GetBonuses())
        if (sq->GetBall()->IsToDraw()) sq->GetBall()->Draw();


    glColor3f(0, 0, 0);  
    
    if (!m_game->GetBalls().empty()) {
        t_vec2 speed = m_game->GetBalls()[0]->GetSpeed();
        char speedText[64];
        sprintf(speedText, "Ball speed: X = %.1f  Y = %.1f", speed.x, speed.y);
        glRasterPos2i(10, m_height-20);
        for (const char* c = speedText; *c; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    if (m_game->GetPlayer() != nullptr) {
        int CurScore = m_game->GetPlayer()->GetScore(),
            ID = m_game->GetPlayer()->GetID();
            
        char speedText[64];
        sprintf(speedText, "Player #%i Score: %i", ID, CurScore);
        glRasterPos2i(10, m_height - 40);
        for (const char* c = speedText; *c; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glutSwapBuffers();
}

void Scene::Mouse(int button, int state, int x, int y) {
    glutPostRedisplay();
}

void Scene::Keyboard(unsigned char key, int, int) {
    if (key == 27) // Escape
        exit(0);
    if (key == 'd' || key == 'D') // Left
        m_game->MoveCar(1, m_width - 5);
    if (key == 'a' || key == 'A') // Right
        m_game->MoveCar(-1, 5);
    if (key == 'z')
        ;//m_game->InitObjects(startX, startY, CarPos, colors, m_width);
    
}

void Scene::Timer(int) 
{
    for (auto* sq : m_game->GetBalls())
        if (sq->IsToDraw()) sq->Update(m_width, m_height);
    m_game->CheckCollideCar();
    m_game->CheckCollideBlock();
    m_game->CheckLoose();
    m_game->CheckCollide2Balls();
    for (auto* sq : m_game->GetBonuses())
        sq->Live(m_game->GetCars()[0]);

    m_game->ActBonus();
    m_game->FreeBonus();
    glutPostRedisplay();
    glutTimerFunc(16, StaticTimer, 0);

}

void Scene::Reshape(int w, int h) {
    m_width = w; 
    m_height = h;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void Scene::StaticDisplay() { GetInstance().Display(); }
void Scene::StaticKeyboard(unsigned char k, int x, int y) { GetInstance().Keyboard(k, x, y); }
void Scene::StaticTimer(int v) { GetInstance().Timer(v); }
void Scene::StaticReshape(int w, int h) { GetInstance().Reshape(w, h); }
void Scene::StaticMouse(int b, int s, int x, int y) { GetInstance().Mouse(b, s, x, y); }

