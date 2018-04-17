//
// Created by wang on 18-4-1.
//

#ifndef FLYINGPROGRESSCONTROL_MOTION_H
#define FLYINGPROGRESSCONTROL_MOTION_H


#include <cmath>
#include <vector>
#include "List.h"

namespace CMotion
{

    class vector3d
    {
    public:
        vector3d() = default;
        vector3d(double x, double y, double z)
        :x(x), y(y), z(z) {}
        ~vector3d() = default;
        double sqr();
        double amp();
        double x = 0;
        double y = 0;
        double z = 0;
    };
    vector3d operator+(const vector3d& A, const vector3d& B);
    vector3d operator-(const vector3d& A, const vector3d& B);
    vector3d operator*(const vector3d& A, double dt);
    vector3d operator/(const vector3d& A, double dt);
    double pointPro(const vector3d& A, const vector3d& B);
    vector3d crossPro(const vector3d& A, const vector3d& B);



    class status
    {
    public:
        status() = default;
        status(
                vector3d p,
                vector3d v,
                vector3d a,
                double t
        ) :  p(p), v(v), a(a), time(t) {}
        ~status()= default;
        vector3d p;        // position {x, y, z}
        vector3d v;        // velocity {vx, vy, vz}
        vector3d a;    // acceleration {ax, ay, az}
        double time = 0;              // time
    };

    class Motion
    {
    public:
        Motion()= default;
        explicit Motion(const status& original_status, double dt=0.1)
        {
            this->_dt = dt;
            this->_status.push_back(original_status);
        }
        ~Motion() = default;
        const CList::rank getSize() const
        { return static_cast<const CList::rank>(_status.size()); }
        const double getDt() const
        { return _dt; }
        const std::vector<status>& getAllStatus() const
        { return _status; }
        const status& getStatus() const;
        const status& getStatus(double time) const;
        const status& setAcceleration(status s, vector3d a);
        const status& appendMotion(const std::vector<vector3d>& a_list);
        const status& appendMotion(const CList::List<vector3d>& a_list);
        const status& appendMotion(vector3d last_a);
        const status& insertMotion(double time, const std::vector<vector3d>& a_list);
    protected:
        const status& updateMotion(double t);
        status move(const status& last);
    private:
        double _dt = 0.1;
        std::vector<status> _status;
    };


}

#endif //FLYINGPROGRESSCONTROL_MOTION_H
