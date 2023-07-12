/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDD_IDDKEY_IMPL_HPP
#define UTILITIES_IDD_IDDKEY_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "IddKeyProperties.hpp"

#include "../core/Logger.hpp"

#include <string>
#include <ostream>

namespace openstudio {

// private namespace
namespace detail {

  /** Implementation class for IddKey. */
  class UTILITIES_API IddKey_Impl
  {
   public:
    /// default constructor for serialization
    IddKey_Impl();

    /// equality operator
    bool operator==(const IddKey_Impl& other) const;

    /// get name
    std::string name() const;

    /// get properties
    const IddKeyProperties& properties() const;

    /// load by parsing text
    static std::shared_ptr<IddKey_Impl> load(const std::string& name, const std::string& text);

    /// print idd
    std::ostream& print(std::ostream& os) const;

   private:
    /// partial constructor used by load
    IddKey_Impl(const std::string& name);

    // parse
    void parse(const std::string& text);

    // name
    std::string m_name;

    // properties
    IddKeyProperties m_properties;

    // configure logging
    REGISTER_LOGGER("utilities.idd.IddKey");
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDKEY_IMPL_HPP
