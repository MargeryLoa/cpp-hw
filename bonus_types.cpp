/* ARCA CPP-project
 * FILE: carriage.h - for car object
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */
#pragma once

#include "prim.h"

#include "stuck.h"
#include "snd_bottom.h"
#include "snd_ball.h"
#include "resize.h"
#include "sp_down.h"

ResizeBonus::ResizeBonus(const t_vec2& pos) : Bonus(pos, { 246, 182, 183 }) {} // Powder Blush

SpeedDownBonus::SpeedDownBonus(const t_vec2& pos) : Bonus(pos, { 56, 90, 72 }) {} // Pine Shadow

SecondBallBonus::SecondBallBonus(const t_vec2& pos) : Bonus(pos, { 149, 127, 137 }) {} // Dusky Heather

SecondBottomBonus::SecondBottomBonus(const t_vec2& pos) : Bonus(pos, { 39, 21, 61 }) {} // Dark Amethyst

StuckBonus::StuckBonus(const t_vec2& pos) : Bonus(pos, { 166, 201, 182 }) {} // Muted Teal


void ResizeBonus::Act(std::vector<Ball*>& balls, std::vector<Carriage*>& carriages)
{
	carriages[0]->SetWidth(carriages[0]->GetWidth() * 1.5);
	carriages[0]->UpdateCar();
	SetToAct(0);
}

void SpeedDownBonus::Act(std::vector<Ball*>& balls, std::vector<Carriage*>& carriages)
{
	for (int i = 0; i < balls.size(); i++)
	{
		t_vec2 OldSpeed = balls[i]->GetSpeed();
		balls[i]->SetSpeed({ (float)0.9 * OldSpeed.x, OldSpeed.y * (float)0.9 });
	}
	SetToAct(0);
}

void SecondBallBonus::Act(std::vector<Ball*>& balls, std::vector<Carriage*>& carriages)
{
	Ball* B = new Ball({ 450, 450 }, { 5, 5 }, { 200, 0, 0 }, 15, 8);
	balls.push_back(B);
	SetToAct(0);
}

void SecondBottomBonus::Act(std::vector<Ball*>& balls, std::vector<Carriage*>& carriages)
{
	carriages[1]->SetToDraw(1);
	SetToAct(0);
}

void StuckBonus::Act(std::vector<Ball*>& balls, std::vector<Carriage*>& carriages)
{
	SetToAct(0);
}