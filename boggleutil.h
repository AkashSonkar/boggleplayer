#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class BoardNode{
/*    0 3 5
      1   6
      2 4 7    */
    public:
    string letter;
    bool visited;
    BoardNode* neighbors[7];
    int row;
    int col;

    //default constructor
    BoardNode(){
        for(int i = 0; i< 8; i++)
            neighbors[i] = 0;
        visited = false;
    }

    //constructor that sets the data
    BoardNode( string l, int r, int c ){                             //change to string s
        letter = l;
        for(int i = 0; i< 8; i++)
            neighbors[i] = 0;
        visited = false;
        row = r;
        col = c;
    }
};


class LexiconNode{
    public:
    bool end;
    string word;
    int location_on_board;
    LexiconNode* children[26]; //26 pointers

    //default constructor
    LexiconNode(){
        for(int i = 0; i< 26; i++)
            children[i] = 0;
        end = false;
    }

    //constructor which sets the data
    LexiconNode( int loc ){
        for(int i = 0; i<26; i++)
            children[i] = 0;
        end = false;
        location_on_board = loc;
    }
};

class Board{                                                                    //do we need this?
public:
    BoardNode *root;
    int width;
    Board(){root = 0; width=0;}
    bool neighborSearch( unsigned int i, const string& word_to_check, vector<BoardNode> &final, BoardNode home );

};

class Lexicon{
public:
    LexiconNode *root;
    int size;

    Lexicon(){
        root = 0;
        size = 0;
    }
    void addWord(string word);
};

#endif //BOGGLEUTIL_H
