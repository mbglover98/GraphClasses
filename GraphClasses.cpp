// GraphClasses.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <chrono>

template <class T>
class graph{

public:
    virtual bool adjacent(T x, T y) = 0;

    virtual std::vector<T> neighbors(T x) = 0; 

    virtual void addEdge(T x, T y) = 0;

        virtual void deleteEdge(T x, T y) = 0;

        virtual void deleteNode(T x) = 0;

        virtual void dfs(T& startNode,void (*visit)(T)) = 0;

        virtual void bfs(T& startNode, void (*visit)(T)) = 0;

};

template <class T>
class adjList : graph<T> {
    std::vector<T> nodes;
    std::map<T, std::vector<T>> connections;

public:
    bool adjacent(T x, T y) {
        std::list<T> queue;
        std::vector<T> xConnections;
        queue.push_back(x);
        while (!queue.empty()) {
            xConnections = connections.find(queue.front())->second;
            for (auto itr : xConnections) {
                if (itr == y)
                    return true;
                queue.push_back(itr);
            }
            queue.unique();
            queue.pop_front();
        }
        return false;
    }

    std::vector<T> neighbors(T x) {
        auto xConnections = connections.find(x);
        std::vector<T> out;
        for (auto itr = xConnections->second.begin(); itr != xConnections->second.end(); itr++) {
            out.push_back(*itr);
        }
        return out;
    }

    void addNode(T t) {
        nodes.push_back(t);
        std::vector<T> empty;
        connections.insert({ t, empty });
        return;
    }

    void addEdge(T x, T y) {
        auto firstNode =connections.find(x);   
        firstNode->second.push_back(y);
        return;
    }

    void deleteNode(T x) {
        
        for (auto itr = connections.begin(); itr != connections.end();++itr) {
            auto loc = std::find(itr->second.begin(), itr->second.end(), x);
            if(loc != itr->second.end())
                itr->second.erase(loc);
        }
        connections.erase(connections.find(x));
        nodes.erase(std::find(nodes.begin(), nodes.end(), x));
    }

    void deleteEdge(T x, T y) {
        auto xNode = connections.find(x);
        auto erasureLoc = std::find(xNode->second.begin(), xNode->second.end(), y);
        if(erasureLoc != xNode->second.end())
            xNode->second.erase(std::find(xNode->second.begin(), xNode->second.end(),y));
    }

    std::vector<T> getNodes() {
        return nodes;
    }

    std::map<T,std::vector<T>> getConnections() {
        return connections;
    }

    void bfs(T& startNode,void (*visit)(T)) {
        std::list<T> queue;
        std::list<T> visited;
        queue.push_back(startNode);
        while (!queue.empty()) {
            std::vector<T> x = connections.find(queue.front())->second;
            for (auto itr : x) {
                if (std::find(visited.begin(),visited.end(), itr) == visited.end() && std::find(queue.begin(), queue.end(), itr) == queue.end())
                    queue.push_back(itr);
            }
            visited.push_back(queue.front());
            visit(queue.front());
            queue.pop_front();
    
        }
    }

    void dfs(T& startNode, void (*visit)(T)) {
        std::list<T> queue;
        std::list<T> visited;
        queue.push_back(startNode);
        while (!queue.empty()) {
            visit(queue.front());
            std::vector<T> x = connections.find(queue.front())->second;
            T currentNode = queue.front();
            std::reverse(x.begin(), x.end());
            if (x.size() == 0) {
                queue.pop_front();
            }

            for (auto itr : x) {
                queue.push_front(itr);
            }

            visited.push_back(currentNode);
            queue.unique();

            for (auto itr : visited) {
                queue.remove(itr);
            }

        }
    }
};

template <class T>
class adjMatrix : graph<T> {
private:
    std::vector<T> nodes;
    std::vector< std::vector<int>> connections;

public:
    bool adjacent(T x, T y) {
        std::list<int> queue;

        auto xIndex = std::distance(nodes.begin(), find(nodes.begin(), nodes.end(), x));
        auto yIndex = std::distance(nodes.begin(), find(nodes.begin(), nodes.end(), y));
        
        queue.push_back(xIndex);
        while (!queue.empty()) {
            int CurrentNode = queue.front();
            for (int i = 0; i < nodes.size(); ++i) {
                if (connections[CurrentNode][i] == 1) {
                    queue.push_back(i);
                    if (i == yIndex) {
                        return true;
                    }
                }
                
            }

            queue.unique();
            queue.pop_front();
        }
        return false;
    }

    std::vector<T> neighbors(T x) {
        std::vector<T> out; 
        auto index = std::distance(nodes.begin(), find(nodes.begin(), nodes.end(), x));
       
        for (int i = 0; i < nodes.size(); i++) {
            if (connections[index][i] != 0) {
                out.push_back(nodes[i]);
            }
        }
        return out;
    }

