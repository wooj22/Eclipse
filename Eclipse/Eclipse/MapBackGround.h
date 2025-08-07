#pragma once
#include "../Direct2D_EngineLib/GameObject.h"

class ParallaxLayer;

class Transform;
class SpriteRenderer;

class BackGround00_Sky;
class BackGround01_Moon;
class BackGround02_Mount;
class BackGround03_Tree;

class BackGround04_Grass;
class BackGround05_Ground;
class BackGround06_House;
class BackGround07_Trash;

class BackGround08_Light;
class BackGround09_Shadow;

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

	BackGround04_Grass* backGround04_Grass;
	BackGround05_Ground* backGround05_Ground;
	BackGround06_House* backGround06_House;
	BackGround07_Trash* backGround07_Trash;

	BackGround08_Light* backGround08_Light;
	BackGround09_Shadow* backGround09_Shadow;

public:
	MapBackGround();
	void SceneStart();
};


/*------------------------------------------------*/
/*-------------- Parallax Layer ------------------*/
/*------------------------------------------------*/

class BackGround00_Sky : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	ParallaxLayer* parallax;

public:
	BackGround00_Sky();
};


class BackGround01_Moon : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	ParallaxLayer* parallax;

public:
	BackGround01_Moon();
};


class BackGround02_Mount : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	ParallaxLayer* parallax;

public:
	BackGround02_Mount();
};


class BackGround03_Tree : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	ParallaxLayer* parallax;

public:
	BackGround03_Tree();
};

class BackGround04_Grass : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	ParallaxLayer* parallax;

public:
	BackGround04_Grass();
};


/*------------------------------------------------*/
/*---------------- Static Map --------------------*/
/*------------------------------------------------*/

class BackGround05_Ground : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	BackGround05_Ground();
};

class BackGround06_House : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	BackGround06_House(); 
};

class BackGround07_Trash : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	BackGround07_Trash();
};


/*------------------------------------------------*/
/*------------ Front Light, Shadow ----------------*/
/*------------------------------------------------*/
class BackGround08_Light : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	ParallaxLayer* parallax;

public:
	BackGround08_Light();
};

class BackGround09_Shadow : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;

public:
	BackGround09_Shadow();
};
