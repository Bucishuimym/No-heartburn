#pragma once

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "pch.h"
#include "framework.h"
#include <iostream>
#include <string>
#include <utility> 

namespace dlinkedlist {
    template <typename DataType>
    struct ListKey
    {

        static int Uid(const DataType& d) { return d.GetUid(); }                   // 返回数据项的唯一整数标识（UID），默认调用 DataType::GetUid()
        static std::string Name(const DataType& d) { return d.GetName(); }         // 返回数据项的名字字符串，默认调用 DataType::GetName()
    };

    template <typename DataType>
    class DoubleList
    {
    public:
        /* 结点对调用方开放：业务层的遍历（如 outFile / ListData / "我的第 n 条"）
           要拿着 Node* 一路走，同时也能直接看 p->data */
        struct Node
        {
            DataType data;
            Node* prev, * next;

            // 默认构造：用于创建哨兵结点或空节点，data 使用默认构造
            Node() : data(), prev(nullptr), next(nullptr) {}                       // 哨兵结点用
            // 带数据的构造：根据传入的数据拷贝构造结点
            explicit Node(const DataType& d) : data(d), prev(nullptr), next(nullptr) {}
        };

    private:
        Node* first;        // 哨兵头结点
        Node* last;         // 最后一个数据结点，空表为 nullptr
        int   length;       // 数据结点个数
        int   maxSize;      // 容量上限，-1 表示不限

        Node* NodeAt(int pos) const;                                               // 根据位序 pos（1 基）返回对应的数据结点指针；pos 非法时返回 nullptr
        void  Unlink(Node* p);                                                     // 从链上摘除结点 p（不释放内存），并更新 length 与 last 指针
    public:
        explicit DoubleList(int maxSize = -1);                                     // 构造：可选限制最大容量，默认不限制
        DoubleList(const DataType a[], int n, int maxSize = -1);                   // 从数组构造，按顺序将数组元素尾插入链表
        DoubleList(const DoubleList& other);                                       // 拷贝构造：深拷贝所有结点数据
        DoubleList& operator=(const DoubleList& other);                            // 拷贝赋值：清空当前并拷贝 other 的数据
        DoubleList(DoubleList&& other) noexcept;                                   // 移动构造：接管 other 的内部资源
        DoubleList& operator=(DoubleList&& other) noexcept;                        // 移动赋值：释放当前资源并接管 other
        ~DoubleList();                                                             // 析构：释放所有结点及哨兵

        bool PushBack(const DataType& item);                                       // 在链表尾部插入元素，成功返回 true（受 maxSize 限制）
        bool PushFront(const DataType& item);                                      // 在链表头部（第 1 位）插入元素，成功返回 true
        bool Insert(int pos, const DataType& item);                                // 在位序 pos（1 基）处插入元素，pos 可为 length+1 表示尾插
        bool DeleteByPos(int pos, DataType& out);                                  // 删除位于 pos 的元素并通过 out 返回被删除的数据，成功返回 true
        bool DeleteByUid(int uid, DataType& out);                                  // 删除首个匹配给定 uid 的元素并通过 out 返回，成功返回 true
        bool DeleteByName(const std::string& name, DataType& out);                 // 删除首个匹配给定 name 的元素并通过 out 返回，成功返回 true
        int  DeleteAllByUid(int uid);                                              // 删除所有匹配 uid 的元素，返回删除的条目数（级联删除）

        template <typename Pred>
        int  RemoveIf(Pred pred);                                                  // 模板方法：对每个元素调用 pred(data)，若为真则删除，返回删除数量
        bool PopBack(DataType& out);                                               // 弹出并返回尾部元素，成功返回 true
        bool PopFront(DataType& out);                                              // 弹出并返回头部元素，成功返回 true
        void Clear();                                                              // 清空所有数据结点，但保留哨兵头结点，重置 length 与 last

        Node* LocateByUid(int uid) const;                                          // 查找首个 uid 匹配的结点指针，找不到返回 nullptr
        Node* LocateByName(const std::string& name) const;                         // 查找首个 name 匹配的结点指针，找不到返回 nullptr
        template <typename Pred>
        Node* FindIf(Pred pred) const;                                             // 通用查找：若 pred(data) 为真则返回该结点指针，否则 nullptr
        bool  Get(int pos, DataType& out) const;                                   // 按位序获取元素值到 out，成功返回 true（链表访问为 O(n)）
        int  Length() const;                                                       // 返回当前元素个数，O(1)
        bool Empty() const;                                                        // 判断是否为空
        bool Full() const;                                                         // 判断是否已达容量上限（当 maxSize >= 0 时）
        int  MaxSize() const;                                                      // 返回配置的最大容量（-1 表示不限制）

