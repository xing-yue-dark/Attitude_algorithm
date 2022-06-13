#include<iostream>
#include"pnp_algorithm.h"
#include<vector>

#define CHESS_SIZE cv::Size(6,9);
#define SQUARE_SIZE cv::Size(17,17);

void dss(std::vector<std::string>& image_pth,std::string& name_pth,std::vector<std::string>& name_pths)
{
    int count{};
    for(int i=0;i<static_cast<int>(name_pths.size());i++)
    {
        std::string image_name = name_pth+"/"+name_pths[i];
        count++;
        image_pth.push_back(image_name);
    }
    std::cout<<"已载入"<<count<<"张图片"<<std::endl;
}

void run(std::vector<std::string>&nameArray)
{
    MyPnp mp;
    std::string name_pth = nameArray[0];
    std::string name_camera = nameArray[1]+"/camera.ymal";
    std::vector<std::string> name_pths;
    std::vector<std::string> image_pth;
    mp.getPths(name_pth,name_pths);
    dss(image_pth,name_pth,name_pths);
    //内角点数量是不计算外围的，计算里面的角点
    cv::Size size = CHESS_SIZE
    cv::Size square = SQUARE_SIZE
    mp.findCorners(image_pth,size,square);
    mp.saveCameraMatrix(name_camera);
}



//
int main(int argh,char*argv[])
{
    try{
        std::cout<<"以捕获到"<<argh<<"个外参"<<std::endl;
        if(argh>1)
        {
            std::vector<std::string>nameArray;
            for(int i=1;i<argh;i++)
            {
                std::string name{argv[i]};
                nameArray.push_back(name);
            }
            run(nameArray);
        }
    }
    catch(...)
    {
        std::cerr<<"出现异常"<<std::endl;
    }
    return 0;
}