// CSCI 2270 Spring 2023
// Author: Matayay Karuna
// Final Project - MovieHashTable.cpp

#include<iostream>
#include <vector>
#include "MovieHashTable.hpp"
using namespace std;

// Constructor for MovieHashTable with default size
MovieHashTable::MovieHashTable() {
    // TODO
    table_size = 0;
    n_collisions = 0;
    table = new MovieNode*[table_size];
}

// Constructor for MovieHashTable with given size
MovieHashTable::MovieHashTable(int s) {
    // TODO
    table_size = s;
    n_collisions = 0;
    table = new MovieNode*[table_size];
}

// Destructor for MovieHashTable that deletes all nodes in the hash table
MovieHashTable::~MovieHashTable() {
    // TODO
    for (int i = 0; i < table_size; i++)
    {
        MovieNode *curr = table[i];
        MovieNode *prev;

        while (curr != nullptr)
        {
            prev = curr;
            curr = curr->next;
            delete prev;
            prev = nullptr;
        }
    }

    delete[] table;
}

/**
 * hash - Hash function for MovieHashTable that returns an index in the hash table for a given movie title
 * @param title
 * @return int
*/
int MovieHashTable::hash(string title) {
    // TODO
    int sum = 0;

    for (int i = 0; i < title.length(); i++)
    {
        sum += int(title[i]);
    }

    int index = (110419629 - sum) % table_size;

    return index;
}

/**
 * insert - Inserts a movie node into the hash table with the specified title
 * @param title
 * @param movie
 * @return void
*/
void MovieHashTable::insert(string title, MovieNode* movie) {
    // TODO
    if (table[hash(title)] == nullptr)
    {
        table[hash(title)] = movie;
        return;
    }
    
    MovieNode *temp = table[hash(title)]->next;
    table[hash(title)]->next = movie;
    movie->next = temp;
    setCollisions();
}

/**
 * search - Searches for a node in the hash table with the specified title
 * @param title
 * @return MovieNode*
*/
MovieNode* MovieHashTable::search(string title) {
    // TODO
    MovieNode *temp = table[hash(title)];

    while (temp != nullptr)
    {
        if (temp->title == title)
        {
            break;
        }

        temp = temp->next;
    }

    return temp;
}

/**
 * getCollisions - Returns the number of collisions that have occurred during insertion into the hash table
 * @return int
*/
int MovieHashTable::getCollisions() {
    // TODO
    return n_collisions;
}

/**
 * setCollisions - Increments the number of collisions that have occurred during insertion into the hash table
 * @return void
*/
void MovieHashTable::setCollisions() {
    // TODO
    n_collisions++;
}
