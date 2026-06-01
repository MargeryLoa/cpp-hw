/* ARCA CPP-project
 * FILE: main.cpp - entry program file.
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */


#include "glut.h"
#include "scene.h"

  /* Function for program entry
   * ARGS:
   *  (int) - argument count
   *  (char**) - argument array
   * RETS:
   *  (int) - exit code
   */
int main(int argc, char** argv) {
    Scene::GetInstance().Init(argc, argv);
    return 0;
}