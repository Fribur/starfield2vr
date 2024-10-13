#pragma once
#include "ScanHelper.h"

using namespace MemoryScan;

namespace Steam::MemoryOffsets
{
    namespace NiCamera
    {
        inline uintptr_t SetFrustumVfunc()
        {
            // ID 210419
            auto pattern = "48 89 5C 24 10 57 48 83 EC 20 48 8B FA 48 8B D9 48 81 C1 08 02";
            static auto address_ptr = FuncRelocation(pattern, 0x37E9DC8, 210419);
            return address_ptr;
        }

        inline uintptr_t CalcFrustumVfunc()
        {
            // ID 210404
            auto pattern = "48 8B C4 48 89 58 10 55 48 8D 68 B1";
            static auto address_ptr = FuncRelocation(pattern, 0x37E8BEC, 210404);
            return address_ptr;
        }

        inline uintptr_t UpdateWorldVfunc()
        {
            // ID 210425
            auto pattern = "48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 E8 FE FF FF 48 81 EC E0 01 00 00 C5 F8 29 70 B8 C5 F8 29 78 A8 C5";
            static auto address_ptr = FuncRelocation(pattern, 0x37E9FB4, 210425);
            return address_ptr;
        }

        inline uintptr_t Vtable() {
            // ID ??
            auto pattern = "48 8D 05 ? ? ? ? 48 89 06 C7 86 30 01 00 00 FF FF FF 00";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0, 0);
            return address_ptr;
        }
    } // namespace NiCamera

    namespace NiAVObject {
        inline uintptr_t UpdateWorld() {
            //ID 210234
            auto pattern = "48 89 5C 24 08 57 48 83 EC 20 F6 42 38 01 48 8B DA";
            static auto address_ptr = FuncRelocation(pattern, 0x37DE144, 210234);
            return address_ptr;
        }
    }

    namespace Gamepad
    {
        inline uintptr_t PollVfunc()
        {
            // ID 179249
            auto pattern = "48 8B C4 48 89 58 10 55 56 57 48 8D 68 A1 48 81 EC A0 00 00 00 48";
            static auto address_ptr = FuncRelocation(pattern, 0x2F7ABA4, 179249);
            return address_ptr;
        }
    } // namespace Gamepad

    namespace Scaleform::Movie
    {
        inline uintptr_t SetViewportVFunc()
        {
            // ID 50603
            auto pattern = "40 53 56 57 48 81 EC 80 00 00 00 48 8B DA";
            static auto address_ptr = FuncRelocation(pattern, 0x0C6E590, 50603);
            return address_ptr;
        }
    } // namespace Scaleform::Movie

    namespace CreationRenderer
    {
        inline uintptr_t RenderGraphFrameStart()
        {
            // ID 1079045
            auto pattern = "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 55 48 81 EC 70";
            static auto address_ptr = FuncRelocation(pattern, 0x36A1E3C, 1079045);
            return address_ptr;
        }

        inline uintptr_t RenderGraphRenderPipelineExecute()
        {
            // ID 202136
            auto pattern = "40 53 48 83 EC 20 48 8B 91 58 02 00 00 4C 8D 41 18";
            static auto address_ptr = FuncRelocation(pattern, 0x3552560, 202136);
            return address_ptr;
        }
        inline uintptr_t DlssUpscaleVtable()
        {
            // ID 1100493
            auto pattern = "";
            // unable to find stable
            return 0;
        }

        inline uintptr_t GetDXGIState() {
            // ID 1079146
            auto val = (uintptr_t)mod + 0x370038C;
            return val;
        }

        inline uintptr_t GetCommandList() {
            // ID 206429
            auto val = (uintptr_t)mod + 0x36BE83C;
            return val;
        }

        inline uintptr_t GlobalFrameCount() {
            // ID 885900
            auto val = (uintptr_t)mod + 0x6A50108;
            return val;
        }

        inline uintptr_t OnUpdateConstantBufferView() {
            // ID 204129
            auto val = (uintptr_t)mod + 0x361602C;
            return val;
        }

        inline uintptr_t OnTaaVFunc7() {
            // ID 208939
            auto val = (uintptr_t)mod + 0x37B4B7C;
            return val;
        }
    } // namespace CreationRenderer

    namespace PlayerCamera {
        inline uintptr_t Singleton() {
            //ID 878523
            auto pattern = "48 89 05 ? ? ? ? 48 8B D7";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A2E148, 878523);
            return address_ptr;
        }
        inline uintptr_t QCameraEquals() {
            //ID 166081
            auto val = (uintptr_t)mod + 0x2ACF680;
            return val;
        }
    }

    namespace Streamline
    {
        inline uintptr_t SetDlssOptions() {
            // ID 1171973
            auto pattern = "4C 8B 05 ? ? ? ? 4D 85 C0 75 21";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A52388, 1171973);
            return address_ptr;

        }
    } // namespace Streamline

    namespace StorageTable::RenderGraphHandle
    {
        inline uintptr_t Singleton()
        {
            // ID 772639
            auto pattern = "48 8B 0D ? ? ? ? 4C 8B 89 E8 00 00 00 49 8B 49 40";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6134828, 772639);
            return address_ptr;
        }
    } // namespace StorageTable::RenderGraphHandle

    inline uintptr_t GlobalFrameCounter()
    {
        // ID 886292
        auto pattern = "8B 1D ? ? ? ? 8B 81 28 01 00 00";
        static auto address_ptr = InstructionRelocation(pattern, 2, 6, 0x6A540E0, 886292);
        return address_ptr;
    }

    inline uintptr_t GlobalWorldFov()
    {
        // ID 1171750
        auto pattern = "C5 FA 10 05 ? ? ? ? 84 C9 74 61";
        static auto address_ptr = InstructionRelocation(pattern, 4, 8, 0x66A8E58, 1171750);
        return address_ptr;
    }

    inline uintptr_t GlobalRenderSettings() {
        // ID 878340
        auto pattern = "48 8B 15 ? ? ? ? C5 FA 10 99 D4 01 00 00";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x682A770, 878340);
        return address_ptr;
    }

    inline uintptr_t GlobalDirectX12Module() {
        // ID 886000
        auto pattern = "48 89 05 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B 1F";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A52358, 886000);
        return address_ptr;
    }

    inline uintptr_t GlobalDirectX12Module2() {
        // ID 885886
        auto pattern = "48 89 0D ? ? ? ? 4C 89 49 08";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A50080, 885886);
        return address_ptr;
    }

    inline uintptr_t GlobalAspectRatio() {
        // ID 873288
        auto pattern = "C5 FA 10 05 ? ? ? ? C5 FA 10 0D ? ? ? ? C5 F8 2F D0";
        static auto address_ptr = InstructionRelocation(pattern, 4, 8, 0x67126B8, 873288);
        return address_ptr;
    }

    inline uintptr_t GlobalSceneGraphRoot()
    {
        // ID 887308
        auto pattern = "48 8D 0D ? ? ? ? 48 3B C8 74 0C";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A71BB0, 887308);
        return address_ptr;
    }

    inline uintptr_t GlobalPlayerRef() {
        // ID 865059
        auto val = (uintptr_t)mod + 0x66C5238;
        return val;
    }
} // namespace Steam::MemoryOffsets

