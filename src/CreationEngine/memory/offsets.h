#pragma once
#include "ScanHelper.h"
#include "offsets_table.h"

using namespace MemoryScan;

namespace Steam::MemoryOffsets
{
    namespace Setting
    {
        inline uintptr_t INIPrefSettingCollection()
        {
            // ID 885866
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(885866);
            return val;
        }

        inline uintptr_t INISettingCollection()
        {
            // ID 885862
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(885862);
            return val;
        }
    } // namespace Setting

    namespace NiCamera
    {
        inline uintptr_t SetFrustumVfunc()
        {
            // ID 210419
            auto        pattern     = "48 89 5C 24 10 57 48 83 EC 20 48 8B FA 48 8B D9 48 81 C1 08 02";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(210419), 210419);
            return address_ptr;
        }

        inline uintptr_t CalcFrustumVfunc()
        {
            // ID 210404
            auto        pattern     = "48 8B C4 48 89 58 10 55 48 8D 68 B1";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(210404), 210404);
            return address_ptr;
        }

        inline uintptr_t UpdateWorldVfunc()
        {
            // ID 210425
            auto        pattern     = "48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 E8 FE FF FF 48 81 EC E0 01 00 00 C5 F8 29 70 B8 C5 F8 29 78 A8 C5";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(210425), 210425);
            return address_ptr;
        }

        inline uintptr_t Vtable()
        {
            // ID ??
            auto        pattern     = "48 8D 05 ? ? ? ? 48 89 06 C7 86 30 01 00 00 FF FF FF 00";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(0), 0);
            return address_ptr;
        }
    } // namespace NiCamera

    namespace NiAVObject
    {
        inline uintptr_t UpdateWorld()
        {
            // ID 210234
            auto        pattern     = "48 89 5C 24 08 57 48 83 EC 20 F6 42 38 01 48 8B DA";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(210234), 210234);
            return address_ptr;
        }
    } // namespace NiAVObject

    namespace Gamepad
    {
        inline uintptr_t PollVfunc()
        {
            // ID 179249
            auto        pattern     = "48 8B C4 48 89 58 10 55 56 57 48 8D 68 A1 48 81 EC A0 00 00 00 48";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(179249), 179249);
            return address_ptr;
        }
    } // namespace Gamepad

    namespace Scaleform::Movie
    {
        inline uintptr_t SetViewportVFunc()
        {
            // ID 50603
            auto        pattern     = "40 53 56 57 48 81 EC 80 00 00 00 48 8B DA";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(50603), 50603);
            return address_ptr;
        }
    } // namespace Scaleform::Movie

    namespace CreationRenderer
    {
        inline uintptr_t RenderGraphFrameStart()
        {
            // ID 1079045
            auto        pattern     = "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 55 48 81 EC 70";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(1079045), 1079045);
            return address_ptr;
        }

        inline uintptr_t RenderGraphRenderPipelineExecute()
        {
            // ID 202136
            auto        pattern     = "40 53 48 83 EC 20 48 8B 91 58 02 00 00 4C 8D 41 18";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(202136), 202136);
            return address_ptr;
        }

        inline uintptr_t DlssUpscaleVtable()
        {
            // ID 1100493
            auto pattern = "";
            // unable to find stable
            return 0;
        }

        inline uintptr_t GetDXGIState()
        {
            // ID 1079146
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(1079146);
            return val;
        }

        inline uintptr_t GetCommandList()
        {
            // ID 206429
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(206429);
            return val;
        }

        inline uintptr_t GlobalFrameCount()
        {
            // ID 885900
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(885900);
            return val;
        }

        inline uintptr_t OnUpdateConstantBufferView()
        {
            // ID 204129
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(204129);
            return val;
        }

        inline uintptr_t OnTaaVFunc7()
        {
            // ID 208939
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(208939);
            return val;
        }
    } // namespace CreationRenderer

    namespace Nvidia {

        inline uintptr_t onSetReflexMarkerInternal()
        {
            // ID 1391721
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(1391721);
            return val;
        }
    }

    namespace PlayerCamera
    {
        inline uintptr_t Singleton()
        {
            // ID 878523
            auto        pattern     = "48 89 05 ? ? ? ? 48 8B D7";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(878523), 878523);
            return address_ptr;
        }

        inline uintptr_t QCameraEquals()
        {
            // ID 166081
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(166081);
            return val;
        }
    } // namespace PlayerCamera

    namespace Streamline
    {
        inline uintptr_t SetDlssOptions()
        {
            // ID 1171973
            auto        pattern     = "4C 8B 05 ? ? ? ? 4D 85 C0 75 21";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(1171973), 1171973);
            return address_ptr;
        }
    } // namespace Streamline

    namespace StorageTable::RenderGraphHandle
    {
        inline uintptr_t Singleton()
        {
            // ID 772639
            auto        pattern     = "48 8B 0D ? ? ? ? 4C 8B 89 E8 00 00 00 49 8B 49 40";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(772639), 772639);
            return address_ptr;
        }
    } // namespace StorageTable::RenderGraphHandle

    inline uintptr_t GlobalFrameCounter()
    {
        // ID 886292
        auto        pattern     = "8B 1D ? ? ? ? 8B 81 28 01 00 00";
        static auto address_ptr = InstructionRelocation(pattern, 2, 6, OffsetsTable::GetOffset(886292), 886292);
        return address_ptr;
    }

    inline uintptr_t GlobalWorldFov()
    {
        // ID 1171750
        auto        pattern     = "C5 FA 10 05 ? ? ? ? 84 C9 74 61";
        static auto address_ptr = InstructionRelocation(pattern, 4, 8, OffsetsTable::GetOffset(1171750), 1171750);
        return address_ptr;
    }

    inline uintptr_t GlobalRenderSettings()
    {
        // ID 878340
        auto        pattern     = "48 8B 15 ? ? ? ? C5 FA 10 99 D4 01 00 00";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(878340), 878340);
        return address_ptr;
    }

    inline uintptr_t GlobalDirectX12Module()
    {
        // ID 886000
        auto        pattern     = "48 89 05 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B 1F";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(886000), 886000);
        return address_ptr;
    }

    inline uintptr_t GlobalDirectX12Module2()
    {
        // ID 885886
        auto        pattern     = "48 89 0D ? ? ? ? 4C 89 49 08";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(885886), 885886);
        return address_ptr;
    }

    inline uintptr_t GlobalAspectRatio()
    {
        // ID 873288
        auto        pattern     = "C5 FA 10 05 ? ? ? ? C5 FA 10 0D ? ? ? ? C5 F8 2F D0";
        static auto address_ptr = InstructionRelocation(pattern, 4, 8, OffsetsTable::GetOffset(873288), 873288);
        return address_ptr;
    }

    inline uintptr_t GlobalSceneGraphRoot()
    {
        // ID 887308
        auto        pattern     = "48 8D 0D ? ? ? ? 48 3B C8 74 0C";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(887308), 887308);
        return address_ptr;
    }

    inline uintptr_t GlobalPlayerRef()
    {
        // ID 865059
        auto val = (uintptr_t)mod + OffsetsTable::GetOffset(865059);
        return val;
    }
} // namespace Steam::MemoryOffsets