        Node* Begin() const;                                                       // 返回第 1 个数据结点指针；空表返回 nullptr
        Node* Next(Node* p) const;                                                 // 返回 p 的下一个结点指针（或 nullptr）
        Node* Prev(Node* p) const;                                                 // 返回 p 的上一个数据结点指针；到头返回 nullptr（不会暴露哨兵）
        Node* Last() const;                                                        // 返回尾部最后一个数据结点指针；空表返回 nullptr
        bool  AtEnd(Node* p) const;                                                // 判断结点指针是否到达末尾（nullptr 视为到尾）

        template <typename Func>
        void  ForEach(Func f) const;                                               // 对每个数据结点执行函数 f（只读遍历，f 不应修改链表结构）
        void Reverse();                                                            // 就地反转链表方向（不改变数据，仅调整指针）
        void PrintListFirst() const;                                               // 从头到尾打印链表内容（调试用）
        void PrintListNext() const;                                                // 从尾到头打印链表内容（调试用）
    };

    template <typename DataType>
    typename DoubleList<DataType>::Node* DoubleList<DataType>::NodeAt(int pos) const
    {
        if (pos < 1 || pos > length)
            return nullptr;

        if (pos <= length / 2) {            // 位序落在前半段：从头往后走
            Node* p = first;
            for (int i = 1; i <= pos; i++)
                p = p->next;
            return p;
        }

        Node* p = last;                     // 后半段：从尾往前走
        for (int i = length; i > pos; i--)
            p = p->prev;
        return p;
    }

    template <typename DataType>
    void DoubleList<DataType>::Unlink(Node* p)
    {
        p->prev->next = p->next;            // p->prev 至少是哨兵，不会为空
        if (p->next != nullptr)
            p->next->prev = p->prev;

        length--;

        // 摘掉的是尾巴时要挪 last：删空了 last 归 nullptr，否则退回到前一个结点。
        // 少这一句就会出现 last 悬空（指向已经 delete 掉的结点），后面尾插直接崩。
        if (p->next == nullptr)
            last = (length == 0) ? nullptr : p->prev;
    }
    //构造 / 析构 / 拷贝 / 移动
    template <typename DataType>
    DoubleList<DataType>::DoubleList(int maxSize)
    {
        first = new Node();                 // 造哨兵
        last = nullptr;                     // 空表：尾是空指针
        length = 0;                         // 老版本漏了这两句，last / length 里是垃圾值
        this->maxSize = maxSize;            // 形参和成员同名，要写 this->
    }

    template <typename DataType>
    DoubleList<DataType>::DoubleList(const DataType a[], int n, int maxSize)
        : DoubleList(maxSize)               // 委托构造：先把哨兵建好，再逐个尾插
    {
        for (int i = 0; i < n; i++)
            PushBack(a[i]);                 // 超过 maxSize 的自动丢掉，不会越界
    }

    template <typename DataType>
    DoubleList<DataType>::DoubleList(const DoubleList& other)
        : DoubleList(other.maxSize)
    {
        for (Node* p = other.first->next; p != nullptr; p = p->next)
            PushBack(p->data);              // 深拷贝：两边各有一份结点，析构时不会双重释放
    }

    template <typename DataType>
    DoubleList<DataType>& DoubleList<DataType>::operator=(const DoubleList& other)
    {
        if (this == &other)                 // 自赋值不判，会先把自己清空再去读 other
            return *this;

        Clear();
        maxSize = other.maxSize;
        for (Node* p = other.first->next; p != nullptr; p = p->next)
            PushBack(p->data);
        return *this;
    }

    template <typename DataType>
    DoubleList<DataType>::DoubleList(DoubleList&& other) noexcept
        : first(other.first), last(other.last), length(other.length), maxSize(other.maxSize)
    {
        other.first = new Node();           // 被搬走的对象要还回一张合法的空表
        other.last = nullptr;
        other.length = 0;
    }

