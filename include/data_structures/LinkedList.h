/**
 * @file LinkedList.h
 * @brief Pointer-tabanlı özel tek yönlü bağlı liste veri yapısı.
 * @author Proje Grubu
 * @date 2026-05-26
 */

#pragma once
#include <stdexcept>

namespace data_structures {

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& item) : data(item), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    
    ~LinkedList() {
        clear();
    }

    // Kopyalama ve taşıma işlemleri (Rule of Five)
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size(0) {
        Node* current = other.head;
        while (current != nullptr) {
            add(current->data);
            current = current->next;
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current != nullptr) {
                add(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    LinkedList(LinkedList&& other) noexcept : head(other.head), tail(other.tail), size(other.size) {
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }

    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            size = other.size;
            other.head = nullptr;
            other.tail = nullptr;
            other.size = 0;
        }
        return *this;
    }

    /**
     * @brief Listenin sonuna yeni bir eleman ekler.
     * @param item Eklenecek veri.
     */
    void add(const T& item) {
        Node* newNode = new Node(item);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    /**
     * @brief Listenin eleman sayısını döndürür.
     */
    int getSize() const {
        return size;
    }

    /**
     * @brief Listenin boş olup olmadığını kontrol eder.
     */
    bool isEmpty() const {
        return head == nullptr;
    }

    /**
     * @brief Belirli bir indeksteki elemanı döndürür.
     */
    T& get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Indeks liste sinirlari disinda!");
        }
        Node* temp = head;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        return temp->data;
    }

    /**
     * @brief Tüm listeyi temizler ve hafızayı boşaltır.
     */
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
        size = 0;
    }

    // =================================================================
    // ITERATOR SINIFI (DÖNGÜLER İÇİN)
    // =================================================================
    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* node) : current(node) {}

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        Iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        T& operator*() const {
            return current->data;
        }
    };

    Iterator begin() const {
        return Iterator(head);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
};

} // namespace data_structures