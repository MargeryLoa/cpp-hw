/* GEMS CPP-project
 * FILE: scene.cpp - implementation of match-3 scene logic
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 28.05.26
 */

#include "glut.h"
#include <cstdlib>
#include <ctime>
#include <cmath>   
#include <algorithm>
#include "scene.h"
#include "prim.h"
#include "square.h"
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
    glClearColor(0.9f, 0.87f, 0.88f, 1.0f);

    float minSide = std::min(m_width, m_height);
    float squareSize = minSide * 0.8f / Game::GRID_SIZE;
    float spacing = squareSize * 0.1f;
    float step = squareSize + spacing;
    float totalSpan = (Game::GRID_SIZE - 1) * step;
    float startX = (m_width - totalSpan - squareSize) / 2.0f + squareSize / 2.0f;
    float startY = (m_height - totalSpan - squareSize) / 2.0f + squareSize / 2.0f;

    std::vector<t_vec3> colors = {
        {128,0,32},{176,48,96},{220,100,140},{75,0,130},{138,43,226},{200,150,255},{85,107,47},{204,85,0},{139,69,19}
    };
    srand((unsigned)time(nullptr));

    m_game = new Game(m_width, m_height);
    m_game->InitGrid(startX, startY, step, squareSize, colors);

    glutMainLoop();
}

void Scene::Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (auto* sq : m_game->GetSquares())
        if (sq->IsToDraw()) sq->Draw();
    glColor3f(1, 1, 1);
    glRasterPos2i(10, m_height - 20);
    const char* msg = "GEMS: click adjacent squares | ESC exit";
    for (const char* c = msg; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    glutSwapBuffers();
}

void Scene::Mouse(int button, int state, int x, int y) {
    if (m_cascadePhase != PHASE_DONE) return;
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    int mx = x, my = m_height - y;
    int hit = -1;
    for (size_t i = 0; i < m_game->GetSquares().size(); ++i) {
        Square* sq = static_cast<Square*>(m_game->GetSquares()[i]);
        t_vec2 pos = sq->GetPosition();
        float sz = sq->GetSize();
        if (mx >= pos.x - sz / 2 && mx <= pos.x + sz / 2 &&
            my >= pos.y - sz / 2 && my <= pos.y + sz / 2) {
            hit = (int)i; break;
        }
    }
    if (hit == -1) { m_selectedIndex = -1; return; }

    if (m_selectedIndex == -1) {
        m_selectedIndex = hit;
        int r = hit / Game::GRID_SIZE, c = hit % Game::GRID_SIZE;
        printf("Selected square %d (X:%d,Y:%d)\n", hit, c, r);
    }
    else {
        int idx1 = m_selectedIndex, idx2 = hit;
        int r1 = idx1 / Game::GRID_SIZE, c1 = idx1 % Game::GRID_SIZE;
        int r2 = idx2 / Game::GRID_SIZE, c2 = idx2 % Game::GRID_SIZE;
        bool neighbor = (abs(r1 - r2) == 1 && c1 == c2) || (abs(c1 - c2) == 1 && r1 == r2);
        if (neighbor) {
            m_game->SwapSquares(idx1, idx2);
            if (m_cascadePhase == PHASE_DONE) {
                m_cascadePhase = PHASE_FIND_MATCH;
                CascadeTimer(0); 
            }
            //m_game->ProcessMatches();
        }
        else printf("Not neighbors: (%d,%d) and (%d,%d)\n", r1, c1, r2, c2);
        m_selectedIndex = -1;
    }
    glutPostRedisplay();
}

void Scene::Keyboard(unsigned char key, int, int) {
    if (key == 27) exit(0);
    if (!m_game->GetSquares().empty()) {
        t_vec2 sp = m_game->GetSquares()[0]->GetSpeed();
        if (key == '+') { sp.x += 0.002f; sp.y += 0.002f; m_game->GetSquares()[0]->SetSpeed(sp); }
        else if (key == '-') { sp.x -= 0.002f; sp.y -= 0.002f; m_game->GetSquares()[0]->SetSpeed(sp); }
    }
}

void Scene::Timer(int) {
    for (auto* sq : m_game->GetSquares()) sq->Update(m_width, m_height);
    glutPostRedisplay();
    glutTimerFunc(16, StaticTimer, 0);
}

void Scene::Reshape(int w, int h) {
    m_width = w; m_height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void Scene::StaticDisplay() { GetInstance().Display(); }
void Scene::StaticKeyboard(unsigned char k, int x, int y) { GetInstance().Keyboard(k, x, y); }
void Scene::StaticTimer(int v) { GetInstance().Timer(v); }
void Scene::StaticReshape(int w, int h) { GetInstance().Reshape(w, h); }
void Scene::StaticMouse(int b, int s, int x, int y) { GetInstance().Mouse(b, s, x, y); }


void Scene::CascadeTimer(int) {
    switch (m_cascadePhase) {
        case PHASE_FIND_MATCH:
            if (m_game->FindAndMarkMatches()) {
                m_cascadePhase = PHASE_WAIT_AFTER_REMOVE;
                glutTimerFunc(500, StaticCascadeTimer, 0);
            } else {
                m_cascadePhase = PHASE_DONE;
            }
            break;

        case PHASE_WAIT_AFTER_REMOVE:
            if (m_game->HasPendingBonuses()) {
                m_game->ApplyPendingBonuses();
                m_cascadePhase = PHASE_WAIT_AFTER_BONUS;
                glutTimerFunc(500, StaticCascadeTimer, 0);
            } else {
                m_game->ApplyGravityAndRefill();
                m_cascadePhase = PHASE_WAIT_AFTER_GRAVITY;
                glutTimerFunc(500, StaticCascadeTimer, 0);
            }
            break;

        case PHASE_WAIT_AFTER_BONUS:
            m_game->ApplyGravityAndRefill();
            m_cascadePhase = PHASE_WAIT_AFTER_GRAVITY;
            glutTimerFunc(500, StaticCascadeTimer, 0);
            break;

        case PHASE_WAIT_AFTER_GRAVITY:
            
            m_cascadePhase = PHASE_FIND_MATCH;
            CascadeTimer(0); 
            break;

        default:
            break;
    }
    glutPostRedisplay();
}

void Scene::StaticCascadeTimer(int value) {
    GetInstance().CascadeTimer(value);
}

