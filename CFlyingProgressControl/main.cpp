#include <iostream>
#include "FlyingProgress.h"

using namespace CMotion;

int main()
{
    //testList();
    //testMotion();
    //testFlyingProgress();

    std::string file_name = "FlyCmdIn.txt";
    std::ifstream fin(file_name);
    std::ofstream fout;
    if (not fin)
    {
        std::cerr << "open " << file_name << " failed" << std::endl;
    }

    std::vector<vector3d> ori_a_list;
    double dt = 0.01;
    FlyingProgress progress(ori_a_list, dt);
    status ori_status;
    Motion motion1(ori_status, dt);
    Motion motion2(ori_status, dt);
    progress.inputCmd(fin);
    fin.close();

    // Create a serial of motion by command
    progress.createMotionByCmd(motion1);
    file_name = "FlyByCmdOut.csv";
    fout.open(file_name);
    if (not fout)
    {
        std::cerr << "open " << file_name << " failed" << std::endl;
    }
    fout.clear();
    progress.showMotionInFile(motion1, fout);
    fout.close();

    file_name = "FlyPlusOut.csv";
    fout.open(file_name);
    if (not fout)
    {
        std::cerr << "open " << file_name << " failed" << std::endl;
    }
    fout.clear();

    // Add a piece of motion and update it
    // progress.showAList(fout);
    progress.addStaticAMotion(3, 1, {0, 0, 20});
    progress.addStaticAMotion(10, 1, {0, 0, -20});
    progress.touchDownB(motion2.getStatus().p.z, 10);
    progress.createMotionByAcc(motion2);
    progress.showMotionInFile(motion2, fout);
    fout.close();


    return 0;
}
