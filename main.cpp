#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <string>

const int INF = 10000;
const int SIZE = 10005;

using namespace std;

vector<int> tin;    //время захода в вершину
vector<int> fup;    //fup[i] - минимум из: tin[i], tin всех обратных рёбер из вершины i, fup всех потомков i в дереве поиска
vector<int> was;    //массив посещённых вершин
int t;

void clear_adjlist (vector<vector<int>>& adjacency_list, int n) {
    for (int j = 0; j < n; j++)
        adjacency_list[j].clear();
}

void init(vector<pair<int, int>>& edges_isthmuses, int n){    //инициализирующая функция
    t = 0;
    was.clear();
    tin.clear();
    fup.clear();
    for(int i = 0; i < n; i++){
        was.push_back(false);
        tin.push_back(0);
        fup.push_back(0);
    }
    edges_isthmuses.clear();
}

void dfs(int src, vector<vector<int>>& adjacency_list, vector<pair<int, int>>& edges_isthmuses, int parent = -1){
    was[src] = true;    //отмечаем, что вершина посещена
    tin[src] = t;    //записываем время захода в вершину
    fup[src] = t;
    t++;
    for(int i = 0; i < adjacency_list[src].size(); i++){    //проходимся по всем смежным вершинам src
        int fin = adjacency_list[src][i];    //fin - смежная вершина src
        if (fin == parent)    //если fin родитель src в dfs дереве, то ничего не делаем
            continue;
        if (was[fin] && fin != parent)    //если ребро обратное, то обновляем fup
            fup[src] = min(fup[src], tin[fin]);
        if (!was[fin]) {    //если это ребро dfs дерева, то
            dfs(fin, adjacency_list, edges_isthmuses, src);    //запускаем dfs из этой вершины
            fup[src] = min(fup[src], fup[fin]);    //обновляем fup
            if (fup[fin] > tin[src]) {   //если fup смежной вершины больше, чем tin исходной, значит, нет ребра между src и её предками и fin и её потомками
                pair<int, int> p;
                p.first = src + 1;
                p.second = fin + 1;
                edges_isthmuses.push_back(p);
            }
        }
    }
}

void isthmus(vector<vector<int>>& adjacency_list, vector<pair<int, int>>& edges_isthmuses, int n){
    init(edges_isthmuses, n);
    for (int i = 0; i < n; i++){
        if (!was[i])
            dfs(i, adjacency_list, edges_isthmuses);    //вызываем поиск в глубину для каждой непосещённой вершины
    }
}



void print_adjlist(vector<vector<int>> adjacency_list, int n) {
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ": ";
        for (int j = 0; j < adjacency_list[i].size(); j++)
            cout << adjacency_list[i][j] + 1 << " ";
        cout << endl;
    }
}

void print_edges_isthmuses(vector<pair<int, int>> edges_isthmuses){
    for (int i = 0; i < edges_isthmuses.size(); i++)
        cout << edges_isthmuses[i].first << " " << edges_isthmuses[i].second << endl;
}

void read_adjlist(vector<vector<int>>& adjacency_list, int n, int m) {
    // m - количество рёбер
    for(int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        adjacency_list[a].push_back(b);
        adjacency_list[b].push_back(a);
    }
}


int GetRandomNumber(int min, int max) {
    int num = min + rand() % (max - min + 1); // Получить случайное число - формула
    return num;
}

void generate_adjlist(vector<vector<int>>& adjacency_list, int n, int m) {
    int tmp[n][n];
    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            i != j ? tmp[i][j] = 0 : tmp[i][j] = 1;
    for (int i = 0; i < m; i++) {
        int a = GetRandomNumber(0, n - 1);
        int b = GetRandomNumber(0, n - 1);
        while (tmp[a][b] != 0) {
            a = GetRandomNumber(0, n - 1);
            b = GetRandomNumber(0, n - 1);
        }
        adjacency_list[a].push_back(b);
        adjacency_list[b].push_back(a);
        tmp[a][b] = 1;
        tmp[b][a] = 1;
    }

}

