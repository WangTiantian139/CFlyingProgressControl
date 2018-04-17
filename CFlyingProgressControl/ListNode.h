//
// Created by wang on 18-4-1.
//

#ifndef FLYINGPROGRESSCONTROL_LISTNODEPOSI_H
#define FLYINGPROGRESSCONTROL_LISTNODEPOSI_H


namespace CList
{
    typedef int rank;

    template <typename T>
    struct ListNode;

    template<typename T>
    using ListNodePosi = ListNode<T>*;

    template<typename T>
    class ListNode
    {
    public:
        ListNode() = default;
        explicit ListNode(T e, ListNodePosi<T> p = nullptr, ListNodePosi<T> s = nullptr)
        : data(e), pred(p), succ(s)
        {}

        ListNodePosi<T> insertAsPred(const T& e);
        ListNodePosi<T> insertAsSucc(const T& e);
        const ListNode <T>& operator++();
        const ListNode <T>& operator--();
        const ListNodePosi<T> operator++(int);
        const ListNodePosi<T> operator--(int);

        T data;
        ListNodePosi<T> pred;
        ListNodePosi<T> succ;

    };

    template<typename T>
    ListNodePosi<T> ListNode<T>::insertAsPred(const T& e)
    {
        auto x = new ListNode(e, this->pred, this);
        this->pred->succ = x;
        this->pred = x;
        return x;
    }

    template<typename T>
    ListNodePosi<T> ListNode<T>::insertAsSucc(const T& e)
    {
        auto x = new ListNode(e, this, this->succ);
        this->succ->pred = x;
        this->succ = x;
        return x;
    }

}

#endif //FLYINGPROGRESSCONTROL_LISTNODEPOSI_H
