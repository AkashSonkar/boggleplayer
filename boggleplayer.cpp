#include <set>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>
#include <fstream>
#include "boggleutil.h"
#include "boggleplayer.h"

using namespace std;

/***************
* pre: vector of strings specifying official lexicon to be used for the game
* each word in set is string of lowercase a-z
* This function loads words into efficient data structure to be used internally
*/
void BogglePlayer::buildLexicon( const set<string>& word_list ){
    set<string>::iterator iter;
    LexiconNode *walk;
    lexicon = new Lexicon();
    lexicon->root = new LexiconNode();

    //add all words in the word_list to the multi-way trie
    for( iter = word_list.begin(); iter != word_list.end(); iter++ ){  
        walk = lexicon->root; //set walk to root
        lexicon->addWord( *iter );
        lexicon->size++; //increment size of lexicon
    }
}

void BogglePlayer::preOrder(LexiconNode* node){
    /*    Visit the root. Traverse the left subtree. Traverse the right subtree.*/
    if(node != NULL)
    {
        for(int i = 0; i<26; i++){
            preOrder(node->children[i]);
        }
    }
}


/***************
* pre: const string passed by reference
* This function determines whether string can be found by following acyclic path
* post: vector<int> - specifies locations of each letter in word (R*width+C)
*       empty vector - if NOT possible to form word
                - or if setBoard() has not yet been called
*/
vector<int> BogglePlayer::isOnBoard( const string& word_to_check ){
    BoardNode *cPrev = board->root;
    BoardNode *cWalk = board->root;
    BoardNode *rPrev = board->root;
    BoardNode *rWalk = board->root;
    vector<BoardNode> firstLetters;
    vector<BoardNode> finalNodes;
    vector<int> locations; //vector to return
    unsigned int width = board->width;
    string UPPER_word_to_check = "";
    bool found = false;

    //return empty vector if setBoard() has NOT been called
    if( !board ) return locations;

    //set word_to_check to uppercase
    for( unsigned int i=0; i<word_to_check.length(); i++ )
         UPPER_word_to_check += word_to_check[i] - 32;
    while( cWalk ){
        cWalk = cWalk->neighbors[4]; //walk down
        rWalk = cPrev;
        rPrev = cPrev;
        while( rWalk ){
            rWalk = rWalk->neighbors[6]; //walk right
            if(UPPER_word_to_check[0] == rPrev->letter[0]) //if first letter matches
            firstLetters.push_back( *rPrev );
            rPrev = rWalk;
        }
        cPrev = cWalk;
    }

    for( unsigned int i=0; i<firstLetters.size() && !found; i++ ){ //for each first letter we have
        found = board->neighborSearch( 1, UPPER_word_to_check, finalNodes, firstLetters[i] );
    }

    //write to locations
    for(unsigned int i=0; i<finalNodes.size(); i++){
        locations.push_back( ((finalNodes[i].row)%width)*width + finalNodes[i].col ); //R*width+C
    }
    return locations;
}


/***************
* pre: number of rows
*      number of columns
*      array of arrays of strings representing what is shown on dice in board
* position diceArray[row][column]
* a face can have more than one char (Qu, Sh, Ch)
*/
void BogglePlayer::setBoard( unsigned int rows, unsigned int cols, string** diceArray ){
    BoardNode* boardSkeleton[rows][cols];

    board->width = cols;

    //initialize nodes in array
    for( unsigned int c=0; c<cols; c++ ){
        for( unsigned int r=0; r<rows; r++ ){
            boardSkeleton[r][c] = new BoardNode();
            boardSkeleton[r][c]->letter = diceArray[r][c];
            boardSkeleton[r][c]->row = r;
            boardSkeleton[r][c]->col = c;
        }
    }
    //start assigning pointers
    for( unsigned int c=0; c<cols; c++ ){
        for( unsigned int r=0; r<rows; r++ ){
        if( r!=0 ){//up exists
            boardSkeleton[r][c]->neighbors[3] = (boardSkeleton[r-1][c]);//3
            if( c!=0 )//up and left exists
                boardSkeleton[r][c]->neighbors[0] = (boardSkeleton[r-1][c-1]);//0
            if( c!=cols-1 )//up and right exists
                boardSkeleton[r][c]->neighbors[5] = (boardSkeleton[r-1][c+1]);//5
        }
        if( r!=rows-1 ){//down exists
            boardSkeleton[r][c]->neighbors[4] = (boardSkeleton[r+1][c]);//4
            if( c!=0 )//down and left exists
                boardSkeleton[r][c]->neighbors[2] = (boardSkeleton[r+1][c-1]);//2
            if( c!=cols-1 )//down and right exists
                boardSkeleton[r][c]->neighbors[7] = (boardSkeleton[r+1][c+1]);//7
        }
        if( c!=0 )//left exists
            boardSkeleton[r][c]->neighbors[1] = (boardSkeleton[r][c-1]);//1
        if( c!=cols-1 )//right exists
            boardSkeleton[r][c]->neighbors[6] = (boardSkeleton[r][c+1]);//6
        }
    }
    board->root = boardSkeleton[0][0];
}

