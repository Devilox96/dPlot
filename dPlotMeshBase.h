//
// Created by devilox on 5/1/21.
//
//-----------------------------//
#ifndef DPLOT_DPLOTMESHBASE_H
#define DPLOT_DPLOTMESHBASE_H
//-----------------------------//
#include <cstring>
#include <stdexcept>
#include <vector>
//-----------------------------//
#include <vulkan/vulkan.h>
//-----------------------------//
class dPlotMeshBase {
public:
    struct Vertex {
        float mPos[3];
        float mColor[3];
    };

    //----------//

    dPlotMeshBase();

    //----------//

    ///---TODO: rename this somehow---///
    void setVulkanStuff(VkPhysicalDevice tGPU,                   VkDevice tLogicalGPU,
                        VkQueue tTransferQueue,                  VkCommandPool tTransferCommandPool);
    void addColor(float tVal, float tRed, float tGreen, float tBlue);

    //----------//

    virtual void generateBuffers() = 0;

    VkBuffer* getVertexBuffer(size_t iNum);
    VkBuffer* getIndexBuffer();

    size_t getVertexCount();
    size_t getIndexCount();

    void destroyBuffers();
protected:
    struct ColorPoint {
        float mVal;
        float mColor[3];
    };

    //----------//

    std::vector <VkBuffer>              mVertexBuffers;
    std::vector <VkDeviceMemory>        mVertexBuffersMemory;

    VkBuffer                            mIndexBuffer            = VK_NULL_HANDLE;
    VkDeviceMemory                      mIndexBufferMemory      = VK_NULL_HANDLE;

    VkPhysicalDevice                    mGPU                    = VK_NULL_HANDLE;
    VkDevice                            mLogicalGPU             = VK_NULL_HANDLE;

    VkQueue                             mTransferQueue          = VK_NULL_HANDLE;
    VkCommandPool                       mCommandPool            = VK_NULL_HANDLE;

    //----------//

    std::vector <Vertex>                mVertices;
    std::vector <uint32_t>              mIndices;

    std::vector <ColorPoint>            mGradient;

    //----------//

    void createVertexBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, const Vertex* tVertices, size_t tSize);
    void createIndexBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, const uint32_t* tIndices, size_t tSize);

    uint32_t findMemoryTypeIndex(uint32_t tAllowedTypes, VkMemoryPropertyFlags tFlags);     //---Fix no return---//
    void createBuffer(VkDeviceSize tBufferSize, VkBufferUsageFlags tUsageFlags, VkMemoryPropertyFlags tPropertyFlags, VkBuffer* tBuffer, VkDeviceMemory* tMemory);
    void copyBuffer(VkQueue tTransferQueue, VkCommandPool tTransferCommandPool, VkBuffer tScrBuffer, VkBuffer tDstBuffer, VkDeviceSize tBufferSize);
};
//-----------------------------//
#endif