    template <typename DataType>
    DoubleList<DataType>& DoubleList<DataType>::operator=(DoubleList&& other) noexcept
    {
        if (this == &other)
            return *this;

        Clear();                            // 先放掉自己原来的结点
        delete first;                       // 连旧哨兵一起换掉

        first = other.first;
        last = other.last;
        length = other.length;
        maxSize = other.maxSize;

        other.first = new Node();
        other.last = nullptr;
        other.length = 0;
        return *this;
    }

    template <typename DataType>
    DoubleList<DataType>::~DoubleList()
    {
        Clear();
        delete first;                       // 哨兵也要还
        first = nullptr;
    }

    /* =========================================================
     * 增
     * ========================================================= */
    template <typename DataType>
    bool DoubleList<DataType>::PushBack(const DataType& item)
    {
        if (Full())
            return false;

        Node* s = new Node(item);           // 新结点的 next 在构造函数里已经是 nullptr 了
        if (last != nullptr) {
            s->prev = last;                 // 非空表：接在原来的尾巴后面
            last->next = s;
        }
        else {
            s->prev = first;                // 空表：它同时是第 1 个结点，前驱要记成哨兵，
            first->next = s;                // 不能是 nullptr——不然删它的时候 p->prev->next 就炸了
        }
        last = s;

        length++;
        return true;
    }

    template <typename DataType>
    bool DoubleList<DataType>::PushFront(const DataType& item)
    {
        if (Full())
            return false;

        Node* s = new Node(item);
        s->prev = first;
        s->next = first->next;
        if (first->next != nullptr)
            first->next->prev = s;
        else
            last = s;                       // 本来空表，它也是尾巴
        first->next = s;

        length++;
        return true;
    }

    template <typename DataType>
    bool DoubleList<DataType>::Insert(int pos, const DataType& item)
    {
        if (Full())
            return false;
        if (pos < 1 || pos > length + 1)    // 合法位序是 1 .. length+1，尾插也算
            return false;
        if (pos == length + 1)
            return PushBack(item);          // 尾插单独走，省得再判一次空表
        if (pos == 1)
            return PushFront(item);

        Node* p = NodeAt(pos);              // pos ∈ [1, length]，一定找得到
        Node* s = new Node(item);
        s->prev = p->prev;
        s->next = p;
        p->prev->next = s;                  // 插在 p 前面：先接好两边，再改 p->prev
        p->prev = s;

        length++;
        return true;
    }

    /* =========================================================
     * 删
     * ========================================================= */
    template <typename DataType>
    bool DoubleList<DataType>::DeleteByPos(int pos, DataType& out)
    {
        Node* p = NodeAt(pos);              // 位序非法时给的是 nullptr
        if (p == nullptr)
            return false;

        out = p->data;                      // 先把数据拷出来再 delete
        Unlink(p);
        delete p;
        return true;
    }

    template <typename DataType>
    bool DoubleList<DataType>::DeleteByUid(int uid, DataType& out)
    {
        Node* p = LocateByUid(uid);
        if (p == nullptr)
            return false;

        out = p->data;
        Unlink(p);
        delete p;
        return true;
    }

    template <typename DataType>
    bool DoubleList<DataType>::DeleteByName(const std::string& name, DataType& out)
    {
        Node* p = LocateByName(name);
        if (p == nullptr)
            return false;

        out = p->data;
        Unlink(p);
        delete p;
        return true;
    }

    template <typename DataType>
    int DoubleList<DataType>::DeleteAllByUid(int uid)
    {
        // 注销用户时用它一次清掉该用户的所有缓存数据，不用在调用方写 for + 手动跳过
        return RemoveIf([uid](const DataType& d) { return ListKey<DataType>::Uid(d) == uid; });
    }

    template <typename DataType>
    template <typename Pred>
    int DoubleList<DataType>::RemoveIf(Pred pred)
    {
        int removed = 0;
        Node* p = first->next;
        while (p != nullptr) {
            Node* nextNode = p->next;       // p 可能要被删掉，先存下下一个，不然就是野指针
            if (pred(p->data)) {
                Unlink(p);
                delete p;
                removed++;
            }
            p = nextNode;
        }
        return removed;
    }

    template <typename DataType>
    bool DoubleList<DataType>::PopBack(DataType& out)
    {
        return DeleteByPos(length, out);    // 空表时 length 是 0，位序非法 → 返回 false
    }

    template <typename DataType>
    bool DoubleList<DataType>::PopFront(DataType& out)
    {
        return DeleteByPos(1, out);
    }

