#include "SMAA.h"
#include <RenderTargetState.h>
#include <SFVR.hpp>
#include <_deps/directxtk12-src/Src/PlatformHelpers.h>
#include <_deps/directxtk12-src/Src/d3dx12.h>

namespace SHADERS::SMAA {
    namespace TEX {
        #include "smaa/SearchTex.h"
        #include "smaa/AreaTex.h"
    }

    namespace EDGE_DETECT {
        namespace PREDICATION {
            namespace LUMA::PS {
                #include "shader-headers/smaa_edge_detect_luma_predication_ps.h"
            }
            namespace COLOR::PS {
                #include "shader-headers/smaa_edge_detect_color_predication_ps.h"
            }
        }

        namespace DEPTH::PS {
            #include "shader-headers/smaa_edge_detect_depth_ps.h"
        }

        namespace LUMA::PS {
            #include "shader-headers/smaa_edge_detect_luma_ps.h"
        }

        namespace COLOR::PS {
            #include "shader-headers/smaa_edge_detect_color_ps.h"
        }

        namespace VS {
            #include "shader-headers/smaa_edge_detect_vs.h"
        }
    }

    namespace BLEND_WEIGHT_CALC {
        namespace PS {
            #include "shader-headers/smaa_blending_weight_ps.h"
        }
        namespace VS {
            #include "shader-headers/smaa_blending_weight_vs.h"
        }
    }

    namespace FINAL_BLEND {
        namespace PS {
            #include "shader-headers/smaa_final_ps.h"
        }
        namespace VS {
            #include "shader-headers/smaa_final_vs.h"
        }
    }

    namespace DEPTH_LINEAR {
        namespace PS {
            #include "shader-headers/smaa_depth_linear_ps.h"
        }
        namespace VS {
            #include "shader-headers/smaa_depth_linear_vs.h"
        }
    }

    namespace FAKE {
        namespace PS {
            #include "shader-headers/smaa_fake_ps.h"
        }
        namespace VS {
            #include "shader-headers/smaa_fake_vs.h"
        }
    }
}
using namespace DirectX::DX12;
using namespace DirectX;

SmaaConstants g_smaa_constants{};
SmaaSettings g_smaa_settings{};

