#include "route_planner.h"
#include <algorithm>
using std::vector;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x,start_y); //start nodes
    end_node = &m_Model.FindClosestNode(end_x, end_y);      //end nodes
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {

    return (*node).distance(*end_node); //return the distance to the end_node 
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    
    current_node-> FindNeighbors(); //populate the neighbours to current node
    for (RouteModel::Node *neighbor : current_node->neighbors)
    {   //set parent, g value and h value to the current node
        neighbor->parent = current_node;
        neighbor->g_value = current_node->g_value + neighbor->distance(*current_node); 
        neighbor->h_value = CalculateHValue(neighbor);
        open_list.emplace_back(neighbor); //neighbor values are stored in open list
        neighbor->visited = true;         //neigbors are visited
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
    
    std::sort(open_list.begin(), open_list.end(), Compare);
    RouteModel::Node *copy_node = open_list.front(); //create a pointer to the node
    open_list.erase(open_list.begin()); //Remove the node from open_list
    return copy_node;
}

bool RoutePlanner::Compare(RouteModel::Node const *a, RouteModel::Node const *b ) {
    return (a->g_value + a->h_value) < (b->g_value + b->h_value);
}

// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) //
{   // Create path_found vector
    distance = 0.0f;
    vector<RouteModel::Node> path_found{};
    RouteModel::Node parent;

    while(current_node-> parent != nullptr) { //start node
        path_found.emplace_back(*current_node);
        parent = *(current_node->parent);
        distance += current_node->distance(parent); //adding the distance from the node to its parent to the distance variable
        current_node = current_node->parent; //
    }
    path_found.emplace_back(*current_node);
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    std::reverse(std::begin(path_found), std::end(path_found)); //reverse the path found to arrange in order
    return path_found;
}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {

    // TODO: Implement your solution here.
    start_node->visited = true;
    open_list.emplace_back(start_node);
    RouteModel::Node *current_node = nullptr;
    while(open_list.size() > 0) { //using openlist to add all the neighbors
        current_node = NextNode(); //using next node to sort the open list
        if(current_node->distance(*end_node) == 0)
        {
            m_Model.path = ConstructFinalPath(current_node); //to achieve at final path
            return;
        }
        AddNeighbors(current_node);
    }

}
