/* ARCA CPP-project
 * FILE: scene.h - for scene management
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */

#pragma once

class Player
{
public:
	Player() { mID = 0; mScore = 0; }
	~Player() = default;

	int GetScore() const { return mScore; }
	int GetID() const { return mID; }

	void ChangeScore(int Delta) { mScore += Delta; }
private:
	int mID;
	int mScore;
};