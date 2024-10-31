/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDD_IDDENUMS_HPP
#define UTILITIES_IDD_IDDENUMS_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Enum.hpp"

#include <boost/optional.hpp>

#include <vector>
#include <set>

namespace openstudio {

/** \class IddFileType
 *  \brief Enumeration of the types of \link openstudio::IddFile IddFile\endlink available
 *  through OpenStudio.
 *
 *  UserCustom \link openstudio::IddFile IddFiles\endlink are loaded directly from disk, and
 *  typically correspond to old or under-development versions of EnergyPlus or OpenStudio. The
 *  rest of the enumeration values designate subsets of the \link openstudio::IddFactorySingleton
 *  IddFactory\endlink (the current versions of the EnergyPlus and OpenStudio IDDs, and all
 *  objects in the factory). */
#ifdef SWIG
  // cppcheck-suppress syntaxError
  OPENSTUDIO_ENUM(IddFileType, )
#else
struct UTILITIES_API IddFileType : public ::EnumBase<IddFileType>
{
 public:
  enum domain : int;

  IddFileType() : EnumBase<IddFileType>(0) {}
  IddFileType(const std::string& t_name) : EnumBase<IddFileType>(t_name) {}
  IddFileType(int t_value) : EnumBase<IddFileType>(t_value) {}

  static std::string enumName() {
    return "IddFileType";
  }

  domain value() const {
    return static_cast<domain>(EnumBase<IddFileType>::integer_value());
  }

 private:
  friend class EnumBase<IddFileType>;
  using PT = std::pair<std::string, int>;
  using VecType = std::vector<PT>;
  static VecType buildStringVec(bool isd);
};

inline std::ostream& operator<<(std::ostream& os, const IddFileType& e) {
  return os << e.valueName() << "(" << e.value() << ")";
}

using OptionalIddFileType = boost::optional<IddFileType>;
#endif
/** \relates IddFileType */
using IddFileTypeVector = std::vector<IddFileType>;
/** \relates IddFileType */
using IddFileTypeSet = std::set<IddFileType>;

/** \relates IddFileType */
using OptionalIddFileTypeVector = boost::optional<std::vector<IddFileType>>;
/** \relates IddFileType */
using OptionalIddFileTypeSet = boost::optional<std::set<IddFileType>>;

/** \class IddObjectType
 *  \brief Enumeration of the \link openstudio::IddObject IddObject\endlink types available
 *  through the \link openstudio::IddFactorySingleton IddFactory\endlink.
 *  \details Catchall is the default constructed \link openstudio::IddObject IddObject\endlink
 *  type. UserCustom is the default type for \link openstudio::IddObject IddObjects\endlink
 *  constructed by \link openstudio::IddObject::load IddObject::load\endlink. UserCustom objects
 *  must be referenced by name through an \link openstudio::IddFile IddFile\endlink or \link
 *  openstudio::IddFileAndFactoryWrapper IddFileAndFactoryWrapper\endlink. They cannot be
 *  accessed through the \link openstudio::IddFactorySingleton IddFactory\endlink (by name or
 *  type). CommentOnly is a convenience object for capturing standalone comments in IDFs. All other
 *  types are derived from the IDD files used to create \link openstudio::IddFactorySingleton
 *  IddFactory\endlink. See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp.
 * */
#ifdef SWIG
  OPENSTUDIO_ENUM(IddObjectType, )
#else
struct UTILITIES_API IddObjectType : public ::EnumBase<IddObjectType>
{
 public:
  enum domain : int;

  IddObjectType() : EnumBase<IddObjectType>(0) {}
  IddObjectType(const std::string& t_name) : EnumBase<IddObjectType>(t_name) {}
  IddObjectType(int t_value) : EnumBase<IddObjectType>(t_value) {}

  static std::string enumName() {
    return "IddObjectType";
  }

  domain value() const {
    return static_cast<domain>(EnumBase<IddObjectType>::integer_value());
  }

 private:
  friend class EnumBase<IddObjectType>;
  using PT = std::pair<std::string, int>;
  using VecType = std::vector<PT>;
  static VecType buildStringVec(bool isd);
};

inline std::ostream& operator<<(std::ostream& os, const IddObjectType& e) {
  return os << e.valueName() << "(" << e.value() << ")";
}

using OptionalIddObjectType = boost::optional<IddObjectType>;
#endif
/** \relates IddObjectType */
using IddObjectTypeVector = std::vector<IddObjectType>;
/** \relates IddObjectType */
using IddObjectTypeSet = std::set<IddObjectType>;

/** \relates IddObjectType */
using OptionalIddObjectTypeVector = boost::optional<std::vector<IddObjectType>>;
/** \relates IddObjectType */
using OptionalIddObjectTypeSet = boost::optional<std::set<IddObjectType>>;

}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDENUMS_HXX
