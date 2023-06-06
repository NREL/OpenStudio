/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDD_IDDKEY_HPP
#define UTILITIES_IDD_IDDKEY_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"

#include <boost/optional.hpp>

#include <string>
#include <vector>
#include <ostream>

namespace openstudio {

struct IddKeyProperties;

namespace detail {
  class IddKey_Impl;
}

/** IddKey represents an enumeration value for an IDD field of type choice. */
class UTILITIES_API IddKey
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor */
  IddKey();

  /** Copy constructor shares implementation. */
  IddKey(const IddKey& other);

  //@}
  /** @name Getters */
  //@{

  /** Returns the key name */
  std::string name() const;

  /** Returns the key properties. */
  const IddKeyProperties& properties() const;

  //@}
  /** @name Queries */
  //@{

  /** Returns true if all data is exactly equal. */
  bool operator==(const IddKey& other) const;

  /** Negation of operator==. */
  bool operator!=(const IddKey& other) const;

  //@}
  /** @name Serialization */
  //@{

  /** Load from text. */
  static boost::optional<IddKey> load(const std::string& name, const std::string& text);

  /** Print to os in standard IDD format */
  std::ostream& print(std::ostream& os) const;

  //@}
 private:
  ///@cond
  // pointer to implementation
  std::shared_ptr<detail::IddKey_Impl> m_impl;

  // construct from impl
  IddKey(const std::shared_ptr<detail::IddKey_Impl>& impl);
  ///@endcond

  // configure logging
  REGISTER_LOGGER("openstudio.idd.IddKey");
};

/** \relates IddKey */
using IddKeyVector = std::vector<IddKey>;

/** \relates IddKey */
using OptionalIddKey = boost::optional<IddKey>;

/** \relates IddKey */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const IddKey& iddKey);

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDKEY_HPP
