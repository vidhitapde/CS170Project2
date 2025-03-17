#include <iostream> 
#include <fstream> 
#include <vector>
#include <sstream> 
#include <algorithm>
#include <cstdlib>

using namespace std;


double leave_one_out_cross_validation(vector<vector<double>> features, vector<double> current_set, double feature_to_add)
{
    return static_cast<double>(rand()) / RAND_MAX;
}


void forward_feature_search_demo(vector<vector<double>> data)
{
    vector<double> current_set_of_features;
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
                double accuracy = leave_one_out_cross_validation(data,current_set_of_features, k+1); 

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
    cout << "Welcome to Vidhi Tapde's Feature Selection Algorithm" << endl; 

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
    vector<double> class_labels;

    string line; 

    while(getline(file,line))
    {
        stringstream sstream(line); 
        double value; 
        vector <double> row;
        sstream >> value;

        class_labels.push_back(value); 

        while(sstream >> value)
        {
            row.push_back(value); 
        }
        data.push_back(row); 


    }
    file.close(); 

    for(int i = 0; i < data.size(); i++)
    {
        cout << "Class: " << class_labels[i] << " Features: "; 
        for(double val: data[i])
        {
            cout << val << " "; 
        }
        cout << endl; 
    }


    cout << "Type the number of the algorithm you want to run." << endl; 

    forward_feature_search_demo(data); //do the search first 
    
    return 0; 

}








    









