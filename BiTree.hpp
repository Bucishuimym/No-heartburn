#pragma

#ifndef BITREE_HPP
#define BITREE_HPP

#include<iostream>
#include "No-heartburn/LinkedQueue.h"

template <typename T>
struct BiNode
{
	T data;
	BiNode<T>* left, * right;
};

template <typename T>
class BiTree
{
public:
	BiTree() { root = CreatBT(root); }
	~BiTree() { ReleaseBT(root); }
	void PreOrder() { PreOrder(root); }
	void InOrder() { InOrder(root); }
	void PostOder() { PostOrder(root); }
	void LevelOrder() { LevelOrder(root); }
private:
	BiNode<T>* CreatBT(BiNode<T>* bt);
	void ReleaseBT(BiNode<T>* bt);
	void PreOrder(BiNode<T>* bt);
	void InOrder(BiNode<T>* bt);
	void PostOrder(BiNode<T>* bt);
	void LevelOrder(BiNode<T>* bt);
	BiNode<T>* root;

};
template <typename T>
BiNode<T>* BiTree<T>::CreatBT(BiNode<T>* bt)
{
	BiNode<T>* bt;
	char ch;
	std::cin >> ch;
	if (ch == '#')
		bt == nullptr;
	else
	{
		bt = new BiNode<T>;
		bt->data = ch;
		bt->left = CreatBT(bt->left);
		bt->right = CreatBT(bt->right);
	}
	return bt;
}
////////////////////////////////////////////////////
template <typename T>
void BiTree<T>::ReleaseBT(BiNode<T>* bt)
{
	if (bt == nullptr) return;
	else
	{
		ReleaseBT(bt->left);
		ReleaseBT(bt->right);
		delete bt;
	}
}
///////////////////////////////////////////////=
template <typename T>
void BiTree<T>::PreOrder(BiNode<T>* bt)
{
	if (bt == nullptr) return;
	else
	{
		std::cout << bt->data << " ";
		PreOrder(bt->left);
		PreOrder(bt->right);

	}
}
///////////////////////////////////////////////
template <typename T>
void BiTree<T>::InOrder(BiNode<T>* bt)
{
	if (bt == nullptr) return;
	else
	{
		PreOrder(bt->left);
		std::cout << bt->data << " ";
		PreOrder(bt->right);
	}
}
///////////////////////////////////////////////
template <typename T>
void BiTree<T>::PostOrder(BiNode<T>* bt)
{
	if (bt == nullptr) return;
	else
	{
		PreOrder(bt->left);
		PreOrder(bt->right);
		std::cout << bt->data << " ";
		
	}
}
////////////////////////////////////////////////////
template <typename T>
void BiTree<T>::LevelOrder(BiNode<T>* bt)
{

}
#endif // !BITREE_HPP