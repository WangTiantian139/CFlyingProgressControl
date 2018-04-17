//
// Created by wang on 18-4-1.
//


#include "FlyingProgress.h"

double pi = 3.141592654;

namespace CMotion
{
    FlyingProgress::FlyingProgress(const std::vector<vector3d>& a_list, double dt)
    {
        for (const auto& i:a_list)
        {
            _lst_acceleration.append(i);
        }
        _dt = dt;
    }
    FlyingProgress::FlyingProgress(const std::vector<int>& motion_list, double dt)
    {
        for (const auto& motion:motion_list)
        {
            _lst_command.append(motion);
        }
        _dt = dt;
    }
    void FlyingProgress::addMotionAsList(const std::vector<vector3d>& a_list)
    {
        for (const auto& i:a_list)
        {
            _lst_acceleration.append(i);
        }
    }
    rank FlyingProgress::addMotionAsList(double start_time, const std::vector<vector3d>& a_list)
    {
        rank r_start = static_cast<rank>(floor(start_time / _dt));
        for (const auto& i:a_list)
        {
            _lst_acceleration.insertAsSucc(r_start++, i);
        }
        return r_start;
    }
    void FlyingProgress::addStaticVMotion(double lasting_time)
    {
        addStaticAMotion(lasting_time, {0, 0, 0});
    }

    rank FlyingProgress::addStaticVMotion(double start_time, double lasting_time)
    {
        return addStaticAMotion(start_time, lasting_time, {0, 0, 0});
    }
    void FlyingProgress::addStaticAMotion(double lasting_time, vector3d acceleration)
    {
        rank r_lasting = static_cast<rank>(floor(lasting_time / _dt));
        for (rank r = 0; r < r_lasting; r++)
        {
            _lst_acceleration.append(acceleration);
        }
    }
    rank FlyingProgress::addStaticAMotion(double start_time, double lasting_time, vector3d acceleration)
    {
        rank r_start = static_cast<rank>(floor(start_time / _dt));
        rank r_lasting = static_cast<rank>(ceil(lasting_time / _dt));
        for (rank r = 0; r < r_lasting; r++)
        {
            _lst_acceleration.insertAsSucc(r+r_start, acceleration);
        }
        return r_start;
    }

    /*
     * Using the vector crossing product, for vectors v = w x p, we obtain
     * |v| = |w| * |p| * sin<w, p>, where sin<w, p> = cos<a, p> and
     * cos<a, p> = unit_a * p / |p|
     * Such that, vectors w = p0 x v0 / (R * |p0| * cos<a0, p0>). In that way,
     * we obtain a constant vector w, which leads to the result vector a(t) we interested
     * by the formula a = w x v. Therefore, the computation is simplified because
     * we can obtain a vector without dealing with components equations. .
     *
     * Formula:
     * | w = (v0 x a0) / (|v0| * |a0|);
     * | a = w x v;
     * | cos<a0, p0> = a0 * p0 / |p|;
     * | a0 * v = 0;
     * | |a0|^2 = 1;
     * | for a0 is an unit vector;
     *  For turing left/right:
     *  We assume a0z = 0, by which we obtain the constant vector w.
     *  Then we do once vector product each turn in the loop.
     *  For the bool left:
     *  If the bool is True, the point will turn left, or the point
     *  will turn right. The difference between the selections is
     *  the direction of a0.
     */
    void FlyingProgress::turnLeft(status& s, double R, Motion& motion, double degree, bool turn_left)
    {
        vector3d a_current;
        status s_cur = s;
        Motion m_temp(s_cur, _dt);
        vector3d unit_a0, w;
        double v0x2_plus_v0y2, vx2, vy2;
        v0x2_plus_v0y2 = pow(s.v.x, 2) + pow(s.v.y, 2);

        vx2 = pow(s.v.x, 2);
        vy2 = pow(s.v.y, 2);
        unit_a0.x = sqrt(vy2) / (sqrt(v0x2_plus_v0y2) + 1e-10);
        unit_a0.y = sqrt(vx2) / (sqrt(v0x2_plus_v0y2) + 1e-10);
        unit_a0.z = 0;
        if (s.v.x > 0 and s.v.y >= 0)
        {
            unit_a0.x *= -1;
            unit_a0.y *= 1;
        } else if (s.v.x <= 0 and s.v.y > 0)
        {
            unit_a0.x *= -1;
            unit_a0.y *= -1;
        } else if (s.v.x < 0 and s.v.y <= 0)
        {
            unit_a0.x *= 1;
            unit_a0.y *= -1;
        } else
        {
            unit_a0.x *= 1;
            unit_a0.y *= 1;
        }
        if (!turn_left)
        {
            unit_a0.x *= -1;
            unit_a0.y *= -1;
        }

        double deg_current = 0;
        degree = fabs(degree);
        w = crossPro(s_cur.v, unit_a0) / R;
        while (deg_current < degree * 1.005)
        {
            a_current = crossPro(w, s_cur.v);
            _lst_acceleration.append(a_current);
            s_cur = m_temp.appendMotion(a_current);
            deg_current += 180 * s.v.amp() * _dt / (R * pi);
        }
    }
    void FlyingProgress::turnRight(status& s, double R, Motion& motion, double degree)
    {
        turnLeft(s, R, motion, degree, false);
    }

