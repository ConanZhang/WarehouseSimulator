/* Filename: warehouse.h
 * Author: Conan Zhang
 * Last Modified: 2/8/2015
 * Version: 1.0
 */

/*
 * Holds declarations of members and functions for warehouses.
 */

#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include<string>
#include<map>
#include <boost/date_time/gregorian/gregorian.hpp>//boost date

#include "food_item.h"//warehouses hold food items

namespace transactions
{
  class warehouse
  {
  public:
    warehouse(std::string name);//Constructor
    ~warehouse();//Destructor

    typedef std::map<std::string, food_item>_inventory;//inventory of warehouse
    _inventory inventory;

    //Functions to process transactions
    void add_food(food_item food);//add existence of food to warehouse
    void receive(std::string UPC, int amount, boost::gregorian::date current_date );//warehouse processing receive
    void request(std::string UPC, int amount );//warehouse processing request
    void update(boost::gregorian::date current_date);//update warehouses of expired items

    //Getters for private members
    std::string get_name();

  private:
    std::string name;//name of warehouse
  };
}

#endif
