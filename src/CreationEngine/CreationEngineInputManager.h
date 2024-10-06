#pragma once
#include <ViGEm/Client.h>

#pragma comment(lib, "setupapi.lib")

class CreationEngineInputManager
{
public:
    void Init();
    void Reset();
    void UpdateDeviceState();

    static CreationEngineInputManager* Get()
    {
        static auto instance(new CreationEngineInputManager);
        return instance;
    }

    CreationEngineInputManager(const CreationEngineInputManager&)            = delete;
    CreationEngineInputManager& operator=(const CreationEngineInputManager&) = delete;
    ~CreationEngineInputManager();

private:
    CreationEngineInputManager();
    bool connected = false;
    PVIGEM_CLIENT client;
    PVIGEM_TARGET pad;
    static bool          Hook();
};
