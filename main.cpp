#include <iostream>
#include <vector>
#include <algorithm>

const int INF = 10000;

using namespace std;
/*
int g[1000][1000];
bool was[1000];
int num;

void dfs(int src){
    cout << src + 1 << " ";
    was[src] = true;
    for (int i = 0; i<num; i++){
        if (g[src][i]!=0 && !was[i]){
            dfs(i);
        }
    }
}
*/

vector<vector<int>> adjacency_list (INF);    //список смежности
vector<int> tin;    //время захода в вершину
vector<int> fup;    //fup[i] - минимум из: tin[i], tin всех обратных рёбер из вершины i, fup всех потомков i в дереве поиска
vector<int> was;    //массив посещённых вершин
int t, n;

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
    for(int i=0; i<adjacency_list[src].size(); i++){    //проходимся по всем смежным вершинам src
        int fin = adjacency_list[src][i];    //fin - смежная вершина src
        if (fin == parent)    //если fin родитель src в dfs дереве, то ничего не делаем
            continue;
        if (was[fin] && fin != parent)    //если ребро обратное, то обновляем fup
            fup[src] = min(fup[src], tin[fin]);
        if (!was[fin]) {    //если это ребро dfs дерева, то
            dfs(fin, src);    //запускаем dfs из этой вершины
            fup[src] = min(fup[src], fup[fin]);    //обновляем fup
            if (fup[fin] > tin[src])    //если fup смежной вершины больше, чем tin исходной, значит, нет ребра между src и её предками и fin и её потомками
                cout << src + 1 << " " << fin + 1 << endl;
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

int main()
{
    /*
    cin >> num;
    for (int i=0; i<num; i++){
        for(int j=0; j<num; j++){
            cin >> g[i][j];
        }
    }
    dfs(0);
    */
    cin >> n;
    int m;
    cin >> m;
    for(int i=0; i<m; i++){
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        adjacency_list[a].push_back(b);
        adjacency_list[b].push_back(a);
    }
    isthmus();
    /*
    for(int i=0; i<n; i++) {
        for (int j = 0; j < adjacency_list[i].size(); j++)
            cout << adjacency_list[i][j]+1 << " ";
        cout << endl;
    }
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


//связные, матрица сежности двудольного графа, хардальные(треангулированные), дополнительные к двудольному, расщепляемые,
//генерация с вероятностью
//способ задания
//посчитать количество перешейков
//описание блоков (поиск блоков)
//
