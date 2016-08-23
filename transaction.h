/* Filename: transaction.h
 * Author: Conan Zhang
 * Last Modified: 2/8/2015
 * Version: 1.0
 */

/*
 * Contains declarations of members and functions for transactions.
 */

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<string>
#include<vector>
#include<queue>
#include <map>
#include <boost/date_time/gregorian/gregorian.hpp>//boost date

#include "warehouse.h"//transactions keep track of warehouses
#include "food_item.h"//transactions keep track of food items

namespace transactions
{
  class transaction
  {
  public:
    transaction(char* file_name);//constructor
    ~transaction();//destructor

    //Functions to create objects
    void create_food_item(std::vector<std::string> words, std::string line);
    void create_warehouse(std::vector<std::string> words, std::string line);
    boost::gregorian::date create_date(std::vector<std::string> words);

    //Functions to process receives and requests
    void process_receive(std::vector<std::string> words);
    void process_request(std::vector<std::string> words);
    void update_warehouses(boost::gregorian::date current_date);//expired food

    //Maps to hold warehouses and food items
    typedef std::map<std::string,warehouse>_warehouses;
    _warehouses warehouses;
    typedef std::map<std::string,food_item>_food_items;
    _food_items food_items;

    //Function to process final report
    std::pair<std::vector<std::pair<std::string, std::string> >, std::vector<std::pair<std::string, std::string> > > get_report();

  private:
    void processRequests();

    //Functions to get words from lines
    std::vector<std::string> &get_words(std::vector<std::string> &words, const std::string &line, char delimiter);
    std::vector<std::string> get_words(const std::string &line, char delimiter);

    //keep track of current date for possible expired food items
    boost::gregorian::date current_date;

    //store requests until all receives have been processed
    std::queue<std::vector<std::string > > requests;
  };
}

#endif
