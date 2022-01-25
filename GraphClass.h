#pragma once


#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <chrono>

template <class T>
class graph {

public:
    virtual bool adjacent(T x, T y) = 0;

    virtual std::vector<T> neighbors(T x) = 0;

    virtual void addEdge(T x, T y) = 0;

    virtual void deleteEdge(T x, T y) = 0;

    virtual void deleteNode(T x) = 0;

    virtual void dfs(T& startNode, void (*visit)(T)) = 0;

    virtual void bfs(T& startNode, void (*visit)(T)) = 0;

};

template <class T>
class adjList : graph<T> {
    std::vector<T> nodes;
    std::map<T, std::vector<T>> connections;

public:
    bool adjacent(T x, T y);

    std::vector<T> neighbors(T x);

    void addNode(T t);

    void addEdge(T x, T y);
    void deleteNode(T x);

    void deleteEdge(T x, T y);

    void bfs(T& startNode, void (*visit)(T));

    void dfs(T& startNode, void (*visit)(T));
};

template <class T>
class adjMatrix : graph<T> {
private:
    std::vector<T> nodes;
    std::vector< std::vector<int>> connections;

public:
    bool adjacent(T x, T y);

    std::vector<T> neighbors(T x);

    void addEdge(T x, T y);

    void deleteNode(T x);


    void dfs(T& startNode, void (*visit)(T));

    void bfs(T& startNode, void (*visit)(T));

    void addNode(T x);

    std::vector<T> getNodes();

    std::vector<std::vector<int>> getConnections();
};
