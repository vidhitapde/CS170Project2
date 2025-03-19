#include <iostream> 
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>
#include<iomanip>
using namespace std;

void printingFunction(vector<int> &data)
{
    for(int x = 0; x < data.size(); x++)
    {
        cout << data[x]; 

        if(x < data.size() - 1)
        {
            cout << ", "; 
        }
    }
}
double accuracywithnofeatures(vector<vector<double>> data)
{
    double count1 = 0.0; 
    double count2 = 0.0; 
    double accuracy = 0.0; 
    for(int i = 0; i < data.size(); i++)
    {
        if(data[i][0] == 1)
        {
            count1++; 
        }
        else if(data[i][0] == 2)
        {
            count2++;
        }

    }
    accuracy = max(count1,count2) / data.size(); 
    return accuracy * 100; 
}

double leave_one_out_cross_validation(vector<vector<double>> data, vector<int> current_set)
{
    double accuracy = 0.0; 
    double number_of_correctly_classified = 0.0; 
    double nearest_neighbor_label = 0.0;

    for(int i = 0; i < data.size(); i++)
    {
        double label_object_to_classify = data[i][0]; 
        vector<double> object_to_classify(data[i].begin() + 1, data[i].end()); 

        double nearest_neighbor_distance = INT_MAX;
        double nearest_neighbor_location = INT_MAX;

        for(int k = 0; k < data.size(); k++)
        {
            if(k != i)
            {
                double distance = 0.0; 
                for(int j = 0; j < current_set.size(); j++)
                {
                    int element = current_set[j]; 
                    distance += pow((data[i][element] - data[k][element]), 2);
                }
                if(distance < nearest_neighbor_distance)
                {
                    nearest_neighbor_distance = distance; 
                    nearest_neighbor_location = k; 
                    nearest_neighbor_label = data[nearest_neighbor_location][0]; 
                }
            }
        }
        if(nearest_neighbor_label == label_object_to_classify)
        {
            number_of_correctly_classified++; 
        }
    }
    accuracy = number_of_correctly_classified / data.size(); 
    return accuracy; 
}

void forward_selection_search(vector<vector<double>> data)
{
    cout << "Beginning Search. " << endl; 

    auto startTimer = chrono::high_resolution_clock::now(); 
    vector<int> current_set; 
    vector<int> best_set; 
    double best_overall_accuracy = 0.0; 
    double numberOfFeatures = data[0].size() - 1;

    for(int i = 0; i < numberOfFeatures; i++)
    {
        cout << "On the " << i + 1 << "th level of the search tree" << endl; 
        double current_set_accuracy = 0.0; 

        int featureAdded = -1; 

        for(int k = 1; k < data[0].size(); k++)
        {
            int best_feature = -1; 

            if(find(current_set.begin(), current_set.end(), k) == current_set.end())
            {
                vector<int> temporary_set = current_set; 
                temporary_set.push_back(k);
                double accuracy = leave_one_out_cross_validation(data,temporary_set);
                
                cout << "Using feature(s) {";

                printingFunction(temporary_set);

                cout << "} accuracy is " << accuracy * 100 << "%\n"; 

                if(accuracy > current_set_accuracy)
                {
                    current_set_accuracy = accuracy; 
                    featureAdded = k; 
                }
            }
        }
        if(featureAdded != -1)
        {
            current_set.push_back(featureAdded); 
            if(current_set_accuracy > best_overall_accuracy)
            {
                best_overall_accuracy = current_set_accuracy; 
                best_set = current_set; 
            }
            else 
            {
                cout << "Warning, Accuracy has decreased! Continuing search in case of local maxima" << endl; 
            }
            cout << "Feature set {"; 
            printingFunction(current_set); 
            cout << "} was best, accuracy is " << current_set_accuracy * 100 << "%\n"; 
        }
    }
    cout << "Finished Search!! "; 
    cout << "The best feature subset is {";
    printingFunction(best_set); 
    cout << "} which has an accuracy of " << best_overall_accuracy * 100 << "%\n"; 
    auto endTimer = chrono::high_resolution_clock::now(); 
    chrono::duration<double> timeTaken = endTimer - startTimer; 
    cout << endl; 
    cout << "Total time taken: " << setprecision(4) << timeTaken.count()<< " seconds" << endl; 

}

