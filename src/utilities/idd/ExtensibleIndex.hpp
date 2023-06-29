/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
class UTILITIES_API ExtensibleIndex
{
 public:
  ExtensibleIndex(unsigned groupIndex, unsigned fieldIndex);

  unsigned group;
  unsigned field;

 private:
  ExtensibleIndex();
};

}  // namespace openstudio

#endif  // UTILITIES_IDD_EXTENSIBLEINDEX_HPP
