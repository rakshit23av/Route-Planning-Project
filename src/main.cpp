#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();
    std::vector<std::byte> contents(size);    
    
    is.seekg(0);
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);
}

int main(int argc, const char **argv)
{    
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
    // TODO 1: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
    // user input for these values using std::cin. Pass the user input to the
    // RoutePlanner object below in place of 10, 10, 90, 90.
    
    //using std::array;
    using std::cin;
    using std::cout;

    /*Declaring the coordinates as type float */
    float start_x, start_y, end_x, end_y;

    cout << "Coordinate 1: start_x" << "\n"; 
    cout << "Coordinate 2: start_y" << "\n";
    cout << "Coordinate 3: end_x" << "\n";
    cout << "Coordinate 4: end_y" << "\n";
    cout << "Begins at (start_x,start_y) and ends at (end_x,end_y)" << "\n"; //start_x = 0, start_y=0, end_x=100, end_y=100

    /* I will set a counter now since to take in 4 values as coordinates one after the other */

    float coordinates[4]; // implementing an array of type float 
    int counter = 0;    //setting counter to zero
    while (counter < 4)   // counter upto 4 values
    {
        float temp = 0;    //temporary varibale to check if the input value is between 0-100
        cout << "Enter the value for coordinate" << "\n";
        cin >> temp;
    
        if (temp <=100 && temp >=0){
        
            coordinates[counter] = temp;
            counter++;
        }
    
        else {
        
            cout << "Invalid value, coordinate should be between 0 and 100" << "\n";
        }
    
    }
    cout << "start_x : " << coordinates[0] << "\n" << "start_y : " << coordinates[1] << "\n" << "end_x : " << coordinates[2] << "\n" << "end_y : " << coordinates[3] << "\n";
    
    // Build Model.
    RouteModel model{osm_data};

    // Create RoutePlanner object and perform A* search.
    RoutePlanner route_planner{model, coordinates[0], coordinates[1], coordinates[2], coordinates[3]};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
