#pragma once
#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include <iostream>
namespace queue {
	template <typename T>
	struct Node {
		T data;
		Node<T>* next;
	};

	template <typename T>
	class LinkedQueue {
	private:
		Node<T>* front, * last;
		int length;
	public:
		LinkedQueue();
		~LinkedQueue();
		void EnQueue(const T& x);                                                  //入队
		void PrintQueue() const;                                                   //打印
		T DeQueue();                                                               //出队
		T GetHead() const;                                                         //获取队头元素
		int GetLength() const { return length; }                                   //获取队长
		bool Empty() const { return length == 0; }                                 //判空
		LinkedQueue(const LinkedQueue<T>& other);                                  // 拷贝构造
		LinkedQueue<T>& operator=(const LinkedQueue<T>& other);                    // 赋值重载
		LinkedQueue(LinkedQueue<T>&& other) noexcept;                              // 移动构造 (可选)
		void Clear();                                                              // 清空
		T GetBack() const;                                                         // 获取队尾
	};
}
namespace queue {
	template <typename T>
	LinkedQueue<T>::LinkedQueue() :length(0) {
		Node<T>* s = new Node<T>;
		s->next = nullptr;
		front = last = s;
	}

	template <typename T>
	LinkedQueue<T>::~LinkedQueue() {
		Node<T>* p = front;
		while (p != nullptr) {
			Node<T>* temp = p;
			p = p->next;
			delete temp;
		}
		front = nullptr;
		last = nullptr;
	}

	// 入队
	template <typename T>
	void LinkedQueue<T>::EnQueue(const T& x) {
		Node<T>* s = new Node<T>;
		s->data = x;
		s->next = nullptr;
		last->next = s;
		last = s;
		length++;
	}

	template <typename T>
	void LinkedQueue<T>::PrintQueue() const {
		if (Empty()) {
			std::cerr << "No data.\n";
			return;
		}

		Node<T>* s = front->next;
		while (s != nullptr) {
			std::cout << s->data << " ";
			s = s->next;
		}
		std::cout << std::endl;
	}

	// 出队
	template <typename T>
	T LinkedQueue<T>::DeQueue() {
		if (Empty()) {
			std::cerr << "error: queue is empty.\n";
			return T{};
		}

		Node<T>* s = front->next;
		T x = s->data;

		front->next = s->next;
		delete s;
		length--;

		if (length == 0) {
			last = front;
		}

		return x;
	}

	template <typename T>
	T LinkedQueue<T>::GetHead() const {
		if (Empty()) {
			std::cerr << "No data.\n";
			return T{};
		}

		Node<T>* s = front->next;
		return s->data;
	}

	// 移动构造
	template <typename T>
	LinkedQueue<T>::LinkedQueue(LinkedQueue<T>&& other) noexcept {
		front = other.front;
		last = other.last;
		length = other.length;

		// 剥夺别人的所有权
		other.front = nullptr;
		other.last = nullptr;
		other.length = 0;
	}

	template <typename T>
	T LinkedQueue<T>::GetBack() const {
		if (Empty()) {
			std::cerr << "No data.\n";
			return T{};
		}
		return last->data;
	}

	template <typename T>
	void LinkedQueue<T>::Clear() {
		Node<T>* p = front->next;
		while (p != nullptr) {
			Node<T>* temp = p;
			p = p->next;
			delete temp;
		}
		front->next = nullptr;
		last = front;
		length = 0;
	}

	template <typename T>
	LinkedQueue<T>& LinkedQueue<T>::operator=(const LinkedQueue<T>& other) {
		if (this != &other) { // 防止自我赋值 q = q;
			Clear();

			Node<T>* p = other.front->next;
			while (p != nullptr) {
				EnQueue(p->data);
				p = p->next;
			}
		}
		return *this;
	}

	template <typename T>
	LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& other) : length(0) {
		Node<T>* s = new Node<T>;
		s->next = nullptr;
		front = last = s;

		Node<T>* p = other.front->next;
		while (p != nullptr) {
			EnQueue(p->data);
			p = p->next;
		}
	}
}
#endif // !LINKEDQUEUE_H