    void addEdge(T x, T y) {
        auto xIndex = std::distance(nodes.begin(), find(nodes.begin(), nodes.end(), x));
        auto yIndex = std::distance(nodes.begin(), find(nodes.begin(), nodes.end(), y));
        connections[xIndex][yIndex] = 1;
    }

    void deleteEdge(T x, T y) {
        auto xIndex = std::distance(nodes.begin(),find(nodes.begin(), nodes.end(), x));
        auto yIndex = std::distance(nodes.begin(), find(nodes.begin(), nodes.end(), y));
        connections[xIndex][yIndex] = 0;
        }

    void deleteNode(T x) {
        auto index = std::distance(nodes.begin(),find(nodes.begin(), nodes.end(), x));

        for (int i = 0; i < nodes.size(); ++i) {
            connections[i].erase(connections[i].begin() + index);
        }
        connections.erase(connections.begin() + index);
        nodes.erase(std::find(nodes.begin(), nodes.end(), x));
    }


    void dfs(T& startNode, void (*visit)(T)) {
        std::list<int> queue;
        std::list<T> visited;
        queue.push_back(std::distance(nodes.begin(), find(nodes.begin(), nodes.end(), startNode)));
        while (!queue.empty()) {
            int x = queue.front();
            visit(nodes[x]);

            for (int i = nodes.size() - 1; i >= 0; --i) {
                if (connections[x][i] == 1 && std::find(visited.begin(), visited.end(), i) == visited.end()) {
                    queue.push_front(i);
                }

            }
            queue.unique();
            visited.push_back(x);
            for (auto itr : visited) {
                queue.remove(itr);
            }
        }
    }

    void bfs(T& startNode, void (*visit)(T)) {
        std::list<int> queue;
        std::list<T> visited;
        queue.push_back(std::distance(nodes.begin(), find(nodes.begin(), nodes.end(), startNode)));
        while (!queue.empty()) {
            int x = queue.front();
            visit(nodes[x]);
            visited.push_back(x);
            for (int i = 0; i < nodes.size(); ++i) {
                if (connections[x][i] == 1 && std::find(visited.begin(), visited.end(), i) == visited.end() && std::find(queue.begin(), queue.end(), i) == queue.end()) {
                    queue.push_back(i);
                }
            }
            queue.unique();
            queue.pop_front();
        }
    }

    void addNode(T x) {
        std::vector<int> newLine;
        for (int i = 0; i < nodes.size();++i) {
            connections[i].push_back(0);
            newLine.push_back(0);
        }
        newLine.push_back(0);
        connections.push_back(newLine);
        
        nodes.push_back(x);
        connections[nodes.size() - 1][nodes.size() - 1] = 1;
    }

    std::vector<T> getNodes() {
        return nodes;
    }

    std::vector<std::vector<int>> getConnections() {
        return connections;
    }
};

template <class T>
void printWComma(T in) {
    std::cout << in << ", ";
}

template<class T>
adjList<T> generateAdjListGraph(int numberOfNodes, double edgeProbability) {
    adjList<T> out;
    for (int i = 0; i < numberOfNodes; ++i) {
        out.addNode(i);
    }

    for (int i = 0; i < numberOfNodes; ++i) {
        for (int j = 0; j < numberOfNodes; ++j) {
            if ((double) rand()/RAND_MAX > edgeProbability &&i!=j) {
                out.addEdge(i, j);
            }
        }
    }
    return out;
}

template <class T>
adjMatrix<T> generateAdjMatrixGraph(int numberOfNodes, double edgeProbability) {
    adjMatrix<T> out;
    for (int i = 0; i < numberOfNodes; ++i) {
        out.addNode(i);
    }

    for (int i = 0; i < numberOfNodes; ++i) {
        for (int j = 0; j < numberOfNodes; ++j) {
            if ((double) rand()/RAND_MAX > edgeProbability && i != j) {
                out.addEdge(i, j);
            }
        }
    }
    return out;
}

void timer() {
    int sizes[5] = { 2,8,64,256,1024 };
    std::cout << "Timing the Matrix Builds" << std::endl;
    for (int i = 0; i < 5; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        generateAdjMatrixGraph<int>(sizes[i], .5);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << sizes[i] << " took " << duration.count() << " microseconds to complete\n";
    }
}

