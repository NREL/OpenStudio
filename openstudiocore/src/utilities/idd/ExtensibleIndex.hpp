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

#ifndef UTILITIES_IDD_EXTENSIBLEINDEX_HPP
#define UTILITIES_IDD_EXTENSIBLEINDEX_HPP

#include "../UtilitiesAPI.hpp"

namespace openstudio {

/** Index pair for IdfObject fields in extensible groups. For example, in 
 *
 *  \code 
 *  ExtensibleIndex extIndex(1,2) 
 *  \endcode
 *
 *  extIndex can be used to access the 3rd field in an object's 2nd extensible group. 
 *  extIndex.group == 1, extIndex.field == 2. */
class UTILITIES_API ExtensibleIndex {
 public:
  ExtensibleIndex(unsigned groupIndex, unsigned fieldIndex);

  unsigned group;
  unsigned field;

 private:
  ExtensibleIndex(); 
};

} // openstudio

#endif // UTILITIES_IDD_EXTENSIBLEINDEX_HPP
