#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include <queue>
#include <ctime>
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <bitset>

using namespace std;

#define fr first
#define sc second
#define mp make_pair
#define pb push_back
#define epr(...) fprintf(stderr, __VA_ARGS__)
#define db(x) cerr << #x << " = " << x << endl
#define db2(x, y) cerr << "(" << #x << ", " << #y << ") = (" << x << ", " << y << ")\n"; 

#define equal equalll
#define less lesss
typedef unsigned long long ull;
const int N = -1;
const int INF = 1e9;
const int T = 64;
const int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
const int dy[8] = {1, -1, 0, 1, -1, 0, 1, -1};
const int REC_H = 2;


int cellCost[8][8] = { 
    {10,  3, 3,  3,  0, 0, 0, 0}, 
    {0, -10, 2, -1,  0, 0, 0, 0}, 
    {0,   0, 2,  1,  0, 0, 0, 0}, 
    {0,   0, 0,  1,  0, 0, 0, 0}, 

    {0, 0, 0, 0,  0, 0, 0, 0}, 
    {0, 0, 0, 0,  0, 0, 0, 0}, 
    {0, 0, 0, 0,  0, 0, 0, 0}, 
    {0, 0, 0, 0,  0, 0, 0, 0} };

bool check(int x, int y) {
    return 0 <= x && x < 8 && 0 <= y && y < 8;
}

struct State {
    ull used;     
    ull white;
    
    int getCntUsed() {
        return __builtin_popcountll(used);
    }
    int getCntWhite() {
        return __builtin_popcountll(white);
    }
    void reverseColor() {
        ull newMask = 0;
        for (int i = 0; i < T; i++) {
            if (((used >> i) & 1) == 1 && ((white >> i) & 1) == 0)
                newMask |= 1ull << i;
        }
        white = used;
    }
    void decode(int data[8][8]) {  /// 1 - white ;; 2 - black
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {
                int t = i * 8 + j;
                if (((used >> t) & 1) == 1) 
                    data[i][j] = (((white >> t) & 1) == 1)? 1: 2;
                else
                    data[i][j] = 0;
            }
    }
    void code(int data[8][8]) {
        used = white = 0;
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {
                int t = i * 8 + j;
                if (data[i][j] != 0) {
                    used |= (1ll << t);
                    if (data[i][j] == 1)
                        white |= (1ll << t);
                }
            }
    }

    bool go(int x, int y, State & res) {
        assert(check(x, y));
        int data[8][8];     
        decode(data);
        if (data[x][y] == 0) return false;
        int cntFlip = 0;
        data[x][y] = 1;
        for (int dir = 0; dir < 8; dir++) {
            int x1 = x + dx[dir];
            int y1 = y + dy[dir];
            int cntBlack = 0;
            for (; check(x1, y1) && data[x1][y1] == 2; x1 += dx[dir], y1 += dy[dir], cntBlack++);
            if (check(x1, y1) && data[x1][y1] == 1) {
                cntFlip += cntBlack;
                x1 = x + dx[dir];
                y1 = y + dy[dir]; 
                for (; check(x1, y1) && data[x1][y1] == 2; x1 += dx[dir], y1 += dy[dir]) 
                    data[x1][y1] = 1;
            }
        }
        if (cntFlip == 0) return false;

        res.code(data);
        return true;
    }
    //void checkState() {
        //assert(getCntWhite() * 2 == getCntUsed());
    //}
    bool isPossibleMove() {
        int data[8][8];
        decode(data);
        int cnt = 0;
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {
                State tmp;
                cnt += go(i, j, tmp);
            }
        return cnt > 0;
    }
};


long long evaluateState(State pos) {
    long long answer = 0;
    int data[8][8];
    pos.decode(data);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (data[i][j] != 0)
                answer += cellCost[i][j] * ((data[i][j] == 1)? 1: -1);
        }
    return answer;
}


State whereBest; 

long long calc(State pos, int h) {
    if (h == REC_H)
        return evaluateState(pos);
    long long result = - INF;
    State bestStep;
    //pos.checkState();

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            State next;
            if (pos.go(i, j, next)) {
                next.reverseColor();
                long long tmp = -calc(next, h + 1);
                if (result < tmp) {
                    result = tmp;
                    bestStep = next;
                }
            }
        }

    if (h == 0) {
        whereBest = bestStep;
    }
    return result;
}


State makeTurn(State pos) {
    bool flagRev = 0; 
    if (pos.getCntWhite() * 2 == pos.getCntUsed() + 1) {
        flagRev = 1;
        pos.reverseColor();    
    }    
    //pos.checkState();

    calc(pos, 0);
    
    if (flagRev) {
        whereBest.reverseColor();                
    }
    return whereBest;
}

void init() {
    for (int i = 0; i < 4; i++)
        for (int j = i; j < 4; j++)
            cellCost[j][i] = cellCost[i][j];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            cellCost[7 - i][7 - j] = cellCost[i][j];
            cellCost[7 - i][j] = cellCost[i][j];
            cellCost[i][7 - j] = cellCost[i][j];
        }
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            assert(cellCost[i][j] != 0);
    for (int i = 0; i < 8; i++, cerr << endl)
        for (int j = 0; j < 8; j++)
            cerr << cellCost[i][j] << " ";

}


State generateStartBoard() {
    int data[8][8];
    memset(data, 0, sizeof(data));
    data[3][3] = 1;
    data[4][4] = 1;
    data[3][4] = 2;
    data[4][3] = 2;

    State tmp;
    tmp.code(data);
    return tmp;
}

void printBoard(State pos) {
    int data[8][8];
    pos.decode(data);
    printf("#");
    for (int i = 0; i < 8; i++)
        printf("%d", i);
    printf("<-y\n");
    for (int i = 0; i < 8; i++) {
        printf("%d", i);
        for (int j = 0; j < 8; j++) {
            if (data[i][j] == 0) 
                printf(".");
            else if (data[i][j] == 1)
                printf("W");
            else
                printf("B");
        }
        printf("\n");
    }
    printf("^\n");
    printf("|\n");
    printf("x\n");

}

void read() {

}


void solve() {
    State pos = generateStartBoard(); 
    for (int turn = 1; ; turn++) {
        printf("=====turn: %d\n", turn);
        printBoard(pos);
        if (!pos.isPossibleMove()) {
            printf("You cann't make turn\n");
        }
        else {
            int x, y;
            bool flag = 1;
            for (;flag;) {
                flag = 0;
                if (scanf("%d %d", &x, &y) != 2) 
                    flag = 1;
                else if (!check(x, y))
                    flag = 1;
                else {
                    State next;  
                    if (!pos.go(x, y, next)) {
                        flag = 1;
                   }
                }
                if (flag)
                    printf("Incorrect turn. Try again\n");         
            }
            
        }


    }
}

void printAns() {

}

void stress() {

}


int main(){
#ifdef DEBUG
    freopen("in", "r", stdin);
    //freopen("out", "w", stdout);
#endif
    if (1) {
        int k = 1;
        for (int tt = 0; tt < k; tt++) {
            read();
            solve();
            printAns();
        }
    }
    else {
        stress();
    }

    return 0;
}

