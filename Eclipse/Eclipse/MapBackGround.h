#pragma once
#include "../Direct2D_EngineLib/GameObject.h"

class Transform;
class SpriteRenderer;
class BackGround00_Sky;
class BackGround01_Moon;
class BackGround02_Mount;
class BackGround03_Tree;

/* Map Background GameObject Manager */
class MapBackGround : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	// child gameObject
	BackGround00_Sky* backGround00_Sky;
	BackGround01_Moon* backGround01_Moon;
	BackGround02_Mount* backGround02_Mount;
	BackGround03_Tree* backGround03_Tree;

public:
	MapBackGround();
};


/*------------------------------------------------*/
/*------------------- Layer ----------------------*/
/*------------------------------------------------*/

class BackGround00_Sky : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	BackGround00_Sky();
};


class BackGround01_Moon : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	BackGround01_Moon();
};


class BackGround02_Mount : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	BackGround02_Mount();
};


class BackGround03_Tree : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	BackGround03_Tree();
};

