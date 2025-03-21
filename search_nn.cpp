#include <iostream> 
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>
using namespace std;

void printingFunction(vector<int> &dataSet) //function responsible for printing out the data sets to display output
{
    for(int x = 0; x < dataSet.size(); x++)
    {
        cout << dataSet[x]; 

        if(x < dataSet.size() - 1) //if it isn't the last feature
        {
            cout << ", "; //keep continuing with the " ,"
        }
    }
}
double accuracywithnofeatures(vector<vector<double>> dataSet) // function responsible for calculating the base accuracy of the data set 
{ // the accuracy if you always predict the most frequent class in the dataset
    double count1 = 0.0; 
    double count2 = 0.0; 
    double accuracy = 0.0; 
    for(int i = 0; i < dataSet.size(); i++) // counts based on type of class_labels within the dataset
    {
        if(dataSet[i][0] == 1)
        {
            count1++; 
        }
        else if(dataSet[i][0] == 2)
        {
            count2++;
        }

    }
    accuracy = max(count1,count2) / dataSet.size(); //finds the max of that to calcuate the accuracy 
    return accuracy * 100; 
}

double leave_one_out_cross_validation(vector<vector<double>> dataSet, vector<int> currentSet, int feature)
{
    vector<vector<double>> inSetdataSet = dataSet; //make a copy of the original dataset
    vector<int> check_set = currentSet;

    //zero out irrelevant features that are not in the check_set,
    for(int row = 0; row < inSetdataSet.size(); row++)
    {
        for(int col = 1; col < inSetdataSet[row].size(); col++)
        {
            if(find(check_set.begin(), check_set.end(), col) == check_set.end())
            {
                inSetdataSet[row][col] = 0.0; 

            }
        }
    }
    //track classification accuracy
    double accuracy = 0.0; 
    double numOfCorrectClassifications = 0.0; 
    double nearest_neighbor_label = 0.0;

    for(int i = 0; i < inSetdataSet.size(); i++) //iterates over each data point
    {
        double label_object_to_classify = inSetdataSet[i][0]; //extracts the label of the object to classify
        vector<double> object_to_classify(inSetdataSet[i].begin() + 1, inSetdataSet[i].end());  //extracts the feature values, this ignores the label column

        double nearest_neighbor_distance = INT_MAX;
        double nearest_neighbor_location = INT_MAX;
        //iterates over the data set to find the nearest neighbor

        for(int k = 0; k < inSetdataSet.size(); k++)
        {
            if(k != i) // so i don't compare the point to itself 
            {
                double distance = 0.0;  
                for(int j = 0; j < check_set.size(); j++) //computes the euclidean distance only using the selected features,
                {
                    int element = check_set[j]; 
                    distance += pow((inSetdataSet[i][element] - inSetdataSet[k][element]), 2);
                }
                if(distance < nearest_neighbor_distance) //updates the nearest neighbor if a closer one is found 
                {
                    nearest_neighbor_distance = distance; 
                    nearest_neighbor_location = k; 
                    nearest_neighbor_label = inSetdataSet[nearest_neighbor_location][0]; 
                }
            }
        }
        if(nearest_neighbor_label == label_object_to_classify) //check if the predicted label matches the label in actuality
        {
            numOfCorrectClassifications++; 
        }
    }
    accuracy = numOfCorrectClassifications / inSetdataSet.size(); //calculates the accuracy, the total number of correctly classified instances based on the data set
    return accuracy; 
}
//finds subset of features using LeaveOneOutCrossValidation and forward selection search
void forward_selection_search(vector<vector<double>> dataSet)
{
    cout << "Beginning Search. " << endl; 

    auto startTimer = chrono::high_resolution_clock::now(); //timer set up to time taken to compute search
    vector<int> currentSet; //current selected features 
    vector<int> best_set; //best feature subset 
    double bestAccuracy = 0.0; //tracks the highest accuracy
    double numberOfFeatures = dataSet[0].size() - 1; //number of features in the dataset
    for(int i = 0; i < numberOfFeatures; i++) //iterates over each level in the search tree
    {
        cout << "On the " << i + 1 << "th level of the search tree" << endl; 
        double currentSet_accuracy = 0.0; //tracks current set best accuracy

        int featureAdded = -1; 

        for(int k = 1; k < dataSet[0].size(); k++) //adding remaining features to current subset
        {

            if(find(currentSet.begin(), currentSet.end(), k) == currentSet.end()) //if current feature is already in the subset
            {
                vector<int> temporary_set = currentSet; //created a temp set including the new features 
                temporary_set.push_back(k);
                double accuracy = leave_one_out_cross_validation(dataSet,temporary_set,k); //computes the accuracy with the new feature added 
                
                cout << "Using feature(s) {"; //prints the accuracy of the current set 

                printingFunction(temporary_set);

                cout << "} accuracy is " << accuracy * 100 << "%\n"; 

                if(accuracy > currentSet_accuracy) //updates the best feature for this level if the accuracy is greater than the current accuracy
                {
                    currentSet_accuracy = accuracy; 
                    featureAdded = k; //adds the feature
                }
            }
        }
        if(featureAdded != -1) //if feature was added, it will update the current set 
        {
            currentSet.push_back(featureAdded); //adds to the current set 
            if(currentSet_accuracy > bestAccuracy) //updates the overall accuracy and feature set, if the addition improves accuracy
            {
                bestAccuracy = currentSet_accuracy; 
                best_set = currentSet; 
            }
            else 
            {
                if(i < numberOfFeatures - 1) // if accuracy is decrease, continue searching case of local maxima
                {
                    cout << "Warning, Accuracy has decreased! Continuing search in case of local maxima" << endl; 
                }
            }
            cout << endl; 
            cout << "Feature set {";  //print the best feature set at this current level of the search tree 
            printingFunction(currentSet); 
            cout << "} was best, accuracy is " << currentSet_accuracy * 100 << "%\n"; 
            cout << endl; 
        }
    } //print the overall best feature subset, as the search is complete for all levels 
    cout << "Finished Search!! "; 
    cout << "The best feature subset is {";
    printingFunction(best_set); 
    cout << "} which has an accuracy of " << bestAccuracy * 100 << "%\n"; 
    auto endTimer = chrono::high_resolution_clock::now(); 
    chrono::duration<double> timeTaken = endTimer - startTimer; 
    cout << endl;  //end timer 
    cout << "Total time taken: " << setprecision(4) << timeTaken.count()<< " seconds" << endl;

}
//finds subset of features using LeaveOneOutCrossValidation and backward elimination search
void backward_elimination_search(vector<vector<double>> dataSet)
{
    cout << "Beginning Search. " << endl; 

    auto startTimer = chrono::high_resolution_clock::now(); //initiating timer
    vector<int> currentSet; //current set of features
    vector<int> best_set; //overall best set of features 
    double bestAccuracy = 0.0; 
    double numberOfFeatures = dataSet[0].size() - 1; // number of features without labels
    vector<int> temporary_set; 
    for(int i = 1; i <= numberOfFeatures; i++) //start with all of the features in my current set then work backwards 
    {
        currentSet.push_back(i); 
        
    }
    for(int i = 0; i < numberOfFeatures; i++) //traverses through the tree
    {
        cout << "On the " << i + 1 << "th level of the search tree" << endl; 
        double currentSet_accuracy = 0.0; 

        int featureRemoved = -1; 

        for(int k = 0; k < currentSet.size(); k++) 
        {
            temporary_set = currentSet; //create a temp set 
            temporary_set.erase(temporary_set.begin() + k); //remove the feature i want to remove from the temp set 
            double accuracy = leave_one_out_cross_validation(dataSet,temporary_set,k); // calculate the accuracy 
                
            cout << "Using feature(s) {";

            printingFunction(temporary_set);

            cout << "} accuracy is " << accuracy * 100 << "%\n";  //print the set with its accuracy

            if(accuracy > currentSet_accuracy) //determines whether or not the accuracy is better than the current accuracy
            {
                currentSet_accuracy = accuracy; 
                featureRemoved = k;  //if so remove that feature 
            }
        }
    
        if(featureRemoved != -1) // if the feature was removed 
        {
            cout << "Removed feature: " << currentSet[featureRemoved] << " with accuracy of " << currentSet_accuracy * 100 << "%\n"; 

            currentSet.erase(currentSet.begin() + featureRemoved); //I remove it from the current set 
            if(currentSet_accuracy > bestAccuracy) //this ensures the best overall accuracy and best set of features is found for backward elimination search 
            {
                bestAccuracy = currentSet_accuracy; 
                best_set = currentSet; 
            }
            else
            {
                if(i < numberOfFeatures - 1) // this sets out a warning ensuring that I keep checking for local maxima, in case of situation, the accuracy improves 
                {
                    cout << "Warning, Accuracy has decreased! Continuing search in case of local maxima" << endl; 
                }
            } //prints the best feature set for the current level of the search tree 
            cout << endl; 
            cout << "Feature set {"; 
            printingFunction(currentSet); 
            cout << "} was best, accuracy is " << currentSet_accuracy * 100 << "%\n"; 
            cout << endl; 
        }
    } //finds the best feature subset of all of the levels, using backward elimination search 
    cout << "Finished Search!! "; 
    cout << "The best feature subset is {";
    printingFunction(best_set); 
    cout << "} which has an accuracy of " << bestAccuracy * 100 << "%\n"; 
    auto endTimer = chrono::high_resolution_clock::now(); //end the timer calculating the amount of time taken for search
    chrono::duration<double> timeTaken = endTimer - startTimer; 
    cout << "Total time taken: " << setprecision(4) << timeTaken.count()<< " seconds" << endl; 

}

