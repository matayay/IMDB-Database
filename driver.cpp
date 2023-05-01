// CSCI 2270 Spring 2023
// Author: Matayay Karuna
// Final Project - driver.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"

using namespace std;

// Function prototypes
MovieNode* parseMovieLine(string line);
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList);
int display_menu();

// Main function
int main(int argc, char* argv[]) {
    // TODO
    if (argc != 4)
    {
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./<program name> <csv file> <hashTable size> <skipList size>" << endl;
        return 0;
    }

    srand(time(0));
    MovieHashTable *hash_table = new MovieHashTable(atoi(argv[2]));
    DirectorSkipList *skip_list = new DirectorSkipList(atoi(argv[3]), 10);

    readMovieCSV(argv[1], *hash_table, *skip_list);
    int choice = 0;

    skip_list->prettyPrint();
    cout << "\n" << "Number of Collisions: " << hash_table->getCollisions() << endl;

    do
    {
        choice = display_menu();
        cin.ignore();

        if (choice == 1)
        {
            string movie_title = "";
            MovieNode *movie;

            cout << "Enter movie name: " << endl;
            getline(cin, movie_title);

            movie = hash_table->search(movie_title);

            if (movie != nullptr)
            {
                cout << "\n" << "The director of " << movie->title << " is " << movie->director << endl;
            }
            else
            {
                cout << "\n" << "Movie not found" << endl;
            }
        }

        else if (choice == 2)
        {
            string director_name = "";
            DirectorSLNode *director;

            cout << "Enter director name: " << endl;
            getline(cin, director_name);

            director = skip_list->search(director_name);

            if (director != nullptr)
            {
                cout << "\n" << director->director << " directed " << director->movies.size() << " movies" << endl;
            }
            else
            {
                cout << "\n" << "Director not found" << endl;
            }
        }

        else if (choice == 3)
        {
            string movie_title = "";
            MovieNode *movie;

            cout << "Enter movie name: " << endl;
            getline(cin, movie_title);

            movie = hash_table->search(movie_title);

            if (movie != nullptr)
            {
                cout << "\n" << "Summary: " << movie->title << " is a " 
                    << movie->year << " " << movie->genre << " film featuring " << movie->actors << endl;
                
                cout << "Plot: " << movie->description << endl;
            }
            else
            {
                cout << "\n" << "Movie not found" << endl;
            }
        }

        else if (choice == 4)
        {
            string director_name = "";
            DirectorSLNode *director;

            cout << "Enter director name: " << endl;
            getline(cin, director_name);

            director = skip_list->search(director_name);

            if (director != nullptr)
            {
                if (director->movies.size() >= 1)
                {
                    cout << "\n" << director->director << " directed the following movies: " << endl;
                    for (int i = 0; i < director->movies.size(); i++)
                    {
                        cout << i+1 << ": " << director->movies[i]->title << endl;
                    }
                }
                else
                {
                    cout << "\n" << director->director << " hasn't directed any movies yet" << endl; 
                }
            }
        }

        else if (choice == 5)
        {
            cout << "Goodbye!" << endl;
        }
    }
    while (choice != 5);

    delete hash_table;
    hash_table = nullptr;
    delete skip_list;
    skip_list = nullptr;

    return 0;
}

/**
 * parseMovieLine - Function to parse a CSV line into a MovieNode object
 * @param line
 * @return MovieNode*
*/
MovieNode* parseMovieLine(string line) {
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '\"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i+1] == '\"') {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12) {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode* movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}

/**
 * readMovieCSV - Function to read a CSV file into a vector of MovieNode objects
 * @param filename
 * @param movieTable
 * @param directorList
 * @return void
*/
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList) {
    // TODO
    ifstream fin;
    vector<MovieNode*> db;
    string line = "";

    fin.open(filename);
    if (fin.fail())
    {
        cout << "failed to open file." << endl;
        return;
    }

    getline(fin, line);
    while (getline(fin, line))
    {
        db.push_back(parseMovieLine(line));
    }

    fin.close();

    for (int i = 0; i < db.size(); i++)
    {
        movieTable.insert(db[i]->title, db[i]);
        directorList.insert(db[i]->director, db[i]);
    }
}

/**
 * display_menu - Function to display the menu options
 * @return void
*/
int display_menu() {
    // TODO
    int option = 0;
    bool pass = false;

    do
    {
        cout << "\n" << "Please select an option: " << endl;
        cout << "1. Find the director of a movie" << endl <<
                "2. Find the number of movies by a director" << endl <<
                "3. Find the description of a movie" << endl <<
                "4. List the movies by a director" << endl <<
                "5. Quit" << endl;
        
        cout << "\n" << "Enter an option: " << endl;
        cin >> option;

        if (option != 1 && option != 2 && option != 3 && option != 4 && option != 5)
        {
            pass = false;
            cout << "Invalid input, try again." << endl;
        }
        else
        {
            pass = true;
        }
    }
    while (!pass);

    return option;
}
