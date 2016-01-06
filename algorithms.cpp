int minimax(std::vector<std::vector<char> > node, int depth, int a, int b, bool maxing){
			int best_val;
			if (depth == 0){
				/*int node_eval = evaluate(node, 1);
				return node_eval;*/
				best_val = evaluate(node, 1);
			}
			if (maxing){
				best_val = a;
				std::vector<std::vector<std::vector<char> > > valid_moves;
				valid_moves = getValidMoves();
				for (int i = 0; i < valid_moves.size(); i++) {
					int v = minimax(valid_moves[i], depth-1, best_val, b, false);
					//a = max(a, v);
					best_val = max(best_val, v);
					if (b <= best_val) break; //prune
				}
			} else {
				best_val = b;
				std::vector<std::vector<std::vector<char> > > valid_human_moves;
				valid_human_moves = getValidHumanMoves();
				for (int i = 0; i < valid_human_moves.size(); i++) {
					int v = minimax(valid_human_moves[i], depth-1, a, best_val, true);
					best_val = min(best_val, v);
					if (best_val <= a) break;
				}
			}
			return best_val;
		}

		int minimax(std::vector<std::vector<char> > node, int depth, int a, int b, bool maxing){
			if (depth == 0){
				int node_eval;
				if (maxing)
					node_eval = evaluate(node, 0);
				else node_eval = evaluate(node, 1);
				return node_eval;
			}
			if (maxing){
				int v = a;
				std::vector<std::vector<std::vector<char> > > valid_moves;
				valid_moves = getValidMoves(node);
				for (int i = 0; i < valid_moves.size(); i++) {
					v = max(v, minimax(valid_moves[i], depth-1, a, b, false));
					a = max(a, v);
					if (b <= a) break; //prune
				}
				return v;
			} else {
				int v = b;
				std::vector<std::vector<std::vector<char> > > valid_human_moves;
				valid_human_moves = getValidHumanMoves(node);
				for (int i = 0; i < valid_human_moves.size(); i++) {
					v = min(v, minimax(valid_human_moves[i], depth-1, a, b, true));
					b = min(b, v);
					if (b <= a) break;
				}
				return v;
			}


			int negamax(std::vector<std::vector<char> > node, int depth, int a, int b, int player){
			if (depth == 0) {
				return evaluate(node, player)*player;
			}
			int best_val = -30000;
			std::vector<std::vector<std::vector<char> > > valid_moves;
			if (player == -1)
				valid_moves = getValidHumanMoves(node);
			else valid_moves = getValidMoves(node);
			for (int i = 0; i < valid_moves.size(); i++) {
				int val = -negamax(valid_moves[i], depth-1, -a, -b, -player);
				best_val = max(best_val, val);
				a = max(a, val);
				if (a >= b) break;
			}
			return best_val;
		}