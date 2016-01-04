#ifdef __unix__			//delete this and the following includes to get rid of glut
	#include <GL/freeglut.h>
#elif defined(_WIN32) || defined(WIN32)
	#include <freeglut.h>
#endif
#include <vector>
#include <iostream>
#include "objParser.h"
#include "chessAI.h"
/*#include "vertex.h"
#include "face.h"
#include "normal.h"*/
using namespace std;

struct object {
	std::vector<vertex> v;
	std::vector<normal> n;
	std::vector<face> f;
}; object pawn, king, bishop, knight, rook, queen, square;

struct board_space {
	int x, y;
	bool active;
	bool enpassantable;
	object model;
	char piece;
};

GLfloat mat_ambient[] = {0.0215, 0.1745, 0.0215, 1.0};
GLfloat mat_diffuse_light[] = {0.6992, 0.5078, 0.2617, 1.0};
GLfloat mat_diffuse_dark[] = {0.5078, 0.3398, 0.1484, 1.0};
GLfloat diffuse_selected[] = {1.0, 1.0, 1.0, 1.0};
GLfloat possible_move_colour[] = {1.0, 0.0, 0.0, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = { 90.0 };
GLfloat shiny[] = { 30.0 };

std::vector<std::vector<board_space> > board;
bool a_piece_is_selected = false;
int selected_piece_x, selected_piece_y;
char selected_piece;
std::vector<std::vector<bool> > possible_moves;
chessAI kasparov;

void printBoard(){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++) {
			cout << board[i][j].piece;
		}
		cout << endl;
	}
}

void initBoard(){	//capitol letters represent light pieces
	std::vector<board_space> temp_vector;
	std::vector<bool> temp_bool;
	board_space temp_space;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			temp_space.x = i;
			temp_space.y = j;
			temp_space.piece = ' ';
			temp_space.active = false;
			temp_vector.push_back(temp_space);
			temp_bool.push_back(false);
		}
		possible_moves.push_back(temp_bool);
		board.push_back(temp_vector);
		temp_vector.clear();
		temp_bool.clear();
	}
	board[0][0].piece = 'r';
	board[0][1].piece = 'h';	// h = horse (knight) because king starts with k
	board[0][2].piece = 'b';
	board[0][4].piece = 'k';
	board[0][3].piece = 'q';
	board[0][5].piece = 'b';
	board[0][6].piece = 'h';
	board[0][7].piece = 'r';
	for (int i = 0; i < 8; i++) board[1][i].piece = 'p';
	board[7][0].piece = 'R';
	board[7][1].piece = 'H';	// h = horse (knight) because king starts with k
	board[7][2].piece = 'B';
	board[7][4].piece = 'K';
	board[7][3].piece = 'Q';
	board[7][5].piece = 'B';
	board[7][6].piece = 'H';
	board[7][7].piece = 'R';
	for (int i = 0; i < 8; i++) board[6][i].piece = 'P';
}

void clearPossibleMoves(){
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			possible_moves[i][j] = false;
		}
	}
}

