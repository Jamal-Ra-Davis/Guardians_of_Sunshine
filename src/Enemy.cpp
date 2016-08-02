#include "../headers/Enemy.h"
#include "../headers/SDL_Helpers.h"

void Enemy::hit(int damageTaken)
{
	if (dead || flinching)
		return;

	health -= damageTaken;
	if (health <= 0)
	{
		health = 0;
		dead = true;
	}
	flinching = true;
	flinchTimer = getMs();
}
void Enemy::getPoofPosition(double *x_, double *y_)
{
	*x_ = x;
	*y_ = y;
}
