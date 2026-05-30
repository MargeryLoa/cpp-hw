#include "game.h"
#include "square.h"
#include "bonus.h"
#include "bomb.h"
#include "recolor.h"
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cstdio>

Game::Game(int w, int h) { (void)w; (void)h; }
Game::~Game() {
    for (auto* sq : m_squares) delete sq;
    for (auto* b : m_pendingBonuses) delete b;
}

/* Function for generating a grid of color indices without three identical in a row
 * ARGS:
 *  (const std::vector<t_vec3>&) - color palette
 * RETS:
 *  (std::vector<std::vector<int>>) - 2D vector of color indices (GRID_SIZE x GRID_SIZE)
 */
std::vector<std::vector<int>> Game::GenerateColorIndices(const std::vector<t_vec3>& colors) {
    std::vector<std::vector<int>> grid(GRID_SIZE, std::vector<int>(GRID_SIZE, -1));

    auto isValid = [&](int row, int col, int idx) -> bool {
        // Check horizontal
        if (col >= 2 && grid[row][col - 1] == idx && grid[row][col - 2] == idx)
            return false;
        // Check vertical
        if (row >= 2 && grid[row - 1][col] == idx && grid[row - 2][col] == idx)
            return false;
        return true;
        };

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            std::vector<int> available;
            for (size_t i = 0; i < colors.size(); ++i)
                available.push_back((int)i);
            std::random_shuffle(available.begin(), available.end());

            int chosen = -1;
            for (int idx : available) {
                if (isValid(row, col, idx)) {
                    chosen = idx;
                    break;
                }
            }
            if (chosen == -1) chosen = available[0];
            grid[row][col] = chosen;
        }
    }
    return grid;
}

/* Function for initializing the grid with squares using pre‑generated color indices
 * ARGS:
 *  (float) - start X coordinate (leftmost center)
 *  (float) - start Y coordinate (bottommost center)
 *  (float) - step between adjacent square centers
 *  (float) - side length of each square
 *  (const std::vector<t_vec3>&) - color palette
 * RETS: None.
 */
void Game::InitGrid(float startX, float startY, float step, float squareSize,
    const std::vector<t_vec3>& palette) {
    m_palette = palette;
    // Generate color indices avoiding three in a row
    auto colorIndices = GenerateColorIndices(palette);

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            t_vec2 pos = { startX + col * step, startY + row * step };
            t_vec3 color = palette[colorIndices[row][col]];
            m_squares.push_back(new Square(pos, { 0,0 }, color, squareSize));
        }
    }
}
void Game::SwapSquares(int idx1, int idx2) {
    Square* a = m_squares[idx1];
    Square* b = m_squares[idx2];
    t_vec3 tmp = a->GetColor();
    a->SetColor(b->GetColor());
    b->SetColor(tmp);
    a->SetToDraw(true);
    b->SetToDraw(true);
}

bool Game::FindAndMarkMatches() {
    bool marked[GRID_SIZE][GRID_SIZE] = { false };
    auto eq = [](const t_vec3& a, const t_vec3& b) {
        return a.r == b.r && a.g == b.g && a.b == b.b;
        };

    // горизонталь
    for (int r = 0; r < GRID_SIZE; ++r) {
        int len = 1;
        for (int c = 1; c <= GRID_SIZE; ++c) {
            if (c < GRID_SIZE && m_squares[r * GRID_SIZE + c]->IsToDraw() &&
                m_squares[r * GRID_SIZE + c - 1]->IsToDraw() &&
                eq(m_squares[r * GRID_SIZE + c]->GetColor(), m_squares[r * GRID_SIZE + c - 1]->GetColor()))
                ++len;
            else {
                if (len >= 3) {
                    printf("Deleted Group ");
                    for (int cc = c - len; cc < c; ++cc) {
                        printf("X:%d,Y:%d ", cc, r);
                        marked[r][cc] = true;
                    }
                    printf("\n");
                }
                len = 1;
            }
        }
    }
    // вертикаль
    for (int c = 0; c < GRID_SIZE; ++c) {
        int len = 1;
        for (int r = 1; r <= GRID_SIZE; ++r) {
            if (r < GRID_SIZE && m_squares[(r - 1) * GRID_SIZE + c]->IsToDraw() &&
                m_squares[r * GRID_SIZE + c]->IsToDraw() &&
                eq(m_squares[(r - 1) * GRID_SIZE + c]->GetColor(), m_squares[r * GRID_SIZE + c]->GetColor()))
                ++len;
            else {
                if (len >= 3) {
                    printf("Deleted Group ");
                    for (int rr = r - len; rr < r; ++rr) {
                        printf("X:%d,Y:%d ", c, rr);
                        marked[rr][c] = true;
                    }
                    printf("\n");
                }
                len = 1;
            }
        }
    }

    bool any = false;
    for (int r = 0; r < GRID_SIZE; ++r)
        for (int c = 0; c < GRID_SIZE; ++c)
            if (marked[r][c]) {
                any = true;
                m_squares[r * GRID_SIZE + c]->SetToDraw(false);
                DropBonus(r, c, m_squares[r * GRID_SIZE + c]->GetColor());
            }
    return any;
}

