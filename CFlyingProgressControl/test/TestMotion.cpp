//
// Created by wang on 18-4-7.
//

#include <iomanip>
#include "TestMotion.h"

using namespace CMotion;

std::vector<vector3d> a_list = {
        {1.0, 1.0, 1.0},
        {2.0, 2.0, 2.0},
        {3.0, 3.0, 3.0},
        {4.0, 4.0, 4.0},
        {5.0, 5.0, 5.0}
};

void testMotion()
{
    status ori_status;
    Motion test_motion(ori_status);

    std::ofstream out;
    std::ifstream in;
    std::string outfile_name;

    outfile_name = "../test/doc/TestMotionMoveOut.csv";
    out.open(outfile_name);
    out.clear();
    if (not out)
    {
        std::cerr<<"open "+outfile_name+" failed." << std::endl;
    }

    out << "t,px,py,pz,vx,vy,vz,ax,ay,az" << std::endl;
    status status1 = test_motion.getStatus();
    double time = (test_motion.getSize() - 1) * test_motion.getDt();
    printCsv(out, time, status1);

    testSetAcceleration(out, test_motion);
    testMove(out, test_motion);
    out.close();
    outfile_name = "../test/doc/TestMotionInsertOut.csv";
    out.open(outfile_name);
    out.clear();
    out << "t,px,py,pz,vx,vy,vz,ax,ay,az" << std::endl;
    testInsertMotion(out, test_motion);
    out.close();
}
void testSetAcceleration(std::ofstream& out, Motion& test_motion)
{
    status sta_curr = test_motion.getStatus();
    sta_curr = test_motion.setAcceleration(sta_curr, {1, 2, 3});
    printCsv(out, 0, sta_curr);
}
void testMove(std::ofstream& out, Motion& test_motion)
{
    status sta_curr = test_motion.getStatus();
    for (int i = 0; i < 10; ++i)
    {
        sta_curr = test_motion.appendMotion(sta_curr.a);
        printCsv(out, sta_curr.time, sta_curr);
    }
}
void testInsertMotion(std::ofstream& out, CMotion::Motion& test_motion)
{
    status sta_curr = test_motion.getStatus(0.5);
    test_motion.insertMotion(sta_curr.time, a_list);
    for (const auto& i:test_motion.getAllStatus())
    {
            printCsv(out, i.time, i);
    }

}
void printCsv(std::ofstream& out, double time, status status1)
{
    using namespace std;
    out << fixed << setprecision(3) ;
    out << time << "," << setw(6);
    out << status1.p.x << ","<<setw(6)
        << status1.p.y << ","<<setw(6)
        << status1.p.z << ","<<setw(6);
    out << status1.v.x << ","<<setw(6)
        << status1.v.y << ","<<setw(6)
        << status1.v.z << ","<<setw(6);
    out << status1.a.x << ","<<setw(6)
        << status1.a.y << ","<< setw(6)
        << status1.a.z <<endl;
}
