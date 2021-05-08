//
// Created by devilox on 5/1/21.
//
//-----------------------------//
#include <array>
#include <iostream>
#include <random>
//-----------------------------//
#include "dPlotMeshBase.h"
//-----------------------------//
#ifndef DPLOT_DDENSITYPLOT2D_H
#define DPLOT_DDENSITYPLOT2D_H
//-----------------------------//
class dDensityPlot2D : public dPlotMeshBase {
public:
    dDensityPlot2D(float tPosX,                             float tPosY,                            float tPosZ,
                   float tWidth,                            float tHeight,
                   size_t tHoriz,                           size_t tVert) {
        mHoriz          = tHoriz;
        mVert           = tVert;

        //----------//

        mMesh.resize(tVert);

        for (auto& iRow: mMesh) {
            iRow.resize(tHoriz);
        }

        mVertices.resize(tHoriz * tVert);
        mIndices.resize((tHoriz - 1) * (tVert - 1) * 3 * 2);                                                            //---(n-1 * (n-1) quads that consist of 2 triangles drawn with 6 indices---//

        //----------//

        for (size_t iRow = 0; iRow < tVert; iRow++) {
            for (size_t iColumn = 0; iColumn < tHoriz; iColumn++) {
                mVertices[iRow * tHoriz + iColumn].mPos[0]       = tWidth / float(tHoriz - 1) * float(iColumn) + tPosX; //---x---//
                mVertices[iRow * tHoriz + iColumn].mPos[1]       = tHeight / float(tVert - 1) * float(iRow) + tPosY;    //---y---//
                mVertices[iRow * tHoriz + iColumn].mPos[2]       = tPosZ;                                               //---z---//

                mVertices[iRow * tHoriz + iColumn].mColor[0]     = 1.0f;                                                //---red---//
                mVertices[iRow * tHoriz + iColumn].mColor[1]     = 1.0f;                                                //---green---//
                mVertices[iRow * tHoriz + iColumn].mColor[2]     = 1.0f;                                                //---blue---//

                mMesh[iRow][iColumn] = 0.0f;
            }
        }

        for (size_t iRow = 0; iRow < tVert - 1; iRow++) {
            for (size_t iColumn = 0; iColumn < tHoriz - 1; iColumn++) {
                mIndices[(iRow * (tHoriz - 1) + iColumn) * 6] = iRow * tHoriz + iColumn;
                mIndices[(iRow * (tHoriz - 1) + iColumn) * 6 + 1] = iRow * tHoriz + iColumn + 1;
                mIndices[(iRow * (tHoriz - 1) + iColumn) * 6 + 2] = (iRow + 1) * tHoriz + iColumn;

                mIndices[(iRow * (tHoriz - 1) + iColumn) * 6 + 3] = (iRow + 1) * tHoriz + iColumn;
                mIndices[(iRow * (tHoriz - 1) + iColumn) * 6 + 4] = iRow * tHoriz + iColumn + 1;
                mIndices[(iRow * (tHoriz - 1) + iColumn) * 6 + 5] = (iRow + 1) * tHoriz + iColumn + 1;
            }
        }
    }

    //----------//

    void generateBuffers() override {
        if (!mGradient.empty()) {
            for (size_t iRow = 0; iRow < mVert; iRow++) {
                for (size_t iColumn = 0; iColumn < mHoriz; iColumn++) {
                    if (mMesh[iRow][iColumn] >= mGradient.back().mVal) {
                        mVertices[iRow * mHoriz + iColumn].mColor[0] = mGradient.back().mColor[0];
                        mVertices[iRow * mHoriz + iColumn].mColor[1] = mGradient.back().mColor[1];
                        mVertices[iRow * mHoriz + iColumn].mColor[2] = mGradient.back().mColor[2];

                        continue;
                    }

                    if (mMesh[iRow][iColumn] <= mGradient.front().mVal) {
                        mVertices[iRow * mHoriz + iColumn].mColor[0] = mGradient.front().mColor[0];
                        mVertices[iRow * mHoriz + iColumn].mColor[1] = mGradient.front().mColor[1];
                        mVertices[iRow * mHoriz + iColumn].mColor[2] = mGradient.front().mColor[2];

                        continue;
                    }

                    for (size_t i = 0; i < mGradient.size() - 1; i++) {
                        if (mMesh[iRow][iColumn] > mGradient[i].mVal && mMesh[iRow][iColumn] < mGradient[i + 1].mVal) {
                            float Multiplier = (mMesh[iRow][iColumn] - mGradient[i].mVal) / (mGradient[i + 1].mVal - mGradient[i].mVal);

                            mVertices[iRow * mHoriz + iColumn].mColor[0] = mGradient[i].mColor[0] + (mGradient[i + 1].mColor[0] - mGradient[i].mColor[0]) * Multiplier;
                            mVertices[iRow * mHoriz + iColumn].mColor[1] = mGradient[i].mColor[1] + (mGradient[i + 1].mColor[1] - mGradient[i].mColor[1]) * Multiplier;
                            mVertices[iRow * mHoriz + iColumn].mColor[2] = mGradient[i].mColor[2] + (mGradient[i + 1].mColor[2] - mGradient[i].mColor[2]) * Multiplier;

                            break;
                        }
                    }
                }
            }
        }

        createVertexBuffer(mTransferQueue, mCommandPool, mVertices.data(), mVertices.size());
        createIndexBuffer(mTransferQueue, mCommandPool, mIndices.data(), mIndices.size());
    }

