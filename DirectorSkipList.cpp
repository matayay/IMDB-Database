// CSCI 2270 Spring 2023
// Author: Matayay Karuna
// Final Project - DirectorSkipList.cpp

#include<iostream>
#include <vector>
#include "DirectorSkipList.hpp"
#include <time.h>
using namespace std;

// Constructor for DirectorSkipList with default capacity and levels
DirectorSkipList::DirectorSkipList() {
    // TODO
    capacity = 2000;
    levels = 10;
    head = new DirectorSLNode("", levels);
    size = 0;
}

// Constructor for DirectorSkipList with given capacity and levels
DirectorSkipList::DirectorSkipList(int _cap, int _levels) {
    // TODO
    capacity = _cap;
    levels = _levels;
    head = new DirectorSLNode("", levels);
    size = 0;
}

// Destructor for DirectorSkipList that deletes all nodes in the skip list (but not the MovieNodes, which are shared with other data structures)
DirectorSkipList::~DirectorSkipList() {
    // TODO
    DirectorSLNode *curr = head;
    DirectorSLNode *prev;

    while (curr->next[0] != nullptr)
    {
        prev = curr;
        curr = curr->next[0];
        delete prev;
        prev = nullptr;
    }

    delete curr;
    curr = nullptr;
}

/**
 * insert - Inserts a movie node into the skip list with the specified director
 * @param director
 * @param _movie
 * @return void
*/
void DirectorSkipList::insert(string director, MovieNode* _movie) {
    // TODO
    DirectorSLNode *prev[levels];
    DirectorSLNode *curr = head;
    for (int i = (levels - 1); i >= 0; i--)
    {
        while (curr->next[i] != nullptr && curr->next[i]->director.compare(director) < 0)
        {
            curr = curr->next[i];
        }
        prev[i] = curr;
    }
    curr = curr->next[0];

    if (curr != nullptr && curr->director == director)
    {
        curr->addMovie(_movie);
        return;
    }

    if (size == capacity)
    {
        cout << "Skiplist is full." << endl; 
        return;
    }

    int new_levels = 1;

    while (new_levels < levels && rand() % 2 == 0)
    {
        new_levels++;
    }

    DirectorSLNode *new_node = new DirectorSLNode(director, new_levels);
    new_node->addMovie(_movie);

    for (int i = 0; i < new_levels; i++)
    {
        new_node->next[i] = prev[i]->next[i];
        prev[i]->next[i] = new_node;
    }
    size++;
}

/**
 * search - Searches for a node in the skip list with the specified director
 * @param director
 * @return DirectorSLNode*
*/
DirectorSLNode *DirectorSkipList::search(string director) {
    // TODO
    DirectorSLNode *curr = head;

    for (int i = (levels - 1); i >= 0; i--)
    {
        while (curr->next[i] != nullptr && curr->next[i]->director.compare(director) < 0)
        {
            curr = curr->next[i];
        }
    }

    curr = curr->next[0];

    if (curr != nullptr && curr->director == director)
    {
        return curr;
    }

    return nullptr;
}

/**
 * prettyPrint - Pretty-prints the skip list
 * @return void
*/
void DirectorSkipList::prettyPrint() {
    // TODO
    DirectorSLNode *curr = head;
    int level_height = 0;

    cout << "\n";

    if (curr->next[0] != nullptr)
    {
        for (int i = 0; i < curr->next.size(); i++)
        {
            if (curr->next[i] != nullptr)
            {
                level_height++;
            }
        }
    }

    while (curr->next[0] != nullptr)
    {
        cout << "("  << level_height << " | " << curr->next[0]->director << ")";
        curr = curr->next[0];

        if (curr->next[0] != nullptr)
        {
            cout << " --> ";

            level_height = 0;
            for (int i = 0; i < curr->next.size(); i++)
            {
                if (curr->next[i] != nullptr)
                {
                    level_height++;
                }
            }
        }
    }

    cout << "\n";
}