    /*
     *  Formula
     *  | ax / ay = vx / vy;
     *  | ax^2 + ay^2 + az^2 = (v^2 / R)^2;
     *  | ax * vx + ay * vy + az * vz = 0;
     *   For rolling over:
     *   We assume that ax and ay are 0.
     *   Such that, with only  variable az, it comes easier for us
     *   to obtain the current vector a.
     */
    void FlyingProgress::rollOver(status& s_ori, double R, Motion& motion)
    {
        vector3d a_current;
        Motion m_temp(s_ori, _dt);
        status s(s_ori);
        double v2 = s.v.sqr();
        double deg = 0;
        while(deg < 358.6)
        {
            double ax2, ay2, az2, vx2_plus_vy2;
            vx2_plus_vy2 = pow(s.v.x, 2) + pow(s.v.y, 2);
            ax2 = v2 * pow(s.v.x, 2) * pow(s.v.z, 2) / (vx2_plus_vy2 * R * R + 1e-10);
            ay2 = v2 * pow(s.v.y, 2) * pow(s.v.z, 2) / (vx2_plus_vy2 * R * R + 1e-10);
            az2 = pow(v2 / R, 2) - ax2 - ay2;
            if (deg < 180 / 2 or deg > 1.5 * 180)
            {
                a_current.z = sqrt(az2);
            } else
            {
                a_current.z = -sqrt(az2);
            }
            if (deg < 180)
            {
                a_current.x = -sqrt(ax2);
                a_current.y = -sqrt(ay2);
            } else
            {
                a_current.x = sqrt(ax2);
                a_current.y = sqrt(ay2);
            }
            _lst_acceleration.append(a_current);
            s = m_temp.appendMotion(a_current);
            v2 = s.v.sqr();
            deg += 180 * s.v.amp() * _dt / (R * pi);
        }

    }
    /*
     *  Take Off
     *  Make sure the current velocity.z = 0, otherwise you cannot obtain the correct height
     *  The acceleration a is a constant number 10
     *  Formula
     *  | 0.5 * a * t^2 = 0.5 * height
     *
     */
    rank FlyingProgress::takeOffA(double start_time, double height, double acceleration_z)
    {
        rank r_start = static_cast<rank>(floor(start_time / _dt));
        double lasting_time = 2 * sqrt(height / acceleration_z);
        rank r_lasting = static_cast<rank>(floor(lasting_time / _dt));
        rank r = r_start;
        for (; r < r_lasting / 2; r++)
        {
            _lst_acceleration.insertAsSucc(r, {0, 0, acceleration_z});
        }
        for (; r < (r_lasting / 2) * 2; r++)
        {
            _lst_acceleration.insertAsSucc(r, {0, 0, - acceleration_z});
        }
        return  r_start;
    }
    void FlyingProgress::takeOffB(double height, double acceleration_z)
    {
        double lasting_time = 2 * sqrt(height / fabs(acceleration_z));
        auto r_lasting = static_cast<rank>(floor(lasting_time / _dt));
        rank r = 0;
        for (; r < r_lasting / 2; r++)
        {
            _lst_acceleration.append({0, 0, acceleration_z});
        }
        for (; r < r_lasting - 2; r++)
        {
            _lst_acceleration.append({0, 0, (-acceleration_z)});
        }
    }
    rank FlyingProgress::touchDownA(double start_time, double height, double acceleration_z)
    {
        return takeOffA(start_time, height, - acceleration_z);
    }