bool SMAA::setup(ID3D12Device* device, const D3D12_RESOURCE_DESC backBuffer_desc)
{
    auto backbuffer_srv_desc = backBuffer_desc;
    backbuffer_srv_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    backbuffer_srv_desc.Flags &= ~D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

    auto rt_desc = backBuffer_desc;

    rt_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rt_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    rt_desc.Flags &= ~D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

    D3D12_HEAP_PROPERTIES heap_props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    spdlog::info("SMAA setup {}x{} format {}", backbuffer_srv_desc.Width, backbuffer_srv_desc.Height, backBuffer_desc.Format);

    try {
        m_srv_heap = std::make_unique<DirectX::DescriptorHeap>(device,
                                                                   D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                                                                   D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
                                                               (UINT)SRV::COUNT);
        m_rtv_heap = std::make_unique<DirectX::DescriptorHeap>(device,
                                                               D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
                                                               D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                                                               (UINT)RTV::COUNT);
    } catch(...) {
        spdlog::error("Failed to create SRV/RTV descriptor heap for SMAA");
        return false;
    }

    {
        if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &backbuffer_srv_desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
                                                   IID_PPV_ARGS(m_backbuffer_copy_resource.GetAddressOf())))) {
            spdlog::error("[VR] Failed to create backbuffer copy for SMAA.");
            return false;
        }

        //TODO explicit resource description
        device->CreateShaderResourceView(m_backbuffer_copy_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::BACKBUFFER_COPY));
        D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
        srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srv_desc.Texture2D.MipLevels = 1;
        srv_desc.Texture2D.MostDetailedMip = 0;
        srv_desc.Texture2D.PlaneSlice = 0;
        srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;
        device->CreateShaderResourceView(m_backbuffer_copy_resource.Get(), &srv_desc, m_srv_heap->GetCpuHandle((UINT)SRV::BACKBUFFER_COPY_SRGB));
    }

    {
        if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &rt_desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
                                                   IID_PPV_ARGS(m_SMAAEdge_resource.GetAddressOf())))) {
            spdlog::error("[VR] Failed to create m_SMAAEdge for SMAA.");
            return false;
        }
        device->CreateShaderResourceView(m_SMAAEdge_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::SMAA_EDGE));
        device->CreateRenderTargetView(m_SMAAEdge_resource.Get(), nullptr, m_rtv_heap->GetCpuHandle((UINT)RTV::SMAA_EDGE));
    }

    {
        if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &rt_desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
                                                   IID_PPV_ARGS(m_SMAABlend_resource.GetAddressOf())))) {
            spdlog::error("[VR] Failed to create m_SMAABlend for SMAA.");
            return false;
        }
        device->CreateShaderResourceView(m_SMAABlend_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::SMAA_BLEND_WEIGHT));
        device->CreateRenderTargetView(m_SMAABlend_resource.Get(), nullptr, m_rtv_heap->GetCpuHandle((UINT)RTV::SMAA_BLEND_WEIGHT));
    }

    {
        if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &rt_desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
                                                   IID_PPV_ARGS(m_depthStencilBuffer_linear_resource.GetAddressOf())))) {
            spdlog::error("[VR] Failed to create m_depthStencilBuffer_linear_resource for SMAA.");
            return false;
        }
        device->CreateShaderResourceView(m_depthStencilBuffer_linear_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::DEPTH_BUFFER_LINEAR));
        device->CreateRenderTargetView(m_depthStencilBuffer_linear_resource.Get(), nullptr, m_rtv_heap->GetCpuHandle((UINT)RTV::DEPTH_BUFFER_LINEAR));
    }

    /*{
        ComPtr<ID3D12Resource> d3d12resource{};
        if (FAILED(device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &rt_desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
                                                   IID_PPV_ARGS(d3d12resource.GetAddressOf())))) {
            spdlog::error("[VR] Failed to create SMAA output texture.");
            return false;
        }

        if (!m_render_target.setup(device, d3d12resource.Get(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM, L"SMAA Render Target")) {
            spdlog::error("[VR] Error setting up SMAA texture RTV/SRV.");
            return false;
        }
    }*/

    {

        D3D12_RESOURCE_DESC depthStencilDesc = {};
        depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthStencilDesc.Width = rt_desc.Width;  // The width of the depth-stencil buffer
        depthStencilDesc.Height = rt_desc.Height;  // The height of the depth-stencil buffer
        depthStencilDesc.DepthOrArraySize = 1;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.Format = DXGI_FORMAT_R32G8X24_TYPELESS ; //DXGI_FORMAT_R24G8_TYPELESS;  // 24-bit depth, 8-bit stencil
        depthStencilDesc.SampleDesc.Count = 1;  // No multisampling
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        
        D3D12_CLEAR_VALUE clearValue = {};
        clearValue.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        clearValue.DepthStencil.Depth = 1.0f;
        clearValue.DepthStencil.Stencil = 0;

        {
            ThrowIfFailed(device->CreateCommittedResource(
                &heap_props,
                D3D12_HEAP_FLAG_NONE,
                &depthStencilDesc,
                D3D12_RESOURCE_STATE_DEPTH_WRITE,
                &clearValue,
                IID_PPV_ARGS(m_depthStencilBuffer.GetAddressOf())));

            try {
                m_dsv_heap = std::make_unique<DirectX::DescriptorHeap>(device,
                                                                           D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
                                                                           D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                                                                           1);
            } catch(...) {
                spdlog::error("Failed to create DSV descriptor heap 1");
                return false;
            }
        }

        {
            ThrowIfFailed(device->CreateCommittedResource(
                &heap_props,
                D3D12_HEAP_FLAG_NONE,
                &depthStencilDesc,
                D3D12_RESOURCE_STATE_DEPTH_READ,
                &clearValue,
                IID_PPV_ARGS(m_depthStencilBuffer_srv_resource.GetAddressOf())));
        }



        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;

        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsv_heap->GetCpuHandle(0);
        device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &dsvDesc, dsvHandle);

        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM ;
        srvDesc.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS ;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Texture2D.MipLevels = 1;
        device->CreateShaderResourceView(m_depthStencilBuffer_srv_resource.Get(), &srvDesc, m_srv_heap->GetCpuHandle((UINT)SRV::DEPTH_BUFFER_READ));
    }

    {

        spdlog::info("Creating precompiled textures and pipeline states");
        create_precompiled_textures(device);

    }

    if (!CreateRootSignature(device) || !CreatePipelineStates(device, DXGI_FORMAT_R8G8B8A8_UNORM))
    {
        spdlog::error("Failed to setup SMAA");
        return false;
    }

    return true;
}