void findIllegalKingMoves() {	//this sets the possible_moves array to false in areas where the king cannot go
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (board[x][y].piece == 'p') {
				if (x+1 < 8 && y+1 < 8) possible_moves[y+1][x+1] = false;
				if (x-1 >= 0 && y+1 < 8) possible_moves[y+1][x-1] = false;
			}
			if (board[x][y].piece == 'b') {
				for (int i = 1; i < 8; i ++) {
					if (x+i < 8 && y+i < 8 && board[x+i][y+i].piece == ' ') possible_moves[y+i][x+i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x+i < 8 && y-i >= 0 && board[x+i][y-i].piece == ' ') possible_moves[y-i][x+i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x-i >= 0 && y+i < 8 && board[x-i][y+i].piece == ' ') possible_moves[y+i][x-i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x-i >= 0 && y-i >= 0 && board[x-i][y-i].piece == ' ') possible_moves[y-i][x-i] = false;
					else break;
				}
			}
			if (board[x][y].piece == 'r') {
				for (int i = 1; i < 8; i ++) {
					if (y+i < 8 && board[x][y+i].piece == ' ') possible_moves[y+i][x] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (y-i >= 0 && board[x][y-i].piece == ' ') possible_moves[y-i][x] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x-i >= 0 && board[x-i][y].piece == ' ') possible_moves[y][x-i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x+i < 8 && board[x+i][y].piece == ' ') possible_moves[y][x+i] = false;
					else break;
				}
			}
			if (board[x][y].piece == 'q') {
				for (int i = 1; i < 8; i ++) {
					if (y+i < 8 && board[x][y+i].piece == ' ') possible_moves[y+i][x] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (y-i >= 0 && board[x][y-i].piece == ' ') possible_moves[y-i][x] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x-i >= 0 && board[x-i][y].piece == ' ') possible_moves[y][x-i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x+i < 8 && board[x+i][y].piece == ' ') possible_moves[y][x+i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x+i < 8 && y+i < 8 && board[x+i][y+i].piece == ' ') possible_moves[y+i][x+i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x+i < 8 && y-i >= 0 && board[x+i][y-i].piece == ' ') possible_moves[y-i][x+i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x-i >= 0 && y+i < 8 && board[x-i][y+i].piece == ' ') possible_moves[y+i][x-i] = false;
					else break;
				}
				for (int i = 1; i < 8; i ++) {
					if (x-i >= 0 && y-i >= 0 && board[x-i][y-i].piece == ' ') possible_moves[y-i][x-i] = false;
					else break;
				}
			}
			if (board[x][y].piece == 'h') {
				if (x-1 >= 0 && y-2 >= 0 && !islower(board[x-1][y-2].piece)) possible_moves[y-2][x-1] = false;
				if (x-1 >= 0 && y+2 < 8 && !islower(board[x-1][y+2].piece)) possible_moves[y+2][x-1] = false;
				if (x+1 < 8 && y+2 < 8 && !islower(board[x+1][y+2].piece)) possible_moves[y+2][x+1] = false;
				if (x+1 < 8 && y-2 >= 0 && !islower(board[x+1][y-2].piece)) possible_moves[y-2][x+1] = false;
				if (y-1 >= 0 && x-2 >= 0 && !islower(board[x-2][y-1].piece)) possible_moves[y-1][x-2] = false;
				if (y+1 < 8 && x-2 >= 0 && !islower(board[x-2][y+1].piece)) possible_moves[y+1][x-2] = false;
				if (y+1 < 8 && x+2 < 8 && !islower(board[x+2][y+1].piece)) possible_moves[y+1][x+2] = false;
				if (y-1 >= 0 && x+2 < 8 && !islower(board[x+2][y-1].piece)) possible_moves[y-1][x+2] = false;
			}
			if (board[x][y].piece == 'k') {
				if (x-1 >= 0 && y-1 >= 0) possible_moves[y-1][x-1] = false;
				if (x-1 >= 0 && y+1 < 8) possible_moves[y+1][x-1] = false;
				if (x+1 < 8 && y-1 >= 0) possible_moves[y-1][x+1] = false;
				if (x-1 >= 0) possible_moves[y][x-1] = false;
				if (x+1 < 8) possible_moves[y][x+1] = false;
				if (y-1 >= 0) possible_moves[y-1][x] = false;
				if (y+1 < 8) possible_moves[y+1][x] = false;
				if (x+1 < 8 && y+1 < 8) possible_moves[y+1][x+1] = false;
			}
		}
	}
}