namespace Xbox::MemoryOffsets
{
    namespace NiCamera
    {
        inline uintptr_t SetFrustumVfunc()
        {
            // ID 210419
            auto pattern = "48 89 5C 24 10 57 48 83 EC 20 48 8B FA 48 8B D9 48 81 C1 08 02";
            static auto address_ptr = FuncRelocation(pattern, 0x3858058, 210419);
            return address_ptr;
        }

        inline uintptr_t CalcFrustumVfunc()
        {
            // ID 210404
            auto pattern = "48 8B C4 48 89 58 10 55 48 8D 68 B1";
            static auto address_ptr = FuncRelocation(pattern, 0x3856E7C, 210404);
            return address_ptr;
        }

        inline uintptr_t UpdateWorldVfunc()
        {
            // ID 210425
            auto pattern = "48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 E8 FE FF FF 48 81 EC E0 01 00 00 C5 F8 29 70 B8 C5 F8 29 78 A8 C5";
            static auto address_ptr = FuncRelocation(pattern, 0x3858244, 210425);
            return address_ptr;
        }

        inline uintptr_t Vtable() {
            // ID ??
            auto pattern = "48 8D 05 ? ? ? ? 48 89 06 C7 86 30 01 00 00 FF FF FF 00";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0, 0);
            return address_ptr;
        }
    } // namespace NiCamera

    namespace NiAVObject {
        inline uintptr_t UpdateWorld() {
            //ID 210234
            auto pattern = "48 89 5C 24 08 57 48 83 EC 20 F6 42 38 01 48 8B DA";
            static auto address_ptr = FuncRelocation(pattern, 0x384C3D4, 210234);
            return address_ptr;
        }
    }

    namespace Gamepad
    {
        inline uintptr_t PollVfunc()
        {
            // ID 179249
            auto pattern = "48 8B C4 48 89 58 10 55 56 57 48 8D 68 A1 48 81 EC A0 00 00 00 48";
            static auto address_ptr = FuncRelocation(pattern, 0x2F6E704, 179249);
            return address_ptr;
        }
    } // namespace Gamepad

    namespace Scaleform::Movie
    {
        inline uintptr_t SetViewportVFunc()
        {
            // ID 50603
            auto pattern = "40 53 56 57 48 81 EC 80 00 00 00 48 8B DA";
            static auto address_ptr = FuncRelocation(pattern, 0x0C6FDB0, 50603);
            return address_ptr;
        }
    } // namespace Scaleform::Movie

    namespace CreationRenderer
    {
        inline uintptr_t RenderGraphFrameStart()
        {
            // ID 1079045
            auto pattern = "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 55 48 81 EC 70";
            static auto address_ptr = FuncRelocation(pattern, 0x371004C, 1079045);
            return address_ptr;
        }

        inline uintptr_t RenderGraphRenderPipelineExecute()
        {
            // ID 202136
            auto pattern = "40 53 48 83 EC 20 48 8B 91 58 02 00 00 4C 8D 41 18";
            static auto address_ptr = FuncRelocation(pattern, 0x35C0760, 202136);
            return address_ptr;
        }
        inline uintptr_t DlssUpscaleVtable()
        {
            // ID 1100493
            auto pattern = "";
            // unable to find stable
            return 0;
        }

        inline uintptr_t GetDXGIState() {
            // ID 1079146
            auto val = (uintptr_t)mod + 0x376E62C;
            return val;
        }

        inline uintptr_t GetCommandList() {
            // ID 206429
            auto val = (uintptr_t)mod + 0x372CA4C;
            return val;
        }

        inline uintptr_t GlobalFrameCount() {
            // ID 885900
            auto val = (uintptr_t)mod + 0x6A8C288;
            return val;
        }

        inline uintptr_t OnUpdateConstantBufferView() {
            // ID 204129
            auto val = (uintptr_t)mod + 0x36842EC;
            return val;
        }

        inline uintptr_t OnTaaVFunc7() {
            // ID 208939
            auto val = (uintptr_t)mod + 0x3822E0C;
            return val;
        }
    } // namespace CreationRenderer

    namespace PlayerCamera {
        inline uintptr_t Singleton() {
            //ID 878523
            auto pattern = "48 89 05 ? ? ? ? 48 8B D7";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A6A090, 878523);
            return address_ptr;
        }
        inline uintptr_t QCameraEquals() {
            //ID 166081
            auto val = (uintptr_t)mod + 0x2AC62E0;
            return val;
        }
    }

    namespace Streamline
    {
        inline uintptr_t SetDlssOptions() {
            // ID 1171973
            auto pattern = "4C 8B 05 ? ? ? ? 4D 85 C0 75 21";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A8E4F8, 1171973);
            return address_ptr;

        }
    } // namespace Streamline

    namespace StorageTable::RenderGraphHandle
    {
        inline uintptr_t Singleton()
        {
            // ID 772639
            auto pattern = "48 8B 0D ? ? ? ? 4C 8B 89 E8 00 00 00 49 8B 49 40";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x616B748, 772639);
            return address_ptr;
        }
    } // namespace StorageTable::RenderGraphHandle

    inline uintptr_t GlobalFrameCounter()
    {
        // ID 886292
        auto pattern = "8B 1D ? ? ? ? 8B 81 28 01 00 00";
        static auto address_ptr = InstructionRelocation(pattern, 2, 6, 0x6A90254, 886292);
        return address_ptr;
    }

    inline uintptr_t GlobalWorldFov()
    {
        // ID 1171750
        auto pattern = "C5 FA 10 05 ? ? ? ? 84 C9 74 61";
        static auto address_ptr = InstructionRelocation(pattern, 4, 8, 0x66E4D98, 1171750);
        return address_ptr;
    }

    inline uintptr_t GlobalRenderSettings() {
        // ID 878340
        auto pattern = "48 8B 15 ? ? ? ? C5 FA 10 99 D4 01 00 00";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6866680, 878340);
        return address_ptr;
    }

    inline uintptr_t GlobalDirectX12Module() {
        // ID 886000
        auto pattern = "48 89 05 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B 1F";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A8E4D0, 886000);
        return address_ptr;
    }

    inline uintptr_t GlobalDirectX12Module2() {
        // ID 885886
        auto pattern = "48 89 0D ? ? ? ? 4C 89 49 08";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6A8C218, 885886);
        return address_ptr;
    }

    inline uintptr_t GlobalAspectRatio() {
        // ID 873288
        auto pattern = "C5 FA 10 05 ? ? ? ? C5 FA 10 0D ? ? ? ? C5 F8 2F D0";
        static auto address_ptr = InstructionRelocation(pattern, 4, 8, 0x674E678, 873288);
        return address_ptr;
    }

    inline uintptr_t GlobalSceneGraphRoot()
    {
        // ID 887308
        auto pattern = "48 8D 0D ? ? ? ? 48 3B C8 74 0C";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, 0x6AADAF0, 887308);
        return address_ptr;
    }

    inline uintptr_t GlobalPlayerRef() {
        // ID 865059
        auto val = (uintptr_t)mod + 0x6701178;
        return val;
    }
} // namespace Xbox::MemoryOffsets

#ifdef XBOX_STORE
namespace GameStore = Xbox;
#else
namespace GameStore = Steam;
#endif