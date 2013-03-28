
#include "boggleutil.h"
#include "boggleplayer.h"

/*******************************************************
* pre: word to be added to the multi-way tree
* addWord is called by buildLexicon
*/
void Lexicon::addWord(string word){
    string newWord; //new word to be assigned to node of last letter
    unsigned int letterFix = 0;
    LexiconNode *newNode;
    LexiconNode *walk = root;

    //for the length of the word
    for( unsigned int i=0; i<word.length(); i++ ){
        //addLetter( word[i], walk, newWord );
        if( (int)word[i]<=90 && (int)word[i]>=65 ) //set letterFix
            letterFix=65; //if capital letter
        else letterFix=97; //if lowercase letter
        //if letter has not been created yet, add new letter
        if(walk->children[ ((int)word[i])-letterFix ] == 0){ //if child does not exist at that location
            newNode = new LexiconNode();
            walk->children[ ((int)word[i])-letterFix ] = newNode; //add new node to location
            walk = walk->children[ ((int)word[i])-letterFix ]; //move walker to location
        }
        else
            walk = walk->children[ ((int)word[i])-letterFix ]; //move walker to location
    }
    //cout << "AFTER EACH WORD ==========" << endl;
    walk->end = true;
    walk->word = word; //assign the word to leaf
}

/*******************************************************
* This function is called from isOnBoard
* Fills vector<BoardNode> final with nodes of found word
* pre: i - the position of the letter we're looking at
*      word_to_check - the word we're finding on the board
*      final - the vector we're filling to be returned at the end
*      home - the BoardNode we were given
* post: final is filled
* return: true - complete word is found
*         false - complete word is not found
*/
bool Board::neighborSearch( unsigned int i, const string& word_to_check, vector<BoardNode> &final, BoardNode home ){
    unsigned int n = 0;
    bool found = false;
    home.visited = true;
    unsigned int newi, newn;

    if( i < word_to_check.length() ){
        for( n=0; n<8 && !found; n++ ){ //for each neighbor
            //if letters match (neighbor is good) and the neighbor has not been visited and found flag is not set
            if( home.neighbors[n] ){
                string tempString( 1, word_to_check[i] );
          
                if( !(home.neighbors[n]->letter.compare( tempString ))){
                    if( !home.neighbors[n]->visited ){
                        if( !found ){
                            //call again with that good neighbor
                            newi = i+1;
                            found = neighborSearch(newi, word_to_check, final, *home.neighbors[n] );
                        }
                    } //TODO: board is bad, or width is lying
                } //TODO: strings are not being compared correctly
            } //TODO: going into above mess even if condition for i is not met
        } //TODO: should we check for length or length-1
    } //TODO: right of last thing in row is first of next column

    if( i == word_to_check.length() ){
        found = true;
    }
    if( found ){ //if complete word is found
        newn = n-1;
        final.push_back( /***/home/*.neighbors[newn]*/ ); //store correct word into final
        return true;
    }

    home.visited = false;
    return false;
}

