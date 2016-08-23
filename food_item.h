/* Filename: food_item.h
 * Author: Conan Zhang
 * Last Modified: 2/8/2015
 * Version: 1.0
 */

/*
 * Contains declarations of members and functions of food_items.
 */

#ifndef FOOD_ITEM_H
#define FOOD_ITEM_H

#include <string>
#include <vector>
#include <utility>//std::pair
#include <boost/date_time/gregorian/gregorian.hpp>//boost date

namespace transactions
{
  class food_item
  {
  public:
    food_item(std::string UPC = "", int shelf_life = 0, std::string name = "");//constructor
    food_item(const food_item & other);//Copy constructor
    ~food_item();//destructor
    
    void add_stock(int amount, boost::gregorian::date expiration_date);//adds stock of item
    void remove_stock(int amount);//removes stock of item
    void update(boost::gregorian::date current_date);//updates expired items

    //getters for private members
    int get_shelf_life() const;
    std::string get_name() const;
    std::string get_UPC() const;
    int get_total_amount() const;

  private:
    std::string UPC;
    int shelf_life;
    std::string name;
    int total_amount;//holds total food stock

    std::vector<std::pair<int, boost::gregorian::date > > stock;//food stock
  };
}

#endif