void backward_elimination_search(vector<vector<double>> data)
{
    cout << "Beginning Search. " << endl; 

    auto startTimer = chrono::high_resolution_clock::now(); 
    vector<int> current_set; 
    vector<int> best_set; 
    double best_overall_accuracy = 0.0; 
    double numberOfFeatures = data[0].size() - 1;
    vector<int> temporary_set; 
    for(int i = 1; i <= numberOfFeatures; i++)
    {
        current_set.push_back(i); 
    }

    for(int i = 0; i < numberOfFeatures; i++)
    {
        cout << "On the " << i + 1 << "th level of the search tree" << endl; 
        double current_set_accuracy = 0.0; 

        int featureRemoved = -1; 

        for(int j = 0; j < current_set.size(); j++)
        {
            int best_feature = -1; 
            temporary_set = current_set; 
            temporary_set.erase(temporary_set.begin() + j); 
            double accuracy = leave_one_out_cross_validation(data,temporary_set);
                
            cout << "Using feature(s) {";

            printingFunction(temporary_set);

            cout << "} accuracy is " << accuracy * 100 << "%\n"; 

            if(accuracy > current_set_accuracy)
            {
                current_set_accuracy = accuracy; 
                featureRemoved = j; 
            }
        }
    
        if(featureRemoved != -1)
        {
            current_set.erase(current_set.begin() + featureRemoved);
            cout << "Removed feature :" << current_set[featureRemoved] << " with accuracy of " << current_set_accuracy * 100 << "%\n"; 
            if(current_set_accuracy > best_overall_accuracy)
            {
                best_overall_accuracy = current_set_accuracy; 
                best_set = current_set; 
            }
            else
            {
                cout << "Warning, Accuracy has decreased! Continuing search in case of local maxima" << endl; 
            }
            cout << "Feature set {"; 
            printingFunction(current_set); 
            cout << "} was best, accuracy is " << current_set_accuracy * 100 << "%\n"; 
        }
    }
    cout << "Finished Search!! "; 
    cout << "The best feature subset is {";
    printingFunction(best_set); 
    cout << "} which has an accuracy of " << best_overall_accuracy * 100 << "%\n"; 
    auto endTimer = chrono::high_resolution_clock::now(); 
    chrono::duration<double> timeTaken = endTimer - startTimer; 
    cout << "Total time taken: " << setprecision(4) << timeTaken.count()<< " seconds" << endl; 

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
        cout << "Error! File Not Found" << endl; 
        return 1; 
    }
    vector<vector<double>> data;
    string line; 
    while(getline(file,line))
    {
        stringstream sstream(line); 
        double value;
        vector<double> row; 
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
    cout << "Type the number of the algorithm you want to run. " << endl; 
    cout << "1) Forward Selection" << endl; 
    cout << "2) Backward Elimination" << endl; 
    int choice; 
    cin >> choice; 
    cout << endl; 
    cout << "This dataset has " << data[0].size() - 1 << " features (not including the class attribute), with " << data.size() << " instances." << endl;
   
    vector<int> current_set(data[0].size()-1);
    for(int i = 0; i < data[0].size()-1; i++)
    {
        current_set[i] = i + 1; 
    }

    cout << "Running nearest neighbor with all " << data[0].size() - 1 << " features, using \"leaving-one-out\" evaluation, I get an accuracy of " << (leave_one_out_cross_validation(data, current_set) * 100) << "%\n" << endl;
    
    if(choice == 1)
    {
        forward_selection_search(data); 
    }
    else if (choice == 2)
    {
        backward_elimination_search(data);
    }
    else
    {
        cout << "Invalid choice " << endl;
    }
    return 0;
}