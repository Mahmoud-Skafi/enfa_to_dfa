/**
 * @Author: Mahmoud Skafi
 * @Github:https://github.com/Mahmoud-Skafi/enfa_to_dfa
 * @Conversion (e-NFA) to DFA
**/
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
using namespace std;

#define int long long
#define debug(x) cout << "[" << #x << " is: " << x << "] " << endl;
#define endl '\n'
#define rep(i, n) for (int i = 0; i < n; i++)
#define repi(i, j, n) for (int i = j; i < n; i++)
#define rof(i, n) for (int i = n - 1; i >= 0; i--)
#define roff(i, n, f) for (int i = n - 1; i >= f; i--)
#define Hello cerr << "Hello!" << '\n'
#define pb push_back
typedef long long ll;
typedef unsigned long long llu;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<llu> vllu;
typedef vector<vi> vvi;
typedef pair<int, int> pii;
typedef tuple<int, int, int> tiii;

const int maxn = 1e5 + 5;
int mod = 1000000007;
int INF = 1e18;

vector<vector<vector<int>>> graph; //Node , Symbol, To
vector<bool> vis;                  // Visited or not
int states;                        //number of state
string symbols;                    //array of symbols

vector<vector<vector<int>>> NFA, DFA;
//DFS Graph
void dfs(int state, int symbol, int startstate, bool firstcall)
{
    //check if the first node (start state q0) is called or not
    if (!firstcall)
    {
        vis[state] = true;
        NFA[startstate][symbol].push_back(state);
    }
    //add all transition with epsilon
    for (int i = 0; i < graph[state][symbol].size(); i++)
        if (vis[graph[state][symbol][i]] == false)
            dfs(graph[state][symbol][i], symbol, startstate, false);
    //add all transition without the epsilon
    for (int i = 0; i < graph[state][symbols.size() - 1].size(); i++)
        if (vis[graph[state][symbols.size() - 1][i]] == false)
            dfs(graph[state][symbols.size() - 1][i], symbol, startstate, false);
}

void print_NFA()
{
    cout << "NFA Table:\n";
    cout << "================================\n";
    for (int i = 0; i < states; i++)
    {

        for (int j = 0; j < symbols.size() - 1; j++)
        {
            int state = i;
            cout << "(q" + to_string(state) + ", " + symbols[j] + "): ";
            for (int k = 0; k < NFA[i][j].size(); k++)
                cout << NFA[i][j][k] << ' ';
            cout << endl;
        }
    }
    cout << endl;
}

void print_DFA()
{
    cout << "DFA Table:\n";
    cout << "================================\n";

    for (int i = 0; i < DFA.size(); i++)
    {
        for (int j = 0; j < DFA[i].size(); j++)
        {
            int state = i;
            cout << "({q";
            bool first = true;
            for (int h = 0; h < NFA.size(); h++)
            {

                //left shift
                if ((1ll << h) & i)
                {
                    if (!first)
                        cout << ' ';
                    first = false;
                    cout << h;
                }
            }
            cout << "}, " << symbols[j] << "): ";
            for (int k = 0; k < DFA[i][j].size(); k++)
            {
                cout << '{';
                first = true;
                for (int h = 0; h < NFA.size(); h++)
                {
                    if ((1ll << h) & DFA[i][j][k])
                    {
                        if (!first)
                            cout << ' ';
                        first = false;
                        cout << h;
                    }
                }
                cout << '}';
            }
            cout << endl;
        }
    }
}

int32_t main()
{
    freopen("input.in", "r", stdin);
    freopen("output.out", "w", stdout);
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);

    cin >> states;  // number of state
    cin >> symbols; // array of string symbols
    symbols += '$'; //add epsilon to the symbols array and $ mean  epsilon
    int edges;
    cin >> edges; // input the transtion table for the e-nfa

    graph.assign(states, vector<vector<int>>(symbols.size())); //the graph (Q,E)
    while (edges--)
    {
        /*build the graph for e-nfa */
        int from, to;
        char symbol;
        cin >> from >> to >> symbol; //input the transtion for all state
        int symbolindex = -1;        // remove epsilon
        for (int i = 0; i < symbols.size(); i++)
        {
            if (symbols[i] == symbol)
            {
                symbolindex = i;
                break;
            }
        }

        graph[from][symbolindex].push_back(to);
    }
    /*
    After we building the e-nfa graph we bulid the nfa graph without the epsilon using epsilon closure.
    Epsilon closure for a given state X is a set of states which can be reached from the states X with 
    only (null) or ε moves including the state X itself
    */
    NFA.assign(states, vector<vector<int>>(symbols.size() - 1)); //NFA(Q*,E) without the epsilon
    for (int i = 0; i < states; i++)
    {
        for (int j = 0; j < symbols.size() - 1; j++)
        {
            int state = i;
            int symbolindex = j;
            vis.assign(states, false);
            dfs(state, symbolindex, state, true);
        }
    }

    print_NFA();

    /*
    number of state is 2^n and the symbols will remain the same.
    1ll:left shift.
    */
    DFA.assign(1ll << states, vector<vector<int>>(symbols.size() - 1)); //DFA(Q**,E)

    for (int i = 0; i < DFA[0].size(); i++) //for Φ case
        DFA[0][i].push_back(0);

    for (int i = 1; i < DFA.size(); i++)
    {
        for (int j = 0; j < DFA[i].size(); j++)
        {
            set<int> result;
            for (int k = 0; k < states; k++) //bit masking
            {
                int val = (1ll << k) & i;
                if (val)
                {
                    for (int h = 0; h < NFA[k][j].size(); h++)
                        result.insert(NFA[k][j][h]);
                }
            }

            int x = 0;
            for (auto it : result)
                x += 1ll << it;

            DFA[i][j].push_back(x);
        }
    }
    print_DFA();
    fclose(stdout);
    return 0;
}