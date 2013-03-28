#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H

#include "boggleutil.h"
#include "baseboggleplayer.h"
#include <vector>
#include <string>

using namespace std;

/*******************************************************
 *  The base class for any computer Boggle player.
 *  Subclass this, and provide definitions for all the public pure virtual member functions shown.
 *  See the README for detailed documentation on these functions.
 */

class BogglePlayer:public BaseBogglePlayer{
      public:
      Board *board;
      Lexicon *lexicon;

    BogglePlayer(){
        board = new Board();
        lexicon = new Lexicon();
    }

    virtual void buildLexicon(const set<string>& word_list);
    virtual bool getAllValidWords(unsigned int minimum_word_length, set<string>* words);
    virtual bool isInLexicon(const string& word_to_check);
    virtual void getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols);
    virtual vector<int> isOnBoard(const string& word_to_check);
    void searchLexicon(LexiconNode *curr, set<string>* &words, unsigned int minimum_word_length );
    void deleteLexicon( LexiconNode *tar);
    virtual void setBoard(unsigned int rows, unsigned int cols, string** diceArray);
    virtual ~BogglePlayer();
    void preOrder(LexiconNode* tree);
};

#endif //BOGGLEPLAYER_H