void generate_bipartite(vector<vector<int>>& adjacency_list, int n, int m) {
    vector<int> l;
    vector<int> r;
    int sizeofl = GetRandomNumber(2, n - 2);
    int sizeofr = n - sizeofl;
    vector<int> flag(n, 0);
    for (int i = 0; i < sizeofl; i++){
        int f;
        do {
            f = GetRandomNumber(0, n - 1);
        } while (flag[f] != 0);
        l.push_back(f);
        flag[f] = 1;
    }
    for (int i = 0; i < n; i++) {
        if (flag[i] == 0)
            r.push_back(i);
    }

    int tmp[n][n];
    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            i != j ? tmp[i][j] = 0 : tmp[i][j] = 1;

    for (int i = 0; i < m; i++) {
        int a, b;
        do {
            a = l[GetRandomNumber(0, sizeofl - 1)];
            b = r[GetRandomNumber(0, sizeofr - 1)];
        } while (tmp[a][b] != 0);
        adjacency_list[a].push_back(b);
        adjacency_list[b].push_back(a);
        tmp[a][b] = 1;
    }
}

void generate_adjlist_pr(vector<vector<int>>& adjacency_list, int n, int probability) {
    int tmp[n][n];
    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            i != j ? tmp[i][j] = 0 : tmp[i][j] = 1;
    int m = n * (n - 1) / 2;
    for (int i = 0; i < m; i++) {
        if (GetRandomNumber(0, 100) < probability) {
            int a = GetRandomNumber(0, n - 1);
            int b = GetRandomNumber(0, n - 1);
            while (tmp[a][b] != 0) {
                a = GetRandomNumber(0, n - 1);
                b = GetRandomNumber(0, n - 1);
            }
            adjacency_list[a].push_back(b);
            adjacency_list[b].push_back(a);
            tmp[a][b] = 1;
            tmp[b][a] = 1;
        }
    }

}

void generate_bipartite_pr(vector<vector<int>>& adjacency_list, int n, int probability) {
    vector<int> l;
    vector<int> r;
    int sizeofl = GetRandomNumber(2, n - 2);
    int sizeofr = n - sizeofl;
    vector<int> flag(n, 0);
    for (int i = 0; i < sizeofl; i++){
        int f;
        do {
            f = GetRandomNumber(0, n - 1);
        } while (flag[f] != 0);
        l.push_back(f);
        flag[f] = 1;
    }
    for (int i = 0; i < n; i++) {
        if (flag[i] == 0)
            r.push_back(i);
    }

    int tmp[n][n];
    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            i != j ? tmp[i][j] = 0 : tmp[i][j] = 1;

    int m = sizeofl * sizeofr;
    for (int i = 0; i < m; i++) {
        if (GetRandomNumber(0, 100) < probability) {
            int a, b;
            do {
                a = l[GetRandomNumber(0, sizeofl - 1)];
                b = r[GetRandomNumber(0, sizeofr - 1)];
            } while (tmp[a][b] != 0);
            adjacency_list[a].push_back(b);
            adjacency_list[b].push_back(a);
            tmp[a][b] = 1;
            tmp[b][a] = 1;
        }
    }
}

void generate_chordal_pr(vector<vector<int>>& adjacency_list, int n, int probability) {
    int tmp[n][n];
    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            i != j ? tmp[i][j] = 0 : tmp[i][j] = 1;

    for (int a = 1; a < n; a++){
        int b;
        do {
            b = GetRandomNumber(0, a - 1);
        } while (tmp[a][b] != 0);
        if (GetRandomNumber(0, 100) < probability){
            for (int i = 0; i < adjacency_list[b].size(); i++){
                if (tmp[a][adjacency_list[b][i]] == 0){
                    adjacency_list[a].push_back(adjacency_list[b][i]);
                    adjacency_list[adjacency_list[b][i]].push_back(a);
                    tmp[a][adjacency_list[b][i]] = 1;
                }
            }
        }
        adjacency_list[a].push_back(b);
        adjacency_list[b].push_back(a);
        tmp[a][b] = 1;
        tmp[b][a] = 1;
    }
}

