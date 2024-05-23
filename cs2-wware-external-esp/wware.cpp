#include "include.h"

#include "entity.cpp"
#include "menu.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT message, WPARAM wparameter, LPARAM lparameter);

class Renderer
{
private:
    HWND hwnd;

    LPDIRECT3D9 d3dDirect;
    LPDIRECT3DDEVICE9 d3dDevice;
    D3DPRESENT_PARAMETERS d3dParameters;

public:
    Renderer(HWND hwnd) : hwnd(hwnd), d3dDirect(NULL), d3dDevice(NULL) { }

    HRESULT Initialize()
    {
        d3dDirect = Direct3DCreate9(D3D_SDK_VERSION);
        if (!d3dDirect) return E_FAIL;

        ZeroMemory(&d3dParameters, sizeof(d3dParameters));
        d3dParameters.Windowed = TRUE;
        d3dParameters.EnableAutoDepthStencil = TRUE;
        d3dParameters.SwapEffect = D3DSWAPEFFECT_FLIP;
        d3dParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
        d3dParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
        d3dParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        if (d3dDirect->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dParameters, &d3dDevice) < 0)
            return E_FAIL;

        return S_OK;
    }

    void ResetDevice()
    {
        ImGui_ImplDX9_InvalidateDeviceObjects();

        if (d3dDevice->Reset(&d3dParameters) == D3DERR_INVALIDCALL)
            IM_ASSERT(0);

        ImGui_ImplDX9_CreateDeviceObjects();
    }

    void Cleanup()
    {
        if (d3dDevice)
        {
            d3dDevice->Release();
            d3dDevice = NULL;
        }

        if (d3dDirect)
        {
            d3dDirect->Release();
            d3dDirect = NULL;
        }
    }

    LPDIRECT3DDEVICE9 GetDevice() const
    {
        return d3dDevice;
    }

    bool IsDeviceLost() const
    {
        return d3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET;
    }
};

class Application
{
private:
    HWND hwnd;
    Renderer renderer;

public:
    Application(HWND hwnd) : hwnd(hwnd), renderer(hwnd) { }

    void Run()
    {
        if (renderer.Initialize() < 0 || !memory.ModifyProcess(L"cs2.exe"))
        {
            Cleanup();
            return;
        }

        std::thread pushEntityThread(PushEntity);

        ShowWindow(hwnd, SW_SHOWDEFAULT);
        UpdateWindow(hwnd);

        ImGui::CreateContext();
        DrawStyle();

        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX9_Init(renderer.GetDevice());

        MSG message;
        ZeroMemory(&message, sizeof(message));

        while (message.message != WM_QUIT)
        {
            if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }

            if (!memory.CheckProcess(L"cs2.exe")) {
                Cleanup();
                return;
            }

            if (GetAsyncKeyState(VK_INSERT) & 1)
                Vars::Menu::open = !Vars::Menu::open;

            if (!Vars::Menu::open)
                SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
            else SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);

            ImGui_ImplDX9_NewFrame();
            ImGui_ImplWin32_NewFrame();

            ImGui::NewFrame();
            {
                DrawList();

                if (Vars::Menu::open)
                    DrawMenu();
            }
            ImGui::EndFrame();

            WindowClamp();

            LPDIRECT3DDEVICE9 d3dDevice = renderer.GetDevice();
            if (d3dDevice->BeginScene() >= 0)
            {
                d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1, 0);

                ImGui::Render();
                ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

                d3dDevice->EndScene();
            }

            if (d3dDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST && renderer.IsDeviceLost())
                renderer.ResetDevice();
        }

        Cleanup();
    }

    void Cleanup()
    {
        renderer.Cleanup();
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplDX9_InvalidateDeviceObjects();

        if (hwnd)
        {
            DestroyWindow(hwnd);
            UnregisterClassA("Wware", GetModuleHandleA(NULL));
        }

        PostQuitMessage(0);
        exit(0);
    }
};

LRESULT WINAPI WindowProc(HWND hwnd, UINT message, WPARAM wparameter, LPARAM lparameter)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wparameter, lparameter))
        return 0;

    switch (message)
    {
    case WM_SYSCOMMAND:
        if ((wparameter & 0xfff0) == SC_KEYMENU)
            return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcA(hwnd, message, wparameter, lparameter);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE pinstance, LPSTR cmdline, int cmdshow)
{
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);

    WNDCLASSEX wclass = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, NULL, NULL, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "Wware", NULL };
    RegisterClassExA(&wclass);

    HWND hwnd = CreateWindowExA(WS_EX_LAYERED | WS_EX_TOPMOST, "Wware", "Wware", WS_POPUP, NULL, NULL, desktop.right, desktop.bottom, NULL, NULL, wclass.hInstance, NULL);
    HDC hdc = GetDC(hwnd);

    BLENDFUNCTION blending = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    UpdateLayeredWindow(hwnd, hdc, NULL, NULL, CreateCompatibleDC(hdc), NULL, NULL, &blending, ULW_ALPHA);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    Application app(hwnd);
    app.Run();

    return 0;
}