bool SMAA::CreateRootSignature(ID3D12Device* device)
{
    HRESULT                 hr;
    CD3DX12_DESCRIPTOR_RANGE1 descriptorRange[1];
    CD3DX12_ROOT_PARAMETER1   rootParams[2];

    descriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, (UINT)SRV::COUNT, 0, 0);
//    descriptorRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0);
//    descriptorRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2, 0);
    rootParams[0].InitAsDescriptorTable(1, &descriptorRange[0], D3D12_SHADER_VISIBILITY_PIXEL);
//    rootParams[1].InitAsDescriptorTable(1, &descriptorRange[1], D3D12_SHADER_VISIBILITY_PIXEL);
//    rootParams[2].InitAsDescriptorTable(1, &descriptorRange[2], D3D12_SHADER_VISIBILITY_PIXEL);
    rootParams[1].InitAsConstants(15, 0, 0, D3D12_SHADER_VISIBILITY_ALL);
    CD3DX12_STATIC_SAMPLER_DESC staticSamplers[2];

    // D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT
    staticSamplers[0].Init(0, D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    // D3D12_FILTER_MIN_MAG_MIP_POINT
    staticSamplers[1].Init(1, D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
    staticSamplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
                                                    | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParams), rootParams,
                               _countof(staticSamplers), staticSamplers, rootSignatureFlags);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_1, signature.GetAddressOf(), error.GetAddressOf());
    if (FAILED(hr)) {
        if (error)
            spdlog::error("Error: D3DX12SerializeVersionedRootSignature {}", static_cast<char*>(error->GetBufferPointer()));
        return false;
    }
    hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(m_rootSignature.GetAddressOf()));
    if (FAILED(hr)) {
        if (error)
            spdlog::error("Error: CreateRootSignature {}", static_cast<char*>(error->GetBufferPointer()));
        return false;
    }
    spdlog::info("[VR] SMAA root signature created.");
    return true;
}

