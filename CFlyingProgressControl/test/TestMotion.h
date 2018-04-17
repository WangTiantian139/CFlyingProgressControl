//
// Created by wang on 18-4-7.
//

#ifndef FLYINGPROGRESSCONTROL_TESTMOTION_H
#define FLYINGPROGRESSCONTROL_TESTMOTION_H

#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../Motion.h"

void testMotion();
void testSetAcceleration(std::ofstream& out, CMotion::Motion& test_motion);
void testMove(std::ofstream& out, CMotion::Motion& test_motion);
void testInsertMotion(std::ofstream& out, CMotion::Motion& test_motion);
void testGetStatus(std::ofstream& out, CMotion::Motion& test_motion);
void printCsv(std::ofstream& out, double time, CMotion::status status1);

#endif //FLYINGPROGRESSCONTROL_TESTMOTION_H
