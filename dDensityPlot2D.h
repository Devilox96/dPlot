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
///---TODO: make vertex buffer larger and use offsets for triple buffering---///
class dDensityPlot2D : public dPlotMeshBase {
public:
    dDensityPlot2D(float tPosX,     float tPosY,    float tPosZ,
                   float tWidth,    float tHeight,
                   size_t tHoriz,   size_t tVert);

    //----------//

    void generateBuffers() override;
    ///---TODO: add size check---///
    void updateValues(const std::vector <std::vector <float>>& tNewData, size_t iNum);
private:

    //----------//

    size_t                                      mHoriz;
    size_t                                      mVert;

    std::vector <std::vector <float>>           mMesh;
};
//-----------------------------//
#endif
