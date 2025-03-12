#ifndef ALGOBASE_H
#define ALGOBASE_H

// STD
#include <vector>
#include <string>

// OpenCV
#include <opencv2/opencv.hpp>

// Macro
#include "exportmacro.hpp"


class VISION_ALGO_API AlgoBase
{
public:
    cv::Mat AddMat(cv::Mat mat1, cv::Mat mat2);
    AlgoBase();    
};

#endif
