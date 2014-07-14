/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_IDF_OBJECTORDERBASE_HPP
#define UTILITIES_IDF_OBJECTORDERBASE_HPP

#include "../UtilitiesAPI.hpp"

#include <utilities/idd/IddEnums.hxx>

#include "../core/Logger.hpp"
#include "../core/Optional.hpp"

/** \file ObjectOrderBase.hpp 
 *  
 *  Handles ordering by IddObjectType. Does not explicitly sort any objects, but provides 
 *  methods for user-manipulation of the order, and a predicate function, less, that compares
 *  IddObjectTypes. */

namespace openstudio {

/** Base class for ordering objects. Provides ordering by IddObjectType enum (using the 
 *  implicit conversion to int), and user-specified ordering of IddObjectTypes. */ 
class UTILITIES_API ObjectOrderBase {
 public:

  // CONSTRUCTORS

  /** Default constructor establishes ordering by IddObjectType enum. */
  ObjectOrderBase();

  /** Constructor specifies that a derived class is in charge. */
  ObjectOrderBase(bool directOrder);

  /** A user-specified order of IddObjectTypes is established on construction. */
  ObjectOrderBase(const IddObjectTypeVector& iddOrder);

  virtual ~ObjectOrderBase() {}

  // GETTERS AND SETTERS

  /** Returns true if ordering by IddObjectType enum. */
  bool orderByIddEnum() const;
  /** Clears out the previous order, and establishes IddObjectType enum as the orderer. */
  virtual void setOrderByIddEnum();

  /** Returns the user-specified order by IddObjectType, if it exists. Otherwise return value 
   *  evaluates to false. */
  OptionalIddObjectTypeVector iddOrder() const;
  /** Clears out the previous order, and establishes order as the new basis for comparison. */
  virtual void setIddOrder(const IddObjectTypeVector& order);
  /** Adds type to the user-specified IddObjectType order, if it exists. Returns false if such 
   *  an order does not exist. */
  bool push_back(IddObjectType type);
  /** Inserts type before insertBeforeType in the user-specified IddObjectType order, if it
   *  exists. Returns false if such an order does not exist. If insertBeforeType is not in the 
   *  order, this method is equivalent to push_back(type). */
  bool insert(IddObjectType type, IddObjectType insertBeforeType);
  /** Inserts type at index of the user-specified IddObjectType order, if it exists, and index
   *  is valid. Returns false if such an order does not exist. Is equivalent to push_back(type)
   *  if index is out of range. */
  bool insert(IddObjectType type, unsigned index);
  /** Moves type from current location and places before insertBeforeType, if a user-specified 
   *  IddObjectType order exists. Otherwise, returns false. Also returns false if type is not 
   *  in the order. If insertBeforeType is not in the order, this method is equivalent to 
   *  move(type,order.size()). */
  bool move(IddObjectType type, IddObjectType insertBeforeType);
  /** Moves type from current location to index in user-specified IddObjectType order, if it 
   *  exists. Returns false otherwise, or if type is not in the order. If index is invalid, 
   *  this method is equivalent to move(type,order.size()). */
  bool move(IddObjectType type, unsigned index);
  /** Swaps type1 and type2 in the user-specified IddObjectType order, if it exists and contains
   *  type1 and type2. Otherwise, returns false. */
  bool swap(IddObjectType type1, IddObjectType type2);
  /** Deletes type from the user-specified IddObjectType order, if it exists and contains type. 
   *  Otherwise, returns false. */
  bool erase(IddObjectType type);

  /** Disables ordering by IddObjectType. Is called by derived classes to signal that they are 
   *  setting the order. */
  void setDirectOrder();

  // SORTING

  /** Predicate function used by derived classes to sort objects and handles. */
  virtual bool less(IddObjectType left, IddObjectType right) const;

  /** Predicate function used by derived classes to sort objects and handles. Assumes 
   *  uninitialized optionals are always greater than. */
  virtual bool less(boost::optional<IddObjectType> left, boost::optional<IddObjectType> right) const;

  // STATE CHECKING

  /** Returns true if ordering by IddObjectType enum, or if ordering by user-specified 
   *  IddObjectType order, and type is explicitly listed. */
  bool inOrder(const IddObjectType& type) const;

  /** Returns the index of type in IddObjectType order if ordering directly by enum, or by 
   *  user-specified order. Otherwise, the return value evaluates to false. */
  OptionalUnsigned indexInOrder(const IddObjectType& type) const;

 protected:

  bool m_orderByIddEnum;
  OptionalIddObjectTypeVector m_iddOrder;   

  // HELPER FUNCTIONS
  IddObjectTypeVector::iterator getIterator(const IddObjectType& type);
  IddObjectTypeVector::const_iterator getIterator(const IddObjectType& type) const;

 private:

  REGISTER_LOGGER("utilities.idf.ObjectOrderBase");
};

} // openstudio

#endif // UTILITIES_IDF_OBJECTORDERBASE_HPP
