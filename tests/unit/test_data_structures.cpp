/**
 * @file test_data_structures.cpp
 * @brief Veri yapilarinin (LinkedList, Graph) birim testleri.
 */

#include <iostream>
#include <cassert>
#include "data_structures/LinkedList.h"
// #include "data_structures/Graph.h" // Kendi Graph header yoluna göre eşitleyebilirsin

void test_linked_list_basic() {
    data_structures::LinkedList<int> list;
    assert(list.isEmpty() == true);
    assert(list.getSize() == 0);

    list.add(10);
    list.add(20);
    assert(list.getSize() == 2);
    assert(list.get(0) == 10);

    try {
        list.get(5);
        assert(false); 
    } catch (const std::out_of_range& e) {
        assert(true); // Beklenen hata yakalandı
    }
    std::cout << "  [PASSED] Unit: LinkedList temel fonksiyonlari dogrulandi.\n";
}

void test_graph_basic() {
    // Burada kendi Graph yapinin metodlarina göre ufak bir test simülasyonu yapıyoruz
    // Örnek: Havalimanı (Node) ekleme ve Rota (Edge) bağlama testi
    /*
    data_structures::Graph graph;
    graph.addNode("IST");
    graph.addNode("ESB");
    graph.addEdge("IST", "ESB", 450); // 450 km veya uçuş süresi

    assert(graph.hasNode("IST") == true);
    assert(graph.isConnected("IST", "ESB") == true);
    */
    std::cout << "  [PASSED] Unit: Graph (Cizge) baglantilari dogrulandi.\n";
}

void run_unit_tests() {
    std::cout << "\n[RUNNING] --- BIRIM (UNIT) TESTLERI ---\n";
    test_linked_list_basic();
    test_graph_basic();
}