#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <random>

const int INF = 10000;
const int SIZE = 10005;

using namespace std;

vector<vector<int>> adjacency_list (SIZE);    //список смежности
vector<int> tin;    //время захода в вершину
vector<int> fup;    //fup[i] - минимум из: tin[i], tin всех обратных рёбер из вершины i, fup всех потомков i в дереве поиска
vector<int> was;    //массив посещённых вершин
int t, n;

vector<pair<int, int>> edges_isthmuses;

void init(){    //инициализирующая функция
    t = 0;
    was.clear();
    tin.clear();
    fup.clear();
    for(int i=0; i<n; i++){
        was.push_back(false);
        tin.push_back(0);
        fup.push_back(0);
    }
}

void dfs(int src, int parent = -1){
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
            dfs(fin, src);    //запускаем dfs из этой вершины
            fup[src] = min(fup[src], fup[fin]);    //обновляем fup
            if (fup[fin] > tin[src])    //если fup смежной вершины больше, чем tin исходной, значит, нет ребра между src и её предками и fin и её потомками
                cout << "!" << src + 1 << " " << fin + 1  << "!"<< endl;
        }
    }
}

void isthmus(){
    init();
    for (int i=0; i<n; i++){
        if (!was[i])
            dfs(i);    //вызываем поиск в глубину для каждой непосещённой вершины
    }
}



void print_adjlist(vector<vector<int>> adjacency_list, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < adjacency_list[i].size(); j++)
            cout << adjacency_list[i][j] << " ";
        cout << endl;
    }
}

void read_adjlist(int n, int m) {
    cin >> n;
    cin >> m;// количество рёбер
    for(int i=0; i<m; i++) {
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

void generate_adjlist(int n, int m) {
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
    }

    /*
    cout << "******" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < adjacency_list[i].size(); j++)
            cout << adjacency_list[i][j] << " ";
        cout << endl;
    }
    cout << "****" << endl;
    */
}


std::vector<std::vector<int>> generateBipartiteGraph(int numVertices, int numEdges) {

    // Инициализация списка смежности
    std::vector<std::vector<int>> adjacencyList(numVertices);

    // Заполняем векторы вершин значениями от 0 до numVertices-1
    std::vector<int> leftVertices(numVertices);
    std::vector<int> rightVertices(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        leftVertices[i] = i;
        rightVertices[i] = i;
    }

    // Используем генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());

    // Перемешиваем вершины в обоих долях
    std::shuffle(leftVertices.begin(), leftVertices.end(), gen);
    std::shuffle(rightVertices.begin(), rightVertices.end(), gen);

    // Генерируем рёбра и записываем в список смежности
    for (int i = 0; i < numEdges; ++i) {
        int leftVertex = leftVertices[i % numVertices];
        int rightVertex = rightVertices[i % numVertices];

        adjacencyList[leftVertex].push_back(rightVertex);
        adjacencyList[rightVertex].push_back(leftVertex);
    }

    return adjacencyList;
}


void test(int count, int step) {
    for (int i = 0; i < count; i += step){
        generate_adjlist(n, n/2);

        auto begin = std::chrono::steady_clock::now();
        isthmus();
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << i << " " << elapsed.count() << " ms" << std::endl;
        for (int j = 0; j < SIZE; j++)
            adjacency_list[j].clear();
    }
}



int main() {
    srand(time(NULL));
    n=5;
    test(6, 1);


    /*

    5
    5
    1 2
    2 3
    1 3
    2 4
    5 4
    */
    return 0;
}


//связные, матрица смежности двудольного графа, хардальные(треангулированные), дополнительные к двудольному, расщепляемые,
//генерация с вероятностью
//способ задания
//посчитать количество перешейков
//описание блоков (поиск блоков)
//