bool SMAA::CreatePipelineStates(ID3D12Device* device, DXGI_FORMAT backBufferFormat)
{
    // edge detection pso
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC edgeDetectPsoDesc = {};
        edgeDetectPsoDesc.InputLayout = { };
        edgeDetectPsoDesc.pRootSignature = m_rootSignature.Get();
        edgeDetectPsoDesc.VS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::EDGE_DETECT::VS::g_vs_main, sizeof(SHADERS::SMAA::EDGE_DETECT::VS::g_vs_main));
        edgeDetectPsoDesc.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::EDGE_DETECT::LUMA::PS::g_ps_main, sizeof(SHADERS::SMAA::EDGE_DETECT::LUMA::PS::g_ps_main));

        edgeDetectPsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        edgeDetectPsoDesc.BlendState.RenderTarget[0].BlendEnable = false;

        edgeDetectPsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        edgeDetectPsoDesc.DepthStencilState.DepthEnable = false;
        edgeDetectPsoDesc.DepthStencilState.StencilEnable = true;
        edgeDetectPsoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
        edgeDetectPsoDesc.SampleMask = UINT_MAX;

        edgeDetectPsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        edgeDetectPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        edgeDetectPsoDesc.NumRenderTargets = 1;
        //TODO depth stenci view format is wrong
        edgeDetectPsoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        edgeDetectPsoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R10G10B10A2_UNORM
        edgeDetectPsoDesc.SampleDesc.Count = 1;
        // edgeDetectPsoDesc.SampleDesc.Quality = 0;

        ThrowIfFailed(device->CreateGraphicsPipelineState(&edgeDetectPsoDesc, IID_PPV_ARGS(m_edgeDetectionLumaPso.GetAddressOf())));
        spdlog::info("[VR] SMAA Edge Detection Luma pipeline state created.");
        edgeDetectPsoDesc.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::EDGE_DETECT::COLOR::PS::g_ps_main, sizeof(SHADERS::SMAA::EDGE_DETECT::COLOR::PS::g_ps_main));
        ThrowIfFailed(device->CreateGraphicsPipelineState(&edgeDetectPsoDesc, IID_PPV_ARGS(m_edgeDetectionColorPso.GetAddressOf())));
        spdlog::info("[VR] SMAA Edge Detection Color pipeline state created.");
        edgeDetectPsoDesc.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::EDGE_DETECT::PREDICATION::LUMA::PS::g_ps_main, sizeof(SHADERS::SMAA::EDGE_DETECT::PREDICATION::LUMA::PS::g_ps_main));
        ThrowIfFailed(device->CreateGraphicsPipelineState(&edgeDetectPsoDesc, IID_PPV_ARGS(m_edgeDetectionLumaPredicationPso.GetAddressOf())));
        spdlog::info("[VR] SMAA Edge Detection Luma Predication pipeline state created.");
        edgeDetectPsoDesc.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::EDGE_DETECT::PREDICATION::COLOR::PS::g_ps_main, sizeof(SHADERS::SMAA::EDGE_DETECT::PREDICATION::COLOR::PS::g_ps_main));
        ThrowIfFailed(device->CreateGraphicsPipelineState(&edgeDetectPsoDesc, IID_PPV_ARGS(m_edgeDetectionColorPredicationPso.GetAddressOf())));
        spdlog::info("[VR] SMAA Edge Detection Color Predication pipeline state created.");
        edgeDetectPsoDesc.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::EDGE_DETECT::DEPTH::PS::g_ps_main, sizeof(SHADERS::SMAA::EDGE_DETECT::DEPTH::PS::g_ps_main));
        ThrowIfFailed(device->CreateGraphicsPipelineState(&edgeDetectPsoDesc, IID_PPV_ARGS(m_edgeDetectionDepthPso.GetAddressOf())));
        spdlog::info("[VR] SMAA Edge Detection Depth pipeline state created.");
    }

    // blending weight calc pso
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC blendingWeightCalsPso = {};
        blendingWeightCalsPso.InputLayout = { };
        blendingWeightCalsPso.pRootSignature = m_rootSignature.Get();
        blendingWeightCalsPso.VS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::BLEND_WEIGHT_CALC::VS::g_vs_main, sizeof(SHADERS::SMAA::BLEND_WEIGHT_CALC::VS::g_vs_main));
        blendingWeightCalsPso.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::BLEND_WEIGHT_CALC::PS::g_ps_main, sizeof(SHADERS::SMAA::BLEND_WEIGHT_CALC::PS::g_ps_main));

        blendingWeightCalsPso.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        blendingWeightCalsPso.BlendState.RenderTarget[0].BlendEnable = false;

        blendingWeightCalsPso.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        blendingWeightCalsPso.DepthStencilState.DepthEnable = false;
        blendingWeightCalsPso.DepthStencilState.StencilEnable = true;
        blendingWeightCalsPso.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

        blendingWeightCalsPso.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        blendingWeightCalsPso.SampleMask = UINT_MAX;
        blendingWeightCalsPso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        blendingWeightCalsPso.NumRenderTargets = 1;
        blendingWeightCalsPso.DSVFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        blendingWeightCalsPso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R10G10B10A2_UNORM
        blendingWeightCalsPso.SampleDesc.Count = 1;
        ThrowIfFailed(device->CreateGraphicsPipelineState(&blendingWeightCalsPso, IID_PPV_ARGS(m_blendingWeightCalcPso.GetAddressOf())));
        spdlog::info("[VR] SMAA Blend Weight Calc Detection pipeline state created.");
    }

    // Neighbour blend pass
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC finalBlend = {};
        finalBlend.InputLayout = { };
        finalBlend.pRootSignature = m_rootSignature.Get();
        finalBlend.VS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::FINAL_BLEND::VS::g_vs_main, sizeof(SHADERS::SMAA::FINAL_BLEND::VS::g_vs_main));
        finalBlend.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::FINAL_BLEND::PS::g_ps_main, sizeof(SHADERS::SMAA::FINAL_BLEND::PS::g_ps_main));


        finalBlend.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        finalBlend.BlendState.AlphaToCoverageEnable = false;
        finalBlend.BlendState.RenderTarget[0].BlendEnable = false;
        finalBlend.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_BLEND_FACTOR;
        finalBlend.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_BLEND_FACTOR;
        finalBlend.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

        finalBlend.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        finalBlend.DepthStencilState.DepthEnable = false;
        finalBlend.DepthStencilState.StencilEnable = false;

        finalBlend.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        finalBlend.SampleMask = UINT_MAX;
        finalBlend.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        finalBlend.NumRenderTargets = 1;
        finalBlend.DSVFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        finalBlend.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // DXGI_FORMAT_R10G10B10A2_UNORM
        finalBlend.SampleDesc.Count = 1;
        ThrowIfFailed(device->CreateGraphicsPipelineState(&finalBlend, IID_PPV_ARGS(m_finalBlendPso.GetAddressOf())));
        spdlog::info("[VR] SMAA Final Blend pipeline state created.");
    }

    // linear depth pso
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC linearDepthPso = {};
        linearDepthPso.InputLayout = { };
        linearDepthPso.pRootSignature = m_rootSignature.Get();
        linearDepthPso.VS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::DEPTH_LINEAR::VS::g_vs_main, sizeof(SHADERS::SMAA::DEPTH_LINEAR::VS::g_vs_main));
        linearDepthPso.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::DEPTH_LINEAR::PS::g_ps_main, sizeof(SHADERS::SMAA::DEPTH_LINEAR::PS::g_ps_main));

        linearDepthPso.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        linearDepthPso.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        linearDepthPso.DepthStencilState.DepthEnable = false;
        linearDepthPso.SampleMask = UINT_MAX;
        linearDepthPso.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        linearDepthPso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        linearDepthPso.NumRenderTargets = 1;
        linearDepthPso.DSVFormat = DXGI_FORMAT_UNKNOWN;
        //TODO make it single float
        linearDepthPso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        linearDepthPso.SampleDesc.Count = 1;
        linearDepthPso.SampleDesc.Quality = 0;

        ThrowIfFailed(device->CreateGraphicsPipelineState(&linearDepthPso, IID_PPV_ARGS(m_depthLinearPso.GetAddressOf())));
        spdlog::info("[VR] depth linear pso.");
    }

    // dummy pso
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC dummypso = {};
        dummypso.InputLayout = { };
        dummypso.pRootSignature = m_rootSignature.Get();
        dummypso.VS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::FAKE::VS::g_vs_main, sizeof(SHADERS::SMAA::FAKE::VS::g_vs_main));
        dummypso.PS = CD3DX12_SHADER_BYTECODE(&SHADERS::SMAA::FAKE::PS::g_ps_main, sizeof(SHADERS::SMAA::FAKE::PS::g_ps_main));

        dummypso.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        dummypso.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        dummypso.DepthStencilState.DepthEnable = false;
        dummypso.SampleMask = UINT_MAX;
        dummypso.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        dummypso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        dummypso.NumRenderTargets = 1;
        dummypso.DSVFormat = DXGI_FORMAT_UNKNOWN;
        dummypso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        dummypso.SampleDesc.Count = 1;
        dummypso.SampleDesc.Quality = 0;

        ThrowIfFailed(device->CreateGraphicsPipelineState(&dummypso, IID_PPV_ARGS(m_dummy_pso.GetAddressOf())));
        spdlog::info("[VR] dummy pso.");
    }
    return true;
}


