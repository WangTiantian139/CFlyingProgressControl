//
// Created by wang on 18-4-1.
//

#ifndef FLYINGPROGRESSCONTROL_FLYINGPROGRESS_H
#define FLYINGPROGRESSCONTROL_FLYINGPROGRESS_H

#include "Motion.h"
#include <fstream>
#include <iostream>
#include <iomanip>

namespace CMotion
{
    using namespace CList;
    class FlyingProgress
    {
    public:
        FlyingProgress() = default;
        explicit FlyingProgress(const std::vector<vector3d>& a_list, double dt = 0.1);
        FlyingProgress(const std::vector<int>& motion_list, double dt = 0.1);
        ~FlyingProgress()= default;
        rank getSize()
        { return _lst_acceleration.size(); }
    // Basic movement
    rank addMotionAsList(double start_time, const std::vector<vector3d>& a_list);
        void addMotionAsList(const std::vector<vector3d>& a_list);
        rank addStaticVMotion(double start_time, double lasting_time);
        void addStaticVMotion(double lasting_time);
        rank addStaticAMotion(double start_time, double lasting_time, vector3d acceleration);
        void addStaticAMotion(double lasting_time, vector3d acceleration);
        // Template movement
        rank takeOffA(double start_time, double height, double acceleration_z = 10);
        void takeOffB(double height, double acceleration_z = 10);
        rank touchDownA(double start_time, double height, double acceleration_z = 10);
        void touchDownB(double height, double acceleration_z = 10);
        void turnLeft(status& s, double R, Motion& motion, double degree = 90, bool turn_left = true);
        void turnRight(status& s, double R, Motion& motion, double degree = 90);
        void rollOver(status& s, double R, Motion& motion);
        void inputCmd(std::ifstream& in);
        void inputCmd();

        void createMotionByCmd(Motion& motion);
        void createMotionByAcc(Motion& motion);
        void showMotionInFile(const Motion& motion, std::ofstream& out);
        void showAList(std::ofstream& out);

    protected:
        ListNodePosi<vector3d> addMotionByAccPiece(ListNodePosi<vector3d> p_lst_a, Motion& motion);
        void printCsv(std::ofstream& out, const status& status1);
        std::vector<vector3d> toVector(double start_time);

    private:
        double _dt = 0.1;
        List<vector3d> _lst_acceleration;
        List<int> _lst_command;

    };
}

#endif //FLYINGPROGRESSCONTROL_FLYINGPROGRESS_H