void getPossibleMoves(int x, int y, char c){
	cout << x << "," << y << endl;
	clearPossibleMoves();
	if (c == 'P') {	
		if (y-1 >= 0 && board[y-1][x].piece == ' ')possible_moves[x][y-1] = true;
		if (y == 6 && board[y-2][x].piece == ' ') possible_moves[x][y-2] = true; //if the pawn has not moved yet
		if (islower(board[y-1][x+1].piece)) possible_moves[x+1][y-1] = true;
		if (islower(board[y-1][x-1].piece)) possible_moves[x-1][y-1] = true;
	}
	if (c == 'H') {
		if (x-1 >= 0 && y-2 >= 0 && !isupper(board[y-2][x-1].piece)) possible_moves[x-1][y-2] = true;
		if (x-1 >= 0 && y+2 < 8 && !isupper(board[y+2][x-1].piece)) possible_moves[x-1][y+2] = true;
		if (x+1 < 8 && y+2 < 8 && !isupper(board[y+2][x+1].piece)) possible_moves[x+1][y+2] = true;
		if (x+1 < 8 && y-2 >= 0 && !isupper(board[y-2][x+1].piece)) possible_moves[x+1][y-2] = true;
		if (y-1 >= 0 && x-2 >= 0 && !isupper(board[y-1][x-2].piece)) possible_moves[x-2][y-1] = true;
		if (y+1 < 8 && x-2 >= 0 && !isupper(board[y+1][x-2].piece)) possible_moves[x-2][y+1] = true;
		if (y+1 < 8 && x+2 < 8 && !isupper(board[y+1][x+2].piece)) possible_moves[x+2][y+1] = true;
		if (y-1 >= 0 && x+2 < 8 && !isupper(board[y-1][x+2].piece)) possible_moves[x+2][y-1] = true;
	}
	if (c == 'R') {
		for (int i = 1; i < 8; i ++) {
			if (x+i < 8 && board[y][x+i].piece == ' ') possible_moves[x+i][y] = true;
			else if (x+i < 8 && islower(board[y][x+i].piece)) {
				possible_moves[x+i][y] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x-i >= 0 && board[y][x-i].piece == ' ') possible_moves[x-i][y] = true;
			else if (x-i >= 0 && islower(board[y][x-i].piece)) {
				possible_moves[x-i][y] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (y+i < 8 && board[y+i][x].piece == ' ') possible_moves[x][y+i] = true;
			else if (y+i < 8 && islower(board[y+i][x].piece)) {
				possible_moves[x][y+i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (y-i >= 0 && board[y-i][x].piece == ' ') possible_moves[x][y-i] = true;
			else if (y-i >= 0 && islower(board[y-i][x].piece)) {
				possible_moves[x][y-i] = true;
				break;
			} else break;
		}
	}
	if (c == 'B') {
		for (int i = 1; i < 8; i ++) {
			if (x+i < 8 && y+i < 8 && board[y+i][x+i].piece == ' ') possible_moves[x+i][y+i] = true;
			else if (x+i < 8 && y+i < 8 && islower(board[y+i][x+i].piece)) {
				possible_moves[x+i][y+i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x+i < 8 && y-i >= 0 && board[y-i][x+i].piece == ' ') possible_moves[x+i][y-i] = true;
			else if (x+i < 8 && y-i >= 0 && islower(board[y-i][x+i].piece)) {
				possible_moves[x+i][y-i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x-i >= 0 && y+i < 8 && board[y+i][x-i].piece == ' ') possible_moves[x-i][y+i] = true;
			else if (x-i >= 0 && y+i < 8 && islower(board[y+i][x-i].piece)) {
				possible_moves[x-i][y+i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x-i >= 0 && y-i >= 0 && board[y-i][x-i].piece == ' ') possible_moves[x-i][y-i] = true;
			else if (x-i >= 0 && y-i >= 0 && islower(board[y-i][x-i].piece)) {
				possible_moves[x-i][y-i] = true;
				break;
			} else break;
		}
	}
	if (c == 'Q') {
		for (int i = 1; i < 8; i ++) {
			if (x+i < 8 && y+i < 8 && board[y+i][x+i].piece == ' ') possible_moves[x+i][y+i] = true;
			else if (x+i < 8 && y+i < 8 && islower(board[y+i][x+i].piece)) {
				possible_moves[x+i][y+i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x+i < 8 && y-i >= 0 && board[y-i][x+i].piece == ' ') possible_moves[x+i][y-i] = true;
			else if (x+i < 8 && y-i >= 0 && islower(board[y-i][x+i].piece)) {
				possible_moves[x+i][y-i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x-i >= 0 && y+i < 8 && board[y+i][x-i].piece == ' ') possible_moves[x-i][y+i] = true;
			else if (x-i >= 0 && y+i < 8 && islower(board[y+i][x-i].piece)) {
				possible_moves[x-i][y+i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x-i >= 0 && y-i >= 0 && board[y-i][x-i].piece == ' ') possible_moves[x-i][y-i] = true;
			else if (x-i >= 0 && y-i >= 0 && islower(board[y-i][x-i].piece)) {
				possible_moves[x-i][y-i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x+i < 8 && board[y][x+i].piece == ' ') possible_moves[x+i][y] = true;
			else if (x+i < 8 && islower(board[y][x+i].piece)) {
				possible_moves[x+i][y] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (x-i >= 0 && board[y][x-i].piece == ' ') possible_moves[x-i][y] = true;
			else if (x-i >= 0 && islower(board[y][x-i].piece)) {
				possible_moves[x-i][y] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (y+i < 8 && board[y+i][x].piece == ' ') possible_moves[x][y+i] = true;
			else if (y+i < 8 && islower(board[y+i][x].piece)) {
				possible_moves[x][y+i] = true;
				break;
			} else break;
		}
		for (int i = 1; i < 8; i ++) {
			if (y-i >= 0 && board[y-i][x].piece == ' ') possible_moves[x][y-i] = true;
			else if (y-i >= 0 && islower(board[y-i][x].piece)) {
				possible_moves[x][y-i] = true;
				break;
			} else break;
		}
	}
	if (c == 'K') {
		int i = 1;
		if (x+i < 8 && y+i < 8 && board[y+i][x+i].piece == ' ') possible_moves[x+i][y+i] = true;
		else if (x+i < 8 && y+i < 8 && islower(board[y+i][x+i].piece)) {
			possible_moves[x+i][y+i] = true;
		}
		if (x+i < 8 && y-i >= 0 && board[y-i][x+i].piece == ' ') possible_moves[x+i][y-i] = true;
		else if (x+i < 8 && y-i >= 0 && islower(board[y-i][x+i].piece)) {
			possible_moves[x+i][y-i] = true;
		}
		if (x-i >= 0 && y+i < 8 && board[y+i][x-i].piece == ' ') possible_moves[x-i][y+i] = true;
		else if (x-i >= 0 && y+i < 8 && islower(board[y+i][x-i].piece)) {
			possible_moves[x-i][y+i] = true;
		}
		if (x-i >= 0 && y-i >= 0 && board[y-i][x-i].piece == ' ') possible_moves[x-i][y-i] = true;
		else if (x-i >= 0 && y-i >= 0 && islower(board[y-i][x-i].piece)) {
			possible_moves[x-i][y-i] = true;
		}
		if (x+i < 8 && board[y][x+i].piece == ' ') possible_moves[x+i][y] = true;
		else if (x+i < 8 && islower(board[y][x+i].piece)) {
			possible_moves[x+i][y] = true;
		}
		if (x-i >= 0 && board[y][x-i].piece == ' ') possible_moves[x-i][y] = true;
		else if (x-i >= 0 && islower(board[y][x-i].piece)) {
			possible_moves[x-i][y] = true;
		}
		if (y+i < 8 && board[y+i][x].piece == ' ') possible_moves[x][y+i] = true;
		else if (y+i < 8 && islower(board[y+i][x].piece)) {
			possible_moves[x][y+i] = true;
		}
		if (y-i >= 0 && board[y-i][x].piece == ' ') possible_moves[x][y-i] = true;
		else if (y-i >= 0 && islower(board[y-i][x].piece)) {
			possible_moves[x][y-i] = true;
		}
		findIllegalKingMoves();
	}
}

void copyCompMoveToMainBoard(vector<vector<char> > char_board) {
	for (int i = 0; i < 8; i ++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].piece = char_board[i][j];
		}
	}
}

void setActive(float x, float y) {	//this sets the location clicked as "active" (currently selected)
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++) {
			board[i][j].active = false;
		}
	}
	int res_x = -5;
	int res_y = -5;
	if (x > -4.5 && x < -3.5)  res_x = -4;
	else if (x > -3.5 && x < -2.5)  res_x = -3;
	else if (x > -2.5 && x < -1.5)  res_x = -2;
	else if (x > -1.5 && x < -0.5)  res_x = -1;
	else if (x > -0.5 && x < 0.5)  res_x = 0;
	else if (x > 0.5 && x < 1.5)  res_x = 1;
	else if (x > 1.5 && x < 2.5)  res_x = 2;
	else if (x > 2.5 && x < 3.5)  res_x = 3;
	if (y > -3.4 && y < -2.6) res_y = -4;
	else if (y > -2.6 && y < -1.9) res_y = -3;
	else if (y > -1.9 && y < -1.1) res_y = -2;
	else if (y > -1.1 && y < -0.3) res_y = -1;
	else if (y > -0.3 && y < 0.4) res_y = 0;
	else if (y > 0.4 && y < 1.2) res_y = 1;
	else if (y > 1.2 && y < 2.0) res_y = 2;
	else if (y > 2.0 && y < 2.8) res_y = 3;

	if (res_x != -5 && res_y != -5) {
		if (!isupper(board[res_y+4][res_x+4].piece) && possible_moves[res_x+4][res_y+4] && a_piece_is_selected){
			board[selected_piece_x][selected_piece_y].piece = ' ';
			board[res_y+4][res_x+4].piece = selected_piece;
			a_piece_is_selected = false;
			clearPossibleMoves();
			glutPostRedisplay();
			//get computer move.
			vector<vector<char> > char_board;
			cout << "--------------------" << endl;
			for (int i = 0; i < 8; i ++){
				vector<char> temp;
				for (int j = 0; j < 8; j++){
					temp.push_back(board[i][j].piece);
					cout << board[i][j].piece;
				} cout << endl;
				char_board.push_back(temp);
				temp.clear();
			}
			cout << "--------------------" << endl;
			kasparov.setBoard(char_board);
			char_board = kasparov.getMove(2);
			copyCompMoveToMainBoard(char_board);
		}
		else if (board[res_y+4][res_x+4].piece != ' ' && isupper(board[res_y+4][res_x+4].piece)){
			getPossibleMoves(res_x+4, res_y+4, board[res_y+4][res_x+4].piece);
			board[res_x+4][res_y+4].active = true;
			a_piece_is_selected = true;
			selected_piece = board[res_y+4][res_x+4].piece;
			selected_piece_x = res_y+4;
			selected_piece_y = res_x+4;
			cout << selected_piece;
		} else {
			clearPossibleMoves();
		}
	} else {
		a_piece_is_selected = false;
	}
}

void mouse(int btn, int state, int x, int y) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float adjusted_x = ((x /(float) width) * 10.0f) - 5.0f;
	float adjusted_y = ((y /(float) height) * 10.0f) - 5.0f;

   if (state == GLUT_DOWN) {
      if (btn==GLUT_LEFT_BUTTON) {
  	  	setActive(adjusted_x, adjusted_y);
  	  	glutPostRedisplay();
      }
      else if (btn == GLUT_RIGHT_BUTTON) {
	  	//do nothing for now
      }
   }
}

void myLightInit() {
   GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat position[] = {1.0, 1.0, 1.0, 0.0};
   GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat local_view[] = {0.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

   glEnable(GL_LIGHTING);   /* turns on lighting */
   glEnable(GL_LIGHT0);     /* turns on light 0  */
   glEnable(GL_NORMALIZE);
}

void initPieces() {
	objParser p;
	char filepawn[] = "objects/pawn.obj";
	p.setFile(filepawn);
	p.parseDatFile();
	pawn.v = p.getVertices(); pawn.n = p.getNormals(); pawn.f = p.getFaces();
	char fileking[] = "objects/king.obj";
	p.setFile(fileking);
	p.parseDatFile();
	king.v = p.getVertices(); king.n = p.getNormals(); king.f = p.getFaces();
	char filequeen[] = "objects/queen.obj";
	p.setFile(filequeen);
	p.parseDatFile();
	queen.v = p.getVertices(); queen.n = p.getNormals(); queen.f = p.getFaces();
	char filebishop[] = "objects/bishop.obj";
	p.setFile(filebishop);
	p.parseDatFile();
	bishop.v = p.getVertices(); bishop.n = p.getNormals(); bishop.f = p.getFaces();
	char fileknight[] = "objects/knightleft.obj";
	p.setFile(fileknight);
	p.parseDatFile();
	knight.v = p.getVertices(); knight.n = p.getNormals(); knight.f = p.getFaces();
	char filerook[] = "objects/rook.obj";
	p.setFile(filerook);
	p.parseDatFile();
	rook.v = p.getVertices(); rook.n = p.getNormals(); rook.f = p.getFaces();
	char filesquare[] = "objects/square.obj";
	p.setFile(filesquare);
	p.parseDatFile();
	square.v = p.getVertices(); square.n = p.getNormals(); square.f = p.getFaces();
}

void drawPiece(object piece, float x, float z) {
	for (int i=0; i < piece.f.size(); ++i) {
		glNormal3f(piece.n[piece.f[i].getB1()].getI(), piece.n[piece.f[i].getB2()].getJ(), piece.n[piece.f[i].getB3()].getK());
		glBegin(GL_POLYGON);  
			glVertex3f(x + piece.v[piece.f[i].getA1()].getX(), piece.v[piece.f[i].getA1()].getY(), z + piece.v[piece.f[i].getA1()].getZ());
			glVertex3f(x + piece.v[piece.f[i].getA2()].getX(), piece.v[piece.f[i].getA2()].getY(), z + piece.v[piece.f[i].getA2()].getZ());
			glVertex3f(x + piece.v[piece.f[i].getA3()].getX(), piece.v[piece.f[i].getA3()].getY(), z + piece.v[piece.f[i].getA3()].getZ());
		glEnd();
	}
}

void drawBoard(){
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
	for (int i = -4; i < 4; i+=2){
		for (int j = -4; j < 4; j+=2){
			if (board[i+4][j+4].active == true)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_selected);
			else if (possible_moves[i+4][j+4] == true)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, possible_move_colour);
			else 
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
			drawPiece(square, i , j);
		}
	}
	for (int i = -3; i < 4; i+=2){
		for (int j = -3; j < 4; j+=2){
			if (board[i+4][j+4].active == true)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_selected);
			else if (possible_moves[i+4][j+4] == true)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, possible_move_colour);
			else 
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
			drawPiece(square, i , j);
		}
	}
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
	for (int i = -3; i < 4; i+=2){
		for (int j = -4; j < 4; j+=2){
			if (board[i+4][j+4].active == true)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_selected);
			else if (possible_moves[i+4][j+4] == true)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, possible_move_colour);
			else 
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
			drawPiece(square, i , j);
		}
	}
	for (int i = -4; i < 4; i+=2){
		for (int j = -3; j < 4; j+=2){
			if (board[i+4][j+4].active == true)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_selected);
			else if (possible_moves[i+4][j+4] == true)
				glMaterialfv(GL_FRONT, GL_DIFFUSE, possible_move_colour);
			else 
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
			drawPiece(square, i , j);
		}
	}
}

void drawPieces() {

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	for (int i = 0; i < 8; i ++) {
		for (int j = 0; j < 8; j++) {
			switch (board[i][j].piece) {
				case 'p':
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
					drawPiece(pawn, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'P' :
				 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
				 	drawPiece(pawn, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'r':
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
					drawPiece(rook, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'R' :
				 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
				 	drawPiece(rook, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'b':
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
					drawPiece(bishop, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'B' :
				 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
				 	drawPiece(bishop, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'q':
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
					drawPiece(queen, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'Q' :
				 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
				 	drawPiece(queen, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'k':
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
					drawPiece(king, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'K' :
				 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
				 	drawPiece(king, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'h':
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_dark);
					drawPiece(knight, board[i][j].y - 4, board[i][j].x - 4);
					break;
				case 'H' :
				 	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_light);
				 	drawPiece(knight, board[i][j].y - 4, board[i][j].x - 4);
					break;
			}
		}
	}

	drawBoard();

	glutSwapBuffers();
}

main(int argc, char **argv) {

	initPieces();
	initBoard();
	printBoard();
	
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Chess by Eric Froese");
	glutMouseFunc(mouse);
	glutDisplayFunc(drawPieces);
	//glutIdleFunc(drawcube); 

	glMatrixMode(GL_PROJECTION);
	glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glRotatef(50.0, 1.0, 0.0, 0.0);
	//glRotatef(30.0, 0.0, 1.0, 0.0); 
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	myLightInit();

	glFrontFace(GL_CCW); 
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glutMainLoop();

	return 0;
}