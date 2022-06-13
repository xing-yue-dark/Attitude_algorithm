#include"pnp_algorithm.h"




//获取路径的所有文件名
bool MyPnp::getPths(std::string& name_Pth,std::vector<std::string>&image_pth)
{
    DIR* a;
    dirent* b;
    if((a = opendir(name_Pth.c_str()))==nullptr)
    {
        return false;
    }
    while(( b = readdir(a))!=nullptr)
    {
        if(b->d_name[0]!='.')
            image_pth.push_back(b->d_name);
    }
    return true;
}
//标定板内焦点数量，棋盘每个方格的真实尺寸
bool MyPnp::findCorners(std::vector<std::string>&image_pth,cv::Size& size,cv::Size& squareSize)
{
    //读取图片
    std::vector<cv::Mat>images;
    for(int i =0 ;i<static_cast<int>(image_pth.size());i++)
    {
        std::string image_name = image_pth[i];
        cv::Mat image = cv::imread(image_name);
        images.push_back(image);
    }

    //提取角点
    std::vector<std::vector<cv::Point2f>> imgsPoints;
    for(int i =0 ;i<static_cast<int>(images.size());i++)
    {
        cv::Mat imgs = images[i];
        cv::Mat gray;
        std::vector<cv::Point2f> imgsp;
        cv::cvtColor(imgs,gray,cv::COLOR_BGR2GRAY);
        auto a = cv::findChessboardCorners(gray,size,imgsp,cv::CALIB_CB_ADAPTIVE_THRESH+cv::CALIB_CB_NORMALIZE_IMAGE);
        cv::find4QuadCornerSubpix(gray,imgsp,cv::Size(5,5));
        imgsPoints.push_back(imgsp);
    }

    //生成内角点的空间三维坐标
    std::vector<std::vector<cv::Point3f>> objectPoints;
    for(int i =0;i<static_cast<int>(images.size());i++)
    {
        std::vector<cv::Point3f> te;
        for(int o = 0; o<size.height;o++)
        {
            for(int p=0;p<size.width;p++)
            {
                cv::Point3f realpoint;
                realpoint.x = o*squareSize.width;
                realpoint.y = p*squareSize.height;
                realpoint.z = 0;
                te.push_back(realpoint);
            }
        }
        objectPoints.push_back(te);
    }

    //假设图片能看到所有角点（角点数量记录）
    std::vector<int>point_number;
    for(int i=0;i<static_cast<int>(images.size());i++)
    {
        point_number.push_back(size.height*size.width);
    }
    //内参矩阵
    cameramatrix = cv::Mat(3,3,CV_32FC1,cv::Scalar::all(0));

    distCoeffs = cv::Mat(1,5,CV_32FC1,cv::Scalar::all(0));
    cv::Size imgsize;
    imgsize.width = images[0].cols;
    imgsize.height = images[0].rows;
    

    cv::calibrateCamera(objectPoints,imgsPoints,imgsize,cameramatrix,distCoeffs,rvecs,tvecs,0);

    return true;



}
bool MyPnp::saveCameraMatrix(std::string& name_pth)
{
    std::string name = "CameraMatrix";
    std::string name1 = "distCoeffs";
    cv::FileStorage file(name_pth,cv::FileStorage::WRITE);
    if(file.isOpened()==false)
    {
        return false;
    }
    file.write(name,cameramatrix);
    file.write(name1,distCoeffs);
    file.release();
    return true;
}
bool MyPnp::loadCameraMatrix(std::string& name_pth)
{
    
    cv::FileStorage file(name_pth,cv::FileStorage::READ);
    if(file.isOpened()==false)
    {
        return false;
    }
    
    file["CameraMatrix"]>>cameramatrix;
    file["distCoeffs"]>>distCoeffs;
    file.release();
    return true;
}

bool MyPnp::pnpfind(std::vector<std::string>&image_pth,cv::Size& size,cv::Size& squareSize)
{

        //读取图片
    std::vector<cv::Mat>images;
    for(int i =0 ;i<static_cast<int>(image_pth.size());i++)
    {
        std::string image_name = image_pth[i];
        cv::Mat image = cv::imread(image_name);
        images.push_back(image);
    }

    //提取角点
    std::vector<std::vector<cv::Point2f>> imgsPoints;
    for(int i =0 ;i<static_cast<int>(images.size());i++)
    {
        cv::Mat imgs = images[i];
        cv::Mat gray;
        std::vector<cv::Point2f> imgsp;
        cv::cvtColor(imgs,gray,cv::COLOR_BGR2GRAY);
        auto a = cv::findChessboardCorners(gray,size,imgsp,cv::CALIB_CB_ADAPTIVE_THRESH+cv::CALIB_CB_NORMALIZE_IMAGE);
        cv::find4QuadCornerSubpix(gray,imgsp,cv::Size(5,5));
        imgsPoints.push_back(imgsp);
    }

    //生成内角点的空间三维坐标
    std::vector<std::vector<cv::Point3f>> objectPoints;
    for(int i =0;i<static_cast<int>(images.size());i++)
    {
        std::vector<cv::Point3f> te;
        for(int o = 0; o<size.height;o++)
        {
            for(int p=0;p<size.width;p++)
            {
                cv::Point3f realpoint;
                realpoint.x = o*squareSize.width;
                realpoint.y = p*squareSize.height;
                realpoint.z = 0;
                te.push_back(realpoint);
            }
        }
        objectPoints.push_back(te);
    }
    for(int i=0;i<static_cast<int>(images.size());i++)
    {
        cv::Mat rvec,tvec;
        cv::solvePnP(objectPoints[i],imgsPoints[i],cameramatrix,distCoeffs,rvec,tvec);
        rvecs.push_back(rvec);
        tvecs.push_back(tvec);
    }
    
    return true;
}

//应该怎么写。。。
bool MyPnp::transform2Dto3D(std::vector<cv::Point2f>&imagesPoints,std::vector<cv::Point3f>&objectPoints)
{
    for(int i = 0;i<static_cast<int>(imagesPoints.size());i++)
    {
        cv::Mat imagepoint(cv::Size(1,3),CV_32FC1,cv::Scalar(imagesPoints[i].x,imagesPoints[i].y,1));
        cv::Mat rvec;
        cv::Rodrigues(rvecs[0],rvec);
        auto point3 = (cameramatrix*rvec*imagepoint)+tvecs[0];
        //cv::Point3f point(*point3.data,*point3.data+1,*point3.data+2);
        auto objectpoint = point3.c;
        objectPoints.push_back(cv::Point3f(*objectpoint.data,*(objectpoint.data+1),*(objectpoint.data+2)));
    }
    return true;
}