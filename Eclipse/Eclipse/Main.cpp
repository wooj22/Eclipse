#include "EclipseApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

    SetProcessDPIAware();

    EclipseApp app(1920, 1080);

    app.Init();
    app.Loop();
    app.UnInit();

    return (int)1;
}
