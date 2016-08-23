/* Filename: warehouse.cpp
 * Author: Conan Zhang
 * Last Modified: 2/8/2015
 * Version: 1.0
 */

/*
 * Warehouses have a name and inventory. 
 * They process requests, receiving, and updating their inventory as well as
 * adding the existence of foods to their stock.
 */

#include "warehouse.h"

#include <boost/foreach.hpp>

namespace transactions
{
  /*
   * Constructor
   */
  warehouse::warehouse(std::string name)
  {
    this->name = name;
  }

  /*
   * Destructor
   */
  warehouse::~warehouse()
  {

  }

  /*
   * Add the existence of food to our inventory.
   */
  void warehouse::add_food(food_item food)
  {
    inventory.insert(std::map<std::string, food_item>::value_type(food.get_UPC() , food));
  }

  /*
   * Process a receive command.
   */
  void warehouse::receive(std::string UPC, int amount, boost::gregorian::date current_date)
  {
    //Calculate expiration date
    boost::gregorian::date_duration expiration(inventory.at(UPC).get_shelf_life());
    current_date += expiration;

    //Apply to stock
    inventory.at(UPC).add_stock(amount, current_date);
  }

  /*
   * Process a request command.
   */
  void warehouse::request(std::string UPC, int amount)
  {
    inventory.at(UPC).remove_stock(amount);
  }

  /*
   * Update the inventory of expired items.
   */
  void warehouse::update(boost::gregorian::date current_date)
  {
       BOOST_FOREACH(_inventory::value_type& food, inventory)
      {
	food.second.update(current_date);
      }
  }

  /*
   * Getter for name.
   */
  std::string warehouse::get_name()
  {
    return name;
  }
}
