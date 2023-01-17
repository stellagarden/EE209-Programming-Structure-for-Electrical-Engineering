// 20190533 Hyemin Lee Assignment1 wc209.c
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

#define START 0
#define GAP 1
#define MAYBE_COMMENT 2
#define COMMENT 3
#define MAYBE_COMMENT_ENDS 4
#define WORD 5
#define END 6
#define ERROR 7

const int SPACE = 0;
const int ENTER = 1;
const int SLASH = 2;
const int STAR = 3;
const int CHAR = 4;
const int N_EOF = 5;

int ch;
int comment_start_l = -1;
int l, w, c=0;  // l # of lines | w # of words | c # of characters
int state = START;
int arrow;
int word_comment = 0;

int determine_arrow(int ch){
// Parameter : ch(input character from stdin) | Return value : The type of input character
// This function will determine what the type of input is. Type is defined as followings.
// (Types : space, enter, slash(/), star(*), EOF, other characters)
// Return value will be regarded as an arrow of DFA.
    if (isspace(ch)){
        if (ch=='\n') return ENTER;
        return SPACE;
    }
    else if (ch=='/') return SLASH;
    else if (ch=='*') return STAR;
    else if (ch==EOF) return N_EOF;
    else return CHAR;
}

int update_state(int arrow, int state){
// Parameter : state(current state), arrow | Return value : state(next state)
// According to my attached DFA, this function will move from current state to next state. Kinds of states are followings.
// (START, GAP, MAYBE_COMMENT, COMMENT, MAYBE_COMMENT_ENDS, WORD, END, ERROR)

// Used global variables : l, w, c, comment_start_l, word_comment
// l,w,c : The number of lines, words, and characters
// comment_start_l : line number of start of comment
// word_comment : If word is directly followed by comment, it's 1. Otherwise, 0.
    switch (state){
        case START:
            c++;
            if (arrow==SPACE) state=GAP;
            else if (arrow==ENTER){
                state=GAP;
                l++;
            }
            else if (arrow==SLASH) state=MAYBE_COMMENT;
            else if (arrow==STAR || arrow==CHAR) state=WORD;
            else if (arrow==N_EOF) {
                state=END;
                c--;
            }
            else fprintf(stderr, "Unknown arrow occur!! : %d", arrow);
            break;

        case GAP:
            c++;
            if (arrow==SPACE) state=GAP;
            else if (arrow==ENTER){
                state=GAP;
                l++;
            }
            else if (arrow==SLASH) state=MAYBE_COMMENT;
            else if (arrow==STAR || arrow==CHAR) state=WORD;
            else if (arrow==N_EOF){
                state=END;
                l++;
                c--;
            }
            else fprintf(stderr, "Unknown ARROW occur!! : %d", arrow);
            break;

        case MAYBE_COMMENT:
            c++;
            if (arrow==SPACE) {
                state=GAP;
                w++;
                word_comment=0;
            }
            else if (arrow==ENTER){
                state=GAP;
                l++;
                w++;
                word_comment=0;
            }
            else if (arrow==SLASH) {
                state=MAYBE_COMMENT;
                word_comment=1;
            }
            else if (arrow==STAR) {
                state=COMMENT;
                comment_start_l=l+1;    //Since current line is not counted yet
                c--;
                if (word_comment) w++;
                word_comment=0;
            }
            else if (arrow==CHAR) {
                state=WORD;
                word_comment=0;
            }
            else if (arrow==N_EOF){
                state=END;
                l++;
                c--;
                w++;
                word_comment=0;
            }
            else fprintf(stderr, "Unknown ARROW occur!! : %d", arrow);
            break;
        
        case COMMENT:
            if (arrow==SPACE || arrow==SLASH || arrow==CHAR) state=COMMENT;
            else if (arrow==ENTER) {
                c++;
                l++;
                state=COMMENT;
            }
            else if (arrow==STAR) state=MAYBE_COMMENT_ENDS;
            else if (arrow==N_EOF) {
                state=ERROR;
                l++;
            }
            else fprintf(stderr, "Unknown ARROW occur!! : %d", arrow);
            break;
                
        case MAYBE_COMMENT_ENDS:
            if (arrow==SPACE || arrow==CHAR) state=COMMENT;
            else if (arrow==ENTER) {
                c++;
                l++;
                state=COMMENT;
            }
            else if (arrow==STAR) state=MAYBE_COMMENT_ENDS;
            else if (arrow==SLASH) state=GAP;
            else if (arrow==N_EOF) {
                state=ERROR;
                l++;
            }
            else fprintf(stderr, "Unknown ARROW occur!! : %d", arrow);
            break;
        
        case WORD:
            c++;
            if (arrow==SPACE) {
                state=GAP;
                w++;
            }
            else if (arrow==ENTER){
                state=GAP;
                w++;
                l++;
            }
            else if (arrow==SLASH){
                state=MAYBE_COMMENT;
                word_comment=1;
            }
            else if (arrow==STAR || arrow==CHAR) state=WORD;
            else if (arrow==N_EOF){
                state=END;
                w++;
                l++;
                c--;
            }
            else fprintf(stderr, "Unknown ARROW occur!! : %d\n", arrow);
            break;
            
        default:
            fprintf(stderr, "Unknown STATE occur!! : %d\n", state);
            break;
        }
    return state;
}

int main(void){
// stdin : ch (input character from stdin) | stdout or stderr : the number of (lines, words, characters) or Error message
// Main function moves around DFA graph. According to ch, arrow is determined. Then state is updated through current state and arrow.
// Updating rule is exactly what DFA indicates.
// Finally, if current state is END or ERROR, main function will print some messages to standard output or error stream.
// And then, return a particular value, as determined.
    do{
        ch = getchar();
        arrow = determine_arrow(ch);
        state = update_state(arrow, state);
    } while(state!=END && state!=ERROR);

    if (state==END){
        printf("%d %d %d\n", l, w, c);
        return EXIT_SUCCESS;
    }
    fprintf(stderr, "Error: line %d: unterminated comment\n", comment_start_l);
    return EXIT_FAILURE;
}