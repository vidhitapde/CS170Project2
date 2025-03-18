#include <iostream> 
#include <fstream> 
#include <vector>
#include <sstream> 
#include <algorithm>
#include <cstdlib>
#include <cmath> 
using namespace std;


double leave_one_out_cross_validation(vector<vector<double>> data, vector<int> current_set)
{
    double accuracy = 0.0; 
    double number_of_correctly_classifed = 0.0;
    double nearest_neighbor_label = 0.0; 
    for(int i = 0; i < data.size(); i++)
    {
        double label_object_to_classify = data[i][0];
        vector<double>object_to_classify(data[i].begin() + 1, data[i].end());

        double nearest_neighbor_distance = INT_MAX; 
        double nearest_neighbor_location = INT_MAX; 

        for(int k = 0; k < data.size(); k++)
        {
            if(k != i)
            {

                double distance = 0.0;

                for(int j: current_set)
                {
                    distance += pow((data[i][j] - data[k][j]),2);
                }

                if(distance < nearest_neighbor_distance)
                {
                    nearest_neighbor_distance = distance; 
                    nearest_neighbor_location = k; 
                    nearest_neighbor_label  = data[nearest_neighbor_location][0]; 

                }
            }
        }
        if(nearest_neighbor_label == label_object_to_classify)
        {
            number_of_correctly_classifed++; 
        }
    } 
    return accuracy = number_of_correctly_classifed / data.size(); 

}


void forward_feature_search_demo(vector<vector<double>> data)
{
    vector<int> current_set_of_features;
    int feature_to_add_at_this_level;
    double best_so_far_accuracy; 
    int num_feature = data[0].size(); 

    for(int i = 0; i < num_feature; i++)
    {
        cout << "On the " << i << "th level of the search tree" << endl; 

        best_so_far_accuracy = 0.0; 

        for(int k = 0; k < num_feature; k++)
        {
            if(find(current_set_of_features.begin(), current_set_of_features.end(), k) == current_set_of_features.end())
            {
                cout << "Considering adding the " << k  << " feature" << endl; 
                double accuracy = leave_one_out_cross_validation(data,current_set_of_features); 

                if(accuracy > best_so_far_accuracy)
                {
                    best_so_far_accuracy = accuracy; 
                    feature_to_add_at_this_level = k; 
                }
            }
        }
        current_set_of_features.push_back(feature_to_add_at_this_level);
        cout << "On level " << i << " I added feature " << feature_to_add_at_this_level << " to current set" << endl; 

    }
}


int main()
{
    cout << "Welcome to 's Feature Selection Algorithm" << endl; 

    cout << "Type in the name of the file to test: "; 

    string fileName;
    cin >> fileName; 

    ifstream file(fileName);

    if(!file.is_open())
    {
        cout << "Error: File Not Found" << endl; 
        return 1; 
    }
    vector<vector<double>> data; 

    string line; 

    while(getline(file,line))
    {
        stringstream sstream(line); 
        double value; 
        vector <double> row;

        if(sstream >> value)
        {
            row.push_back(value); 

            while(sstream >> value)
            {
                row.push_back(value);
            }
            data.push_back(row);

        }


    }
    file.close(); 


    cout << "Type the number of the algorithm you want to run." << endl; 

    int choice; 

    cin >> choice; 

    if(choice == 1)
    {
        forward_feature_search_demo(data); //do the search first 
    }
    else if (choice == 2)
    {
        return 0; // implement //backward_feature_search_demo(data); 
    }
    else
    {
        cout << "Invalid choice " << endl; 
    }

    
    return 0; 

}








    