    template <typename DataType>
    void DoubleList<DataType>::Clear()
    {
        Node* p = first->next;
        while (p != nullptr) {
            Node* nextNode = p->next;
            delete p;
            p = nextNode;
        }
        first->next = nullptr;              // 哨兵留着，链表回到空表状态
        last = nullptr;
        length = 0;
    }

    //查
    template <typename DataType>
    typename DoubleList<DataType>::Node* DoubleList<DataType>::LocateByUid(int uid) const
    {
        for (Node* p = first->next; p != nullptr; p = p->next) {
            if (ListKey<DataType>::Uid(p->data) == uid)
                return p;
        }
        return nullptr;                     // 找不到给空指针，调用方判空
    }

    template <typename DataType>
    typename DoubleList<DataType>::Node* DoubleList<DataType>::LocateByName(const std::string& name) const
    {
        for (Node* p = first->next; p != nullptr; p = p->next) {
            if (ListKey<DataType>::Name(p->data) == name)
                return p;
        }
        return nullptr;
    }

    template <typename DataType>
    template <typename Pred>
    typename DoubleList<DataType>::Node* DoubleList<DataType>::FindIf(Pred pred) const
    {
        for (Node* p = first->next; p != nullptr; p = p->next) {
            if (pred(p->data))
                return p;
        }
        return nullptr;
    }

    template <typename DataType>
    bool DoubleList<DataType>::Get(int pos, DataType& out) const
    {
        Node* p = NodeAt(pos);
        if (p == nullptr)
            return false;

        out = p->data;
        return true;
    }

    //表状态
    template <typename DataType>
    int DoubleList<DataType>::Length() const
    {
        return length;                      // 老版本是走一遍链表现数的，O(n)；维护好 length 就是 O(1)
    }

    template <typename DataType>
    bool DoubleList<DataType>::Empty() const
    {
        return length == 0;
    }

    template <typename DataType>
    bool DoubleList<DataType>::Full() const
    {
        return maxSize >= 0 && length >= maxSize;
    }

    template <typename DataType>
    int DoubleList<DataType>::MaxSize() const
    {
        return maxSize;
    }

    //遍历
    template <typename DataType>
    typename DoubleList<DataType>::Node* DoubleList<DataType>::Begin() const
    {
        return first->next;                 // 空表返回 nullptr，配合 AtEnd 正好不进循环
    }

    template <typename DataType>
    typename DoubleList<DataType>::Node* DoubleList<DataType>::Next(Node* p) const
    {
        return (p == nullptr) ? nullptr : p->next;
    }

    template <typename DataType>
    typename DoubleList<DataType>::Node* DoubleList<DataType>::Prev(Node* p) const
    {
        if (p == nullptr || p->prev == first)
            return nullptr;                 // 走到第 1 个结点就停，不放哨兵出去
        return p->prev;
    }

    template <typename DataType>
    typename DoubleList<DataType>::Node* DoubleList<DataType>::Last() const
    {
        return last;
    }

    template <typename DataType>
    bool DoubleList<DataType>::AtEnd(Node* p) const
    {
        return p == nullptr;
    }

    template <typename DataType>
    template <typename Func>
    void DoubleList<DataType>::ForEach(Func f) const
    {
        for (Node* p = first->next; p != nullptr; p = p->next)
            f(p->data);                     // f 里只读不改，改链表会让 p 失效
    }

    //其他
    template <typename DataType>
    void DoubleList<DataType>::Reverse()
    {
        if (length < 2)
            return;                         // 空表 / 只有一个结点，不用翻

        Node* oldHead = first->next;
        Node* oldTail = last;

        for (Node* p = oldHead; p != nullptr; ) {
            Node* nextNode = p->next;
            std::swap(p->next, p->prev);    // 每个数据结点的两个指针互换，链子的方向就反过来了
            p = nextNode;
        }

        oldHead->next = nullptr;            // 现在是新尾巴
        oldTail->prev = first;              // 现在是新头前接哨兵
        first->next = oldTail;
        last = oldHead;
    }

    template <typename DataType>
    void DoubleList<DataType>::PrintListFirst() const
    {
        for (Node* p = first->next; p != nullptr; p = p->next)
            std::cout << p->data << " ";
        std::cout << std::endl;
    }

    template <typename DataType>
    void DoubleList<DataType>::PrintListNext() const
    {
        for (Node* p = last; p != nullptr; p = Prev(p))
            std::cout << p->data << " ";
        std::cout << std::endl;
    }
}

#endif // !DLINKEDLIST_H
