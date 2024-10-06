/*
#pragma once
#include "RE/Starfield.h"

namespace RE
{
    class RightEyeScene
    {
    public:
        void SetUp()
        {
            spdlog::info("Setting up RightEyeScene");
            auto mainRendererGraphScene = Main::WorldRoot();
            auto worldCamera            = mainRendererGraphScene->worldCamera;
            NiRect<float> viewport = {0.0, 0.5, 1.0, 0.0};
            NiRect<float> scissors = {0.0, 0.5, 1.0, 0.0};
            worldCamera->SetViewport(&viewport);
            worldCamera->SetScissors(&scissors);
            auto cloningProcess         = NiCloneProcess();
            m_camera                 = reinterpret_cast<NiCamera*>(worldCamera->createClone(&cloningProcess));
            m_camera->name              = "RightEyeCamera";
            m_camera->cameraHandleID = 0xFFFFFF;
            m_camera->RegisterAsRenderCamera();
            viewport = {0.5, 1.0, 1.0, 0.0};
            scissors = {0.5, 1.0, 1.0, 0.0};
            m_camera->SetViewport(&viewport);
            m_camera->SetScissors(&scissors);
            m_camera->SetNearFar(worldCamera->viewFrustum._near, worldCamera->viewFrustum._far);
            m_camera->SetClipspaceType(worldCamera->clipSpaceType);
            m_camera->local.translate.x = 0.068f;
            NiFrustum frustum = worldCamera->viewFrustum;
//            frustum.left = 0.5f * frustum.left;
//            frustum.right = 0.5f * frustum.right;
            m_camera->SetNiFrustumInternal(&frustum);

            mainRendererGraphScene->worldCameraRoot->AttachChild(m_camera, true);
            NiUpdateData updateData{};
            m_camera->Update(&updateData);
            auto cameraViewHandle               = StorageTable::CameraViewHandle::Singleton();
            auto cameraViewData                 = cameraViewHandle->storageData.pColumnCameraViewData->getObject(mainRendererGraphScene->sceneViewHandleID);
            auto cameraViewDataSnapshot         = *cameraViewData;
            cameraViewDataSnapshot.cameraViewID = m_camera->cameraHandleID;
            cameraViewHandle->Register(&sceneHandleID);
            StorageTable::CameraViewHandle::writeCameraViewData(sceneHandleID, &cameraViewDataSnapshot);
            StorageTable::CameraViewHandle::writeImageSpaceData(sceneHandleID, &mainRendererGraphScene->globalScene.imageSpaceHandlerId);
            auto featureSetup = StorageTable::CameraViewHandle::getFeatureSetup(mainRendererGraphScene->sceneViewHandleID);
            auto featureSetupSnapshot = *featureSetup;
            StorageTable::CameraViewHandle::writeFeatureSetupData(sceneHandleID, &featureSetupSnapshot);

        }

        */
/*void SetupFeatures() {
            REL::Relocation<uint64_t*> renderGraphSettings{ REL::ID(878340)};
            //auto offset = reinterpret_cast<uint64_t>(*renderGraphSettings + 96);
            auto offset = *renderGraphSettings + 96;
//            using func_t = void(uint64_t*, uint32_t);
//            REL::Relocation<func_t> func{ REL::ID(184552) };
            SetupFeaturesInternal(offset, sceneHandleID);

        }*//*


        static void SetupFeaturesInternal(uint64_t offset, uint32_t sceneHandleId) {
            using func_t = decltype(&RightEyeScene::SetupFeaturesInternal);
            REL::Relocation<func_t> func{ REL::ID(184552) };
            return func(offset, sceneHandleId);
        }
        void SetupShadowData(uint32_t sceneHandleId) {
            using func_t = decltype(&RightEyeScene::SetupShadowData);
            REL::Relocation<func_t> func{ REL::ID(180022) };
            func(this, sceneHandleId);
        }

        void Setup2() {
            m_sceneNode = new BSSceneNode("RightEyeSceneNode");
            //auto spObjects = new NiNode();
            //m_sceneNode->AttachChild(spObjects, true);

        }


    public:
        uint32_t     sceneHandleID{0xFFFFFF};
        NiNode*      worldRoot{};
        NiCamera*    m_camera;
        BSSceneNode* m_sceneNode{};
    };
} // namespace RE
*/
