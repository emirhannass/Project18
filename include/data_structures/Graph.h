#pragma once
#include "data_structures/LinkedList.h"

namespace data_structures {

template <typename T>
struct GraphNode {
    T data;
    // Bu düğümden gidilebilecek diğer düğümlerin (komşuların) listesi
    data_structures::LinkedList<GraphNode<T>*> neighbors; 

    GraphNode(const T& item) : data(item), neighbors() {}
};

template <typename T>
class Graph {
private:
    // Graf içindeki tüm düğümlerin listesi
    data_structures::LinkedList<GraphNode<T>*> all_nodes;

public:
    Graph() = default;
    ~Graph() {
        // Tüm dinamik düğüm hafızasını temizleyen kod yazılacak
    }

    void addNode(const T& item) {
        all_nodes.add(new GraphNode<T>(item));
    }

    // İki uçuş arasında yasal geçiş varsa kenar ekler
    void addEdge(GraphNode<T>* from, GraphNode<T>* to) {
        from->neighbors.add(to);
    }

    const data_structures::LinkedList<GraphNode<T>*>& getNodes() const {
        return all_nodes;
    }
};

} // namespace data_structures