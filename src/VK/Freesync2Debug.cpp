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

#include "stdafx.h"
#include "Freesync2Debug.h"

namespace CAULDRON_VK
{
    void Freesync2Debug::OnCreate(Device* pDevice, VkRenderPass renderPass, UploadHeap* pUploadHeap, ResourceViewHeaps *pResourceViewHeaps, StaticBufferPool  *pStaticBufferPool, DynamicBufferRing *pDynamicBufferRing)
    {
        m_pDevice = pDevice;
        m_pResourceViewHeaps = pResourceViewHeaps;
        m_pDynamicBufferRing = pDynamicBufferRing;

        {
            VkSamplerCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            info.magFilter = VK_FILTER_LINEAR;
            info.minFilter = VK_FILTER_LINEAR;
            info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            info.minLod = -1000;
            info.maxLod = 1000;
            info.maxAnisotropy = 1.0f;
            VkResult res = vkCreateSampler(m_pDevice->GetDevice(), &info, NULL, &m_sampler);
            assert(res == VK_SUCCESS);
        }

        std::vector<VkDescriptorSetLayoutBinding> layoutBindings(2);
        layoutBindings[0].binding = 0;
        layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        layoutBindings[0].descriptorCount = 1;
        layoutBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        layoutBindings[0].pImmutableSamplers = NULL;

        layoutBindings[1].binding = 1;
        layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layoutBindings[1].descriptorCount = 1;
        layoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        layoutBindings[1].pImmutableSamplers = NULL;

        m_FS2DebugTexture[0].InitFromFile(pDevice, pUploadHeap, m_TextureNameList[0].c_str(), false);
        pUploadHeap->FlushAndFinish();
        m_FS2DebugTexture[0].CreateSRV(&m_FS2DebugTextureSRV[0]);
        m_pResourceViewHeaps->CreateDescriptorSetLayoutAndAllocDescriptorSet(&layoutBindings, &m_descriptorSetLayout[0], &m_descriptorSet[0]);
        m_pDynamicBufferRing->SetDescriptorSet(0, sizeof(Freesync2DebugConsts), m_descriptorSet[0]);
        SetDescriptorSet(m_pDevice->GetDevice(), 1, m_FS2DebugTextureSRV[0], &m_sampler, m_descriptorSet[0]);

        m_FS2DebugTexture[1].InitFromFile(pDevice, pUploadHeap, m_TextureNameList[1].c_str(), false);
        pUploadHeap->FlushAndFinish();
        m_FS2DebugTexture[1].CreateSRV(&m_FS2DebugTextureSRV[1]);
        m_pResourceViewHeaps->CreateDescriptorSetLayoutAndAllocDescriptorSet(&layoutBindings, &m_descriptorSetLayout[1], &m_descriptorSet[1]);
        m_pDynamicBufferRing->SetDescriptorSet(0, sizeof(Freesync2DebugConsts), m_descriptorSet[1]);
        SetDescriptorSet(m_pDevice->GetDevice(), 1, m_FS2DebugTextureSRV[1], &m_sampler, m_descriptorSet[1]);

        m_FS2DebugPS.OnCreate(m_pDevice, renderPass, "FS2DebugPS.glsl", pStaticBufferPool, pDynamicBufferRing, m_descriptorSetLayout[0], NULL, VK_SAMPLE_COUNT_1_BIT);
    }

    void Freesync2Debug::OnDestroy()
    {
        vkDestroyImageView(m_pDevice->GetDevice(), m_FS2DebugTextureSRV[0], NULL);
        vkDestroyImageView(m_pDevice->GetDevice(), m_FS2DebugTextureSRV[1], NULL);

        m_FS2DebugTexture[0].OnDestroy();
        m_FS2DebugTexture[1].OnDestroy();

        m_FS2DebugPS.OnDestroy();

        m_pResourceViewHeaps->FreeDescriptor(m_descriptorSet[0]);
        m_pResourceViewHeaps->FreeDescriptor(m_descriptorSet[1]);

        vkDestroySampler(m_pDevice->GetDevice(), m_sampler, nullptr);

        vkDestroyDescriptorSetLayout(m_pDevice->GetDevice(), m_descriptorSetLayout[0], NULL);
        vkDestroyDescriptorSetLayout(m_pDevice->GetDevice(), m_descriptorSetLayout[1], NULL);
	}

    void Freesync2Debug::Draw(VkCommandBuffer cmd_buf, int testPattern)
    {
        VkDescriptorBufferInfo cbFS2DebugHandle;
        Freesync2DebugConsts *pFS2DebugConsts;
        m_pDynamicBufferRing->AllocConstantBuffer(sizeof(Freesync2DebugConsts), (void **)&pFS2DebugConsts, &cbFS2DebugHandle);

        pFS2DebugConsts->texturePeakLumaPerNits = 80.0f / m_LumaList[testPattern - 1];
        m_FS2DebugPS.Draw(cmd_buf, cbFS2DebugHandle, m_descriptorSet[testPattern - 1]);
    }
}