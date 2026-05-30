#pragma once
#include <vector>
#include "bonus.h"
#include "prim.h"

class Square;

class Game {
public:
    static constexpr int GRID_SIZE = 20;

    Game(int screenW, int screenH);
    ~Game();

    void InitGrid(float startX, float startY, float step, float squareSize, const std::vector<t_vec3>& colors);
    void SwapSquares(int idx1, int idx2);
    void ProcessMatches();              
    void UpdateGravityAndRefill();      
    bool FindAndMarkMatches();         
    void ApplyPendingBonuses();
    void ApplyGravityAndRefill();    
    void DropBonus(int row, int col, const t_vec3& color);
    void RecolorSquare(int idx, const t_vec3& newColor);
    void DestroySquare(int idx);
    bool IsProcessing() const { return m_processing; }
    void SetProcessing(bool p) { m_processing = p; }
    std::vector<Square*>& GetSquares() { return m_squares; }
    const std::vector<t_vec3>& GetColors() const { return m_palette; }
    void AddPendingBonus(Bonus* b) { m_pendingBonuses.push_back(b); }
    void ClearPendingBonuses() { for (auto b : m_pendingBonuses) delete b; m_pendingBonuses.clear(); }
    bool HasPendingBonuses() const { return !m_pendingBonuses.empty(); }

private:
    std::vector<Square*> m_squares;
    std::vector<t_vec3> m_palette;
    std::vector<Bonus*> m_pendingBonuses;
    bool m_processing = false;

    void MarkMatches(bool marked[GRID_SIZE][GRID_SIZE]);
    bool IsValidMatch(int r1, int c1, int r2, int c2);
    
    void FindExtraCandidates(int tgtRow, int tgtCol, int targetIdx, std::vector<int>& out);
    void FindBombArea(int tgtRow, int tgtCol, std::vector<int>& out);
    std::vector<std::vector<int>> GenerateColorIndices(const std::vector<t_vec3>& colors);
};

