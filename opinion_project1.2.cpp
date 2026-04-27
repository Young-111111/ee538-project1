#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
void read_opinions(string filename);
void read_edges(string filename);
void build_adj_list();

int total_nodes = 0;
/****************************************************************/

vector<int> opinions;
vector<vector<int>> edge_list;

// adj_list[target] stores all voters that influence target.
// Example: edge 0 2 means 0 influences 2, so adj_list[2] contains 0.
vector<vector<int>> adj_list;

void build_adj_list()
{
    adj_list.clear();
    adj_list.resize(total_nodes);

    for (const auto& edge : edge_list)
    {
        int source = edge[0];
        int target = edge[1];
        adj_list[target].push_back(source);
    }
}

double calculate_fraction_of_ones()
{
    if (total_nodes == 0) return 0.0;

    int count_ones = 0;
    for (int opinion : opinions)
    {
        if (opinion == 1) count_ones++;
    }

    return static_cast<double>(count_ones) / total_nodes;
}

int get_majority_friend_opinions(int node)
{
    int count_ones = 0;
    int total_friends = 0;

    for (int friend_id : adj_list[node])
    {
        total_friends++;
        if (opinions[friend_id] == 1) count_ones++;
    }

    int count_zeros = total_friends - count_ones;

    if (count_ones > count_zeros) return 1;
    return 0;  // tie or majority 0
}

bool update_opinions()
{
    vector<int> next_opinions(total_nodes);
    bool changed = false;

    for (int i = 0; i < total_nodes; i++)
    {
        next_opinions[i] = get_majority_friend_opinions(i);
        if (next_opinions[i] != opinions[i])
        {
            changed = true;
        }
    }

    opinions = next_opinions;
    return changed;
}

int main()
{
    read_opinions("opinions.txt");
    read_edges("edge_list.txt");

    build_adj_list();

    cout << "Total nodes: " << total_nodes << endl;

    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;

    cout << "Iteration " << iteration << ": fraction of 1's = "
         << calculate_fraction_of_ones() << endl;

    while (opinions_changed && iteration < max_iterations)
    {
        opinions_changed = update_opinions();
        iteration++;

        cout << "Iteration " << iteration << ": fraction of 1's = "
             << calculate_fraction_of_ones() << endl;
    }

    double final_fraction = calculate_fraction_of_ones();

    if (final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if (final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;

    return 0;
}

void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;

    while (file >> id >> opinion)
    {
        if (id >= static_cast<int>(opinions.size()))
        {
            opinions.resize(id + 1, 0);
        }

        opinions[id] = opinion;
        if (id >= total_nodes) total_nodes = id + 1;
    }

    file.close();
}

void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;

    while (file >> source >> target)
    {
        edge_list.push_back({source, target});
        if (source >= total_nodes) total_nodes = source + 1;
        if (target >= total_nodes) total_nodes = target + 1;
    }

    file.close();

    if (static_cast<int>(opinions.size()) < total_nodes)
    {
        opinions.resize(total_nodes, 0);
    }
}
