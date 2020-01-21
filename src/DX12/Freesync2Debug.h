// AMD AMDUtils code
//
// Copyright(c) 2019 Advanced Micro Devices, Inc.All rights reserved.
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include "PostProc\PostProcPS.h"

namespace CAULDRON_DX12
{
    class Freesync2Debug
    {
    public:
        void OnCreate(Device* pDevice, UploadHeap* pUploadHeap, ResourceViewHeaps* pResourceViewHeaps, StaticBufferPool* pStaticBufferPool, DynamicBufferRing* pDynamicBufferRing, DXGI_FORMAT outFormat);
        void OnDestroy();
        void Draw(ID3D12GraphicsCommandList* pCommandList, int testPattern);

    private:
        DynamicBufferRing *m_pDynamicBufferRing = NULL;
        CBV_SRV_UAV m_FS2DebugTextureSRV[2];
        Texture m_FS2DebugTexture[2];
        PostProcPS m_FS2DebugPS;

        struct Freesync2DebugConsts
        {
            float texturePeakLumaPerNits;
        };

        std::vector<std::string> m_TextureNameList = {"..\\media\\color_ramp_bt2020_dcip3\\dcip3_1000_EXR_ARGB_16F_1.DDS",
                                                      "..\\media\\color_ramp_bt2020_dcip3\\bt2020_1000_EXR_ARGB_16F_1.DDS"};
        std::vector<float> m_LumaList = { 1000.0f, 1000.0f };
	};
}