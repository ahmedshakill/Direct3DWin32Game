//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"


enum class ShapeType {
    Triangle,
    Square,
    Circle
};

class Triangle {
public:
    float left = 0.f, right = 0.f;
};
class Square {
public:
    float left = 0.f, right = 0.f;
};
class Circle {
public:
    float left = 0.f, right = 0.f;
};
class Shape {
public:
   float left=0.f,bottom=0.f,width=0.f,up=0.f;
   Triangle triangle;
   Square square;
   Circle circle;
   Shape() {
      
   }
   Shape(float left, float ShapeWidth, float bottom)
   {
       this->width = ShapeWidth;
       this->up = 100.0f;
       this->bottom = this->up+bottom;
       this->left = left;
       triangle.left = this->left;
       triangle.right = triangle.left+width+10;
       square.left = triangle.right;
       square.right = square.left + width;
       circle.left = square.right;
       circle.right = circle.left + width;
   }
};



// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();


    void updateTexture(/*float x, float y*/);
    

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    std::unique_ptr<DirectX::Keyboard>      m_keyboard;
    std::unique_ptr<DirectX::Mouse>         m_mouse;

    std::unique_ptr<DirectX::CommonStates>  m_states;
    std::unique_ptr<DirectX::BasicEffect>   m_effect;
    
    using VertexType = DirectX::VertexPositionTexture;
    
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_inputLayout;

    //Textures
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_background;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_triangle;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_quare;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_circle;
    //SpriteBatch
    std::unique_ptr<DirectX::SpriteBatch>   m_spriteBatch;

    //SpriteFont
    std::unique_ptr<DirectX::SpriteFont>             m_font;

    //Geometry
    RECT                                    m_tileRect;
    RECT                                    m_fullscreenRect;


    //Math
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    //Buffer 
    wchar_t                                             szFile[260];
    bool                                                textureSelected = false;
    bool                                                displayShape = false;
    ShapeType                                           shapeType;
    Shape                                               shape;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;
};
