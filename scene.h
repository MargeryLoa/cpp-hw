/* GEMS CPP-project
 * FILE: scene.h - for match-3 scene management
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 28.05.26
 */

#pragma once
#include "game.h"


class Scene {
public:
    static Scene& GetInstance();         
    void Init(int argc, char** argv);
    void Display();
    void Keyboard(unsigned char key, int x, int y);
    void Timer(int val);
    void Reshape(int w, int h);
    void Mouse(int button, int state, int x, int y);

    static void StaticDisplay();
    static void StaticKeyboard(unsigned char key, int x, int y);
    static void StaticTimer(int val);
    static void StaticReshape(int w, int h);
    static void StaticMouse(int button, int state, int x, int y);

private:
    Scene() = default;
    ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Game* m_game = nullptr;
    int m_width = 900, m_height = 900;
    int m_selectedIndex = -1;

    enum CascadePhase {
        PHASE_FIND_MATCH,
        PHASE_WAIT_AFTER_REMOVE,
        PHASE_APPLY_BONUSES,
        PHASE_WAIT_AFTER_BONUS,
        PHASE_GRAVITY,
        PHASE_WAIT_AFTER_GRAVITY,
        PHASE_DONE
    };
    CascadePhase m_cascadePhase = PHASE_DONE;
    void CascadeTimer(int value);
    static void StaticCascadeTimer(int value);
};

