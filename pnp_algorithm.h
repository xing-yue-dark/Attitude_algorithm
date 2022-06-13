#include<iostream>
#include<dirent.h>
#include<unistd.h>
#include<opencv2/opencv.hpp>
#include<vector>
//findChessboardCorners 提取角点

class MyPnp
{
private:
    cv::Mat cameramatrix;
    cv::Mat distCoeffs;
    std::vector<cv::Mat>rvecs;
    std::vector<cv::Mat>tvecs;
public:
    bool getPths(std::string& name_Pth,std::vector<std::string>&image_pth);
    bool findCorners(std::vector<std::string>&image_pth,cv::Size& size,cv::Size& squareSize);
    bool saveCameraMatrix(std::string& name_pth);
    bool loadCameraMatrix(std::string& name_pth);
    bool pnpfind(std::vector<std::string>&image_pth,cv::Size& size,cv::Size& squareSize);
    bool transform2Dto3D(std::vector<cv::Point2f>&imagesPoints,std::vector<cv::Point3f>&objectPoints);
};