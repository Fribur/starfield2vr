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
        char pad[0x20];
        DirectX12Module1* m_directX12Module1;
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
        auto instance = Get();
        auto pX12Module = *instance->m_directX12Module;
        if(pX12Module == nullptr) {
            return nullptr;
        }
        if(hw_scheduling) {
            return pX12Module->m_directX12Module1->m_directX12Module2->m_sl_commandQueue->m_async_commandQueue;
        } else {
            return pX12Module->m_directX12Module1->m_directX12Module2->m_commandQueue;
        }
    }
private:
    void InstallHooks();
    inline CreationEngineDirectX12Module() {
        InstallHooks();
    };
    ~CreationEngineDirectX12Module() = default;
    CreationEngine::Nvidia::DirectX12Module** m_sl_directX12Module{};
    CreationEngine::DirectX12Module** m_directX12Module{};
};