    void FlyingProgress::touchDownB(double height, double acceleration_z)
    {
        takeOffB(height, - acceleration_z);
    }
    // Motion Operation
    void FlyingProgress::createMotionByAcc(Motion& motion)
    {
        motion.appendMotion(_lst_acceleration);
    }
    void FlyingProgress::createMotionByCmd(Motion& motion)
    {
        _lst_acceleration.insertAsFirst({0, 0, 0});
        auto p_lst_a = _lst_acceleration.begin();
        for (auto p_motion = _lst_command.begin(); p_motion != _lst_command.end(); p_motion = p_motion->succ)
        {
            if (0 == p_motion->data)
            {
                addStaticAMotion(1, {10, 0, 0});
            } else if (1 == p_motion->data)
            {
                addStaticVMotion(1);
            } else if (2 == p_motion->data)
            {
                takeOffB(20);
            } else if (3 == p_motion->data)
            {
                touchDownB(motion.getStatus().p.z);
            } else if (4 == p_motion->data)
            {
                rollOver(const_cast<status&>(motion.getStatus()), 20, motion);
            } else if (5 == p_motion->data)
            {
                turnLeft(const_cast<status&>(motion.getStatus()), 20, motion);
            } else if (6 == p_motion->data)
            {
                turnRight(const_cast<status&>(motion.getStatus()), 20, motion);
            } else
            {
                continue;
            }
            if(p_lst_a==_lst_acceleration.end()){}
            {
                p_lst_a = p_lst_a->pred;
            }
            p_lst_a = addMotionByAccPiece(p_lst_a, motion);
        }
    }
    ListNodePosi<vector3d> FlyingProgress::addMotionByAccPiece(ListNodePosi<vector3d> p, Motion& motion)
    {
        if(p==_lst_acceleration.end())
        {
            p = p->pred;
        }
        for (; p != _lst_acceleration.end(); p = p->succ)
        {
            motion.appendMotion(p->data);
        }
        p = _lst_acceleration.last();
        return p;
    }

    std::vector<vector3d> FlyingProgress::toVector(double start_time)
    {
        std::vector<vector3d> vec_a_list;
        auto r_start = static_cast<rank>(floor(start_time / _dt));
        auto p_a = _lst_acceleration.begin();
        for (rank r = 0; p_a != _lst_acceleration.end() and r < r_start; p_a = p_a->succ, ++r)
        { }
        for (; p_a != _lst_acceleration.end(); p_a = p_a->succ)
        {
            vec_a_list.push_back(p_a->data);
        }
        return vec_a_list;
    }

    // File method
    void FlyingProgress::showMotionInFile(const Motion& motion, std::ofstream& out)
    {
        using namespace std;
        if (not out)
        {
            cerr<<"open outfile failed." << std::endl;
        }
        // print as CSV format
        out << "t,px,py,pz,vx,vy,vz,ax,ay,az" << std::endl;

        for (const auto& i : motion.getAllStatus())
        {
            printCsv(out, i);
        }
    }
    void FlyingProgress::printCsv(std::ofstream& out, const  status& status1)
    {
        using namespace std;
        out << fixed << setprecision(3) ;
        out << status1.time << ",";
        out << status1.p.x << ","
            << status1.p.y << ","
            << status1.p.z << ",";
        out << status1.v.x << ","
            << status1.v.y << ","
            << status1.v.z << ",";
        out << status1.a.x << ","
            << status1.a.y << ","
            << status1.a.z <<endl;
    }
    void FlyingProgress::inputCmd(std::ifstream& in)
    {
        using namespace std;
        int n_cmd;
        while (in >> n_cmd)
        {
            _lst_command.append(n_cmd);
        }
    }
    void FlyingProgress::showAList(std::ofstream& out)
    {
        int index = 0;
        for (auto i = _lst_acceleration.begin(); i != _lst_acceleration.end(); i = i->succ)
        {
            out << index << '\t' << i->data.x << ',' << i->data.y << ',' << i->data.z << std::endl;
            index++;
        }
    }
}
