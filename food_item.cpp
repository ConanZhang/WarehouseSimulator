/* Filename: food_item.cpp
 * Author: Conan Zhang
 * Last Modified: 2/8/2015
 * Version: 1.0
 */

/*
 * Food_items have a UPC, shelf_life, and name.
 * They can add, remove, and update stock they keep track of.
 */

#include <algorithm>

#include "food_item.h"

namespace transactions
{
  /*
   * Constructor
   */
  food_item::food_item(std::string UPC, int shelf_life, std::string name)
  {
    this->UPC = UPC;
    this->shelf_life = shelf_life;
    this->name = name;
  }

  /*
   * Copy constructor
   */
  food_item::food_item(const food_item & other)
  {
    this->UPC = other.UPC;
    this->shelf_life = other.shelf_life;
    this->name = other.name;
  }

  /*
   * Destructor
   */
  food_item::~food_item()
  {
  }

  /*
   * Add to amount of stock this food has
   */
  void food_item::add_stock(int amount, boost::gregorian::date expiration_date)
  {
    total_amount += amount;
    stock.push_back(std::make_pair(amount, expiration_date));
  }

  /*
   * Remove amount of stock this food has
   */
  void food_item::remove_stock(int amount)
  {
    if(total_amount > amount)
      {
	total_amount-=amount;
	
	//Remove amount from stock 
	for(std::vector<std::pair<int, boost::gregorian::date> >::size_type i = 0; i != stock.size(); i++)
	  {
	    //Stop removing if have more stock than amount
	    if(stock[i].first > amount)
	      {
		stock[i].first -= amount;

		//Delete previous entries if not on first element
		if(i > 0)
		  {
		    stock.erase(stock.begin() + i - 1);
		  }
		break;
	      }
	    //Erase all stock up to this element
	    else if(stock[i].first == amount)
	      {
		stock.erase(stock.begin() + i);
		break;
	      }
	    //Continue erasing
	    else
	      {
		amount-=stock[i].first;
	      }
	  }
      }
    //Remove all stock and clear storage
    else if(total_amount <= amount)
      {
	total_amount = 0;
	stock.clear();
      }
  }

 /*
   * Remove expired stock this food has
   */
  void food_item::update(boost::gregorian::date current_date)
  {
    int expired_amount = 0;
      //Remove expired stock 
	for(std::vector<std::pair<int, boost::gregorian::date> >::size_type i = 0; i != stock.size(); i++)
	  {
	    //Stop removing if have more stock than amount
	    if(stock[i].second > current_date)
	      {
		//Delete previous entries if not on first element
		if(i > 0)
		  {
		    stock.erase(stock.begin() + i - 1);
		  }
		break;
	      }
	    //Continue removing
	    else if(stock[i].second <= current_date)
	      {
		//Clear all stock if it's all expired
		if(i == stock.size() )
		  {
		    stock.clear();
		  }
		//Keep track of how much stock has expired
		expired_amount += stock[i].first;
	      }
	  }
	//Subtract expired from total
	total_amount -= expired_amount;

	//We might have expired the entire inventory
	if(total_amount < 0)
	  {
	    total_amount = 0;
	  }
  }

  /*
   * Getters
   */
  int food_item::get_shelf_life() const
  {
    return shelf_life;
  }

  std::string food_item::get_UPC() const
  {
    return UPC;
  }

  std::string food_item::get_name() const
  {
    return name;
  }

  int food_item::get_total_amount() const
  {
    return total_amount;
  }
}
