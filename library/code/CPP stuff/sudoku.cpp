#include <bits/stdc++.h>
using namespace std;

bool sharesUnit(int r1, int c1, int r2, int c2) {
    return (r1 == r2 || c1 == c2 || (r1 / 3 == r2 / 3 && c1 / 3 == c2 / 3));
}

class SudokuBoard {
private:
    struct Cell {
        int value;
        vector<int> candidates;

        Cell() : value(0), candidates({1,2,3,4,5,6,7,8,9}) {}
    };

    vector<vector<Cell>> board;
    vector<vector<Cell>> pre;

public:
    SudokuBoard(const string& puzzle) : board(9, vector<Cell>(9)) {
        if (puzzle.length() != 81) {
            cerr << "Error: Puzzle must be exactly 81 characters.\n";
            return;
        }
        for (int i = 0; i < 81; ++i) {
            char ch = puzzle[i];
            int row = i / 9;
            int col = i % 9;

            if (ch >= '1' && ch <= '9') {
                setValue(row, col, ch - '0');
            }
            else if (ch == '.') {
                setValue(row, col, 0);
            }
            else {
                cerr << "Warning: Unexpected character '" << ch << "' at position " << i << ". Treated as empty.\n";
                setValue(row, col, 0);
            }
        }
        pre = board;
    }

    void setValue(int row, int col, int val) {
        if (row >= 0 && row < 9 && col >= 0 && col < 9 && val >= 0 && val <= 9) {
            board[row][col].value = val;
            if (val != 0) {
                board[row][col].candidates.clear();
                update(row, col, val);
            }
        }
    }

    void removeCandidate(int row, int col, int val) {
        if (board[row][col].value == 0) {
            vector<int>& candidates = board[row][col].candidates;
            candidates.erase(remove(candidates.begin(), candidates.end(), val), candidates.end());
        }
    }
    
    bool here_Candidate(int row, int col, int val) {
        for (auto v : board[row][col].candidates) {
            if (v == val) {
                return true;
            }
        }
        return false;
    }

    void implementCandidates(set<pair<array<int, 2>, vector<int>>> movesCandidates) {
        for (auto v : movesCandidates) {
            for (auto cand : v.second) {
                removeCandidate(v.first[0], v.first[1], cand);
            }
        }
    }

    int evaluateCandidates(set<pair<array<int, 2>, vector<int>>> movesCandidates) {
        int count = 0;
        for (auto v : movesCandidates) {
            for (auto cand : v.second) {
                if (here_Candidate(v.first[0], v.first[1], cand)) {
                    ++count;
                }
                removeCandidate(v.first[0], v.first[1], cand);
            }
        }
        return count;
    }

