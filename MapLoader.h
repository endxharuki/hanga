#pragma once

#include <list>;
#include "ObjectPool.h"

bool LoadMap(char* load, std::list<GameObject*>* map, ObjectPool* pool);