int main()
{
    adjMatrix<int> matrixTest = generateAdjMatrixGraph<int>(10, .6);
    adjList<int> listTest = generateAdjListGraph<int>(10, .6);
    timer();


    std::cout << "\nMatrix Tests\n";
    std::vector<std::vector<int>> test = matrixTest.getConnections();
    std::vector<int> test2 = matrixTest.getNodes(); 

    std::cout << "    ";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << " ";
    }
    std::cout << "\n\n";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << "   ";
        for (int j = 0; j < test2.size(); ++j) {
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\ncheck for path between 1 and 3: " << matrixTest.adjacent(1, 3);
    std::cout << "\n\ndelete connection between 1 and 2\n";
    matrixTest.deleteEdge(1, 2);
    test = matrixTest.getConnections();

    std::cout << "    ";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << " ";
    }
    std::cout << "\n\n";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << "   ";
        for (int j = 0; j < test2.size(); ++j) {
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nadd connection between 1 and 7\n";
    matrixTest.addEdge(1, 7);
    test = matrixTest.getConnections();

    std::cout << "    ";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << " ";
    }
    std::cout << "\n\n";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << "   ";
        for (int j = 0; j < test2.size(); ++j) {
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\ndelete node 1\n";
    matrixTest.deleteNode(1);
    test = matrixTest.getConnections();
    test2 = matrixTest.getNodes();
    std::cout << "    ";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << " ";
    }
    std::cout << "\n\n";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << "   ";
        for (int j = 0; j < test2.size(); ++j) {
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::vector<int> matrixNeighbors = matrixTest.neighbors(4);
    std::cout << "\nNeighbors of 4 = ";
    for (int i = 0; i < matrixNeighbors.size(); ++i) {
        std::cout << matrixNeighbors[i] << ",";
    }

    std::cout << std::endl << "\nAdding node 11\n";

    matrixTest.addNode(11);
    test = matrixTest.getConnections();
    test2 = matrixTest.getNodes();
    std::cout << "    ";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << " ";
    }
    std::cout << "\n\n";
    for (int i = 0; i < test2.size(); ++i) {
        std::cout << test2[i] << "   ";
        for (int j = 0; j < test2.size(); ++j) {
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nBreadth First Search passed print with Comma\n";
    matrixTest.bfs(test2[0],&printWComma<int>);
    std::cout << "\n\nDepth First Search passed print with Comma\n";
    matrixTest.dfs(test2[0], &printWComma<int>);

    ///////////////////////////////////////////////////////////////
    std::cout << "\n\nList tests\n\n";
    std::map<int, std::vector<int>> test3 = listTest.getConnections();
    std::vector<int> test4 = listTest.getNodes();

    for (auto itr = test3.begin(); itr != test3.end(); itr++) {
        std::cout << itr->first << " - ";
        for (auto i = itr->second.begin(); i != itr->second.end(); ++i) {
            std::cout << *i << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "\ncheck for path between 0 and 2: " << listTest.adjacent(0, 2);
    std::cout << "\n\ndelete connection between 1 and 2\n";
    listTest.deleteEdge(1, 2);
    test3 = listTest.getConnections();

    for (auto itr = test3.begin(); itr != test3.end(); itr++) {
        std::cout << itr->first << " - ";
        for (auto i = itr->second.begin(); i != itr->second.end(); ++i) {
            std::cout << *i << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nadd connection between 1 and 7\n";
    listTest.addEdge(1, 7);
    test3 = listTest.getConnections();

    for (auto itr = test3.begin(); itr != test3.end(); itr++) {
        std::cout << itr->first << " - ";
        for (auto i = itr->second.begin(); i != itr->second.end(); ++i) {
            std::cout << *i << ", ";
        }
        std::cout << std::endl;
    }std::cout << std::endl;
    

    std::cout << "\ndelete node 1\n";
    listTest.deleteNode(1);
    test3 = listTest.getConnections();
    test4 = listTest.getNodes();
    for (auto itr = test3.begin(); itr != test3.end(); itr++) {
        std::cout << itr->first << " - ";
        for (auto i = itr->second.begin(); i != itr->second.end(); ++i) {
            std::cout << *i << ", ";
        }
        std::cout << std::endl;
    }

    std::vector<int> listNeighbors = listTest.neighbors(4);
    std::cout << "Neighbors of 4 = ";
    for (int i = 0; i < listNeighbors.size(); ++i) {
        std::cout << listNeighbors[i] << ",";
    }

    std::cout << std::endl << "\nAdding node 11\n\n";

    listTest.addNode(11);
    test3 = listTest.getConnections();
    test4 = listTest.getNodes();

    for (auto itr = test3.begin(); itr != test3.end(); itr++) {
        std::cout << itr->first << " - ";
        for (auto i = itr->second.begin(); i != itr->second.end(); ++i) {
            std::cout << *i << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nBreadth First Search passed print with Comma\n";
    listTest.bfs(test2[0], &printWComma<int>);
    std::cout << "\n\nDepth First Search passed print with Comma\n";
    listTest.dfs(test2[0], &printWComma<int>);
}