void upload_resource(ID3D12Device* m_device, ID3D12CommandQueue* command_queue, const unsigned char* raw_data, const ComPtr<ID3D12Resource>& resource, int row_pitch, int slice_pitch) {
    ResourceUploadBatch upload(m_device);

    upload.Begin();

    D3D12_SUBRESOURCE_DATA initData = { raw_data, row_pitch, slice_pitch};
    upload.Upload(resource.Get(), 0, &initData, 1);

    upload.Transition(resource.Get(),
                      D3D12_RESOURCE_STATE_COPY_DEST,
                      D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    // Upload the resources to the GPU.
    auto finish = upload.End(command_queue);

    // Wait for the upload thread to terminate
    finish.wait();
}

ComPtr<ID3D12CommandQueue> create_command_queue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type) {
    // Describe the command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = type;                            // Type of the command queue (e.g., direct, copy, compute)
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // Priority of the command queue
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;  // No additional flags
    queueDesc.NodeMask = 0;                           // Single GPU operation, so node mask is 0

    // Create the command queue
    ComPtr<ID3D12CommandQueue> commandQueue;
    HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create command queue.");
    }

    return commandQueue;
}


void SMAA::create_precompiled_textures(ID3D12Device* m_device)
{
    spdlog::info("[VR] Creating precompiled textures for SMAA.");
    auto& hook = g_framework->get_d3d12_hook();
    auto command_queue = create_command_queue(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);

    D3D12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    auto desc = CD3DX12_RESOURCE_DESC(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
        64, 16, 1, 1,
        DXGI_FORMAT_R8_UNORM,
        1, 0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

    {

        ThrowIfFailed(m_device->CreateCommittedResource(
            &defaultHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(m_searchTex_resource.GetAddressOf())));

        upload_resource(m_device, command_queue.Get(), SHADERS::SMAA::TEX::searchTexBytes, m_searchTex_resource,64, 64*16);
        m_device->CreateShaderResourceView(m_searchTex_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::SEARCH_TEX));

    }

    {
        desc.Format = DXGI_FORMAT_R8G8_UNORM;
        desc.Width = 160;
        desc.Height = 560;

        ThrowIfFailed(m_device->CreateCommittedResource(
            &defaultHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(m_areaTex_resource.GetAddressOf())));
        upload_resource(m_device, command_queue.Get(), SHADERS::SMAA::TEX::areaTexBytes, m_areaTex_resource, 320, 320*560);
        m_device->CreateShaderResourceView(m_areaTex_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::AREA_TEX));
    }
}