/***************
* pre: int - minimum word length
*      pointer to set of strings
* post: false - if setBoard() OR buildLexicon() have NOT yet been called
*       true - if both have been called
* This function fills set with all words that:
* [1] are at least the given minimum length
* [2] are in lexicon specified by most recent call to buildLexicon()
* [3] can be found by following acyclic simple path on board specified
*     by most recent call to setBoard()
*/
bool BogglePlayer::getAllValidWords( unsigned int minimum_word_length,
                                   set<string>* words ){
    if( !( board && lexicon ) ) return false;                  
        searchLexicon( lexicon->root, words, minimum_word_length );
    return true;
}

/***************
* Recursively called by getAllValidWords()
* Traverses lexicon multiway trie in postorder (ABC...XYZN) and
* writes words that appear on the game board to a set
* pre: curr - current LexiconNode
*      words - set of valid words that exist on the board
*      minimum_word_length - bananas in pajamas are coming down the stairs
*/
void BogglePlayer::searchLexicon( LexiconNode *curr, set<string>* &words,
                                unsigned int minimum_word_length ){
    if( !curr ) return; //if node does not exist
    for( int i=0; i<26; i++ ) //for each child pointer
    {
        if( curr->children[i] )  //if node exists at that child pointer
            searchLexicon( curr->children[i], words, minimum_word_length ); //recursively call search on child
    }
    //at a complete word, word is on board, and word meets minimum word length
    if( (curr->end==true) && (!(isOnBoard( curr->word ).empty())) && (curr->word.length()>minimum_word_length) )
    {
        words->insert( curr->word );
    }
    return;
}

/***************
* pre: const string passed by reference
* This function determines whether the parameter is in the lexicon
* post: true - if word is in lexicon
*       false - if word is not in lexicon
           or if buildLexicon() has NOT been called
*/
bool BogglePlayer::isInLexicon( const string& word_to_check ){
    //cout << "INSIDE isInLexicon" << endl;
    LexiconNode *walk = lexicon->root;
    unsigned int i = 0;
    char d;

    if( !lexicon->root ) return false; //if buildLexicon() has NOT been called
    while(true){ //lecture 15, slide 22
        //[2] if walker walked off lexicon OR we've iterated more times than word is long
        if( !walk || i>word_to_check.length() ) return false;
        //[3] if end flag reached and we're at end of word we're checking
        if( (walk->end==true) && i==word_to_check.length() ) return true;
        //[4a] look at value of ith digit in key, let this digit value be d
        d = word_to_check[i];
        //[4b] set walk = walk.children[d]
        walk = walk->children[ ((int)d)-97 ];
        i++; //[5]
    }
}



/***************
* This function is called when "custom" board is called from GUI
* Populates array of arrays of first argument, and ints pointed to by second and third
* NOTE: must allocate memory for first argument
* very similar to setBoard()
*/
void BogglePlayer::getCustomBoard( string** &new_board, unsigned int *rows, unsigned int *cols ){

    setBoard(*rows, *cols, new_board);
    return;
}

/***************
* Deconstructor
* Delete EVERYTHING!
*/
BogglePlayer :: ~BogglePlayer(){
    BoardNode *cPrev = board->root;
    BoardNode *cWalk = board->root;
    BoardNode *rPrev = board->root;
    BoardNode *rWalk = board->root;
    //  LexiconNode *prev, *walk;

    //Free Board                                                                                       took out big loop
    cWalk = cWalk->neighbors[4]; //walk down
    rWalk = cPrev;
    rPrev = cPrev;
    while( rWalk ){
        rWalk = rWalk->neighbors[6]; //walk right
        delete rPrev;
        rPrev = rWalk;
    }
    cPrev = cWalk;
    //end Free Board

    //Free Lexicon
    deleteLexicon( lexicon->root );
}

/***************
* Lexicon Deleter
* Delete the lexicon!
* Uses postorder(ABC...XYZN) traversal
*/
void BogglePlayer :: deleteLexicon( LexiconNode *tar ){
    if( !tar ) return; //if node does not exist
    for( int i=0; i<26; i++ ) //for each tentacle
        if( tar->children[i] )  //if meatball exists at tip of noodly appendage
            deleteLexicon( tar->children[i] ); //eat the meatball
    delete tar; //eat the fork
}
