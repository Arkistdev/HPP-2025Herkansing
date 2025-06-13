#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <thread>
#include <algorithm>
#include <ctime>

using namespace std;

int getabsmaxelement(vector<int> input) // 2n + 1
{
    int res = abs(input.front()); // 1 lookup
    for (int i : input) // 2n
    {
        if (abs(i) > res) // 1 comparison
        {
            res = abs(i); // 1 arithmatic operation
        }
    }
    return res;
}

int getdigit(int input, int digit) // 2 + 2 * digit
{
    input = abs(input); // 1 arithmatic operation
    while(digit > 0) // 2* digit
    {
        input = (int)input / 10; // 1 arithmatic operation

        digit--; // 1 decrement
    }

    return input % 10; // 1 arithmatic operation
}

vector<vector<int>> distribution(vector<int> input, int depth) // 2 depth + 3
{
    vector<vector<int>> dist(10);
    
    for(int i : input) // 2 depth + 3 
    {
        dist[getdigit(i, depth)].push_back(i); // 1 write, 2 + 2(number of digits) 
    }

    return dist;
}

vector<int> gathering(vector<vector<int>> input) // n
{
    vector<int> res;
    for (vector<int> i : input) // n
    {
        for (int j : i) // i 
        {   
            res.push_back(j); // 1 write
        }
    }

    return res;
}

vector<vector<int>> split0(vector<int> lst) // 2i
{
    vector<int> pos;
    vector<int> neg;
    for (auto i : lst) // 2i
    {
        if (i >= 0) // 1 comparison
        {
            pos.push_back(i); // 1 write
        }
        else
        {
            neg.push_back(i); // 1 write
        }
    }
    return {pos, neg};
}


vector<int> bucket_sort(vector<int> input) // 1 + 2n + 2 + ((range 0 to maxdigits)+ 2i + 3 + n) + n = ((range 0 to maxdigits) + 2i + 4 + n) + 3n + 3
{
    if (input.size() == 0) // 1 comparison
    {
        return {};
    }

    int depth = 0;
    int m = round(log10(getabsmaxelement(input))); // 2n + 2 arithmatic operation
    vector<vector<int>> dist;

    while (depth <= m) // (2 * (range 0 to maxdigits) + 3 + n) + n
    {
        // cout << "depth " << depth << "(";
        // for(auto i : input)
        // {
        //     cout << i <<",";
        // }
        // cout << ")" << endl;

        dist = distribution(input, depth); // 2 depth + 3
        input = gathering(dist); // n

        depth ++; // increment
    }
    return input;
}

void appendlist(vector<int> &lst1, vector<int> lst2) // n
{
    for (int i : lst2)
    {
        lst1.push_back(i); // 1 write
    }
}

vector<int> bucket_sort_negative(vector<int> input) // 3n + ((range 0 to maxdigits) + 2i + 4 + n) + 3n + 6 + a 
{
    auto split = split0(input); // 2n

    vector<int> pos = bucket_sort(split[0]); // ((range 0 to maxdigits) + 2i + 4 + n) + 3n + 3
    vector<int> neg = bucket_sort(split[1]); // 3

    reverse(neg.begin(), neg.end()); // ik weet niet zo goed wat de complexiteit is van de std::reverse functie, ook na opzoeken van de broncode was het voor mij niet duidelijk dus ik gebruik de letter a als de complexiteit van dit algoritme
    appendlist(neg, pos); // n

    return neg;
}