    void update(int row, int col, int val) {
        for (int i = 0; i < 9; ++i) {
            removeCandidate(row, i, val);
            removeCandidate(i, col, val);
        }

        int boxStartRow = (row / 3) * 3;
        int boxStartCol = (col / 3) * 3;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                removeCandidate(boxStartRow + i, boxStartCol + j, val);
            }
        }
    }

    friend ostream& operator<<(ostream& os, const SudokuBoard& sb) {
        int count = 0;
        for (int i = 0; i < 9; ++i) {
            if (i % 3 == 0 && i != 0) os << "------+-------+------\n";
            for (int j = 0; j < 9; ++j) {
                if (j % 3 == 0 && j != 0) os << "| ";
                int val = sb.board[i][j].value;
                if (val == 0)
                    os << ". ";
                else {
                    ++count;
                    os << val << ' ';
                }
            }
            os << '\n';
        }
        os << "filled: " << count << "\n";
        return os;
    }
    
    vector<int>& getCandidates(int row, int col) {
        return board[row][col].candidates;
    }

    void printCandidates() {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                Cell cell = board[row][col];
                if (cell.value == 0) {
                    cout << "Cell (" << row + 1 << "," << col + 1 << "): ";
                    for (int c : cell.candidates) {
                        cout << c << " ";
                    }
                    cout << '\n';
                }
            }
        }
    }

    void s81() {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                int val = board[i][j].value;
                if (val == 0)
                    cout << ".";
                else {
                    cout << val;
                }
            }
        }
        cout << endl;
    }

    void store() {
        pre = board;
    }

    void restore() {
        board = pre;
    }

    void implement(set<array<int, 3>> moves) {
        for (auto v : moves) {
            setValue(v[0], v[1], v[2]);
        }
    }

    void printMoves(set<array<int, 3>> moves) {
        cout << "Changes: " << endl;
        for (auto v : moves) {
            cout << "Cell (" << v[0] + 1 << "," << v[1] + 1 << "): " << v[2] << endl;
        }
        cout << "End of changes." << endl;
    }

    void printMovesCandidates(set<pair<array<int, 2>, vector<int>>> moves) {
        cout << "Changes candidates: " << endl;
        for (auto v : moves) {
            cout << "Cell (" << v.first[0] + 1 << "," << v.first[1] + 1 << "): ";
            for (auto cand : v.second) {
                cout << cand << " ";
            }
            cout << endl;
        }
        cout << "End of changes candidates." << endl;
    }

    bool check_conflicts() {
        auto check_unit = [](const vector<int>& unit) {
            vector<bool> seen(10, false);
            for (int val : unit) {
                if (val == 0) {
                    continue;
                }
                if (seen[val]) {
                    return false;
                }
                seen[val] = true;
            }
            return true;
        };
    
        for (int i = 0; i < 9; ++i) {
            vector<int> row, col;
            for (int j = 0; j < 9; ++j) {
                row.push_back(board[i][j].value);
                col.push_back(board[j][i].value);
            }
            if (!check_unit(row) || !check_unit(col)) return false;
        }
    
        // 3x3 boxes
        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                vector<int> box;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        box.push_back(board[boxRow * 3 + i][boxCol * 3 + j].value);
                    }
                }
                if (!check_unit(box)) return false;
            }
        }
    
        return true;
    }
    
    bool is_full() {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                if (board[i][j].value == 0)
                    return false;
        return true;
    }

    bool changing() {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (pre[i][j].value != board[i][j].value) {
                    return true;
                }
            }
        }
        return false;
    }
    
    array<double, 2> total_ave_candidates() {
        int num = 0, count;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j].value == 0) {
                    num += board[i][j].candidates.size();
                    ++count;
                }
                
            }
        }
        return {num, 1.0000*num/count};
    }
    
    double calculateDifficulty() {
        // Define techniques with their names, complexity scores, and impact functions
        vector<tuple<string, int, function<int()>>> techniques = {
            {"last_free_cell", 100, [this]() { return last_free_cell().size(); }},
            {"last_remaining_cell", 100, [this]() { return last_remaining_cell().size(); }},
            {"last_possible_number", 100, [this]() { return last_possible_number().size(); }},
            {"hidden_singles", 200, [this]() { return hidden_singles().size(); }},
            {"obvious_pairs", 350, [this]() { 
                auto pairs = obvious_pairs();
                int impact = 0;
                for (const auto& p : pairs) impact += p.second.size();
                return impact;
            }},
            {"obvious_triples", 400, [this]() { 
                auto triples = obvious_triples();
                int impact = 0;
                for (const auto& t : triples) impact += t.second.size();
                return impact;
            }},
            {"hidden_pairs", 475, [this]() {
                auto hp = hidden_pairs();
                int impact = 0;
                for (const auto& h : hp) impact += h.second.size();
                return impact;
            }},
            {"hidden_triples", 575, [this]() {
                auto ht = hidden_triples();
                int impact = 0;
                for (const auto& h : ht) impact += h.second.size();
                return impact;
            }},
            {"pointing_pairs", 300, [this]() {
                auto pp = pointing_pairs();
                int impact = 0;
                for (const auto& p : pp) impact += p.second.size();
                return impact;
            }},
            {"pointing_triples", 350, [this]() {
                auto pt = pointing_triples();
                int impact = 0;
                for (const auto& p : pt) impact += p.second.size();
                return impact;
            }},
            {"x_wing", 600, [this]() {
                auto xw = x_wing();
                int impact = 0;
                for (const auto& x : xw) impact += x.second.size();
                return impact;
            }},
            {"y_wing", 700, [this]() {
                auto yw = y_wing();
                int impact = 0;
                for (const auto& y : yw) impact += y.second.size();
                return impact;
            }},
            {"swordfish", 800, [this]() {
                auto sf = swordfish();
                int impact = 0;
                for (const auto& s : sf) impact += s.second.size();
                return impact;
            }},
        };

        vector<pair<int, int>> applicable; // Stores (complexity, impact)
        for (const auto& tech : techniques) {
            int impact = get<2>(tech)();
            if (impact > 0) {
                applicable.emplace_back(get<1>(tech), impact);
            }
        }

        int num_techs = applicable.size();
        if (num_techs == 0) return 0.0;

        double sum = 0.0;
        for (const auto& [complexity, impact] : applicable) {
            sum += static_cast<double>(complexity) / (num_techs * impact);
        }

        int total_candidates = 0;
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (board[row][col].value == 0) {
                    total_candidates += board[row][col].candidates.size();
                }
            }
        }

        if (total_candidates == 0) return 0.0;

        return sum / total_candidates;
    }

    //actual solving
    set<array<int, 3>> last_free_cell(bool print = false) {
        set<array<int, 3>> moves;

        for (int row = 0; row < 9; ++row) {
            int last_col = -1, sum = 45, count = 9;
            for (int col = 0; col < 9; ++col) {
                Cell cell = board[row][col];
                if (cell.value == 0) {
                    last_col = col;
                }
                sum -= cell.value;
                --count;
            }
            if (count == 1 && last_col != -1) {
                moves.insert({row, last_col, sum});
            }
        }

        for (int col = 0; col < 9; ++col) {
            int last_row = -1, sum = 45, count = 9;
            for (int row = 0; row < 9; ++row) {
                Cell cell = board[row][col];
                if (cell.value == 0) {
                    last_row = row;
                }
                sum -= cell.value;
                --count;
            }
            if (count == 1 && last_row != -1) {
                moves.insert({last_row, col, sum});
            }
        }
    
        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                int last_row = -1, last_col = -1, sum = 45, count = 9;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        int row = boxRow * 3 + i;
                        int col = boxCol * 3 + j;
                        Cell cell = board[row][col];
                        if (cell.value == 0) {
                            last_row = row;
                            last_col = col;
                        }
                        sum -= cell.value;
                        --count;
                    }
                }
                if (count == 1 && last_row != -1 && last_col != -1) {
                    moves.insert({last_row, last_col, sum});
                }
            }
        }
    
        if (print) {
            printMoves(moves);
        }
        return moves;
    }
    
    set<array<int, 3>> last_remaining_cell(bool print = false) {
        set<array<int, 3>> moves;

        for (int row = 0; row < 9; ++row) {
            for (int num = 1; num <= 9; ++num) {
                int count = 0, last_col = -1;
                for (int col = 0; col < 9; ++col) {
                    Cell cell = board[row][col];
                    if (cell.value == 0 && find(cell.candidates.begin(), cell.candidates.end(), num) != cell.candidates.end()) {
                        ++count;
                        last_col = col;
                    }
                }
                if (count == 1) {
                    moves.insert({row, last_col, num});
                }
            }
        }

        for (int col = 0; col < 9; ++col) {
            for (int num = 1; num <= 9; ++num) {
                int count = 0, last_row = -1;
                for (int row = 0; row < 9; ++row) {
                    Cell cell = board[row][col];
                    if (cell.value == 0 && find(cell.candidates.begin(), cell.candidates.end(), num) != cell.candidates.end()) {
                        ++count;
                        last_row = row;
                    }
                }
                if (count == 1) {
                    moves.insert({last_row, col, num});
                }
            }
        }
    
        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                for (int num = 1; num <= 9; ++num) {
                    int count = 0, last_row = -1, last_col = -1;
                    for (int i = 0; i < 3; ++i) {
                        for (int j = 0; j < 3; ++j) {
                            int row = boxRow * 3 + i;
                            int col = boxCol * 3 + j;
                            Cell cell = board[row][col];
                            if (cell.value == 0 && find(cell.candidates.begin(), cell.candidates.end(), num) != cell.candidates.end()) {
                                ++count;
                                last_row = row;
                                last_col = col;
                            }
                        }
                    }
                    if (count == 1) {
                        moves.insert({last_row, last_col, num});
                    }
                }
            }
        }
    
        if (print) {
            printMoves(moves);
        }
        return moves;
    }
    
    set<array<int, 3>> last_possible_number(bool print = false) { //obvious singles
        set<array<int, 3>> moves;

        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                Cell cell = board[row][col];
                if (cell.value == 0 && cell.candidates.size() == 1) {
                    moves.insert({row, col, cell.candidates[0]});
                }
            }
        }
        if (print) {
            printMoves(moves);
        }
        return moves;
    }

    set<array<int, 3>> hidden_singles(bool print = false) {
        set<array<int, 3>> moves;
        
        auto find_hidden_singles = [&](vector<pair<int, int>> cells) {
            array<vector<pair<int, int>>, 9> cand_num;
            for (auto cell : cells) {
                for (auto cand : board[cell.first][cell.second].candidates) {
                    cand_num[cand - 1].push_back(cell);
                }
            }
            for (int i = 0; i < 9; ++i) {
                if (cand_num[i].size() == 1) {
                    auto [row, col] = cand_num[i][0];
                    moves.insert({row, col, i + 1});
                }
            }
        };

        for (int row = 0; row < 9; ++row) {
            vector<pair<int, int>> cells;
            for (int col = 0; col < 9; ++col) {
                cells.push_back({row, col});
            }
            find_hidden_singles(cells);
        }

        for (int col = 0; col < 9; ++col) {
            vector<pair<int, int>> cells;
            for (int row = 0; row < 9; ++row) {
                cells.push_back({row, col});
            }
            find_hidden_singles(cells);
        }

        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                vector<pair<int, int>> cells;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        int row = boxRow * 3 + i;
                        int col = boxCol * 3 + j;
                        cells.push_back({row, col});
                    }
                }
                find_hidden_singles(cells);
            }
        }

        if (print) {
            printMoves(moves);
        }
        return moves;
    }

    set<pair<array<int, 2>, vector<int>>> obvious_pairs(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> movesCandidates;
        set<tuple<int, int, int, int, vector<int>>> printedPairs;

        auto find_obvious_pairs = [&](vector<pair<int, int>> cells) {
            vector<pair<pair<int, int>, vector<int>>> two_cand_cells;

            for (auto [row, col] : cells) {
                vector<int> cand = board[row][col].candidates;
                if (board[row][col].value == 0 && cand.size() == 2) {
                    two_cand_cells.push_back({{row, col}, cand});
                }
            }

            for (int i = 0; i < two_cand_cells.size(); ++i) {
                for (int j = i + 1; j < two_cand_cells.size(); ++j) {
                    if (two_cand_cells[i].second == two_cand_cells[j].second) {
                        auto [row1, col1] = two_cand_cells[i].first;
                        auto [row2, col2] = two_cand_cells[j].first;
                        vector<int> candidates = two_cand_cells[i].second;
                        tuple<int, int, int, int, vector<int>> currentPair = {
                            row1, col1, row2, col2, candidates};

                        if (printedPairs.find(currentPair) == printedPairs.end()) {
                            printedPairs.insert(currentPair);

                            if (cand) {
                                cout << "Obvious pair at (" << row1 + 1 << "," << col1 + 1 << ") and ("
                                    << row2 + 1 << "," << col2 + 1 << ") with candidates {";
                                for (int m = 0; m < candidates.size(); ++m) {
                                    cout << candidates[m];
                                    if (m != candidates.size() - 1) {
                                        cout << " ";
                                    }
                                };
                                cout << "}\n";
                            }
                        }

                        for (auto [row, col] : cells) {
                            if ((row == row1 && col == col1) || (row == row2 && col == col2)) continue;
                            movesCandidates.insert({{row, col}, candidates});
                        }
                    }
                }
            }
        };

        for (int row = 0; row < 9; ++row) {
            vector<pair<int, int>> cells;
            for (int col = 0; col < 9; ++col) {
                cells.push_back({row, col});
            }
            find_obvious_pairs(cells);
        }

        for (int col = 0; col < 9; ++col) {
            vector<pair<int, int>> cells;
            for (int row = 0; row < 9; ++row) {
                cells.push_back({row, col});
            }
            find_obvious_pairs(cells);
        }

        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                vector<pair<int, int>> cells;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        int row = boxRow * 3 + i;
                        int col = boxCol * 3 + j;
                        cells.push_back({row, col});
                    }
                }
                find_obvious_pairs(cells);
            }
        }

        if (print) {
            printMovesCandidates(movesCandidates);
        }
        return movesCandidates;
    }

    set<pair<array<int, 2>, vector<int>>> obvious_triples(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> movesCandidates;
        set<tuple<int, int, int, int, int, int, vector<int>>> printedTriples;  // to track already printed triples
    
        auto find_obvious_triples = [&](vector<pair<int, int>> cells) {
            vector<pair<pair<int, int>, vector<int>>> three_cand_cells;
    
            for (auto [row, col] : cells) {
                vector<int> cand = board[row][col].candidates;
                if (board[row][col].value == 0 && cand.size() == 3) {
                    three_cand_cells.push_back({{row, col}, cand});
                }
            }
    
            for (int i = 0; i < three_cand_cells.size(); ++i) {
                for (int j = i + 1; j < three_cand_cells.size(); ++j) {
                    for (int k = j + 1; k < three_cand_cells.size(); ++k) {
                        if (three_cand_cells[i].second == three_cand_cells[j].second && three_cand_cells[j].second == three_cand_cells[k].second) {
                            auto [row1, col1] = three_cand_cells[i].first;
                            auto [row2, col2] = three_cand_cells[j].first;
                            auto [row3, col3] = three_cand_cells[k].first;
                            vector<int> candidates = three_cand_cells[i].second;
    
                            // Check if the triple has been printed before
                            tuple<int, int, int, int, int, int, vector<int>> currentTriple = {
                                row1, col1, row2, col2, row3, col3, candidates};
    
                            if (printedTriples.find(currentTriple) == printedTriples.end()) {
                                printedTriples.insert(currentTriple);
    
                                if (cand) {
                                    cout << "Obvious triples at (" << row1 + 1 << "," << col1 + 1 << "), ("
                                        << row2 + 1 << "," << col2 + 1 << ") and ("
                                        << row3 + 1 << "," << col3 + 1 << ") with candidates {";
                                    for (int m = 0; m < candidates.size(); ++m) {
                                        cout << candidates[m];
                                        if (m != candidates.size() - 1) {
                                            cout << " ";
                                        }
                                    }
                                    cout << "}\n";
                                }
    
                                for (auto [row, col] : cells) {
                                    if ((row == row1 && col == col1) || (row == row2 && col == col2) || (row == row3 && col == col3)) continue;
                                    movesCandidates.insert({{row, col}, candidates});
                                }
                            }
                        }
                    }
                }
            }
        };
    
        // Iterate through rows, columns, and 3x3 subgrids
        for (int row = 0; row < 9; ++row) {
            vector<pair<int, int>> cells;
            for (int col = 0; col < 9; ++col) {
                cells.push_back({row, col});
            }
            find_obvious_triples(cells);
        }
    
        for (int col = 0; col < 9; ++col) {
            vector<pair<int, int>> cells;
            for (int row = 0; row < 9; ++row) {
                cells.push_back({row, col});
            }
            find_obvious_triples(cells);
        }
    
        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                vector<pair<int, int>> cells;
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        int row = boxRow * 3 + i;
                        int col = boxCol * 3 + j;
                        cells.push_back({row, col});
                    }
                }
                find_obvious_triples(cells);
            }
        }
    
        if (print) {
            printMovesCandidates(movesCandidates);
        }
        return movesCandidates;
    }
    
    set<pair<array<int, 2>, vector<int>>> hidden_pairs(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> movesCandidates;
        set<tuple<array<array<int, 2>, 2>, array<int, 2>>> seenPairs;
    
        auto find_hidden_pairs = [&](vector<pair<int, int>> cells) {
            array<vector<pair<int, int>>, 9> candidate_cells;
            for (auto [row, col] : cells) {
                for (int cand : board[row][col].candidates) {
                    candidate_cells[cand - 1].push_back({row, col});
                }
            }
    
            for (int i = 0; i < 9; ++i) {
                for (int j = i + 1; j < 9; ++j) {
                    const auto& cells_i = candidate_cells[i];
                    const auto& cells_j = candidate_cells[j];
    
                    if (cells_i.size() == 2 && cells_j.size() == 2 && cells_i == cells_j) {
                        auto [r1, c1] = cells_i[0];
                        auto [r2, c2] = cells_i[1];
    
                        vector<int> pair_cands = {i + 1, j + 1};
    
                        // Only apply if either cell has more than 2 candidates
                        auto& cand1 = board[r1][c1].candidates;
                        auto& cand2 = board[r2][c2].candidates;
    
                        if (cand1.size() <= 2 && cand2.size() <= 2) continue;  // Changed from < 2
    
                        array<array<int, 2>, 2> cell_pair = {{{r1, c1}, {r2, c2}}};
                        if (cell_pair[0] > cell_pair[1]) swap(cell_pair[0], cell_pair[1]);
    
                        array<int, 2> sorted_pair_cands = {pair_cands[0], pair_cands[1]};
                        sort(sorted_pair_cands.begin(), sorted_pair_cands.end());
    
                        auto key = make_tuple(cell_pair, sorted_pair_cands);
                        if (seenPairs.count(key)) continue;
                        seenPairs.insert(key);
    
                        // Determine which candidates to remove
                        vector<int> to_remove1, to_remove2;
                        for (int c : cand1) {
                            if (c != pair_cands[0] && c != pair_cands[1]) {
                                to_remove1.push_back(c);
                            }
                        }
                        for (int c : cand2) {
                            if (c != pair_cands[0] && c != pair_cands[1]) {
                                to_remove2.push_back(c);
                            }
                        }
    
                        // Only add moves if we're actually removing candidates
                        if (!to_remove1.empty()) {
                            movesCandidates.insert({{r1, c1}, to_remove1});
                        }
                        if (!to_remove2.empty()) {
                            movesCandidates.insert({{r2, c2}, to_remove2});
                        }
    
                        if (print) {  // Changed from cand to print to match parameter name
                            cout << "Hidden pair at (" << r1 + 1 << "," << c1 + 1 << ") and ("
                                 << r2 + 1 << "," << c2 + 1 << ") with candidates {"
                                 << pair_cands[0] << " " << pair_cands[1] << "}\n";
                        }
                    }
                }
            }
        };
        return movesCandidates;
    }
    
    set<pair<array<int, 2>, vector<int>>> hidden_triples(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> movesCandidates;
        set<tuple<array<array<int, 2>, 3>, array<int, 3>>> seenTriples; // Track the triplets we've already seen
    
        auto find_hidden_triples = [&](vector<pair<int, int>> cells) {
            array<vector<pair<int, int>>, 9> candidate_cells;
            for (auto [row, col] : cells) {
                for (int cand : board[row][col].candidates) {
                    candidate_cells[cand - 1].push_back({row, col});
                }
            }
    
            // Iterate through each candidate pair
            for (int i = 0; i < 9; ++i) {
                for (int j = i + 1; j < 9; ++j) {
                    for (int k = j + 1; k < 9; ++k) {
                        const auto& cells_i = candidate_cells[i];
                        const auto& cells_j = candidate_cells[j];
                        const auto& cells_k = candidate_cells[k];
    
                        // We need exactly three cells from each of the three candidates
                        if (cells_i.size() == 3 && cells_j.size() == 3 && cells_k.size() == 3 && cells_i == cells_j && cells_j == cells_k) {
                            auto [r1, c1] = cells_i[0];
                            auto [r2, c2] = cells_i[1];
                            auto [r3, c3] = cells_i[2];
    
                            vector<int> triple_cands = {i + 1, j + 1, k + 1};
    
                            // Only apply if the cells have other candidates
                            auto& cand1 = board[r1][c1].candidates;
                            auto& cand2 = board[r2][c2].candidates;
                            auto& cand3 = board[r3][c3].candidates;
    
                            if (cand1.size() <= 3 && cand2.size() <= 3 && cand3.size() <= 3) continue;
    
                            array<array<int, 2>, 3> cell_triple = {{{r1, c1}, {r2, c2}, {r3, c3}}};
                            sort(cell_triple.begin(), cell_triple.end());
    
                            array<int, 3> sorted_triple_cands = {triple_cands[0], triple_cands[1], triple_cands[2]};
                            sort(sorted_triple_cands.begin(), sorted_triple_cands.end());
    
                            auto key = make_tuple(cell_triple, sorted_triple_cands);
                            if (seenTriples.count(key)) continue;
                            seenTriples.insert(key);
    
                            // Determine which candidates to remove
                            vector<int> to_remove1, to_remove2, to_remove3;
                            for (int c : cand1) if (c != triple_cands[0] && c != triple_cands[1] && c != triple_cands[2]) to_remove1.push_back(c);
                            for (int c : cand2) if (c != triple_cands[0] && c != triple_cands[1] && c != triple_cands[2]) to_remove2.push_back(c);
                            for (int c : cand3) if (c != triple_cands[0] && c != triple_cands[1] && c != triple_cands[2]) to_remove3.push_back(c);
    
                            if (!to_remove1.empty())
                                movesCandidates.insert({{r1, c1}, to_remove1});
                            if (!to_remove2.empty())
                                movesCandidates.insert({{r2, c2}, to_remove2});
                            if (!to_remove3.empty())
                                movesCandidates.insert({{r3, c3}, to_remove3});
    
                            if (cand) {
                                cout << "Hidden triple at (" << r1 + 1 << "," << c1 + 1 << "), ("
                                     << r2 + 1 << "," << c2 + 1 << ") and ("
                                     << r3 + 1 << "," << c3 + 1 << ") with candidates {"
                                     << triple_cands[0] << " " << triple_cands[1] << " " << triple_cands[2] << "}\n";
                            }
                        }
                    }
                }
            }
        };
    
        // Iterate through rows, columns, and 3x3 subgrids
        for (int row = 0; row < 9; ++row) {
            vector<pair<int, int>> cells;
            for (int col = 0; col < 9; ++col) cells.push_back({row, col});
            find_hidden_triples(cells);
        }
    
        for (int col = 0; col < 9; ++col) {
            vector<pair<int, int>> cells;
            for (int row = 0; row < 9; ++row) cells.push_back({row, col});
            find_hidden_triples(cells);
        }
    
        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                vector<pair<int, int>> cells;
                for (int i = 0; i < 3; ++i)
                    for (int j = 0; j < 3; ++j)
                        cells.push_back({boxRow * 3 + i, boxCol * 3 + j});
                find_hidden_triples(cells);
            }
        }
    
        if (print) printMovesCandidates(movesCandidates);
    
        return movesCandidates;
    }
    
    set<pair<array<int, 2>, vector<int>>> pointing_pairs(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> moves;
    
        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                for (int num = 1; num <= 9; ++num) {
                    vector<pair<int, int>> positions;
                    bool sameRow = true, sameCol = true;
                    int targetRow = -1, targetCol = -1;

                    // Collect candidate positions in box
                    for (int i = 0; i < 3; ++i) {
                        for (int j = 0; j < 3; ++j) {
                            int row = boxRow * 3 + i;
                            int col = boxCol * 3 + j;
                            if (board[row][col].value == 0 && 
                                find(board[row][col].candidates.begin(), 
                                     board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                                positions.emplace_back(row, col);
                                if (targetRow == -1) targetRow = row;
                                else if (row != targetRow) sameRow = false;
                                if (targetCol == -1) targetCol = col;
                                else if (col != targetCol) sameCol = false;
                            }
                        }
                    }

                    // Handle pointing pairs (exactly 2 cells)
                    if (positions.size() != 2) continue;

                    // Process row alignment
                    if (sameRow) {
                        int row = targetRow;
                        for (int col = 0; col < 9; ++col) {
                            if (col/3 == boxCol) continue; // Skip original box
                            if (board[row][col].value == 0 &&
                                find(board[row][col].candidates.begin(),
                                     board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                                moves.insert({{row, col}, {num}});
                            }
                        }
                        if (cand && !moves.empty()) {
                            cout << "Pointing pair in row " << targetRow+1 
                                 << " for " << num << "\n";
                        }
                    }

                    // Process column alignment
                    if (sameCol) {
                        int col = targetCol;
                        for (int row = 0; row < 9; ++row) {
                            if (row/3 == boxRow) continue; // Skip original box
                            if (board[row][col].value == 0 &&
                                find(board[row][col].candidates.begin(),
                                     board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                                moves.insert({{row, col}, {num}});
                            }
                        }
                        if (cand && !moves.empty()) {
                            cout << "Pointing pair in column " << targetCol+1 
                                 << " for " << num << "\n";
                        }
                    }
                }
            }
        }

        if (print) printMovesCandidates(moves);
        return moves;
    }

    set<pair<array<int, 2>, vector<int>>> pointing_triples(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> moves;
    
        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                for (int num = 1; num <= 9; ++num) {
                    vector<pair<int, int>> positions;
                    bool sameRow = true, sameCol = true;
                    int targetRow = -1, targetCol = -1;

                    // Collect candidate positions in box
                    for (int i = 0; i < 3; ++i) {
                        for (int j = 0; j < 3; ++j) {
                            int row = boxRow * 3 + i;
                            int col = boxCol * 3 + j;
                            if (board[row][col].value == 0 && 
                                find(board[row][col].candidates.begin(), 
                                     board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                                positions.emplace_back(row, col);
                                if (targetRow == -1) targetRow = row;
                                else if (row != targetRow) sameRow = false;
                                if (targetCol == -1) targetCol = col;
                                else if (col != targetCol) sameCol = false;
                            }
                        }
                    }

                    // Handle pointing triples (exactly 3 cells)
                    if (positions.size() != 3) continue;

                    // Process row alignment
                    if (sameRow) {
                        int row = targetRow;
                        for (int col = 0; col < 9; ++col) {
                            if (col/3 == boxCol) continue; // Skip original box
                            if (board[row][col].value == 0 &&
                                find(board[row][col].candidates.begin(),
                                     board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                                moves.insert({{row, col}, {num}});
                            }
                        }
                        if (cand && !moves.empty()) {
                            cout << "Pointing triple in row " << targetRow+1 
                                 << " for " << num << "\n";
                        }
                    }

                    // Process column alignment
                    if (sameCol) {
                        int col = targetCol;
                        for (int row = 0; row < 9; ++row) {
                            if (row/3 == boxRow) continue; // Skip original box
                            if (board[row][col].value == 0 &&
                                find(board[row][col].candidates.begin(),
                                     board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                                moves.insert({{row, col}, {num}});
                            }
                        }
                        if (cand && !moves.empty()) {
                            cout << "Pointing triple in column " << targetCol+1 
                                 << " for " << num << "\n";
                        }
                    }
                }
            }
        }

        if (print) printMovesCandidates(moves);
        return moves;
    }

    set<pair<array<int, 2>, vector<int>>> x_wing(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> moves;

        for (int num = 1; num <= 9; ++num) {
            // Check row-based X-Wing
            map<vector<int>, vector<int>> rowMap; // key: sorted pair of columns, value: rows
            for (int row = 0; row < 9; ++row) {
                vector<int> cols;
                for (int col = 0; col < 9; ++col) {
                    if (board[row][col].value == 0 &&
                        find(board[row][col].candidates.begin(),
                            board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                        cols.push_back(col);
                    }
                }
                if (cols.size() == 2) {
                    sort(cols.begin(), cols.end()); // Ensure columns are sorted
                    rowMap[cols].push_back(row);
                }
            }

            // Process row-based X-Wing candidates
            for (const auto& [cols, rows] : rowMap) {
                if (rows.size() == 2) {
                    // Eliminate 'num' from other rows in these columns
                    for (int col : cols) {
                        for (int row = 0; row < 9; ++row) {
                            if (find(rows.begin(), rows.end(), row) != rows.end()) continue;
                            if (board[row][col].value == 0 &&
                                find(board[row][col].candidates.begin(),
                                    board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                                moves.insert({{row, col}, {num}});
                            }
                        }
                    }
                    if (cand) {
                        cout << "X-Wing (rows) for " << num << " in rows "
                            << rows[0]+1 << "," << rows[1]+1
                            << " and columns " << cols[0]+1 << "," << cols[1]+1 << "\n";
                    }
                }
            }

            // Check column-based X-Wing
            map<vector<int>, vector<int>> colMap; // key: sorted pair of rows, value: columns
            for (int col = 0; col < 9; ++col) {
                vector<int> rows;
                for (int row = 0; row < 9; ++row) {
                    if (board[row][col].value == 0 &&
                        find(board[row][col].candidates.begin(),
                            board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                        rows.push_back(row);
                    }
                }
                if (rows.size() == 2) {
                    sort(rows.begin(), rows.end()); // Ensure rows are sorted
                    colMap[rows].push_back(col);
                }
            }

            // Process column-based X-Wing candidates
            for (const auto& [rows, cols] : colMap) {
                if (cols.size() == 2) {
                    // Eliminate 'num' from other columns in these rows
                    for (int row : rows) {
                        for (int col = 0; col < 9; ++col) {
                            if (find(cols.begin(), cols.end(), col) != cols.end()) continue;
                            if (board[row][col].value == 0 &&
                                find(board[row][col].candidates.begin(),
                                    board[row][col].candidates.end(), num) != board[row][col].candidates.end()) {
                                moves.insert({{row, col}, {num}});
                            }
                        }
                    }
                    if (cand) {
                        cout << "X-Wing (columns) for " << num << " in columns "
                            << cols[0]+1 << "," << cols[1]+1
                            << " and rows " << rows[0]+1 << "," << rows[1]+1 << "\n";
                    }
                }
            }
        }

        if (print) printMovesCandidates(moves);
        return moves;
    }

    set<pair<array<int, 2>, vector<int>>> y_wing(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> moves;
    
        // Step 1: collect all bivalue cells
        vector<array<int, 2>> bivalueCells;
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (board[row][col].value == 0 && board[row][col].candidates.size() == 2) {
                    bivalueCells.push_back({row, col});
                }
            }
        }
    
        // Step 2: test all combinations
        for (auto& pivot : bivalueCells) {
            int pr = pivot[0], pc = pivot[1];
            auto pivotCands = board[pr][pc].candidates;
            int A = pivotCands[0], B = pivotCands[1];
    
            for (auto& p1 : bivalueCells) {
                int r1 = p1[0], c1 = p1[1];
                if ((r1 == pr && c1 == pc) || !sharesUnit(pr, pc, r1, c1)) continue;
                auto& cand1 = board[r1][c1].candidates;
                if (!(cand1[0] == A || cand1[1] == A)) continue;
                int C = (cand1[0] == A) ? cand1[1] : cand1[0];
    
                for (auto& p2 : bivalueCells) {
                    int r2 = p2[0], c2 = p2[1];
                    if ((r2 == pr && c2 == pc) || (r2 == r1 && c2 == c1)) continue;
                    if (!sharesUnit(pr, pc, r2, c2)) continue;
                    auto& cand2 = board[r2][c2].candidates;
                    if (!(cand2[0] == B || cand2[1] == B)) continue;
                    int C2 = (cand2[0] == B) ? cand2[1] : cand2[0];
                    if (C2 != C) continue;
    
                    // Step 3: Only eliminate in cells that share a unit with both pincers
                    for (int r = 0; r < 9; ++r) {
                        for (int c = 0; c < 9; ++c) {
                            if ((r == pr && c == pc) || (r == r1 && c == c1) || (r == r2 && c == c2)) continue;
                            if (board[r][c].value != 0) continue;
    
                            if (!sharesUnit(r, c, r1, c1) || !sharesUnit(r, c, r2, c2)) continue;
    
                            auto& cellCands = board[r][c].candidates;
                            if (find(cellCands.begin(), cellCands.end(), C) != cellCands.end()) {
                                moves.insert({{r, c}, {C}});
                            }
                        }
                    }
    
                    if (cand && !moves.empty()) {
                        cout << "Y-Wing: pivot (" << pr+1 << "," << pc+1 << ") with A=" << A << ", B=" << B
                             << " => pincers at (" << r1+1 << "," << c1+1 << ") and (" << r2+1 << "," << c2+1
                             << ") eliminate " << C << "\n";
                    }
                }
            }
        }
    
        if (print) printMovesCandidates(moves);
        return moves;
    }
    
    set<pair<array<int, 2>, vector<int>>> swordfish(bool print = false, bool cand = false) {
        set<pair<array<int, 2>, vector<int>>> moves;
        
        // Check each digit
        for (int digit = 1; digit <= 9; ++digit) {
            // For rows and columns
            for (int direction = 0; direction < 2; ++direction) {
                vector<vector<int>> positions(9); // Store positions of candidates
                
                // Find positions of candidates in each line
                for (int i = 0; i < 9; ++i) {
                    for (int j = 0; j < 9; ++j) {
                        int row = direction ? j : i;
                        int col = direction ? i : j;
                        
                        if (board[row][col].value == 0 && 
                            find(board[row][col].candidates.begin(), 
                                 board[row][col].candidates.end(), digit) != 
                                 board[row][col].candidates.end()) {
                            positions[i].push_back(direction ? row : col);
                        }
                    }
                }
                
                // Look for three lines with 2-3 candidates each
                for (int i = 0; i < 9; ++i) {
                    if (positions[i].size() >= 2 && positions[i].size() <= 3) {
                        for (int j = i + 1; j < 9; ++j) {
                            if (positions[j].size() >= 2 && positions[j].size() <= 3) {
                                for (int k = j + 1; k < 9; ++k) {
                                    if (positions[k].size() >= 2 && positions[k].size() <= 3) {
                                        // Combine positions from all three lines
                                        set<int> uniquePositions;
                                        for (int pos : positions[i]) uniquePositions.insert(pos);
                                        for (int pos : positions[j]) uniquePositions.insert(pos);
                                        for (int pos : positions[k]) uniquePositions.insert(pos);
                                        
                                        // If we have exactly 3 positions, we found a Swordfish
                                        if (uniquePositions.size() == 3) {
                                            bool found = false;
                                            
                                            // Remove candidates from other cells in these columns/rows
                                            for (int pos : uniquePositions) {
                                                for (int line = 0; line < 9; ++line) {
                                                    if (line != i && line != j && line != k) {
                                                        int row = direction ? pos : line;
                                                        int col = direction ? line : pos;
                                                        
                                                        if (board[row][col].value == 0 && 
                                                            find(board[row][col].candidates.begin(), 
                                                                 board[row][col].candidates.end(), 
                                                                 digit) != board[row][col].candidates.end()) {
                                                            found = true;
                                                            moves.insert({{row, col}, {digit}});
                                                        }
                                                    }
                                                }
                                            }
                                            
                                            if (found && print) {
                                                cout << "Swordfish found for digit " << digit << " in "
                                                     << (direction ? "columns " : "rows ") 
                                                     << i + 1 << ", " << j + 1 << ", " << k + 1 << endl;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        if (print && !moves.empty()) {
            printMovesCandidates(moves);
        }
        
        return moves;
    }
    
};

void debugObviousPairs(SudokuBoard& board) {
    cout << "obvious_pairs: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.obvious_pairs()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}

void debugObviousTriples(SudokuBoard& board) {
    cout << "obvious_triples: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.obvious_triples()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}

void debugHiddenPairs(SudokuBoard& board) {
    cout << "hidden_pairs: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.hidden_pairs()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}

void debugHiddenTriples(SudokuBoard& board) {
    cout << "hidden_triples: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.hidden_triples()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}

void debugPointingPairs(SudokuBoard& board) {
    cout << "pointing_pairs: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.pointing_pairs()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}

void debugPointingTriples(SudokuBoard& board) {
    cout << "pointing_triples: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.pointing_triples()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}

void debugXWing(SudokuBoard& board) {
    cout << "x_wing: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.x_wing()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}

void debugYWing(SudokuBoard& board) {
    cout << "y_wing: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.y_wing()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}

void debugSwordfish(SudokuBoard& board) {
    cout << "swordfish: " << endl;
    cout << "candid: " << board.evaluateCandidates(board.swordfish()) << endl;
    cout << "filled:" << board.last_free_cell().size() + board.last_remaining_cell().size()
         + board.last_possible_number().size() + board.hidden_singles().size() << endl;
}


// Define a vector of all the methods
vector<pair<string, void(*)(SudokuBoard&)>> methods = {
    {"obvious_pairs",      debugObviousPairs},
    {"obvious_triples",    debugObviousTriples},
    {"hidden_pairs",       debugHiddenPairs},
    {"hidden_triples",     debugHiddenTriples},
    {"pointing_pairs",     debugPointingPairs},
    {"pointing_triples",   debugPointingTriples},
    {"x_wing",             debugXWing},
    {"y_wing",             debugYWing},
    {"swordfish",          debugSwordfish}
};


// Helper function to output the results to a CSV file
void writeToCSV(const string& filename, const string& data) {
    ofstream file(filename, ios::app);  // Open the file in append mode
    if (file.is_open()) {
        file << data << endl;
        file.close();
    }
}

// Function to apply a sequence of methods and output the result in CSV format
void applyMethods(SudokuBoard& board, const vector<pair<string, void(*)(SudokuBoard&)>>& methodSequence) {
    board.store();  // Save state

    for (const auto& [name, func] : methodSequence)
        func(board);

    int filledCells = board.last_free_cell().size() +
                      board.last_remaining_cell().size() +
                      board.last_possible_number().size() +
                      board.hidden_singles().size();

    // Calculate candidate statistics
    int totalCandidates = board.total_ave_candidates()[0];

    double avgCandidates = board.total_ave_candidates()[1];

    // Build output string
    string output;
    for (size_t i = 0; i < methodSequence.size(); ++i) {
        output += methodSequence[i].first;
        if (i < methodSequence.size() - 1) output += " -> ";
    }

    output += "," + to_string(filledCells);
    output += "," + to_string(totalCandidates);
    output += "," + to_string(avgCandidates);
    output += "," + to_string(board.calculateDifficulty());

    writeToCSV("sudoku_results.csv", output);

    board.restore();  // Revert state
}


// Function to apply combinations of methods of varying lengths and output results
void applyCombinations(SudokuBoard& board) {
    ofstream ofs("sudoku_results.csv");
    ofs << "Method Sequence,Filled Cells,Total Candidates,Average Candidates per Cell,Difficulty\n";
    ofs.close();


    // Length-1 sequences
    for (int i = 0; i < methods.size(); ++i) {
        vector<pair<string, void(*)(SudokuBoard&)>> seq = {methods[i]};
        applyMethods(board, seq);
    }

    // Length-2 sequences
    for (int i = 0; i < methods.size(); ++i) {
        for (int j = 0; j < methods.size(); ++j) {
            vector<pair<string, void(*)(SudokuBoard&)>> seq = {methods[i], methods[j]};
            applyMethods(board, seq);
        }
    }

    // Length-3 sequences
    for (int i = 0; i < methods.size(); ++i) {
        for (int j = 0; j < methods.size(); ++j) {
            for (int k = 0; k < methods.size(); ++k) {
                vector<pair<string, void(*)(SudokuBoard&)>> seq = {methods[i], methods[j], methods[k]};
                applyMethods(board, seq);
            }
        }
    }
}

int main () {
    freopen("Sudoku.in", "r", stdin);
    ofstream file("sudoku_results.csv");
    string puzzle;
    cin >> puzzle;
    SudokuBoard board(puzzle);
    cout << board;
    board.printCandidates();
    do {
        board.store();
        board.implement(board.last_free_cell(1));
        board.implement(board.last_remaining_cell(1));
        board.implement(board.last_possible_number(1));
        board.implement(board.hidden_singles(1));
    } while (board.changing());
    board.store();
    cout << board;
    writeToCSV("sudoku_results.csv", "Method Sequence, Filled Cells");

    applyCombinations(board);  // Apply combinations and output to CSV

    cout << "Results have been written to sudoku_results.csv" << endl;
    cout << board;
    board.s81();
    cout << board.calculateDifficulty();
    // board.implementCandidates(board.obvious_pairs(1, 1));
    // board.implementCandidates(board.obvious_triples(1, 1));
    // board.implementCandidates(board.hidden_pairs(1, 1));
    // board.implementCandidates(board.hidden_triples(1, 1));
    // board.implementCandidates(board.pointing_pairs(1, 1));
    // board.implementCandidates(board.pointing_triples(1, 1));
    // board.implementCandidates(board.x_wing(1, 1));
    // board.implementCandidates(board.y_wing(1, 1));
    // board.implementCandidates(board.swordfish(1, 1));
    // board.implement(board.last_free_cell(1));
    // board.implement(board.last_remaining_cell(1));
    // board.implement(board.last_possible_number(1));
    // board.implement(board.hidden_singles(1));
}