void Game::ApplyGravityAndRefill() {
    for (int col = 0; col < GRID_SIZE; ++col) {
        std::vector<t_vec3> visible;
        for (int row = 0; row < GRID_SIZE; ++row) {
            Square* sq = m_squares[row * GRID_SIZE + col];
            if (sq->IsToDraw()) visible.push_back(sq->GetColor());
        }
        int newRow = 0;
        for (size_t i = 0; i < visible.size(); ++i) {
            Square* sq = m_squares[newRow * GRID_SIZE + col];
            sq->SetColor(visible[i]);
            sq->SetToDraw(true);
            ++newRow;
        }
        for (int row = newRow; row < GRID_SIZE; ++row) {
            Square* sq = m_squares[row * GRID_SIZE + col];
            sq->SetColor(m_palette[rand() % m_palette.size()]);
            sq->SetToDraw(true);
        }
    }
}

void Game::DropBonus(int row, int col, const t_vec3& origColor) {
    if (rand() % 15 != 0) return; // шанс 1/15
    // поиск цели в радиусе 3
    std::vector<int> candidates;
    for (int dr = -3; dr <= 3; ++dr)
        for (int dc = -3; dc <= 3; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int nr = row + dr, nc = col + dc;
            if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE) {
                int idx = nr * GRID_SIZE + nc;
                if (m_squares[idx]->IsToDraw()) candidates.push_back(idx);
            }
        }
    if (candidates.empty()) return;
    int target = candidates[rand() % candidates.size()];
    int tgtRow = target / GRID_SIZE, tgtCol = target % GRID_SIZE;

    if (rand() % 2 == 0) { // Recolor
        std::vector<int> extra;
        FindExtraCandidates(tgtRow, tgtCol, target, extra);
        m_pendingBonuses.push_back(new RecolorBonus(target, origColor, extra));
    }
    else { // Bomb
        std::vector<int> area;
        FindBombArea(tgtRow, tgtCol, area);
        if (!area.empty()) m_pendingBonuses.push_back(new BombBonus(area));
    }
}

void Game::FindExtraCandidates(int tgtRow, int tgtCol, int targetIdx, std::vector<int>& out) {
    for (int dr = -3; dr <= 3; ++dr)
        for (int dc = -3; dc <= 3; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int nr = tgtRow + dr, nc = tgtCol + dc;
            if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE) {
                int idx = nr * GRID_SIZE + nc;
                if (!m_squares[idx]->IsToDraw()) continue;
                int dist = std::abs(nr - tgtRow) + std::abs(nc - tgtCol);
                if (idx != targetIdx && dist > 1)  // не сосед
                    out.push_back(idx);
            }
        }
    std::random_shuffle(out.begin(), out.end());
    if (out.size() > 2) out.resize(2);
}

void Game::FindBombArea(int tgtRow, int tgtCol, std::vector<int>& out) {
    for (int dr = -3; dr <= 3; ++dr)
        for (int dc = -3; dc <= 3; ++dc) {
            int nr = tgtRow + dr, nc = tgtCol + dc;
            if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE) {
                int idx = nr * GRID_SIZE + nc;
                if (m_squares[idx]->IsToDraw()) out.push_back(idx);
            }
        }
    std::random_shuffle(out.begin(), out.end());
    if (out.size() > 5) out.resize(5);
}

void Game::RecolorSquare(int idx, const t_vec3& newColor) {
    m_squares[idx]->SetColor(newColor);
    m_squares[idx]->SetToDraw(true);
}

void Game::DestroySquare(int idx) {
    m_squares[idx]->SetToDraw(false);
}

void Game::ApplyPendingBonuses() {
    for (auto* b : m_pendingBonuses) {
        // вывод отладочной информации
        // для простоты оставим как было, но можно реализовать метод Print в бонусах
        b->Apply(this);
    }
    ClearPendingBonuses();
}

/* Function for starting the match processing after a swap
 * ARGS: None.
 * RETS: None.
 */
void Game::ProcessMatches() {
    if (m_processing) return;
    m_processing = true;
    // Циклическая обработка матчей (упрощённо, без пауз для демонстрации)
    while (FindAndMarkMatches()) {
        ApplyGravityAndRefill();
    }
    m_processing = false;
}

/* Function for public wrapper to apply gravity and refill
 * ARGS: None.
 * RETS: None.
 */
void Game::UpdateGravityAndRefill() {
    ApplyGravityAndRefill();
}

/* Function for marking matches (unused, kept for interface completeness)
 * ARGS:
 *  (bool[GRID_SIZE][GRID_SIZE]) - output marked array (ignored)
 * RETS: None.
 */
void Game::MarkMatches(bool marked[GRID_SIZE][GRID_SIZE]) {
    (void)marked; // suppress unused parameter warning
    // Actual marking logic is embedded in FindAndMarkMatches
}

/* Function for checking if two squares form a valid match (same color and visible)
 * ARGS:
 *  (int) - row of first square
 *  (int) - column of first square
 *  (int) - row of second square
 *  (int) - column of second square
 * RETS:
 *  (bool) - true if both squares are visible and have the same color
 */
bool Game::IsValidMatch(int r1, int c1, int r2, int c2) {
    Square* s1 = m_squares[r1 * GRID_SIZE + c1];
    Square* s2 = m_squares[r2 * GRID_SIZE + c2];
    if (!s1->IsToDraw() || !s2->IsToDraw()) return false;
    t_vec3 col1 = s1->GetColor();
    t_vec3 col2 = s2->GetColor();
    return (col1.r == col2.r && col1.g == col2.g && col1.b == col2.b);
}