    //----------//



    //----------//

    ///---TODO: add size check---///
    void updateValues(const std::vector <std::vector <float>>& tNewData, size_t iNum) {
        for (size_t iRow = 0; iRow < mVert; iRow++) {
            for (size_t iColumn = 0; iColumn < mHoriz; iColumn++) {
                mMesh[iRow][iColumn] = tNewData[iColumn][iRow];
            }
        }

        if (!mGradient.empty()) {
            for (size_t iRow = 0; iRow < mVert; iRow++) {
                for (size_t iColumn = 0; iColumn < mHoriz; iColumn++) {
                    if (mMesh[iRow][iColumn] >= mGradient.back().mVal) {
                        mVertices[iRow * mHoriz + iColumn].mColor[0] = mGradient.back().mColor[0];
                        mVertices[iRow * mHoriz + iColumn].mColor[1] = mGradient.back().mColor[1];
                        mVertices[iRow * mHoriz + iColumn].mColor[2] = mGradient.back().mColor[2];

                        continue;
                    }

                    if (mMesh[iRow][iColumn] <= mGradient.front().mVal) {
                        mVertices[iRow * mHoriz + iColumn].mColor[0] = mGradient.front().mColor[0];
                        mVertices[iRow * mHoriz + iColumn].mColor[1] = mGradient.front().mColor[1];
                        mVertices[iRow * mHoriz + iColumn].mColor[2] = mGradient.front().mColor[2];

                        continue;
                    }

                    for (size_t i = 0; i < mGradient.size() - 1; i++) {
                        if (mMesh[iRow][iColumn] > mGradient[i].mVal && mMesh[iRow][iColumn] < mGradient[i + 1].mVal) {
                            float Multiplier = (mMesh[iRow][iColumn] - mGradient[i].mVal) / (mGradient[i + 1].mVal - mGradient[i].mVal);

                            mVertices[iRow * mHoriz + iColumn].mColor[0] = mGradient[i].mColor[0] + (mGradient[i + 1].mColor[0] - mGradient[i].mColor[0]) * Multiplier;
                            mVertices[iRow * mHoriz + iColumn].mColor[1] = mGradient[i].mColor[1] + (mGradient[i + 1].mColor[1] - mGradient[i].mColor[1]) * Multiplier;
                            mVertices[iRow * mHoriz + iColumn].mColor[2] = mGradient[i].mColor[2] + (mGradient[i + 1].mColor[2] - mGradient[i].mColor[2]) * Multiplier;

                            break;
                        }
                    }
                }
            }
        }

        //----------//

        VkDeviceSize    BufferSize              = sizeof(Vertex) * mVertices.size();
        VkBuffer        StagingBuffer;
        VkDeviceMemory  StagingBufferMemory;
        void*           Data;

        createBuffer(
                BufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                &StagingBuffer,
                &StagingBufferMemory
        );

        vkMapMemory(mLogicalGPU, StagingBufferMemory, 0, BufferSize, 0, &Data);
        memcpy(Data, mVertices.data(), static_cast <size_t>(BufferSize));
        vkUnmapMemory(mLogicalGPU, StagingBufferMemory);

        copyBuffer(mTransferQueue, mCommandPool, StagingBuffer, mVertexBuffers[iNum], BufferSize);

        vkDestroyBuffer(mLogicalGPU, StagingBuffer, nullptr);
        vkFreeMemory(mLogicalGPU, StagingBufferMemory, nullptr);
    }
private:

    //----------//

    size_t                                      mHoriz;
    size_t                                      mVert;

    std::vector <std::vector <float>>           mMesh;
};
//-----------------------------//
#endif
