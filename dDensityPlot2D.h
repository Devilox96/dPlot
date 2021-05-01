//
// Created by devilox on 5/1/21.
//
//-----------------------------//
#include <array>
#include <vector>
#include <iostream>
//-----------------------------//
#include "dPlotMeshBase.h"
//-----------------------------//
#ifndef DPLOT_DDENSITYPLOT2D_H
#define DPLOT_DDENSITYPLOT2D_H
//-----------------------------//
template <size_t Horiz, size_t Vert>
class dDensityPlot2D : public dPlotMeshBase {
public:
    dDensityPlot2D(float tWidth,                            float tHeight,
                   VkPhysicalDevice tGPU,                   VkDevice tLogicalGPU,
                   VkQueue tTransferQueue,                  VkCommandPool tTransferCommandPool) {
        mGPU            = tGPU;
        mLogicalGPU     = tLogicalGPU;
        mTransferQueue  = tTransferQueue;
        mCommandPool    = tTransferCommandPool;

        //----------//

        for (size_t iRow = 0; iRow < Vert; iRow++) {
            for (size_t iColumn = 0; iColumn < Horiz; iColumn++) {
                mVertices[iRow * Horiz + iColumn].mPos[0]       = tWidth / float(Horiz - 1) * float(iColumn);               //---x---//
                mVertices[iRow * Horiz + iColumn].mPos[1]       = tHeight / float(Vert - 1) * float(iRow);                  //---y---//
                mVertices[iRow * Horiz + iColumn].mPos[2]       = 0.0f;                                                 //---z---//

                mVertices[iRow * Horiz + iColumn].mColor[0]     = 1.0f;                                                 //---red---//
                mVertices[iRow * Horiz + iColumn].mColor[1]     = 1.0f;                                                 //---green---//
                mVertices[iRow * Horiz + iColumn].mColor[2]     = 1.0f;                                                 //---blue---//
            }
        }

        for (size_t iRow = 0; iRow < Vert - 1; iRow++) {
            for (size_t iColumn = 0; iColumn < Horiz - 1; iColumn++) {
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6] = iRow * Horiz + iColumn;
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 1] = iRow * Horiz + iColumn + 1;
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 2] = (iRow + 1) * Horiz + iColumn;

                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 3] = (iRow + 1) * Horiz + iColumn;
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 4] = iRow * Horiz + iColumn + 1;
                mIndices[(iRow * (Horiz - 1) + iColumn) * 6 + 5] = (iRow + 1) * Horiz + iColumn + 1;
            }
        }
    }

    //----------//

    void addColor(float tVal, float tRed, float tGreen, float tBlue) {
        mGradient.emplace_back({tVal, tRed, tGreen, tBlue});
    }

    void generateBuffers() {
        createVertexBuffer(mTransferQueue, mCommandPool, mVertices.data(), mVertices.size());
        createIndexBuffer(mTransferQueue, mCommandPool, mIndices.data(), mIndices.size());
    }

    //----------//

    size_t getVertexCount() {
        return mVertices.size();
    }
    size_t getIndexCount() {
        return mIndices.size();
    }
private:
    struct GradientPoint {
        float mVal;
        float mColor[3];
    };

    //----------//

    std::array <std::array <float, Horiz>, Vert>                mMesh;

    std::array <Vertex, Horiz * Vert>                           mVertices;
    std::array <uint32_t, (Horiz - 1) * (Vert - 1) * 3 * 2>     mIndices;                                               //---(n-1 * (n-1) quads that consist of 2 triangles drawn with 6 indices---//

    std::vector <GradientPoint>                                 mGradient;
};
//-----------------------------//
#endif
