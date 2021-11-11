//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>(DXGI_FORMAT_R8G8B8A8_UNORM);
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_keyboard  = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    shape = Shape(100.0f,100.0f,22.0f);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}



void Game::updateTexture(float x,float y)
{


    auto device = m_deviceResources->GetD3DDevice();
    //auto context = m_deviceResources->GetD3DDeviceContext();

    ComPtr<ID3D11Resource> resource;

    // Get Texture file name using filedialogue
    {
        OPENFILENAME ofn = {};
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = nullptr;//window;
        ofn.lpstrFile = szFile;

        szFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameW(&ofn)) {
            textureSelected = true;
        };

    }
    if(textureSelected)
    {

        DX::ThrowIfFailed(CreateWICTextureFromFile(device, szFile,
        resource.GetAddressOf(),
        m_texture.ReleaseAndGetAddressOf()));
        ComPtr<ID3D11Texture2D> cat;

        DX::ThrowIfFailed(resource.As(&cat));


        CD3D11_TEXTURE2D_DESC catDesc;
        cat->GetDesc(&catDesc);

        m_origin.x =  float(catDesc.Width / 2);
        m_origin.y =  float(catDesc.Height / 2);

        auto windowSize = m_deviceResources->GetOutputSize();

        m_screenPos.x = float(windowSize.right) / 2.f;
        m_screenPos.y = float(windowSize.bottom) / 2.f;
    }

    x;
    y;

}


// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

    auto kb = m_keyboard->GetState();
    if (kb.Escape)
    {
        ExitGame();
    }

    auto mouse = m_mouse->GetState();
    
    if (mouse.leftButton)
    {  
        //std::wstringstream oss;
        //oss << mouse.x << "  " << mouse.y << "\n";
        //oss << shape.triangle.left << "  " << shape.triangle.right << "\n";
        //oss << shape.up << "  " << shape.bottom << "\n";
        //std::wstring str = oss.str();
        //OutputDebugStringW(str.c_str());
        if (mouse.x >= shape.triangle.left && mouse.x < shape.triangle.right-20 && mouse.y >= shape.up && mouse.y <= shape.bottom)
        {          
            shapeType = ShapeType::Triangle;
            updateTexture((float)mouse.x, (float)mouse.y);
        }
        if (mouse.x >= shape.square.left+10 && mouse.x < shape.square.right-20 && mouse.y >= shape.up && mouse.y <= shape.bottom)
        {
            shapeType = ShapeType::Square;
            std::wstringstream oss;
            oss << "Square\n";
            std::wstring str = oss.str();
            OutputDebugStringW(str.c_str());
            //updateTexture((float)mouse.x, (float)mouse.y);
        }
        if (mouse.x >= shape.circle.left+10 && mouse.x < shape.circle.right-20 && mouse.y >= shape.up && mouse.y <= shape.bottom)
        {
            std::wstringstream oss;
            oss << "Circle\n";
            std::wstring str = oss.str();
            OutputDebugStringW(str.c_str());
            shapeType = ShapeType::Circle;
            //updateTexture((float)mouse.x, (float)mouse.y);
        }
    }
    

}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    //float time = float(m_timer.GetTotalSeconds());

    m_spriteBatch->Begin();

    m_spriteBatch->Draw(m_background.Get(), m_fullscreenRect);

    m_font->DrawString(m_spriteBatch.get(), L"Triangle", XMFLOAT2(shape.triangle.left, shape.up), Colors::GreenYellow);
    m_font->DrawString(m_spriteBatch.get(), L"Square", XMFLOAT2(shape.square.left, shape.up), Colors::GreenYellow);
    m_font->DrawString(m_spriteBatch.get(), L"Circle", XMFLOAT2(shape.circle.left, shape.up), Colors::GreenYellow);

    if (textureSelected == true)
    {
        m_spriteBatch->Draw(m_texture.Get(), m_screenPos, nullptr,
            Colors::White, 0.f, m_origin);
    }
    m_spriteBatch->End();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::ForestGreen);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    
    m_spriteBatch   = std::make_unique<SpriteBatch>(context);
    m_states        = std::make_unique<CommonStates>(device);
    //ComPtr<ID3D11Resource> resource;
    ComPtr<ID3D11Resource> resourc2;


    wchar_t fontFilePath[MAX_PATH];
    DX::FindMediaFile(fontFilePath, MAX_PATH, L"SegoeUI_18.spritefont");
    m_font = std::make_unique<SpriteFont>(device, fontFilePath);


    // Get Texture file name using filedialogue
    //{
    //    OPENFILENAME ofn = {};
    //    ZeroMemory(&ofn, sizeof(ofn));
    //    ofn.lStructSize = sizeof(OPENFILENAME);
    //    ofn.hwndOwner = nullptr;//window;
    //    ofn.lpstrFile = szFile;

    //    szFile[0] = '\0';
    //    ofn.nMaxFile = sizeof(szFile);
    //    ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    //    ofn.nFilterIndex = 1;
    //    ofn.lpstrFileTitle = NULL;
    //    ofn.nMaxFileTitle = 0;
    //    ofn.lpstrInitialDir = NULL;
    //    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    //    GetOpenFileNameW(&ofn);
    //}

    //DX::ThrowIfFailed(CreateWICTextureFromFile(device, szFile,
    //    resource.GetAddressOf(),
    //    m_texture.ReleaseAndGetAddressOf()));


    wchar_t strFilePath[MAX_PATH];
    DX::FindMediaFile(strFilePath, MAX_PATH, L"sunset.jpg");
    DX::ThrowIfFailed(CreateWICTextureFromFile(device, strFilePath, 
        resourc2.GetAddressOf(),
        m_background.ReleaseAndGetAddressOf()));


    /*ComPtr<ID3D11Texture2D> cat;
    ComPtr<ID3D11Texture2D> backGround;

    DX::ThrowIfFailed(resource.As(&cat));
    DX::ThrowIfFailed(resourc2.As(&backGround));

    CD3D11_TEXTURE2D_DESC catDesc;
    cat->GetDesc(&catDesc);
    
    m_origin.x =float(catDesc.Width / 2);
    m_origin.y =float(catDesc.Height / 2);*/

    /*m_tileRect.left = catDesc.Width * 2;
    m_tileRect.right = catDesc.Width * 6;
    m_tileRect.top = catDesc.Height * 2;
    m_tileRect.bottom = catDesc.Height * 6;*/
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    auto size = m_deviceResources->GetOutputSize();
    m_screenPos.x = float(size.right) /  2.f;
    m_screenPos.y = float(size.bottom) / 2.f;

    m_fullscreenRect = m_deviceResources->GetOutputSize();
    auto viewport = m_deviceResources->GetScreenViewport();
    m_spriteBatch->SetViewport(viewport);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_texture.Reset();
    m_spriteBatch.reset();
    m_states.reset();
    m_background.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