void tests()
{
    // getmaxelement
    cout << "getmax element tests" << endl;
    cout << "test getabsmaxelement({1, 2, 3, 4, 5, 6}) == 6: " << (getabsmaxelement({1, 2, 3, 4, 5, 6}) == 6) << endl;
    cout << "test getabsmaxelement({32, 2135, 75, 12, 3}) == 2135: " << (getabsmaxelement({32, 2135, 75, 12, 3}) == 2135) << endl;
    cout << endl;
    
    // getdigit
    cout << "get digit tests" << endl;
    cout << "test getdigit(4213215, 3) == 3: " << (getdigit(4213215, 3) == 3) << endl;
    cout << "test getdigit(0, 2) == 0: " << (getdigit(0, 2) == 0) << endl;
    cout << endl;
    // distribution
    cout << "distribution tests" << endl;
    cout << "distribution({97, 3, 100}, 0) == vector<vector<int>>{{100}, {}, {}, {3}, {}, {}, {}, {97}, {}, {}}: " << (distribution({97, 3, 100}, 0) == vector<vector<int>>{{100}, {}, {}, {3}, {}, {}, {}, {97}, {}, {}}) << endl;
    cout << "test distribution({1, 2, 3, 4, 5, 6, 7, 8, 9, 0}, 0) == vector<vector<int>>{{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}}: " << (distribution({1, 2, 3, 4, 5, 6, 7, 8, 9, 0}, 0) == vector<vector<int>>{{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}})<< endl;
    cout << "test distribution({23, 154, 642, 123, 583, 126, 800}, 2) == vector<vector<int>>{{23}, {154, 123, 126}, {}, {}, {}, {583}, {642}, {}, {800}, {}}: " << (distribution({23, 154, 642, 123, 583, 126, 800}, 2) == vector<vector<int>>{{23}, {154, 123, 126}, {}, {}, {}, {583}, {642}, {}, {800}, {}})<< endl;

    // gathering
    cout << "gathering tests" << endl;
    cout << "test gathering({{100}, {}, {}, {3}, {}, {}, {}, {97}, {}, {}}) == vector<int>{100, 3, 97}: " << (gathering({{100}, {}, {}, {3}, {}, {}, {}, {97}, {}, {}}) == vector<int>{100, 3, 97}) << endl;
    cout << "test gathering({{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}}) == vector<int>{9, 8, 7, 6, 5, 4, 3, 2, 1, 0}: " << (gathering({{0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}}) == vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) << endl;
    cout << "test gathering({{23}, {154, 123, 126}, {}, {}, {}, {583}, {642}, {}, {800}, {}}) == vector<int>{23, 154, 123, 126, 583, 642, 800}: " << (gathering({{23}, {154, 123, 126}, {}, {}, {}, {583}, {642}, {}, {800}, {}}) == vector<int>{23, 154, 123, 126, 583, 642, 800}) << endl;
    cout << endl;

    // bucket_sort
    cout << "bucked sort tests" << endl;
    cout << "bucket_sort({97, 3, 100}) == vector<int>{3, 97, 100}" << (bucket_sort({97, 3, 100}) == vector<int>{3, 97, 100}) << endl;
    cout << "bucket_sort({24, 93, 134, 436, 313, 73, 0}) == vector<int>{0, 24, 73, 93, 134, 313, 436}" << (bucket_sort({24, 93, 134, 436, 313, 73, 0}) == vector<int>{0, 24, 73, 93, 134, 313, 436}) << endl;
    cout << endl;
}

int sumlst(vector<int> lst)
{
    int total = 0;
    for (int i : lst)
    {
        total += i;
    }
    return total;
}

float benaderen(auto sortfunc, int numel, int runs, int mode)
{
    vector<string> modes = {"scramble", "op volgorde", "reversed"};
    vector<int> sortlst;
    int n = numel;
    while(n >= 0)
    {
        sortlst.push_back(n);
        n--;
    }
    if (mode == 1)
    {
        reverse(sortlst.begin(), sortlst.end());
    }

    // cout << "list initialized" << endl;
    vector<int> times;
    for(int j = 1; j <= runs; j++)
    {
        clock_t start;
        int elapsed;

        if (mode == 0)
        {
            random_shuffle(sortlst.begin(), sortlst.end());
        }
        // cout << "run " << j << " starting sorting." << endl;

        start = clock();
        sortfunc(sortlst);
        elapsed = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
        // cout << "elapsed(ms):" << elapsed << endl;

        times.push_back(elapsed);
    }

    float gem = sumlst(times) / runs;
    cout << "average of " << runs <<" runs with " << numel << " elements with mode " << modes[mode] << " in ms: " << gem << endl;
    return gem;
}

int main()
{
    // tests();
    vector<float> path;
    // for (int i = 10000000; i < pow(10, 9); i += 10000000)
    // {
    //     cout << "number of elements: " << i;
    //     float b = benaderen(bucket_sort, i, 10);
    //     cout << ", time: " << b << endl;
    //     path.push_back(b);
    // }
    vector<int> testlist = { 10000000, 20000000, 40000000, 60000000, 80000000, 100000000}; // 100, 1000, 10000, 100000, 200000, 400000, 600000, 800000, 1000000,
    vector<int> results;
    for (int i: testlist)
    {
        benaderen(bucket_sort_negative, i, 3, 0);
        // benaderen(bucket_sort_negative, i, 3, 1);
        // benaderen(bucket_sort_negative, i, 3, 2);
    }

    return 0;
}