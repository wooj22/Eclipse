﻿#include "Input.h"
#include "RectTransform.h"
#include "Camera.h"

// static member init
HWND Input::hWnd = nullptr;
POINT Input::mouseClient = { 0, 0 };
SHORT Input::prevState[256] = { 0 };
SHORT Input::currState[256] = { 0 };

float Input::horizontalAxis = 0.0f;
float Input::verticalAxis = 0.0f;


void Input::Init(HWND hwnd)
{
    hWnd = hwnd;
    GetCursorPos(&mouseClient);
    ScreenToClient(hWnd, &mouseClient);

    // 초기 키 상태
    for (int i = 0; i < 256; ++i)
        currState[i] = GetAsyncKeyState(i);
}

void Input::Update()
{
    memcpy(prevState, currState, sizeof(SHORT) * 256);

    // key
    for (int i = 0; i < 256; ++i)
        currState[i] = GetAsyncKeyState(i);

    // mouse
    GetCursorPos(&mouseClient);
    ScreenToClient(hWnd, &mouseClient);

    // horizontal
    horizontalAxis = 0.0f;
    if (GetKey('A') || GetKey(VK_LEFT))  horizontalAxis -= 1.0f;
    if (GetKey('D') || GetKey(VK_RIGHT)) horizontalAxis += 1.0f;

    // vertical
    verticalAxis = 0.0f;
    if (GetKey('W') || GetKey(VK_UP))    verticalAxis += 1.0f;
    if (GetKey('S') || GetKey(VK_DOWN))  verticalAxis -= 1.0f;
}

bool Input::GetKey(int vKey)
{
    return (currState[vKey] & 0x8000) != 0;
}

bool Input::GetKeyDown(int vKey)
{
    return !(prevState[vKey] & 0x8000) && (currState[vKey] & 0x8000);
}

bool Input::GetKeyUp(int vKey)
{
    return (prevState[vKey] & 0x8000) && !(currState[vKey] & 0x8000);
}

POINT Input::GetMouseScreenPosition()
{
    POINT mouse = mouseClient;
    D2D1_POINT_2F pt = { static_cast<float>(mouse.x), static_cast<float>(mouse.y) };

    // Unity 스타일 좌표계 변환 행렬 (renderMatrix 제외)
    D2D1::Matrix3x2F unityMatrix = RectTransform::unityMatrix;

    // 역행렬 계산
    D2D1::Matrix3x2F inverse = unityMatrix;
    if (inverse.Invert())
    {
        D2D1_POINT_2F converted = inverse.TransformPoint(pt);

        POINT result = {
            static_cast<LONG>(converted.x),
            static_cast<LONG>(converted.y)
        };
        return result;
    }

    // 역행 실패 시 원래 좌표 반환
    return mouse;
}

POINT Input::GetMouseWorldPosition()
{
    // TODO ::  직교행렬 연산
    // screen -> view -> world
    return { 0,0 };
}

float Input::GetAxisHorizontal()
{
    return horizontalAxis;
}

float Input::GetAxisVertical()
{
    return verticalAxis;
}