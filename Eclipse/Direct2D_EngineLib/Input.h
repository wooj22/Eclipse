#pragma once
#include <Windows.h>

class Input
{
private:
    static HWND hWnd;
    static POINT mouseClient;
    static SHORT prevState[256];
    static SHORT currState[256];

    static float horizontalAxis;
    static float verticalAxis;

public:
    static void Init(HWND hwnd);
    static void Update();

    static bool GetKey(int vKey);
    static bool GetKeyDown(int vKey);
    static bool GetKeyUp(int vKey);

    static POINT GetMouseScreenPosition();
    static POINT GetMouseWorldPosition();

    static float GetAxisHorizontal();
    static float GetAxisVertical();
};