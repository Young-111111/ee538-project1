#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(string filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(string filename); // reads file into edge_list, defined later
void build_adj_matrix(); // convert edge_list to adjacency matrix

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.


/****************************************************************/

/******** Create adjacency matrix and vector of opinions */
// simple vector to hold each node's opinion (0 or 1)
std::vector<int> opinions;

// global adjacency matrix initialized later
std::vector<std::vector<int>> adj;

// edge list: each row contains {source, target}
std::vector<std::vector<int>> edge_list;

void build_adj_matrix()
{
  adj.resize(total_nodes);
    for(int i=0; i<total_nodes; i++)
    {
        adj[i].resize(total_nodes, 0); // initialize with 0's
    }
    for(const auto& edge : edge_list)// ragnge-based loop to fill adjacency matrix,computer doesn't need to know how many edges there are, just loop through all of them
    {
        int source = edge[0];
        int target = edge[1];
        adj[source][target] = 1; // directed edge from source to target
    }
}

double calculate_fraction_of_ones()
{
    if(total_nodes == 0) return 0.0; // avoid division by zero
    double count_ones = 0;
    for(const auto& opinion : opinions)
    {
        // count number of 1's and divide by total_nodes
        if(opinion == 1) count_ones++;
    }
    return count_ones / total_nodes;
   
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    int count_majority_friend_ones = 0; // initialize count for this node
    int total_friends= 0 ; // count total neighbors to determine majority
    for(int j=0; j<total_nodes; j++)
    {
        // check if j is a neighbor of node
        if(adj[j][node] == 1)
        {
            // count opinions of neighbors
            // if opinion of neighbor is 1, add to count, else subtract from count
            total_friends++; // if neighbor j has opinion 1, add to count
            if(opinions[j] == 1) count_majority_friend_ones++;
        }
    }
    if(count_majority_friend_ones > total_friends/2.0) return 1; // majority of neighbors have opinion 1
    else  return 0; // majority of neighbors have opinion 0

}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{   
    std::vector<int> next_opinions(total_nodes); // temporary vector to hold next opinions
    for(int i=0; i<total_nodes; i++)
    {
        // for each node, calculate majority opinion of neighbors and update opinion vector
        // if any opinion changes, set a flag to true
        next_opinions[i] = get_majority_friend_opinions(i);

    }
    if (next_opinions != opinions) {
        opinions = next_opinions;
        return true;
    }
    return false;
}

int main() {
    // no preallocation; vectors grow on demand

    // Read input files
    read_opinions("opinions.txt"); 
    read_edges("edge_list.txt");

    // convert edge list into adjacency matrix once we know total_nodes
    build_adj_matrix();
    
    cout << "Total nodes: " << total_nodes << endl;
    // Run simulation
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;
    
    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;
    
    /// (6)  //////////////////////////////////////////////
    while (opinions_changed && iteration < max_iterations) 
    {
        opinions_changed = update_opinions();
        if (!opinions_changed) {
            break; 
        }
            iteration++;
            cout << "Iteration " << iteration << ": fraction of 1's = " 
                 << calculate_fraction_of_ones() << endl;
        
    }

    ////////////////////////////////////////////////////////
    // Print final result
    double final_fraction = calculate_fraction_of_ones();
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << final_fraction << endl;
    
    if(final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if(final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;
    
    return 0;
}


/*********** Functions to read files **************************/ 

// Read opinion vector from file.
void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;
    while(file >> id >> opinion)
    {
        opinions.push_back(opinion);
        if(id >= total_nodes) total_nodes = id+1;
    }
    file.close();
}

// Read edge list from file and update total nodes as needed.
void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;
    
    while(file >> source >> target)
    {
        edge_list.push_back({source, target});
        if(source >= total_nodes) total_nodes = source+1;
        if(target >= total_nodes) total_nodes = target+1;
    }
    file.close();
}

/********************************************************************** */
