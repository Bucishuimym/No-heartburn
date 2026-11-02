#pragma once
#ifndef GRAPHMATRIX_H
#define GRAPHMATRIX_H

#include <iostream>
#include "LinkedQueue.h"

const int MaxSize = 10;

namespace mgraph {
    template <typename T>
    class MGraph {
    public:
        MGraph(T a[], int n);                                                  //构造
        bool AddEdgeByIndex(int u, int v);                                     //提供通过索引添加边
        bool AddEdge(const T& u, const T& v);                                  //提供通过值添加边
        void PrintMatrix();                                                    //打印矩阵
        ~MGraph() {}                                                           //析构函数
        void DFTraverse(int v);                                                //深度优先遍历图
        void BFTraverse(int v);                                                //广度优先遍历图
        bool operator==(const MGraph<T>& other) const;                         //==重载
        bool operator!=(const MGraph<T>& other) const;                         //!=重载
    private:
        T vertex[MaxSize];                                                     //顶点数据
        int edge[MaxSize][MaxSize];                                            //邻接矩阵
        int vertexNum,                                                         //顶点数
            edgeNum;                                                           //边数
        int GetIndex(const T& val);                                            //根据值找索引
        void DFS(int v, bool visited[]);                                       //辅助函数
    };

    template <typename T>
    MGraph<T>::MGraph(T a[], int n) {
        vertexNum = n;
        edgeNum = 0;
        for (int i = 0; i < vertexNum; i++) vertex[i] = a[i];
        for (int i = 0; i < vertexNum; i++)
            for (int j = 0; j < vertexNum; j++)
                edge[i][j] = 0;
    }

    // 线性查找：找到顶点的值对应的索引
    template <typename T>
    int MGraph<T>::GetIndex(const T& val) {
        for (int i = 0; i < vertexNum; i++) {
            if (vertex[i] == val) return i;
        }
        return -1; // 没找到
    }

    // 外部调用AddEdge("u","v")
    template <typename T>
    bool MGraph<T>::AddEdge(const T& u, const T& v) {
        int i = GetIndex(u);
        int j = GetIndex(v);

        if (i == -1 || j == -1) {
            std::cerr << "error: vertex does not exist\n";
            return false;
        }
        return AddEdgeByIndex(i, j);
    }

    template <typename T>
    bool MGraph<T>::AddEdgeByIndex(int i, int j) {
        if (i < 0 || i >= vertexNum || j < 0 || j >= vertexNum) return false;
        if (edge[i][j] == 1) return true; // 边已存在

        edge[i][j] = 1;
        edge[j][i] = 1; // 无向图
        edgeNum++;
        return true;
    }

    // 深度优先遍历 (DFS)
    template <typename T>
    void MGraph<T>::DFTraverse(int v) {
        bool visited[MaxSize] = { false };
        DFS(v, visited);
        std::cout << std::endl;
    }

    template <typename T>
    void MGraph<T>::DFS(int v, bool visited[]) {
        std::cout << vertex[v] << " "; 
        visited[v] = true;

        for (int j = 0; j < vertexNum; j++) {
            if (edge[v][j] == 1 && !visited[j]) {
                DFS(j, visited);
            }
        }
    }

    // 广度优先遍历 (BFS)
    template <typename T>
    void MGraph<T>::BFTraverse(int v) {
        bool visited[MaxSize] = { false };

        queue::LinkedQueue<int> q;
        q.EnQueue(v);
        visited[v] = true;

        while (!q.Empty()) {
            int current = q.DeQueue();

            std::cout << vertex[current] << " "; // 打印
            for (int j = 0; j < vertexNum; j++) {
                if (edge[current][j] == 1 && !visited[j]) {
                    visited[j] = true; 
                    q.EnQueue(j);
                }
            }
        }
        std::cout << std::endl;
    }

    template <typename T>
    bool MGraph<T>::operator==(const MGraph<T>& other) const {
        if (vertexNum != other.vertexNum || edgeNum != other.edgeNum) {
            return false;
        }

        for (int i = 0;i < vertexNum;i++) {
            if (vertex[i] != other.vertex[i]) {
                return false;
            }
        }

        for (int i = 0;i < vertexNum;i++) {
            for (int j = 0;j < vertexNum;j++) {
                if (edge[i][j] != other.edge[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    template <typename T>
    bool MGraph<T>::operator!=(const MGraph<T>& other) const {
        return !(*this == other);
    }

    template <typename T>
    void MGraph<T>::PrintMatrix() {
        std::cout < " ";
        for (int i = 0;i < vertexNum;i++)
            std::cout << vertex[i] << " ";
        std::cout << "\n";
        for (int i = 0;i < vertexNum;i++) {
            std::cout << vertex[i] << " ";
            for (int j = 0;j < vertexNum;j++) {
                std::cout << edge[i][j] << " ";
            }
        }
        std::cout << "\n";
    }
} // namespace mgraph

#endif // !GRAPHMATRIX_H