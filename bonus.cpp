/* ARCA CPP-project
 * FILE: carriage.h - for car object
 * PROGRAMMER: Baydakova M.
 * GROUP: 5030102/40004
 * UPDATE: 31.05.2026
 */

#include "bonus.h"
#include "ball.h"
#include "carriage.h"


Bonus::Bonus(const t_vec2& pos)
{
	t_vec2 speed = { 0, -1 };
	t_vec3 color = { 0, 0, 0 };
	mBonusBall = new Ball(pos, speed, color, 10, 8);
	mToDelete = false;
}

Bonus::Bonus(const t_vec2& pos, const t_vec3& color)
{
	t_vec2 speed = { 0, -1 };
	mBonusBall = new Ball(pos, speed, color, 10, 8);
	mToDelete = false;
}

int Bonus::CheckCollect(Carriage* C)
{
	t_vec2 center = mBonusBall->GetPosition(),
		rectPos = C->GetPosition();
	float w = C->GetWidth(), h = C->GetHeight();

	float closestX = std::max(rectPos.x, std::min(center.x, rectPos.x + w));
	float closestY = std::max(rectPos.y, std::min(center.y, rectPos.y + h));
	float dx = center.x - closestX;
	float dy = center.y - closestY;
	float dist = sqrt(dx * dx + dy * dy);
	if (dist >= mBonusBall->GetRadius()) return 0;

	if (fabs(dx) > fabs(dy))
		return -1;
	else
		return 1;

}

void Bonus::Live(Carriage *C)
{
	t_vec2 OldPos = mBonusBall->GetPosition();

	if (OldPos.y >= 10)
	{
		mBonusBall->SetPosition({ OldPos.x, OldPos.y + mBonusBall->GetSpeed().y });
		if (CheckCollect(C))
		{
			SetToDelete(true);
			SetToAct(true);
			return;
		}
	}
	else
		SetToDelete(true);
}