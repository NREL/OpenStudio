/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef UTILITIES_IDD_IDDENUMS_HPP
#define UTILITIES_IDD_IDDENUMS_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Enum.hpp"

#include <boost/optional.hpp>

#include <vector>
#include <set>

#include <QMetaType>

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
  OPENSTUDIO_ENUM(IddFileType, )
#else
struct UTILITIES_API IddFileType: public ::EnumBase<IddFileType> {
 public: 
  enum domain : int;

  IddFileType()
   : EnumBase<IddFileType>(0) {} 
  IddFileType(const std::string &t_name) 
   : EnumBase<IddFileType>(t_name) {} 
  IddFileType(int t_value) 
   : EnumBase<IddFileType>(t_value) {} 

  static std::string enumName() 
  { return "IddFileType"; }

  domain value() const { return static_cast<domain>(EnumBase<IddFileType>::value()); }

   private:
    friend class EnumBase<IddFileType>;
    typedef std::pair<std::string, int> PT;
    typedef std::vector<PT> VecType;
    static VecType buildStringVec(bool isd);
  };

  inline std::ostream &operator<<(std::ostream &os, const IddFileType &e)
  {
    return os << e.valueName() << "(" << e.value() << ")";
  }

  typedef boost::optional<IddFileType> OptionalIddFileType ;
#endif
/** \relates IddFileType */
typedef std::vector<IddFileType> IddFileTypeVector;
/** \relates IddFileType */
typedef std::set<IddFileType> IddFileTypeSet;

/** \relates IddFileType */
typedef boost::optional<std::vector<IddFileType> > OptionalIddFileTypeVector;
/** \relates IddFileType */
typedef boost::optional<std::set<IddFileType> > OptionalIddFileTypeSet;



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
struct UTILITIES_API IddObjectType: public ::EnumBase<IddObjectType> {
 public: 
  enum domain : int;

  IddObjectType()
   : EnumBase<IddObjectType>(0) {} 
  IddObjectType(const std::string &t_name) 
   : EnumBase<IddObjectType>(t_name) {} 
  IddObjectType(int t_value) 
   : EnumBase<IddObjectType>(t_value) {} 

  static std::string enumName() 
  { return "IddObjectType"; }

  domain value() const { return static_cast<domain>(EnumBase<IddObjectType>::value()); }

   private:
    friend class EnumBase<IddObjectType>;
    typedef std::pair<std::string, int> PT;
    typedef std::vector<PT> VecType;
    static VecType buildStringVec(bool isd);
  };

  inline std::ostream &operator<<(std::ostream &os, const IddObjectType &e)
  {
    return os << e.valueName() << "(" << e.value() << ")";
  }

  typedef boost::optional<IddObjectType> OptionalIddObjectType ;
#endif
/** \relates IddObjectType */
typedef std::vector<IddObjectType> IddObjectTypeVector;
/** \relates IddObjectType */
typedef std::set<IddObjectType> IddObjectTypeSet;

/** \relates IddObjectType */
typedef boost::optional<std::vector<IddObjectType> > OptionalIddObjectTypeVector;
/** \relates IddObjectType */
typedef boost::optional<std::set<IddObjectType> > OptionalIddObjectTypeSet;

} // openstudio

Q_DECLARE_METATYPE(openstudio::IddFileType)
Q_DECLARE_METATYPE(openstudio::IddObjectType)

#endif // UTILITIES_IDD_IDDENUMS_HXX
