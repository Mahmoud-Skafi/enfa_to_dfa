
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
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#define IOS                           \
    ios_base::sync_with_stdio(false); \
    cin.tie(0);                       \
    cout.tie(0)
#define rep(i, n) for (int i = 0; i < n; i++)
#define repi(i, j, n) for (int i = j; i < n; i++)
#define rof(i, n) for (int i = n - 1; i >= 0; i--)
#define roff(i, n, f) for (int i = n - 1; i >= f; i--)
#define Hello cerr << "Hello!" << '\n'
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
using namespace std;
#define pb push_back

const int N = 109;
int n, m;
vi nt[N][N], nt1[N][N];
set<int> closure[N];

vi nta[N][N]; // stores the nfa table
int dt[N][N]; // stores the dfa table with entries into ds
vi ds[N];     // stores the label of dfa state (union of nfa states)
int tot;      // total no. of states in dfa
void PrintNFA()
{
    cout << "\n NFA Table:\n";
    cout << "================================\n";
    // NFA transition table
    for (int i = 0; i < n; i++)
    {
        cout << "Q" << i << "\t";
        for (int j = 1; j <= m; j++)
        {
            cout << "[";
            for (int ii : nt1[i][j])
                cout << ii << "";
            cout << "]\t";
        }
        cout << endl;
    }
    cout << endl;
}
void PrintDFA()
{
    cout << "\n DFA Table:\n";
    cout << "===============================\n";
    for (int i = 0; i < tot; i++)
    {
        cout << "Q" << i << "\t";
        cout << "[";
        for (int k = 0; k < ds[i].size(); k++)
            cout << ds[i][k];
        cout << "]\t";

        for (int j = 0; j < m; j++)
        {
            cout << "[";
            for (int k = 0; k < ds[dt[i][j]].size(); k++)
            {
                cout << ds[dt[i][j]][k];
            }
            cout << "]\t";
        }
        cout << endl;
    }
    cout << endl;
}

void ToNFA()
{

    cin >> n; //Number. of states
    cin >> m; //Number. of input symbols

    // Enter Transition table
    for (int i = 0; i < n; i++)
    {
        //Stata number [i]
        for (int j = 0; j <= m; j++)
        {
            int temp;
            cin >> temp; // Number of transitions
            nt[i][j].resize(temp);
            for (int k = 0; k < nt[i][j].size(); k++)
            {
                cin >> nt[i][j][k];
            }
        }
    }

    // Finding epsilon closure for each state
    for (int i = 0; i < n; i++)
    {
        queue<int> q;
        vector<bool> vis(n, false);
        q.push(i);
        vis[i] = 1;
        while (!q.empty())
        {
            int top = q.front();
            q.pop();
            for (int k = 0; k < nt[top][0].size(); k++)
            {
                int cur = nt[top][0][k];
                if (vis[cur] == 0)
                {
                    vis[cur] = 1;
                    q.push(cur);
                }
            }
        }
        for (int j = 0; j < n; j++)
        {
            if (vis[j] == 1)
                closure[i].insert(j);
        }
    }

    // Find epsilon's
    for (int i = 0; i < n; i++)
    {
        for (int ii : closure[i])
        {
            for (int j = 1; j <= m; j++)
            {
                for (int k = 0; k < nt[ii][j].size(); k++)
                {
                    int cur = nt[ii][j][k];
                    for (int iii : closure[cur])
                    {
                        nt1[i][j].push_back(iii);
                    }
                }
            }
        }
    }

    PrintNFA();
}
void ToDFA()
{

    // set no of states ans symbols
    n = 3, m = 2;
    // input the count of transitions
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int sz;
            cin >> sz;
            nta[i][j].resize(sz);
        }
    }

    // input the actual nfa transition table
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            for (int k = 0; k < nta[i][j].size(); k++)
            {
                cin >> nta[i][j][k];
            }
        }
    }

    queue<int> q;

    // add {0} as the initial state
    vector<int> v;
    v.pb(0);
    q.push(0);
    ds[tot++] = v;

    // keep adding new states reachable from initial state
    while (!q.empty())
    {

        int top = q.front();
        q.pop();

        for (int j = 0; j < m; j++)
        {
            vector<int> cur;
            for (int i = 0; i < ds[top].size(); i++)
            {
                for (int k = 0; k < nta[ds[top][i]][j].size(); k++)
                {
                    cur.pb(nta[ds[top][i]][j][k]);
                }
            }

            sort(cur.begin(), cur.end());
            cur.resize(unique(cur.begin(), cur.end()) - cur.begin());

            // check if this state is encountered before
            int prev = -1;
            for (int i = 0; i < tot; i++)
            {
                if (ds[i] == cur)
                {
                    prev = i;
                    break;
                }
            }
            if (prev == -1)
            {
                ds[tot] = cur;
                q.push(tot);
                dt[top][j] = tot;
                tot++;
            }
            else
            {
                dt[top][j] = prev;
            }
        }
    }

    PrintDFA();
}
int main()
{
    freopen("input.in", "r", stdin);
    freopen("output.out", "w", stdout);
    // freopen("inputDFA.out", "w", stdout);

    ToNFA();
    ToDFA();
    fclose(stdout);
    return 0;
}
