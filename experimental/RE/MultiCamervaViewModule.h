/*
#pragma once
#include "RE/Scene.h"
#include "RE/Starfield.h"

namespace RE
{

    class MultiCameraViewModule
    {
    public:
        MultiCameraViewModule() : m_rightEyeScene(new RightEyeScene()) {}

        void Init()
        {
            if (initialized) {
                return;
            }
            spdlog::info("MultiCameraViewModule Init");
            m_rightEyeScene->SetUp();
//            RecompileAllRenderGraphParams();
            REL::Relocation<uintptr_t> globAspectRatio{ REL::ID(873288) };
            auto aspectRatio = reinterpret_cast<float*>(globAspectRatio.address());
//            *aspectRatio = 1.0;
            initialized = true;
            shouldRecreateResources.exchange(true);

            spdlog::info("MultiCameraViewModule Init done");
        }

        void Update(uint32_t  renderGraphId)
        {
            if(renderGraphId != Main::WorldRoot()->renderHandleID)
            {
                return;
            }
            Init();


            auto mainRendererGraphScene = Main::WorldRoot();
            auto multiViewData          = StorageTable::RenderGraphHandle::GetMultiViewData(mainRendererGraphScene->renderHandleID);
            auto heap                   = multiViewData->multiViewDataArray._data.heap;
            if (multiViewData->multiViewDataArray._local) {
                heap = multiViewData->multiViewDataArray._data.local;
            }
            if(multiViewData->multiViewDataArray._size == 1) {
                auto index  = multiViewData->multiViewDataArray._size++;
                heap[index] = m_rightEyeScene->sceneHandleID;
            }
        }

        static void ResetUpscalerHistoryData() {
            auto mainRendererGraphScene = Main::WorldRoot();
            if(mainRendererGraphScene->renderHandleID == 0xFFFFFF) {
                return;
            }
            StorageTable::RenderGraphHandle::ResetUpscalerHistory(mainRendererGraphScene->renderHandleID);
        }

        void SwapCameras(unsigned long frameCount) {
            if(!initialized) {
                return;
            }
            RE::NiRect<float> viewport = {0.0, 1.0, 1.0, 0.0};
            RE::NiRect<float> viewport2 = {0.0, 0.0, 0.0, 0.0};
            if (frameCount % 2 != 0) {
                RE::Main::WorldCamera()->SetViewport(&viewport);
                RE::Main::WorldCamera()->SetScissors(&viewport);
                m_rightEyeScene->m_camera->SetViewport(&viewport2);
                m_rightEyeScene->m_camera->SetScissors(&viewport2);
            }
            else {
                RE::Main::WorldCamera()->SetViewport(&viewport2);
                RE::Main::WorldCamera()->SetScissors(&viewport2);
                m_rightEyeScene->m_camera->SetViewport(&viewport);
                m_rightEyeScene->m_camera->SetScissors(&viewport);
            }

        }

        static void RecompileAllRenderGraphParams() {
            REL::Relocation<uint64_t*> globGraphicContextOuter{ REL::ID(885886)};
            auto offset = reinterpret_cast<uint64_t*>(*globGraphicContextOuter + 0xD8);
            RecompileAllRenderGraphParamsAndResources(*offset);
        }

        static void RecompileAllRenderGraphParamsAndResources(uint64_t globContextOuter_offD8) {
            using func_t = decltype(&MultiCameraViewModule::RecompileAllRenderGraphParamsAndResources);
            REL::Relocation<func_t> func{ REL::ID(1537333) };
            return func(globContextOuter_offD8);
        }

    public:
        bool initialized{false};
        std::atomic<bool> shouldRecreateResources{false};
        RightEyeScene* m_rightEyeScene;
        uint64_t*         commandContextHolderAddr{0};
    };
} // namespace RE
*/
