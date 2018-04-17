//
// Created by wang on 18-4-1.
//

#include "Motion.h"

namespace CMotion
{

    vector3d operator+(const vector3d& A, const vector3d& B)
    {
        vector3d C;
        C.x = A.x + B.x;
        C.y = A.y + B.y;
        C.z = A.z + B.z;
        return C;
    }

    vector3d operator-(const vector3d& A, const vector3d& B)
    {
        vector3d C;
        C.x = A.x - B.x;
        C.y = A.y - B.y;
        C.z = A.z - B.z;
        return C;
    }

    vector3d operator*(const vector3d& A, double dt)
    {
        vector3d B;
        B.x = A.x * dt;
        B.y = A.y * dt;
        B.z = A.z * dt;
        return B;
    }

    vector3d operator/(const vector3d& A, double dt)
    {
        vector3d B;
        B.x = A.x / dt;
        B.y = A.y / dt;
        B.z = A.z / dt;
        return B;
    }
    double pointPro(const vector3d& A, const vector3d& B)
    {
        return A.x * B.x + A.y * B.y + A.z * B.z;
    }
    vector3d crossPro(const vector3d& A, const vector3d& B)
    {
        vector3d ans;
        ans.x = A.y * B.z - A.z * B.y;
        ans.y = -A.x * B.z + A.z * B.x;
        ans.z = A.x * B.y - A.y * B.x;
        return ans;
    }
    double vector3d::sqr()
    {
        return pow(x, 2) + pow(y, 2) + pow(z, 2);
    }
    double vector3d::amp()
    {
        return sqrt(this->sqr());
    }


    const status& Motion::getStatus() const
    {
        return  _status.back();
    }

    const status& Motion::getStatus(double time) const
    {
        for (auto& i : _status)
        {
            if (i.time >= time)
            {
                return i;
            }
        }
        return _status.back();
    }
    const status& Motion::setAcceleration(status s, vector3d a)
    {
        auto& target = *(_status.end()-1);
        for (auto& i:_status)
        {
            if (i.time >= s.time)
            {
                target = i;
                break;
            }
        }
        target.a = a;
        return target;
    }
    status Motion::move(const status& last)
    {
        status current;
        current.time = last.time + _dt;
        current.a = {0.0, 0.0, 0.0};
        current.v = last.v + last.a * _dt;
        current.p = last.p + last.v * _dt + last.a * (0.5 * _dt * _dt);
        return current;
    }
    const status& Motion::updateMotion(double t)
    {
        auto start = _status.end()-1;
        for (auto i = _status.begin(); i != _status.end(); ++i)
        {
            if (i->time >= t)
            {
                start = i;
                break;
            }
        }
        for (auto last = start, curr = ++start; curr != _status.end(); ++curr,++last)
        {
            last->a = curr->a;
            *curr = move(*last);
        }
        return _status.back();
    }
    const status& Motion::appendMotion(vector3d last_a)
    {
        _status.back().a = last_a;
        status sta_curr = move(_status.back());
        _status.push_back(sta_curr);
        return _status.back();
    }
    const status& Motion::appendMotion(const std::vector<vector3d>& a_list)
    {
        auto p_last = _status.end();
        --p_last;
        for (auto i : a_list)
        {
            p_last->a = i;
            _status.insert(p_last, move(*p_last));
            ++p_last;
        }
        return this->updateMotion((p_last)->time);
    }
    const status& Motion::appendMotion(const CList::List<vector3d>& a_list)
    {
        status s_current = _status.back();
        for (auto i = a_list.begin(); i != a_list.end(); i = i->succ)
        {
            s_current.a = i->data;
            _status.push_back(move(s_current));
            s_current = _status.back();
        }
    }
    const status& Motion::insertMotion(double time, const std::vector<vector3d>& a_list)
    {
        auto p_last = _status.end()-1;
        for (auto i = _status.begin(); i != _status.end(); ++i)
        {
            if (i->time >= time)
            {
                p_last = i;
                break;
            }
        }
        auto p_insertion = p_last+1;
        for (auto& i : a_list)
        {
            p_last->a = i;
            if (p_last != p_insertion)
            {
                p_last = _status.insert(p_last, move(*p_last));
            } else
            {
                break;
            }
        }
        return this->updateMotion(p_last->time);
    }
}// namespace CMotion