void generate_split_pr(vector<vector<int>>& adjacency_list, int n, int probability) {
    generate_bipartite_pr(adjacency_list, n, probability);

    vector<int> l;
    vector<int> r;
    vector<int> side_list (n, 0);

    for (int i = 0; i < n; i++) {
        int fl = 0;
        if (side_list[i] == 1)
            fl = 1;
        for (int j = 0; j < adjacency_list[i].size(); j++) {
            side_list[adjacency_list[i][j]] = abs(fl - 1);
        }
    }
    for (int i = 0; i < n; i++) {
        if (side_list[i] == 0) l.push_back(i);
        if (side_list[i] == 1) r.push_back(i);
    }

    int full_side = l.size();
    int f = 0;
    if (GetRandomNumber(0, 1) == 1) {
        full_side = r.size();
        f = 1;
    }
    for (int i = 0; i < full_side; i++) {
        for (int j = 0; j < full_side; j++){
            if (i != j) {
                if (f == 0) adjacency_list[l[i]].push_back(l[j]);
                if (f == 1) adjacency_list[r[i]].push_back(r[j]);
            }
        }
    }
}

void build_additional(vector<vector<int>>& adjacency_list, int n) {
    int tmp[n][n];
    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            i != j ? tmp[i][j] = 0 : tmp[i][j] = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < adjacency_list[i].size(); j++)
            tmp[i][adjacency_list[i][j]] = 1;
    clear_adjlist(adjacency_list, n);
    for (int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if (tmp[i][j] == 0)
                adjacency_list[i].push_back(j);
}


void test(vector<vector<int>>& adjacency_list, vector<pair<int, int>>& edges_isthmuses, int start, int count, int step, int prob_start, int prob_cnt, int prob_step, string func) {
    for (int j = prob_start; j <= prob_cnt; j += prob_step) {
        cout << func << ", probability = " << j << endl;
        for (int i = start; i <= count; i += step) {
            if (func == "adjlist_pr")
                generate_adjlist_pr(adjacency_list, i, j);
            if (func == "bipartite_pr")
                generate_bipartite_pr(adjacency_list, i, j);
            if (func == "bipartite_pr_additional") {
                generate_bipartite_pr(adjacency_list, i, j);
                build_additional(adjacency_list, i);
            }
            if (func == "chordal_pr")
                generate_chordal_pr(adjacency_list, i, j);
            if (func == "split_pr")
                generate_split_pr(adjacency_list, i, j);

            auto begin = std::chrono::steady_clock::now();
            isthmus(adjacency_list, edges_isthmuses, i);
            auto end = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            std::cout << i << " " << edges_isthmuses.size() << " " << elapsed.count() << " microseconds" << std::endl;
            clear_adjlist(adjacency_list, i);
        }
        cout << "__________" << endl << endl;
    }
}

void tests(vector<vector<int>>& adjacency_list, vector<pair<int, int>>& edges_isthmuses, int start, int count, int step, int prob_start, int prob_cnt, int prob_step) {
    test(adjacency_list, edges_isthmuses, start, count, step, prob_start, prob_cnt, prob_step, "adjlist_pr");
    test(adjacency_list, edges_isthmuses, start, count, step, prob_start, prob_cnt, prob_step, "bipartite_pr");
    test(adjacency_list, edges_isthmuses, start, count, step, prob_start, prob_cnt, prob_step, "bipartite_pr_additional");
    test(adjacency_list, edges_isthmuses, start, count, step, prob_start, prob_cnt, prob_step, "chordal_pr");
    test(adjacency_list, edges_isthmuses, start, count, step, prob_start, prob_cnt, prob_step, "split_pr");
}

vector<vector<int>> adjacency_list (SIZE);    //список смежности
vector<pair<int, int>> edges_isthmuses;
int n;

int main() {
    srand(time(NULL));
    //test(adjacency_list, edges_isthmuses, 100, 10);
    n = 5;
    //generate_adjlist(adjacency_list, n, 3);
    //print_adjlist(adjacency_list, n);
    //clear_adjlist(adjacency_list, n);
    //cout << endl;
    //isthmus(adjacency_list, edges_isthmuses, n);
    //test(adjacency_list, edges_isthmuses, 100, 10);

    tests(adjacency_list, edges_isthmuses, 10, 500, 10, 5, 15, 10);
    tests(adjacency_list, edges_isthmuses, 10, 500, 10, 85, 95, 10);
    return 0;
}


//связные, матрица смежности двудольного графа, хардальные(треангулированные), дополнительные к двудольному, расщепляемые,
//генерация с вероятностью
//способ задания
//посчитать количество перешейков
//описание блоков (поиск блоков)
//
