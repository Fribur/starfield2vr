#pragma once

#include <d3d12.h>

namespace CreationEngine::Nvidia {
    struct SlInterposterCommandQueue {
        void* vtable;
        __int64 pad;
        ID3D12CommandQueue* m_async_commandQueue;

    };
    struct DirectX12CommandQueueHolder {
        char pad[0x28];
        SlInterposterCommandQueue* m_sl_commandQueue;
    };

    static_assert(offsetof(DirectX12CommandQueueHolder, m_sl_commandQueue) == 0x28);
    struct DirectX12Module {
        char pad[0x7A20];
        DirectX12CommandQueueHolder* m_commandQueueHolder;
    };
    static_assert(offsetof(DirectX12Module, m_commandQueueHolder) == 0x7A20);

}

namespace CreationEngine
{
    struct DirectX12Module2 {
        char pad[0x28];
        union {
            ID3D12CommandQueue* m_commandQueue;
            Nvidia::SlInterposterCommandQueue* m_sl_commandQueue;
        };
    };
    struct DirectX12Module1 {
        char pad[8];
        DirectX12Module2* m_directX12Module2;
    };
    struct DirectX12Module {
        char pad[0x28];
        DirectX12Module1* m_directX12Module1;
    };
}

namespace RE::CreationRendererPrivate {
    struct D3D12CommandQueueWrapper
    {
        char pad[96];
        ID3D12CommandQueue *pCommandQueue;
    };

    struct D3D12Context {
        char pad[8];
        D3D12CommandQueueWrapper *pQueue[3];
    };

    struct ServiceLocator {
        char pad[0x28];
        D3D12Context *pD3D12Context;
    };

}

class CreationEngineDirectX12Module
{
public:
    inline static CreationEngineDirectX12Module* Get()
    {
        static auto instance(new CreationEngineDirectX12Module);
        return instance;
    }
    inline static ID3D12CommandQueue* GetCommandQueue(bool hw_scheduling) {
        static auto instance = Get();
        auto pServiceLocator = *instance->g_ServiceLocator;
        if(pServiceLocator == nullptr) {
            return nullptr;
        }
        auto pD3D12Context = pServiceLocator->pD3D12Context;
        if(pD3D12Context == nullptr) {
            return nullptr;
        }
        auto pQueue = pD3D12Context->pQueue[0]; // 0 - Direct, 1 - compute, 2 - copy
        if (pQueue == nullptr) {
            return nullptr;
        }
        return pQueue->pCommandQueue;
    }
private:
    void InstallHooks();
    inline CreationEngineDirectX12Module() {
        InstallHooks();
    };
    ~CreationEngineDirectX12Module() = default;
//    CreationEngine::Nvidia::DirectX12Module** m_sl_directX12Module{};
    RE::CreationRendererPrivate::ServiceLocator** g_ServiceLocator{};
};
