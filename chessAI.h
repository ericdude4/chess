#include <vector>
#include <stdlib.h>

/*
	The computer chess AI takes in a 2-d char vector array and computes a move. It then returns
	a 2-d char vector array with the new board state after the computers move.
*/

class chessAI {
   public:
	 	std::vector<std::vector<char> > board;
	 	std::vector<std::vector<char> > best_board;
	 	bool terminal_found;

	 	void setBoard(std::vector<std::vector<char> > in) {
	 		board = in;
	 	}

		std::vector<std::vector<char> > getMove(int ply){
			std::vector<std::vector<std::vector<char> > > valid_moves;
			terminal_found = false;
			valid_moves = getValidMoves();
			int heur = minimax(board, 2, -10000, 10000, false);
			std::cout << "heur= " << heur << std::endl;
			//return valid_moves[rand () % valid_moves.size()];
				//return valid_moves[5];
			return best_board;
		}	 	
	private:

		void printBoard(std::vector<std::vector<char> > print) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					std::cout << print[i][j];
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}

		std::vector<std::vector<std::vector<char> > > getValidMoves(){
			std::vector<std::vector<std::vector<char> > > valid_moves;
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {
					if (board[x][y] == 'p' && board[x+1][y] == ' ') {
						board[x+1][y] = 'p';
						board[x][y] = ' ';
						valid_moves.push_back(board);
						board[x+1][y] = ' ';
						board[x][y] = 'p';
						if (x == 1 && board[x+1][y] == ' ' && board[x+2][y] == ' ') {
							board[x+2][y] = 'p';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+2][y] = ' ';
							board[x][y] = 'p';
						}
						if (isupper(board[x+1][y+1])){
							board[x+1][y+1] = 'p';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+1][y+1] = ' ';
							board[x][y] = 'p';
						}
						if (isupper(board[x+1][y-1])){
							board[x+1][y-1] = 'p';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+1][y-1] = ' ';
							board[x][y] = 'p';
						}
					}
					if (board[x][y] == 'h') {
						if (x-1 >= 0 && y-2 >= 0 && !islower(board[x-1][y-2])){ 
							board[x-1][y-2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y-2] = ' ';
							board[x][y] = 'h';
						}
						if (x-1 >= 0 && y+2 < 8 && !islower(board[x-1][y+2])){ 
							board[x-1][y+2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y+2] = ' ';
							board[x][y] = 'h';
						}
						if (x+1 < 8 && y+2 < 8 && !islower(board[x+1][y+2])){
							board[x+1][y+2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+1][y+2] = ' ';
							board[x][y] = 'h';
						}
						if (x+1 < 8 && y-2 >= 0 && !islower(board[x+1][y-2])){
							board[x+1][y-2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+1][y-2] = ' ';
							board[x][y] = 'h';
						}
						if (y-1 >= 0 && x-2 >= 0 && !islower(board[x-1][y-2])){
							board[x-1][y-2] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-1][y-2] = ' ';
							board[x][y] = 'h';
						}
						if (y-1 >= 0 && x-2 >= 0 && !islower(board[x-2][y-1])){
							board[x-2][y-1] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-2][y-1] = ' ';
							board[x][y] = 'h';
						}
						if (y+1 < 8 && x-2 >= 0 && !islower(board[x-2][y+1])){
							board[x-2][y+1] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x-2][y+1] = ' ';
							board[x][y] = 'h';
						}
						if (y+1 < 8 && x+2 < 8 && !islower(board[x+2][y+1])){
							board[x+2][y+1] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+2][y+1] = ' ';
							board[x][y] = 'h';
						}
						if (y-1 >= 0 && x+2 < 8 && !isupper(board[x+2][y-1])){
							board[x+2][y-1] = 'h';
							board[x][y] = ' ';
							valid_moves.push_back(board);
							board[x+2][y-1] = ' ';
							board[x][y] = 'h';
						}
					}
					if (board[x][y] == 'b') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y+i < 8 && board[x+i][y+i] == ' ') {
								board[x+i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = ' ';
								board[x][y] = 'b';
							}
							else if (x+i < 8 && y+i < 8 && isupper(board[x+i][y+i])) {
								board[x+i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = ' ';
								board[x][y] = 'b';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y-i >= 0 && board[x+i][y-i] == ' ') {
								board[x+i][y-i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = ' ';
								board[x][y] = 'b';
							}
							else if (x+i < 8 && y-i >= 0 && isupper(board[x+i][y-i])) {
								board[x+i][y-i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = ' ';
								board[x][y] = 'b';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y+i < 8 && board[x-i][y+i] == ' ') {
								board[x-i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = ' ';
								board[x][y] = 'b';
							}
							else if (x-i >= 0 && y+i < 8 && isupper(board[x-i][y+i])) {
								board[x-i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = ' ';
								board[x][y] = 'b';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y-i >= 0 && board[x-i][y-i] == ' ') {
								board[x-i][y+i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = ' ';
								board[x][y] = 'b';
							}
							else if (x-i >= 0 && y-i >= 0 && isupper(board[x-i][y-i])) {
								board[x-i][y-i] = 'b';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = ' ';
								board[x][y] = 'b';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'r') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && board[x+i][y] == ' '){
								board[x+i][y] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = ' ';
								board[x][y] = 'r';
							}
							else if (x+i < 8 && isupper(board[x+i][y])) {
								board[x+i][y] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = ' ';
								board[x][y] = 'r';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && board[x-i][y] == ' '){
								board[x-i][y] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = ' ';
								board[x][y] = 'r';
							}
							else if (x-i >= 0 && isupper(board[x-i][y])) {
								board[x-i][y] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = ' ';
								board[x][y] = 'r';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y+i < 8 && board[x][y+i] == ' '){
								board[x][y+i] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = ' ';
								board[x][y] = 'r';
							}
							else if (y+i < 8 && isupper(board[x][y+i])) {
								board[x][y+i] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = ' ';
								board[x][y] = 'r';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y-i >= 0 && board[x][y-i] == ' '){
								board[x][y-i] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = ' ';
								board[x][y] = 'r';
							}
							else if (y-i >= 0 && isupper(board[x][y-i])) {
								board[x][y-i] = 'r';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = ' ';
								board[x][y] = 'r';
								break;
							} else break;
						}
					}
					if (board[x][y] == 'q') {
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && board[x+i][y] == ' '){
								board[x+i][y] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = ' ';
								board[x][y] = 'q';
							}
							else if (x+i < 8 && isupper(board[x+i][y])) {
								board[x+i][y] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y] = ' ';
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && board[x-i][y] == ' '){
								board[x-i][y] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = ' ';
								board[x][y] = 'q';
							}
							else if (x-i >= 0 && isupper(board[x-i][y])) {
								board[x-i][y] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y] = ' ';
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y+i < 8 && board[x][y+i] == ' '){
								board[x][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = ' ';
								board[x][y] = 'q';
							}
							else if (y+i < 8 && isupper(board[x][y+i])) {
								board[x][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y+i] = ' ';
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (y-i >= 0 && board[x][y-i] == ' '){
								board[x][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = ' ';
								board[x][y] = 'q';
							}
							else if (y-i >= 0 && isupper(board[x][y-i])) {
								board[x][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x][y-i] = ' ';
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y+i < 8 && board[x+i][y+i] == ' ') {
								board[x+i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = ' ';
								board[x][y] = 'q';
							}
							else if (x+i < 8 && y+i < 8 && isupper(board[x+i][y+i])) {
								board[x+i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y+i] = ' ';
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x+i < 8 && y-i >= 0 && board[x+i][y-i] == ' ') {
								board[x+i][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = ' ';
								board[x][y] = 'q';
							}
							else if (x+i < 8 && y-i >= 0 && isupper(board[x+i][y-i])) {
								board[x+i][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x+i][y-i] = ' ';
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y+i < 8 && board[x-i][y+i] == ' ') {
								board[x-i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = ' ';
								board[x][y] = 'q';
							}
							else if (x-i >= 0 && y+i < 8 && isupper(board[x-i][y+i])) {
								board[x-i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = ' ';
								board[x][y] = 'q';
								break;
							} else break;
						}
						for (int i = 1; i < 8; i ++) {
							if (x-i >= 0 && y-i >= 0 && board[x-i][y-i] == ' ') {
								board[x-i][y+i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y+i] = ' ';
								board[x][y] = 'q';
							}
							else if (x-i >= 0 && y-i >= 0 && isupper(board[x-i][y-i])) {
								board[x-i][y-i] = 'q';
								board[x][y] = ' ';
								valid_moves.push_back(board);
								board[x-i][y-i] = ' ';
								board[x][y] = 'q';
								break;
							} else break;
						}
					}
				}
			}
			return valid_moves;
		}

		//returns the larger of two integers as an integer
		int max (int a, int b) {
			if (a > b) return a;
			else return b;
		}
		//returns the smaller of two integers as an integer
		int min (int a, int b) {
			if (a < b) return a;
			else return b;
		}

		int minimax(std::vector<std::vector<char> > node, int depth, int a, int b, bool maxing){
			if (depth == 0 || terminal_found){
				best_board = node;
				return evaluate(node);
			}
			if (maxing){
				int v = -10000;
				std::vector<std::vector<std::vector<char> > > valid_moves;
				valid_moves = getValidMoves();
				for (int i = 0; i < valid_moves.size(); i++) {
					v = max(v, minimax(valid_moves[i], depth-1, a, b, false));
					a = max(a, v);
					if (b <= a) break; //prune
				}
				return v;
			} else {
				int v = 10000;
				std::vector<std::vector<std::vector<char> > > valid_moves;
				valid_moves = getValidMoves();
				for (int i = 0; i < valid_moves.size(); i++) {
					v = min(v, minimax(valid_moves[i], depth-1, a, b, false));
					b = min(b, v);
					if (b <= a) break;
				}
				return v;
			}
		}

		int evaluate(std::vector<std::vector<char> > state) {
			int result = 0;
			for (int i = 0; i < state.size(); i++) {
				for (int j = 0; j < state[i].size(); j++) {
					if (isupper(state[i][j])) result --;
					else if (islower(state[i][j])) result ++;
				}
			}
		}
};