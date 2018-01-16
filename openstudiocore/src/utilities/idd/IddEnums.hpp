/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