void SMAA::Apply(std::unique_ptr<d3d12::CommandContext>& cmd_ctx, ID3D12Resource* backbuffer, D3D12_CPU_DESCRIPTOR_HANDLE* rtv_handle, ID3D12Resource* dsv) {
    auto commandList = cmd_ctx->cmd_list.Get();
    const auto render_target_desc = backbuffer->GetDesc();

    // copy backbuffer first so we can use it as a texture for blending
    {
        D3D12_RESOURCE_BARRIER barriers[2]{ CD3DX12_RESOURCE_BARRIER::Transition(backbuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_SOURCE),
                                            CD3DX12_RESOURCE_BARRIER::Transition(m_backbuffer_copy_resource.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                                                                                 D3D12_RESOURCE_STATE_COPY_DEST) };
        commandList->ResourceBarrier(2, barriers);

        commandList->CopyResource(m_backbuffer_copy_resource.Get(), backbuffer);

        barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
        barriers[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barriers[1].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->ResourceBarrier(2, barriers);
    }
    float clear_color[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

    D3D12_VIEWPORT viewport{};
    viewport.Width    = (float)render_target_desc.Width;
    viewport.Height   = (float)render_target_desc.Height;
    viewport.MinDepth = D3D12_MIN_DEPTH;
    viewport.MaxDepth = D3D12_MAX_DEPTH;

    D3D12_RECT scissor_rect{};
    scissor_rect.left   = 0;
    scissor_rect.top    = 0;
    scissor_rect.right  = (LONG)render_target_desc.Width;
    scissor_rect.bottom = (LONG)render_target_desc.Height;
    auto dsv_handle = m_dsv_heap->GetCpuHandle(0);

    if (dsv != nullptr && !IsBadReadPtr(dsv, sizeof(uintptr_t)) && (g_smaa_settings.debugOutput == 5 || g_smaa_settings.predication || g_smaa_settings.edgeDetectionMethod == 0) ) {
        D3D12_RESOURCE_BARRIER barriers[2]{
            CD3DX12_RESOURCE_BARRIER::Transition(dsv, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_COPY_SOURCE),
            CD3DX12_RESOURCE_BARRIER::Transition(m_depthStencilBuffer_srv_resource.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST)
        };
        commandList->ResourceBarrier(2, barriers);
        auto original_dsv_desc = dsv->GetDesc();
        auto dsv_desc = m_depthStencilBuffer_srv_resource->GetDesc();
        if (original_dsv_desc.Width != dsv_desc.Width || original_dsv_desc.Height != dsv_desc.Height) {
            D3D12_BOX src_box{0,0,0, (UINT)original_dsv_desc.Width, (UINT)original_dsv_desc.Height, 1};
            UINT offsetX = 0;
            UINT offsetY = 0;
            if(original_dsv_desc.Width > dsv_desc.Width) {
                src_box.left = (original_dsv_desc.Width - dsv_desc.Width) >> 1;
                src_box.right = src_box.left + dsv_desc.Width;
            } else {
                offsetX = (dsv_desc.Width - original_dsv_desc.Width) >> 1;
            }

            if(original_dsv_desc.Height > dsv_desc.Height) {
                src_box.top = (original_dsv_desc.Height - dsv_desc.Height) >> 1;
                src_box.bottom = src_box.top + dsv_desc.Height;
            } else {
                offsetY = (dsv_desc.Height - original_dsv_desc.Height) >> 1;
            }

            const CD3DX12_TEXTURE_COPY_LOCATION& destination2 = CD3DX12_TEXTURE_COPY_LOCATION(m_depthStencilBuffer_srv_resource.Get(), 0);
            const CD3DX12_TEXTURE_COPY_LOCATION& source      = CD3DX12_TEXTURE_COPY_LOCATION(dsv, 0);
            commandList->CopyTextureRegion(&destination2, offsetX, offsetY, 0,
                                           &source, &src_box);
//            spdlog::error("Depth stencil buffer size mismatch. Original: {}x{}, New: {}x{}", original_dsv_desc.Width, original_dsv_desc.Height, dsv_desc.Width, dsv_desc.Height);
        } else {
            commandList->CopyResource(m_depthStencilBuffer_srv_resource.Get(), dsv);
        }
        barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
        barriers[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_DEPTH_WRITE;
        barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barriers[1].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->ResourceBarrier(2, barriers);

    }

    commandList->ClearDepthStencilView(dsv_handle, D3D12_CLEAR_FLAG_STENCIL | D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    auto constants = g_smaa_constants;

    auto rpc_window_size
        = XMFLOAT4(1.0f / ((float)render_target_desc.Width), 1.0f / ((float)render_target_desc.Height),
                                 ((float)render_target_desc.Width), ((float)render_target_desc.Height));

    // Clear the render target within the scissor rectangle
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissor_rect);
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    commandList->SetGraphicsRoot32BitConstants(1, 4, &rpc_window_size, 0);
    commandList->SetGraphicsRoot32BitConstants(1, 11, &constants, 4);
    ID3D12DescriptorHeap* descriptorHeaps[] = {
        m_srv_heap->Heap()
    };
    commandList->SetDescriptorHeaps(1, descriptorHeaps);
    commandList->SetGraphicsRootDescriptorTable(0, m_srv_heap->GetFirstGpuHandle());

    if (dsv != nullptr) {
        D3D12_RESOURCE_BARRIER barriers[1]{
            CD3DX12_RESOURCE_BARRIER::Transition(m_depthStencilBuffer_linear_resource.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
        };
        commandList->ResourceBarrier(1, barriers);
        commandList->ClearRenderTargetView(m_rtv_heap->GetCpuHandle((UINT)RTV::DEPTH_BUFFER_LINEAR), clear_color, 1, &scissor_rect);
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_heaps[] = { m_rtv_heap->GetCpuHandle((UINT)RTV::DEPTH_BUFFER_LINEAR) };
        commandList->OMSetRenderTargets(1, rtv_heaps, FALSE, nullptr);
        commandList->SetPipelineState(m_depthLinearPso.Get());
        commandList->DrawInstanced(6, 1, 0, 0);

        barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barriers[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->ResourceBarrier(1, barriers);
    }

    // edge detection pass
    {
        D3D12_RESOURCE_BARRIER barriers[1]{
            CD3DX12_RESOURCE_BARRIER::Transition(m_SMAAEdge_resource.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
        };
        commandList->ResourceBarrier(1, barriers);

        commandList->ClearRenderTargetView(m_rtv_heap->GetCpuHandle((UINT)RTV::SMAA_EDGE), clear_color, 1, &scissor_rect);
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_heaps[] = { m_rtv_heap->GetCpuHandle((UINT)RTV::SMAA_EDGE) };
        commandList->OMSetRenderTargets(1, rtv_heaps, FALSE, &dsv_handle);

        ID3D12PipelineState* pso;
        if(g_smaa_settings.edgeDetectionMethod == 0) {
            pso = m_edgeDetectionDepthPso.Get();
        } else if(g_smaa_settings.edgeDetectionMethod == 1) {
            if(g_smaa_settings.predication) {
                pso = m_edgeDetectionLumaPredicationPso.Get();
            } else {
                pso = m_edgeDetectionLumaPso.Get();
            }
        } else {
            if(g_smaa_settings.predication) {
                pso = m_edgeDetectionColorPredicationPso.Get();
            } else {
                pso = m_edgeDetectionColorPso.Get();
            }
        }

        commandList->SetPipelineState(pso);
        commandList->OMSetStencilRef(1);
        commandList->OMSetBlendFactor(clear_color);
        commandList->DrawInstanced(6, 1, 0, 0);

        barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barriers[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->ResourceBarrier(1, barriers);
    }

    // blending weight calc pass
    {
        D3D12_RESOURCE_BARRIER barriers[1]{
            CD3DX12_RESOURCE_BARRIER::Transition(m_SMAABlend_resource.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
        };
        commandList->ResourceBarrier(1, barriers);

        commandList->ClearRenderTargetView(m_rtv_heap->GetCpuHandle((UINT)RTV::SMAA_BLEND_WEIGHT), clear_color, 1, &scissor_rect);
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_heaps[] = { m_rtv_heap->GetCpuHandle((UINT)RTV::SMAA_BLEND_WEIGHT) };
        commandList->OMSetRenderTargets(1, rtv_heaps, FALSE, &dsv_handle);
        commandList->SetPipelineState(m_blendingWeightCalcPso.Get());
        commandList->OMSetStencilRef(1);
        commandList->OMSetBlendFactor(clear_color);
        commandList->DrawInstanced(6, 1, 0, 0);

        barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barriers[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->ResourceBarrier(1, barriers);
    }
    auto device = g_framework->get_d3d12_hook()->get_device();

    // final pass
    if(g_smaa_settings.debugOutput == 0){
        commandList->SetPipelineState(m_finalBlendPso.Get());
//        commandList->ClearRenderTargetView(*rtv_handle, clear_color, 1, &scissor_rect);
        D3D12_RENDER_TARGET_VIEW_DESC rtv_desc{};
        rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        rtv_desc.Texture2D.MipSlice = 0;
        rtv_desc.Texture2D.PlaneSlice = 0;

        auto rtv_srgb_handle = m_rtv_heap->GetCpuHandle((UINT)RTV::MAIN_RT_SRGB);
        device->CreateRenderTargetView(backbuffer, &rtv_desc, rtv_srgb_handle);
        commandList->OMSetRenderTargets(1, &rtv_srgb_handle, FALSE, nullptr);

        commandList->OMSetStencilRef(0);
        float blend_factor[4]{1.0, 1.0, 1.0, 1.0};
        commandList->OMSetBlendFactor(blend_factor);
        commandList->DrawInstanced(6, 1, 0, 0);
        scissor_rect.left = 700;
        scissor_rect.top = 700;
        scissor_rect.right = 710;
        scissor_rect.bottom = 710;
        float red_square[4]{1.0f, 0.0f, 0.0f, 1.0f};
        commandList->ClearRenderTargetView(*rtv_handle, red_square, 1, &scissor_rect);

    } else {
        commandList->SetPipelineState(m_dummy_pso.Get());
        commandList->ClearRenderTargetView(*rtv_handle, clear_color, 1, &scissor_rect);
        commandList->OMSetRenderTargets(1, rtv_handle, FALSE, nullptr);
        commandList->OMSetStencilRef(0);

        ID3D12DescriptorHeap* heap;
        D3D12_GPU_DESCRIPTOR_HANDLE handle;
        if(g_smaa_settings.debugOutput == 1) {
            device->CreateShaderResourceView(m_SMAAEdge_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::DEBUG_SRV));
        } else if (g_smaa_settings.debugOutput == 2) {
            device->CreateShaderResourceView(m_SMAABlend_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::DEBUG_SRV));
        } else if (g_smaa_settings.debugOutput == 3) {
            device->CreateShaderResourceView(m_areaTex_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::DEBUG_SRV));
        } else if (g_smaa_settings.debugOutput == 4) {
            device->CreateShaderResourceView(m_searchTex_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::DEBUG_SRV));
        } else {
            device->CreateShaderResourceView(m_depthStencilBuffer_linear_resource.Get(), nullptr, m_srv_heap->GetCpuHandle((UINT)SRV::DEBUG_SRV));
        }

        commandList->DrawInstanced(6, 1, 0, 0);

    }

    D3D12_RESOURCE_BARRIER barriers[1]
        {
            CD3DX12_RESOURCE_BARRIER::Transition(backbuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT),
        };
    commandList->ResourceBarrier(1,barriers);
}
