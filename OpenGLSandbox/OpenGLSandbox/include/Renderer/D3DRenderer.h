#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#include <iostream>
#include <memory>
#include <functional>

#include "Window/Window.h"

namespace fsi
{
    template <typename T>
    struct ComDeleter
    {
        void operator()(T* obj)
        {
            std::cout << "Releasing Com object!" << std::endl;
            obj->Release();
        }
    };

    template <typename T>
    using ComPtr = std::unique_ptr<T, ComDeleter<T>>;


    class D3DRenderer
    {
    public:
        D3DRenderer(std::shared_ptr<Window>& window);
        ~D3DRenderer();

        void handleEvents(const Event& event);
        void update(float timeDelta);
        void render();

        void resize(int width, int height);

        void setFrameTime(long long frameTime);
        
        void enableFXAA(bool enabled);

        void attachWindow(Window* window);

    private:
        static const UINT FrameCount = 2;
        void createContext();
        void populateCommandList();
        void waitForPreviousFrame();

        IDXGISwapChain3* m_swapChain;
        ID3D12Device* m_device;
        ID3D12Resource* m_renderTargets[FrameCount];

        ID3D12CommandAllocator* m_commandAllocator;
        ID3D12CommandQueue* m_commandQueue;
        
        ID3D12RootSignature* m_rootSignature;
        ID3D12DescriptorHeap* m_rtvHeap;
        ID3D12PipelineState* m_pipelineState;
        ID3D12GraphicsCommandList* m_commandList;
        UINT m_rtvDescriptorSize;

        D3D12_VIEWPORT m_viewport;
        D3D12_RECT m_scissorRect;

        ID3D12Resource* m_vertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

        UINT m_frameIndex;
        HANDLE m_fenceEvent;
        ID3D12Fence* m_fence;
        UINT64 m_fenceValue;

        std::shared_ptr<Window> m_window;
    };
}