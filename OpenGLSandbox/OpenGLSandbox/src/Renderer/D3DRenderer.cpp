#include "Renderer/D3DRenderer.h"

namespace fsi
{
    HRESULT res;

    D3DRenderer::D3DRenderer(std::shared_ptr<Window>& window)
        : m_window(window)
        , m_swapChain(nullptr)
        , m_device(nullptr)
        , m_commandAllocator(nullptr)
        , m_commandQueue(nullptr)
        , m_rtvDescriptorSize(0)
        
    {
        m_viewport.Width = static_cast<float>(m_window->getSize().x);
        m_viewport.Height = static_cast<float>(m_window->getSize().y);
        m_viewport.MinDepth = 0.f;
        m_viewport.MaxDepth = 1.f;
        m_viewport.TopLeftX = 0.f;
        m_viewport.TopLeftY = 0.f;

        m_scissorRect.left = 0;
        m_scissorRect.top = 0;
        m_scissorRect.right = static_cast<LONG>(m_window->getSize().x);
        m_scissorRect.bottom = static_cast<LONG>(m_window->getSize().y);

        //createContext();
#ifdef _DEBUG
        {
            ID3D12Debug* debugController;
            res = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
            if (res == S_OK)
                debugController->EnableDebugLayer();
            debugController->Release();
        }
        
#endif
        IDXGIFactory4* factory;
        if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        {
            std::cout << "Failed to create factory!" << std::endl;
            return;
        }

        if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
        {
            std::cout << "Failed to create device!" << std::endl;
            return;
        }

        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        if (FAILED(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue))))
        {
            std::cout << "Failed to create command queue!" << std::endl;
            return;
        }

        DXGI_SWAP_CHAIN_DESC scd;
        ZeroMemory(&scd, sizeof(scd));
        scd.BufferCount = FrameCount;
        scd.BufferDesc.Width = m_window->getSize().x;
        scd.BufferDesc.Height = m_window->getSize().y;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        scd.OutputWindow = m_window->getWindowHandle();
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.Windowed = TRUE;

        IDXGISwapChain* swapChain;
        if (FAILED(factory->CreateSwapChain(m_commandQueue, &scd, &swapChain)))
        {
            std::cout << "Failed to create command queue!" << std::endl;
            return;
        }

        m_swapChain = reinterpret_cast<IDXGISwapChain3*>(swapChain);

        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
        
        {
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = FrameCount;
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            res = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

            m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        {
            auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

            for (UINT n = 0; n < FrameCount; n++)
            {
                res = m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
                m_device->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);
                rtvHandle.ptr += 1 * m_rtvDescriptorSize;
            }
        }

        m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));

        {
            D3D12_ROOT_SIGNATURE_DESC rsd;
            rsd.NumParameters = 0;
            rsd.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
            rsd.NumStaticSamplers = 0;
            rsd.pParameters = nullptr;
            rsd.pStaticSamplers = nullptr;

            ID3DBlob* signature;
            ID3DBlob* error;

            res = D3D12SerializeRootSignature(&rsd, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
            res = m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
        }

        {
            ID3DBlob* vsBlob = nullptr;
            ID3DBlob* psBlob = nullptr;

#ifdef _DEBUG
            UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
            UINT compileFlags = 0;
#endif

            res = D3DCompileFromFile(L"res/d3d/VertexShader.hlsl", nullptr, nullptr, "VS",
                "vs_5_0", compileFlags, 0, &vsBlob, nullptr);

            res = D3DCompileFromFile(L"res/d3d/PixelShader.hlsl", nullptr, nullptr, "PS",
                "ps_5_0", compileFlags, 0, &psBlob, nullptr);

            D3D12_INPUT_ELEMENT_DESC inputElementsDescs[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
            };

            D3D12_RASTERIZER_DESC rastDesc = {};
            D3D12_BLEND_DESC blendDesc = {};

            D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
            psoDesc.InputLayout = { inputElementsDescs, _countof(inputElementsDescs) };
            psoDesc.pRootSignature = m_rootSignature;
            psoDesc.VS = { reinterpret_cast<UINT8*>(vsBlob->GetBufferPointer()), vsBlob->GetBufferSize() };
            psoDesc.PS = { reinterpret_cast<UINT8*>(psBlob->GetBufferPointer()), psBlob->GetBufferSize() };
            psoDesc.RasterizerState = rastDesc;
            psoDesc.BlendState = blendDesc;
            psoDesc.DepthStencilState.DepthEnable = FALSE;
            psoDesc.DepthStencilState.StencilEnable = FALSE;
            psoDesc.SampleMask = UINT_MAX;
            psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            psoDesc.NumRenderTargets = 1;
            psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
            psoDesc.SampleDesc.Count = 1;

            res = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
        }

        res = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, m_pipelineState, IID_PPV_ARGS(&m_commandList));
        res = m_commandList->Close();

        {
            FLOAT triangleVertices[] = 
            {
                0.f, 0.25f, 0.f, 1.f, 0.f, 0.f, 1.f,
                0.25f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f,
                -02.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f
            };

            const UINT vertexBufferSize = sizeof(triangleVertices);

            D3D12_HEAP_PROPERTIES heapProps;
            heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
            heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            heapProps.CreationNodeMask = 0;
            heapProps.VisibleNodeMask = 0;

            D3D12_RESOURCE_DESC resDesc;
            resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            resDesc.Alignment = 0;
            resDesc.Width = sizeof(triangleVertices);
            resDesc.Height = 0;
            resDesc.DepthOrArraySize = 0;
            resDesc.MipLevels = 1;
            resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

            res = m_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
                &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer));

            // Copy the triangle data to the vertex buffer.
            UINT8* pVertexDataBegin;
            res = m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin));
            memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
            m_vertexBuffer->Unmap(0, nullptr);

            // Initialize the vertex buffer view.
            m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
            m_vertexBufferView.StrideInBytes = 7 * sizeof(FLOAT);
            m_vertexBufferView.SizeInBytes = vertexBufferSize;
        }

        // Create synchronization objects and wait until assets have been uploaded to the GPU.
        {
            res = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
            m_fenceValue = 1;

            // Create an event handle to use for frame synchronization.
            m_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
            if (m_fenceEvent == nullptr)
            {
                HRESULT_FROM_WIN32(GetLastError());
            }

            // Wait for the command list to execute; we are reusing the same command 
            // list in our main loop but for now, we just want to wait for setup to 
            // complete before continuing.
            waitForPreviousFrame();
        }
    }

    D3DRenderer::~D3DRenderer()
    {
        /*m_swapChain->Release();
        m_backBuffer->Release();
        m_device->Release();
        m_context->Release();
        m_vertexLayout->Release();
        m_indexBuffer->Release();
        m_vertexBuffer->Release();
        m_vertexShader->Release();
        m_pixelShader->Release();*/

        waitForPreviousFrame();
        CloseHandle(m_fenceEvent);
    }

    void D3DRenderer::handleEvents(const Event& event)
    {

    }

    void D3DRenderer::update(float timeDelta)
    {

    }

    void D3DRenderer::render()
    {
        populateCommandList();

        ID3D12CommandList* ppCommandLists[] = { m_commandList };

        //float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };
        //m_context->ClearRenderTargetView(m_backBuffer, clearColor);

        //m_context->DrawIndexed(6, 0, 0);

        m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        m_swapChain->Present(1, 0);
    }

    void D3DRenderer::resize(int width, int height)
    {

    }

    void D3DRenderer::setFrameTime(long long frameTime)
    {

    }

    void D3DRenderer::enableFXAA(bool enabled)
    {

    }

    void D3DRenderer::attachWindow(Window* window)
    {

    }

    void D3DRenderer::createContext()
    {
        

        /*D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            nullptr, 0, D3D11_SDK_VERSION, &scd, &m_swapChain, &m_device, nullptr, &m_context);

        ID3D11Texture2D* backBufferTex = nullptr;
        m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTex);
        m_device->CreateRenderTargetView(backBufferTex, nullptr, &m_backBuffer);
        backBufferTex->Release();
        m_context->OMSetRenderTargets(1, &m_backBuffer, nullptr);*/
    }
    
    void D3DRenderer::populateCommandList()
    {
        m_commandAllocator->Reset();

        m_commandList->Reset(m_commandAllocator, m_pipelineState);
        
        m_commandList->SetGraphicsRootSignature(m_rootSignature);
        m_commandList->RSSetViewports(1, &m_viewport);
        m_commandList->RSSetScissorRects(1, &m_scissorRect);

        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        D3D12_RESOURCE_TRANSITION_BARRIER transBarrier;
        transBarrier.pResource = m_renderTargets[m_frameIndex];
        transBarrier.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        transBarrier.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition = transBarrier;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        m_commandList->ResourceBarrier(1, &barrier);

        auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
        m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };
        m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
        m_commandList->DrawInstanced(3, 1, 0, 0);

        transBarrier.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        transBarrier.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

        m_commandList->ResourceBarrier(1, &barrier);

        m_commandList->Close();
    }

    void D3DRenderer::waitForPreviousFrame()
    {
        const UINT64 fence = m_fenceValue;
        m_commandQueue->Signal(m_fence, fence);
        m_fenceValue++;

        if (m_fence->GetCompletedValue() < fence)
        {
            m_fence->SetEventOnCompletion(fence, m_fenceEvent);
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }

        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    }
}