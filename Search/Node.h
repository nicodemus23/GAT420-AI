#pragma once
#include <vector>

// stores data of type T with links to parent and children
template<typename T>
struct node
{   
    node(const T& data) : data{ data } {} // constructor
    // each node has a:
    //  data field:
    T data = T(); // data of type T

    // visited flag:
    bool visited{ false }; // pointer to parent node
    // parent pointer:
    node* parent{ nullptr }; // vector of pointers to child nodes
    // vector of child pointers:
    std::vector<node*> children; // vector of pointers to child nodes
};

//////////////////////////////////////////////

//*Templates are a feature of the C++ programming language that allows functions and classes to operate with generic types.This allows a function or class to work on many different data 
//types without being rewritten for each one.Because the node is a templated struct, different data types can be used for the node.T could be a string, int, char, class or any other type.