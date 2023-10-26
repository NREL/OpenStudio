/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_IDFOBJECTDIFF_IMPL_HPP
#define UTILITIES_IDF_IDFOBJECTDIFF_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include <boost/optional.hpp>

#include <string>

namespace openstudio {
namespace detail {

  class UTILITIES_API IdfObjectDiff_Impl
  {
   public:
    IdfObjectDiff_Impl();

    IdfObjectDiff_Impl(boost::optional<unsigned> index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue);

    virtual ~IdfObjectDiff_Impl() = default;

    /// get the index that changed, uninitialized means no field (e.g. object level comment changed)
    boost::optional<unsigned> index() const;

    /// returns true if oldValue == newValue
    bool isNull() const;

    /// get the old value, uninitialized optional means that the field did not previously exist
    boost::optional<std::string> oldValue() const;

    /// get the new value, uninitialized optional means that the field no longer exists
    boost::optional<std::string> newValue() const;

   private:
    boost::optional<unsigned> m_index;
    boost::optional<std::string> m_oldValue;
    boost::optional<std::string> m_newValue;
  };

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_IDF_IDFOBJECTDIFF_IMPL_HPP
