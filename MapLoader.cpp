#include "MapLoader.h"
#include "MainInGame.h"
#include "MainInGame.h"

#include "LandBlock.h"
#include "DashBlock.h"
#include "BulletEnemmy.h"
#include "Box.h"
#include "WoodBox.h"
#include "ThornBlock.h"
#include "RockGenerater.h"
#include "BreakBlock.h"
#include "PermationBlock.h"
#include "StarCoin.h"
#include "Goal.h"
#include "OneEyeEnemy.h"
#include "WallEnem.h"
#include "TrampolineBlock.h"
#include "Coin.h"
#include "Switch.h"
#include "MoveSwitchBlock.h"
#include "MoveBlock.h"
#include "DashBlock.h"
#include "BulletSwingEnemmy.h"
#include "ThornProtrudeBlock.h"

#include <fstream>

static float g_mapWide;
static float g_mapHeight;

static int loadCurrentLayer;

static int sumScore;
static int sumEnemy;
static int sumCoin;

void LoadSelectObj(std::ifstream* ifs, std::string name, std::list<GameObject*>& map, ObjectPool* pool);

bool LoadMap(char* load, std::list<GameObject*>* map, ObjectPool* pool)
{
    sumScore = 0;
    sumEnemy = 0;
    sumCoin = 0;
    Camera* camera = Camera::GetInstance();

    std::ifstream ifs(load);
    std::string str;

    if (ifs.fail())return false;

    // マップの範囲を読み取る
    ifs >> str;
    g_mapWide = std::stof(str);
    ifs >> str;
    g_mapHeight = std::stof(str);

    camera->SetRightLimit(g_mapWide);
    camera->SetDownLimit(g_mapHeight);


    // オブジェクト読み込み
    while (ifs >> str)
    {
        if (str == "Layer:")
        {
            ifs >> str;
            loadCurrentLayer = std::stoi(str);
            if (loadCurrentLayer > MAX_LAYER)loadCurrentLayer = MAX_LAYER;
        }

        if (str == "(")
        {
            ifs >> str;
            LoadSelectObj(&ifs, str, map[loadCurrentLayer], pool);
        }
    }
    ifs.close();

    // スコア計算
    sumScore = sumCoin * 100 + sumEnemy * 200 + 4000 + 2100;
    OnGameData::GetInstance()->SetMaxScore(sumScore);

    // 目標指定
    OnGameData::GetInstance()->SetTargetCoinNum(sumCoin * 0.5f);
    OnGameData::GetInstance()->SetTargetEnemyNum(sumEnemy * 0.7f);

    return true;
}

void LoadSelectObj(std::ifstream* ifs, std::string name, std::list<GameObject*>& map, ObjectPool* pool)
{
    D3DXVECTOR2 pos, size;
    std::string str;

    // 共通部分
    *ifs >> str;
    pos.x = std::stof(str);
    *ifs >> str;
    pos.y = std::stof(str);
    *ifs >> str;
    size.x = std::stof(str);
    *ifs >> str;
    size.y = std::stof(str);

    if (name == "DIRT")
    {
        map.push_back(new LandBlock(pos, size, 0));
    }
    else if (name == "GRASS")
    {
        map.push_back(new LandBlock(pos, size, 1));
    }
    else if (name == "THORN_BLOCK")
    {
        map.push_back(new ThornBlock(pos, size));
    }
    else if (name == "THORN_PROTUDE")
    {
        pool->Add(new ThornProtrudeBlock(pos, size));
    }
    else if (name == "PERMATION_BLOCK_DIRT")
    {
        map.push_back(new PermationBlock(pos, size, 0));
    }
    else if (name == "BREAK_BLOCK")
    {
        pool->Add(new BreakBlock(pos, size));
    }
    else if (name == "BEnem")
    {
        pool->Add(new BulletEnemmy(pos, size));
        ++sumEnemy;
    }
    else if (name == "ONE_EYE")
    {
        pool->Add(new OneEyeEnemy({pos.x, pos.y - 10.0f}, size));
        ++sumEnemy;
    }
    else if (name == "WOOD_BOX")
    {
        pool->Add(new WoodBox(pos, size));
    }
    else if (name == "IRON_BOX")
    {
        pool->Add(new Box(pos, size));
    }
    else if (name == "MOVE_BLOCK")
    {
        pool->Add(new MoveBlock(pos, size));
    }
    else if (name == "MOVE_SWITCH_BLOCK")
    {
        pool->Add(new MoveSwitchBlock(pos, size));
    }
    else if (name == "SWING_BULLET")
    {
        pool->Add(new BulletSwingEnemmy(pos, size));
    }
    else if (name == "DASH_BLOCK_R" || name == "DASH_BLOCK_L")
    {
        *ifs >> str;
        bool right = std::stoi(str);
        map.push_back(new DashBlock(pos, size, right));
    }
    else if (name == "SWITCH")
    {
        *ifs >> str;
        int dir = std::stoi(str);
        pool->Add(new Switch(pos, size, dir));
    }
    else if (name == "PLAYER")
    {
        MainInGame::player = new Player({pos.x, pos.y - 10.0f}, D3DXVECTOR2(140, 160.0f));
    }
    else if (name == "GOAL")
    {
        Goal* goal = new Goal({pos.x, pos.y - CHIP_Y_SIZE * 0.9f}, {CHIP_X_SIZE * 1.5f, CHIP_Y_SIZE * 3.0f});
        pool->Add(goal);
        MainInGame::goal = goal;
    }
    else if (name == "ROCK_GENERATOR")
    {   
        *ifs >> str;
        bool left = std::stoi(str);
        pool->Add(new RockGenerater(pos, size, left));
    }
    else if (name == "WALL_ENEM")
    {
        pos = D3DXVECTOR2(0.0f, g_mapHeight / 2);
        size = D3DXVECTOR2(size.x, g_mapHeight);
        pool->Add(new WallEnem(pos, size));
    }
    else if (name == "COIN")
    {
        ++sumCoin;
        pool->Add(new Coin(pos, size));
    }
    else if (name == "STAR_COIN")
    {
        pool->Add(new StarCoin(pos, size));
    }

    *ifs >> str;    // )読み用
}
