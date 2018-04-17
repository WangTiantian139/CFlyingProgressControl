//
// Created by wang on 18-4-1.
//

#ifndef FLYINGPROGRESSCONTROL_LIST_H
#define FLYINGPROGRESSCONTROL_LIST_H

#include "ListNode.h"

namespace CList
{

    template<typename T>
    class List
    {
    public:
//ctr
        List()
        { init(); };
        List(const List<T>& L);
        List(const List<T>& L, rank r, int n);
        List(const List<T>& L, ListNodePosi<T> p, int n);
//dtr
        ~List();
//read-only interface
        const rank size()const
        { return _size; }
        const bool empty()const
        { return _size <= 0; }
        const ListNodePosi<T> first()const
        { return this->header->succ; }
        const ListNodePosi<T> last()const
        { return this->tailer->pred; }
        const ListNodePosi<T> begin()const
        { return this->header->succ; }
        const ListNodePosi<T> end() const
        { return this->tailer; }
        const bool isValid(ListNodePosi<T> p)
        {
            return p and (tailer != p) and (header != p);
        }
        const T& operator[](rank r);

//read&write interface
        ListNodePosi<T> insertAsFirst(const T& e);
        ListNodePosi<T> insertAsLast(const T& e);
        ListNodePosi<T> append(const T& e)
        { return this->insertAsLast(e); }
        ListNodePosi<T> insertAsSucc(ListNodePosi<T> p, const T& e);
        ListNodePosi<T> insertAsPred(ListNodePosi<T> p, const T& e);
        ListNodePosi<T> insertAsSucc(rank r, const T& e);
        ListNodePosi<T> insertAsPred(rank r, const T& e);
        T remove(ListNodePosi<T> posi);
//traverse
        void traverse(void(*)(T&));
        template<typename VST>
        void traverse(VST&);

    protected:
        void init();
        int clear();

    private:
        int _size;
        ListNodePosi<T> header;
        ListNodePosi<T> tailer;
    };

    template<typename T>
    void List<T>::init()
    {
        this->header = new ListNode<T>;
        this->tailer = new ListNode<T>;
        this->header->succ = tailer;
        this->header->pred = nullptr;
        this->tailer->pred = header;
        this->tailer->succ = nullptr;
        this->_size = 0;
    }

    template<typename T>
    const T& List<T>::operator[](rank r)
    {
        ListNodePosi<T> p = this->begin();
        for (rank i = 0; i < r; i++)
        {
            p = p->succ;
        }
        return p->data;
    }
    template<typename T>
    ListNodePosi<T> List<T>::insertAsFirst(const T& e)
    {
        return header->insertAsSucc(e);
    }
    template<typename T>
    ListNodePosi<T> List<T>::insertAsSucc(ListNodePosi<T> p, const T& e)
    {
        return p->insertAsSucc(e);
    }
    template<typename T>
    ListNodePosi<T> List<T>::insertAsLast(const T& e)
    {
        return tailer->insertAsPred(e);
    }
    template<typename T>
    ListNodePosi<T> List<T>::insertAsPred(ListNodePosi<T> p, const T& e)
    {
        return p->insertAsPred(e);
    }
    template<typename T>
    List<T>::~List()
    {
        clear();
        delete header;
        delete tailer;
    }
    template<typename T>
    int List<T>::clear()
    {
        int oldsize = this->_size;
        while (_size > 0)
        {
            remove(header->succ);
            _size--;
        }
        return oldsize;
    }
    template<typename T>
    T List<T>::remove(ListNodePosi<T> posi)
    {
        T e = posi->data;
        posi->pred->succ = posi->succ;
        posi->succ->pred = posi->pred;
        delete posi;
        this->_size--;
        return e;
    }
    template<typename T>
    void List<T>::traverse(void (*visit)(T&))
    {
        for (ListNodePosi<T> p = this->begin(); p != this->end(); ++p)
        {
            visit(p->data);
        }
    }
    template<typename T>
    template<typename VST>
    void List<T>::traverse(VST& visit)
    {
        for (ListNodePosi<T> p = this->begin(); p != this->end(); ++p)
        {
            visit(p->data);
        }
    }
    template<typename T>
    ListNodePosi<T> List<T>::insertAsSucc(rank r, const T& e)
    {
        ListNodePosi<T> p = this->begin();
        for (rank i = 0; i < r and p != this->end(); i++)
        {
            p = p->succ;
        }
        return insertAsSucc(p, e);
    }
    template<typename T>
    ListNodePosi<T> List<T>::insertAsPred(rank r, const T& e)
    {
        ListNodePosi<T> p = this->begin();
        for (rank i = 0; i < r; i++)
        {
            p = p->succ;
        }
        return insertAsPred(p, e);
    }

}

#endif //FLYINGPROGRESSCONTROL_LIST_H
