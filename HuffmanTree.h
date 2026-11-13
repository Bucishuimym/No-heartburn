#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#ifndef HAFFMANTREE_H
#define HAFFMANTREE_H

#include "No-heartburn/LinkedQueue.h"

template <typename T>
struct Node
{
	T data;
	int freq;
	Node<T>* left, * right;
	Node(T c, int f) : data(c), freq(f), left(nullptr), right(nullptr) {}
};

template <typename T>
struct Comp {
	bool operator()(Node<T>* a, Node<T>* b) {
		return a->freq > b->freq;
	}
};

//建树
template <typename T>
Node<T>* buildHuffmanTree(const std::unordered_map<T, int>& freqmap)
{
	std::priority_queue < Node<T>*, std::vector<Node<T>*>, Comp<T>> pq;
	for (auto it = freqmap.begin(); it != freqmap.end(); ++it)
	{
		pq.push(new Node<T>(it->first, it->second));
	}
	while (pq.size() > 1)
	{
		Node<T>* a = pq.top(); pq.pop();
		Node<T>* b = pq.top(); pq.pop();
		Node<T>* parent = new Node<T>('\0', a->freq + b->freq);
		parent->left = a;
		parent->right = b;
		pq.push(parent);
	}
	return pq.empty() ? nullptr : pq.top();
}


//生成编码表
template <typename T>
void generateCodes(Node<T>* roots, const std::string& code,std::unordered_map<char,std::string>& codes) 
{
	if (!roots) return;
	if (!roots->left && !roots->right)
	{
		codes[roots->data] = code.empty() ? "0" : code;
		return;
	}
	generateCodes(roots->left, code + "0", codes);
	generateCodes(roots->right, code + "1", codes);
}
//编码
std::string encode(const std::string& text, const std::unordered_map<char, std::string>& codes)
{
	std::string result;
	for (char c : text)result += codes.at(c);
	return result;
}
//解码
template <typename T>
std::string decode(const std::string& encode,Node<T>*root)
{
	std::string result;
	Node<T>* cur = root;
	for (char bit : encode)
	{
		cur = (bit == '0') ? cur->left : cur->right;
		if (!cur->left && !cur->right)
		{
			result += cur->data;
			cur = root;
		}
	}
	return result;
}
//释放
template <typename T>
void freeTree(Node<T>* root)
{
	if (!root) return;
	freeTree(root->left);
	freeTree(root->right);
	delete root;
}

void HuffmanTree()
{
	std::string text = "asdfghjkl";
	std::unordered_map<char, int>freq;
	for (char c : text)freq[c]++;
	Node<char>* root = buildHuffmanTree(freq);
	std::unordered_map<char, std::string>codes;
	generateCodes(root, "", codes);
	std::cout << "编码表:\n";
	for (auto& p : codes)
	{
		std::cout << p.first << ": " << p.second << "\n";
	}
	std::string encoded = encode(text, codes);
	std::string decoded = decode(encoded, root);
	std::cout << "原始" << text << "\n编码：" << encoded << "\n解码：" << decoded << "\n";
	freeTree(root);
}
#endif // !HAFFMANTREE_H