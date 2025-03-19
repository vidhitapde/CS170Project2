#include <iostream> 
#include <fstream> 
#include <vector>
#include <sstream> 
#include <algorithm>
#include <cmath>
#include <chrono>  
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
  vector<int> best_set_of_features;
  double best_overall_accuracy = 0.0;
  for(int i = 0; i < data[0].size() - 1; i++)
  {
      cout << "On the " << i + 1 << "th level of the search tree" << endl;




      double best_so_far_accuracy = 0.0;
      int feature_to_add_at_this_level = -1;




      for(int k = 1; k < data[0].size() - 1; k++)
      {
          int best_feature = -1;
          if(find(current_set_of_features.begin(), current_set_of_features.end(), k) == current_set_of_features.end())
          {
              cout << "Considering adding the " << k  << " feature" << endl;
              vector<int> temp_set = current_set_of_features;
              temp_set.push_back(k);
              double accuracy = leave_one_out_cross_validation(data,temp_set);




              cout << "Evaluating feature " << k  << " Accuracy: " << accuracy * 100 << "%\n" ;




              if(accuracy > best_so_far_accuracy)
              {
                  best_so_far_accuracy = accuracy;
                  feature_to_add_at_this_level = k;
              }
            
          }
      }
      if(feature_to_add_at_this_level != -1)
      {
          current_set_of_features.push_back(feature_to_add_at_this_level);
          cout << "Best feature added: " << i + 1 << " - New Accuracy: " << feature_to_add_at_this_level * 100 << "%\n";
          if(best_so_far_accuracy > best_overall_accuracy)
          {
              best_overall_accuracy = best_so_far_accuracy;
              best_set_of_features = current_set_of_features;
          }
          else
          {
              cout << "Accuracy decrease, continue for local maxima" << endl;
          }
      }
      cout << "Current feature set found:  {";
      for(int x = 0; x < current_set_of_features.size(); x++)
      {
          cout << current_set_of_features[x];
          if(x < current_set_of_features.size() - 1)
          {
                cout << ", ";
          }
      }
      cout << "} with accuracy: " << best_so_far_accuracy * 100 << "%\n";
  }
  cout << "Best feature set found: {";
        for(int x = 0; x < best_set_of_features.size(); x++)
      {
          cout << best_set_of_features[x];
          if(x < best_set_of_features.size() - 1)
          {
                cout << ", ";
          }
      }

  cout << "} with accuracy: " << best_overall_accuracy * 100 << "%\n";

}




void backward_feature_search_demo(vector<vector<double>> data)
{
  vector<int> current_set_of_features;
  vector<int> best_set_of_features;
  double best_overall_accuracy = 0.0;
   vector<int> temp_set;


  for(int i = 1; i <= data[0].size() - 1; i++)
  {
   //need to add all the features to the current set of features
       current_set_of_features.push_back(i);
  }
  for(int i = 0; i < data[0].size() - 1; i++)
  {
      cout << "On the " << i + 1 << "th level of the search tree" << endl;




      double best_so_far_accuracy = 0.0;
      int feature_to_remove_at_this_level = -1;




      for(int k = 0; k < current_set_of_features.size(); k++)
      {
       temp_set.clear();
          int best_feature = -1;


              cout << "Considering removing the " << current_set_of_features[k]  << " feature" << endl;
              for (int j = 0; j < current_set_of_features.size(); j++)
              {
               if(j != k)
               {
                   temp_set.push_back(current_set_of_features[j]);
               }
              }
              double accuracy = leave_one_out_cross_validation(data,temp_set);




              cout << "Evaluating feature " << current_set_of_features[k] << " Accuracy: " << accuracy * 100 << "%\n" ;




              if(accuracy > best_so_far_accuracy)
              {
                  best_so_far_accuracy = accuracy;
                  feature_to_remove_at_this_level = k;
              }
         
      }
      if(feature_to_remove_at_this_level != -1)
      {
       int remove = current_set_of_features[feature_to_remove_at_this_level];
       current_set_of_features.erase(current_set_of_features.begin() + feature_to_remove_at_this_level);
          cout << "Removed feature : " << remove << " - New Accuracy: " << best_so_far_accuracy * 100 << "%\n";
          if(best_so_far_accuracy > best_overall_accuracy)
          {
              best_overall_accuracy = best_so_far_accuracy;
              best_set_of_features = current_set_of_features;
          }
          else
          {
              cout << "Accuracy decrease, continue for local maxima" << endl;
          }
      }
      cout << "Current feature set found:  {";
       for(int x = 0; x < current_set_of_features.size(); x++)
      {
          cout << current_set_of_features[x];
          if(x < current_set_of_features.size() - 1)
          {
                cout << ", ";
          }
      }
      cout << "} with accuracy: " << best_so_far_accuracy * 100 << "%\n";
  }
  cout << "Best feature set found: {";
        for(int x = 0; x < best_set_of_features.size(); x++)
      {
          cout << best_set_of_features[x];
          if(x < best_set_of_features.size() - 1)
          {
                cout << ", ";
          }
      }
  cout << "} with accuracy: " << best_overall_accuracy * 100 << "%\n";

}


int main()
{
   cout << "Welcome to Vidhi Tapde's Feature Selection Algorithm" << endl;



   string fileName;
   cout << "Type the number of the file you want to run." << endl;
   cout << "Choose 1 for CS170_Small_Dataset__35.txt" << endl;
   cout << "Choose 2 for CS170_Large_Dataset__22.txt" << endl;




   int choose;


   cin >> choose;


   if(choose == 1)
   {
       fileName = "/Users/vidhitapde/CS170Project2/CS170_Small_Data__35.txt";
   }
   else if (choose == 2)
   {
       fileName = "/Users/vidhitapde/CS170Project2/CS170_Large_Data__22.txt";
   }
   else
   {
       cout << "Invalid choice " << endl;
   }


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
       backward_feature_search_demo(data);
   }
   else
   {
       cout << "Invalid choice " << endl;
   }


  
   return 0;


}