namespace Xbox::MemoryOffsets
{
    namespace Setting
    {
        inline uintptr_t INIPrefSettingCollection()
        {
            // ID 885866
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(885866);
            return val;
        }

        inline uintptr_t INISettingCollection()
        {
            // ID 885862
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(885862);
            return val;
        }
    } // namespace Setting

    namespace NiCamera
    {
        inline uintptr_t SetFrustumVfunc()
        {
            // ID 210419
            auto        pattern     = "48 89 5C 24 10 57 48 83 EC 20 48 8B FA 48 8B D9 48 81 C1 08 02";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(210419), 210419);
            return address_ptr;
        }

        inline uintptr_t CalcFrustumVfunc()
        {
            // ID 210404
            auto        pattern     = "48 8B C4 48 89 58 10 55 48 8D 68 B1";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(210404), 210404);
            return address_ptr;
        }

        inline uintptr_t UpdateWorldVfunc()
        {
            // ID 210425
            auto        pattern     = "48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 E8 FE FF FF 48 81 EC E0 01 00 00 C5 F8 29 70 B8 C5 F8 29 78 A8 C5";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(210425), 210425);
            return address_ptr;
        }

        inline uintptr_t Vtable()
        {
            // ID ??
            auto        pattern     = "48 8D 05 ? ? ? ? 48 89 06 C7 86 30 01 00 00 FF FF FF 00";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(0), 0);
            return address_ptr;
        }
    } // namespace NiCamera

    namespace NiAVObject
    {
        inline uintptr_t UpdateWorld()
        {
            // ID 210234
            auto        pattern     = "48 89 5C 24 08 57 48 83 EC 20 F6 42 38 01 48 8B DA";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(210234), 210234);
            return address_ptr;
        }
    } // namespace NiAVObject

    namespace Gamepad
    {
        inline uintptr_t PollVfunc()
        {
            // ID 179249
            auto        pattern     = "48 8B C4 48 89 58 10 55 56 57 48 8D 68 A1 48 81 EC A0 00 00 00 48";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(179249), 179249);
            return address_ptr;
        }
    } // namespace Gamepad

    namespace Scaleform::Movie
    {
        inline uintptr_t SetViewportVFunc()
        {
            // ID 50603
            auto        pattern     = "40 53 56 57 48 81 EC 80 00 00 00 48 8B DA";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(50603), 50603);
            return address_ptr;
        }
    } // namespace Scaleform::Movie

    namespace CreationRenderer
    {
        inline uintptr_t RenderGraphFrameStart()
        {
            // ID 1079045
            auto        pattern     = "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 55 48 81 EC 70";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(1079045), 1079045);
            return address_ptr;
        }

        inline uintptr_t RenderGraphRenderPipelineExecute()
        {
            // ID 202136
            auto        pattern     = "40 53 48 83 EC 20 48 8B 91 58 02 00 00 4C 8D 41 18";
            static auto address_ptr = FuncRelocation(pattern, OffsetsTable::GetOffset(202136), 202136);
            return address_ptr;
        }

        inline uintptr_t DlssUpscaleVtable()
        {
            // ID 1100493
            auto pattern = "";
            // unable to find stable
            return 0;
        }

        inline uintptr_t GetDXGIState()
        {
            // ID 1079146
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(1079146);
            return val;
        }

        inline uintptr_t GetCommandList()
        {
            // ID 206429
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(206429);
            return val;
        }

        inline uintptr_t GlobalFrameCount()
        {
            // ID 885900
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(885900);
            return val;
        }

        inline uintptr_t OnUpdateConstantBufferView()
        {
            // ID 204129
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(204129);
            return val;
        }

        inline uintptr_t OnTaaVFunc7()
        {
            // ID 208939
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(208939);
            return val;
        }
    } // namespace CreationRenderer


    namespace Nvidia {

        inline uintptr_t onSetReflexMarkerInternal()
        {
            // ID 1391721
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(1391721);
            return val;
        }
    }


    namespace PlayerCamera
    {
        inline uintptr_t Singleton()
        {
            // ID 878523
            auto        pattern     = "48 89 05 ? ? ? ? 48 8B D7";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(878523), 878523);
            return address_ptr;
        }

        inline uintptr_t QCameraEquals()
        {
            // ID 166081
            auto val = (uintptr_t)mod + OffsetsTable::GetOffset(166081);
            return val;
        }
    } // namespace PlayerCamera

    namespace Streamline
    {
        inline uintptr_t SetDlssOptions()
        {
            // ID 1171973
            auto        pattern     = "4C 8B 05 ? ? ? ? 4D 85 C0 75 21";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(1171973), 1171973);
            return address_ptr;
        }
    } // namespace Streamline

    namespace StorageTable::RenderGraphHandle
    {
        inline uintptr_t Singleton()
        {
            // ID 772639
            auto        pattern     = "48 8B 0D ? ? ? ? 4C 8B 89 E8 00 00 00 49 8B 49 40";
            static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(772639), 772639);
            return address_ptr;
        }
    } // namespace StorageTable::RenderGraphHandle

    inline uintptr_t GlobalFrameCounter()
    {
        // ID 886292
        auto        pattern     = "8B 1D ? ? ? ? 8B 81 28 01 00 00";
        static auto address_ptr = InstructionRelocation(pattern, 2, 6, OffsetsTable::GetOffset(886292), 886292);
        return address_ptr;
    }

    inline uintptr_t GlobalWorldFov()
    {
        // ID 1171750
        auto        pattern     = "C5 FA 10 05 ? ? ? ? 84 C9 74 61";
        static auto address_ptr = InstructionRelocation(pattern, 4, 8, OffsetsTable::GetOffset(1171750), 1171750);
        return address_ptr;
    }

    inline uintptr_t GlobalRenderSettings()
    {
        // ID 878340
        auto        pattern     = "48 8B 15 ? ? ? ? C5 FA 10 99 D4 01 00 00";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(878340), 878340);
        return address_ptr;
    }

    inline uintptr_t GlobalDirectX12Module()
    {
        // ID 886000
        auto        pattern     = "48 89 05 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B 1F";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(886000), 886000);
        return address_ptr;
    }

    inline uintptr_t GlobalDirectX12Module2()
    {
        // ID 885886
        auto        pattern     = "48 89 0D ? ? ? ? 4C 89 49 08";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(885886), 885886);
        return address_ptr;
    }

    inline uintptr_t GlobalAspectRatio()
    {
        // ID 873288
        auto        pattern     = "C5 FA 10 05 ? ? ? ? C5 FA 10 0D ? ? ? ? C5 F8 2F D0";
        static auto address_ptr = InstructionRelocation(pattern, 4, 8, OffsetsTable::GetOffset(873288), 873288);
        return address_ptr;
    }

    inline uintptr_t GlobalSceneGraphRoot()
    {
        // ID 887308
        auto        pattern     = "48 8D 0D ? ? ? ? 48 3B C8 74 0C";
        static auto address_ptr = InstructionRelocation(pattern, 3, 7, OffsetsTable::GetOffset(887308), 887308);
        return address_ptr;
    }

    inline uintptr_t GlobalPlayerRef()
    {
        // ID 865059
        auto val = (uintptr_t)mod + OffsetsTable::GetOffset(865059);
        return val;
    }
} // namespace Xbox::MemoryOffsets

#ifdef XBOX_STORE
namespace GameStore = Xbox;
#else
namespace GameStore = Steam;
#endif
