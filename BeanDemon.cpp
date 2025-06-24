#include "BeanDemon.h"
#include "Player.h"
#include "MainInGame.h"
#include "Damaged.h"

void BeanDemon::overlapPlayerDamage()
{
    Player* player = MainInGame::player;

	ColidDir dir = baseColid->IsDirColid(player->GetComponent<ColiderPool>()->GetColider()[0]);

	if (dir == NONE) return;

	if (dir == UP)
	{
		if (isStan)
		{
			SubLife();
			damaged = true;
			isStan = false;
			stateMachine.SelectState(Damage);
		}


		Transform2D* trans = player->GetTransform();
		float dir = trans->GetVel().x >= 0.0f ? 1.0f : -1.0f;
		trans->SetOutVel(dir * -1.0f * 20.0f, -27.0f);
		trans->SetVel(0.0f, 0.0);
	}
	else
	{
		player->GetComponent<Damaged>()->Damage(1);
	}
	
}

void BeanDemon::overlapBullet()
{
    std::list<GameObject*>* pool = MainInGame::objectPool.GetPool();

	for (GameObject* object : *pool)
	{
		if (object == nullptr) continue;

		ColiderPool* targetPool = object->GetComponent<ColiderPool>();
		if (targetPool == nullptr) continue;

		std::vector<Colider2D> objColid = targetPool->GetColider();

		if (objColid[0].GetTag() != "Bullet") continue;

		if (GetComponent<ColiderPool>()->GetColider()[0].IsColid(objColid[0]))
		{
			// ’e‚ÌÁ‹
			MainInGame::objectPool.DeleteRequest(object);

			// ƒXƒ^ƒ“‚ÖˆÚs
			stateMachine.SelectState(Stan);
			isStan = true;
		}
	}
}
