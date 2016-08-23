/* Filename: transaction.cpp
 * Author: Conan Zhang
 * Last Modified: 2/8/2015
 * Version: 1.0
 */

/*
 * Reads in data from a file to perform transactions on warehouses and food items.
 */

#include<fstream>//ifstream reading from file
#include<sstream>//string stream reading line
#include <stdlib.h>//atoi
#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>

#include "transaction.h"

namespace transactions
{
  /*
   * Constructor
   */
  transaction::transaction(char* file_name)
  {
    //Read from file
    std::ifstream reading_data(file_name);
    std::string line;

    while(reading_data)
      {
	getline(reading_data, line);

	//Only read until end of file
	if(reading_data.fail())
	  break;

	//break line into words and store into vector
	std::vector<std::string> words = get_words(line, ' ');
	
	std::string word = words[0];
	//Process commands
	if(word == "FoodItem")
	  {
	    create_food_item(words, line);
	  }
	else if(word == "Warehouse")
	  {
	    create_warehouse(words, line);
	  }
	else if(word == "Start")
	  {
	    current_date = create_date(words);
	  }
	else if(word == "Receive:")
	  {
	    process_receive(words);
	  }
	else if(word == "Request:")
	  {
            requests.push(words);
	  }
	else if(word == "Next")
	  {
            processRequests();
            
	    //Increment day
	    boost::gregorian::date_duration day(1);
	    current_date += day;

	    update_warehouses(current_date);
	  }
	else
	  { 
            processRequests();
	    break;
	  }
      }
    //Finish reading data
    reading_data.close();
  }

  /*
   * Destructor
   */
  transaction::~transaction()
  {

  }

  /*
   * Creates the existence of food items to keep track of.
   */
  void transaction::create_food_item(std::vector<std::string> words, std::string line)
  {
    //Get shelf_life
    int shelf_life = atoi(words[8].c_str());

    //Get name from substring
    size_t begin_name = line.find("Name: ");
    begin_name += 6;
    std::string name = line.substr(begin_name);

    //Create food item and insert into map to keep track of
    food_item fi(words[4], shelf_life, name);

    food_items.insert(std::map<std::string, food_item>::value_type(words[4], fi));
  }

  /*
   * Creates the existence of warehouses to keep track of.
   */
  void transaction::create_warehouse(std::vector<std::string> words, std::string line)
  {
    //Get name from substring
    size_t begin_name = line.find("Warehouse - ");
    begin_name += 12;
    std::string name = line.substr(begin_name);

    //Make warehouse
    warehouse wh(name);

    //Add existence of food items into warehouse
    BOOST_FOREACH(_food_items::value_type& food, food_items)
      {
	wh.add_food(food.second);
      }

    //Keep track of warehouse in map
    warehouses.insert(std::map<std::string, warehouse>::value_type(name, wh));
  }

  /*
   * Creates the existence of a date to keep track of.
   */
  boost::gregorian::date transaction::create_date(std::vector<std::string> words)
  {
    //Get month, day, and year
    std::string date = words[2];

    std::string _month = date.substr(0, 2);
    int month = atoi(_month.c_str());

    std::string _day = date.substr(3,2);
    int day = atoi(_day.c_str());

    std::string _year = date.substr(6);
    int year = atoi(_year.c_str());
 
    //create date from extracted values
    boost::gregorian::date _date(year, month, day);
  
    return _date;
  }

  /*
   * Processes receive commands and passes them to the appropriate warehouse
   */
  void transaction::process_receive(std::vector<std::string> words)
  {
    //Get UPC and amount
    std::string UPC = words[1];
    int amount = atoi(words[2].c_str());

    //Remove UPC and amount from words to only leave name
    words.erase(words.begin(), words.begin()+3);

    //Concatenate name of warehouse
    std::string name = boost::algorithm::join(words, " ");

    //Send receive command to warehouse
    warehouses.at(name).receive(UPC, amount, current_date);
  }

  /*
   * Processes request commands and passes them to the appropriate warehouse
   */
  void transaction::process_request(std::vector<std::string> words)
  {
    //Get UPC and amount
    std::string UPC = words[1];
    int amount = atoi(words[2].c_str());

    //Remove UPC and amount from words to only leave name
    words.erase(words.begin(), words.begin()+3);

    //Concatenate name of warehouse
    std::string name = boost::algorithm::join(words, " ");

    //Send request command to warehouse
    warehouses.at(name).request(UPC, amount);
  }

  /*
   * Processes the advancement of a day and updates warehouses to possibility of expired food items
   */
  void transaction::update_warehouses(boost::gregorian::date current_date)
  {
    //Update warehouse to current_date
    BOOST_FOREACH(_warehouses::value_type& _warehouse, warehouses)
      {
	_warehouse.second.update(current_date);
      }
  }

  /*
   * Parses words from a line into a vector.
   * Paraphrased from stackoverflow: http://stackoverflow.com/questions/236129/split-a-string-in-c
   */
  std::vector<std::string> &transaction::get_words(std::vector<std::string> &words, const std::string &line, char delimiter)
  {
    //Parse words
    std::stringstream reading(line);
  
    //Push words separated by delimiter into vector
    std::string word;
    while(getline(reading, word, delimiter))
      {
	words.push_back(word);
      }
  
    return words;
  }

  /*
   * Simplifies call to get_words.
   */
  std::vector<std::string> transaction::get_words(const std::string &line, char delimiter)
  {
    //make vector for splitting
    std::vector<std::string> words;
    get_words(words, line, delimiter);

    return words;
  }

  /*
   * Processes the final report for our transaction simulation.
   * The return type is a pair of vectors where the vectors hold
   * pairs of strings of which we need to print.
   */
  std::pair<std::vector<std::pair<std::string, std::string> >, std::vector<std::pair<std::string, std::string> > > transaction:: get_report()
  {
    //Create vectors to hold our reports
    std::vector<std::pair<std::string, std::string > > unstocked;
    std::vector<std::pair<std::string, std::string > > wellstocked;

    //Check every food item
    BOOST_FOREACH(_food_items::value_type& food, food_items)
      {
	//Keep track of items warehouses where the food is unstocked and wellstocked
	int _unstocked = 0;
	int _wellstocked = 0;

	//Inside every warehouse
	BOOST_FOREACH(_warehouses::value_type& _warehouse, warehouses)
	  {
	    //If food amount in inventory is 0
	    if(_warehouse.second.inventory.at(food.first).get_total_amount() == 0)
	      {
		_unstocked++;
	      }
	    else
	      {
		_wellstocked++;
	      }
	  }

	//If we are unstocked in every warehouse
	if(_unstocked == warehouses.size())
	  {
	    unstocked.push_back(make_pair(food.second.get_UPC() , food.second.get_name()));
	  }
	//If we are still stocked in two or more warehouses
	else if(_wellstocked >= 2)
	  {
	    wellstocked.push_back(make_pair(food.second.get_UPC() , food.second.get_name()));
	  }
      }

    //Give the report to our main function for reporting
    return std::make_pair(unstocked, wellstocked);
  }

  void transaction::processRequests()
  {
      while(requests.size() > 0)
      {
        std::vector<std::string > words = requests.front();
        process_request(words);
        requests.pop();
      }
  }
}