int main()
{
    cout << "Welcome to Vidhi Tapde's Feature Selection Algorithm" << endl; 

    cout << "Type in the name of the file to test: ";  //prompt user to enter file name (include path)
    string fileName;
    cin >> fileName; 
    //open the file for reading
    ifstream file(fileName);
    if(!file.is_open()) //check if successfully opened 
    {
        cout << "Error! File Not Found" << endl; 
        return 1; 
    }
    vector<vector<double>> dataSet; //2D vector stores the dataet

    string line;  //variable to store each line read from the file 
    while(getline(file,line)) //reads line by line
    {
        stringstream sstream(line);  //convers the line to a stream
        string stringVal;
        string value; 
        vector<double> rowVal; //temp vector to store row values
        
        if(line.find(',') != string::npos) //if line contains commas
        {
            while(getline(sstream, stringVal, ',')) 
            {
                rowVal.push_back(stod(stringVal)); //split the line using ',' store each stringVal as a double in rowVal
            }
            if(!rowVal.empty())
            {
                dataSet.push_back(rowVal);  //if rowVal is not empty, push it to datSet a vector of vectors 
            }
        }
        else
        {
            if(sstream >> value) //read the first value
            {
                rowVal.push_back(stod(value)); //stores the first value
            
                while(sstream >> value) // read remaining values from the line 
                {
                    rowVal.push_back(stod(value)); 
                }
                dataSet.push_back(rowVal); //add row to dataset 
            }
        }

    }

    file.close(); //closes the file 
    cout << "Type the number of the algorithm you want to run. " << endl; 
    cout << "1) Forward Selection" << endl; 
    cout << "2) Backward Elimination" << endl; 
    int choice; 
    cin >> choice; 
    cout << endl; 
    //prints the number of features and instances in dataset
    cout << "base accuracy " << accuracywithnofeatures(dataSet) << endl; 
    cout << "This dataset has " << dataSet[0].size() - 1 << " features (not including the class attribute), with " << dataSet.size() << " instances." << endl;
    //calculates the current set values, for all features 
    vector<int> currentSet(dataSet[0].size()-1);
    for(int i = 0; i < dataSet[0].size()-1; i++)
    {
        currentSet[i] = i + 1; 
    }

    //for this number of features, all of the features, the accuracy of it
    cout << "Running nearest neighbor with all " << dataSet[0].size() - 1 << " features, using \"leaving-one-out\" evaluation, I get an accuracy of " << (leave_one_out_cross_validation(dataSet, currentSet,-1) * 100) << "%\n" << endl;
    //user chooses a choice of which algorithm they would like to use.
    if(choice == 1)
    {
        forward_selection_search(dataSet); 
    }
    else if (choice == 2)
    {
        backward_elimination_search(dataSet);
    }
    else
    {
        cout << "Invalid choice " << endl;
    }
    return 0;
}