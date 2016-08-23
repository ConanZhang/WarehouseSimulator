/* Filename: report.cpp
 * Author: Conan Zhang
 * Last Modified: 2/9/2015
 * Version: 1.0
 */

/*
 * Reads in data from a file to perform transactions on warehouses and prints
 * out a report based on those transactions.
 */

#include <iostream>//cout, endl
#include <vector>
#include <utility>//pair
#include <string>

#include "transaction.h"

using namespace std;
using namespace transactions;

/*
 * Main function to initiate our transaction simulation
 */
int main(int argc, char* argv[])
{
  //Make sure user only provides one argument
  if(argc != 2)
   {
     cout << "Please provide one and only one argument." << endl;
     return 0;
   }

  //Begin a simulation from the provided argument
  transaction simulation(argv[1]);
  
  //Receive the generated report
   pair<vector<pair<string, string> >, vector<pair<string, string> > > _report = simulation.get_report();

  cout << "Report by Conan Zhang" << endl;
  cout << endl;
  
  cout << "Unstocked Products:" << endl;

  //Print unstocked products from report
  for(vector<pair<string, string> >::size_type i = 0; i != _report.first.size() ; i++)
    {
      cout << _report.first[i].first << ' ' << _report.first[i].second << endl;
    } 
  cout << endl;

  cout << "Well-Stocked Products:" << endl;

  //Print wellstocked products from report
  for(vector<pair<string, string> >::size_type i = 0; i != _report.second.size() ; i++)
    {
      cout << _report.second[i].first << ' ' << _report.second[i].second << endl;
    }

  cout << endl;

  //End program
  return 0;
}
