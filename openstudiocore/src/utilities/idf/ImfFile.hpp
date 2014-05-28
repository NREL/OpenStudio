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

#ifndef UTILITIES_IDF_IMFFILE_HPP
#define UTILITIES_IDF_IMFFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "IdfObject.hpp"

#include "../idd/IddObject.hpp"
#include "../idd/IddFileAndFactoryWrapper.hpp"

#include "../core/Path.hpp"
#include "../core/Containers.hpp"

#include <string>
#include <ostream>
#include <vector>
#include <map>

namespace openstudio{

/** ImfFile represents an Imf file. ImfFile does not have any copy
*   or assignment operators so it is not a shared object.  ImfFile
*   does not support advanced imf syntax, only sectioning by
*   \code
*   ##def Schedules[]
*   \endcode
*   , etc.
*/
class UTILITIES_API ImfFile
{
 public:

  /** @name Constructors */
  //@{

  /// default constructor
  ImfFile(IddFileType iddFileType=IddFileType::EnergyPlus);

  //@}
  /** @name Getters */
  //@{

  /** Get the IddFile. If one is not explicitly held by this object, returns one from
   *  IddFactory per the documentation for this class. */
  IddFile iddFile() const;

  /** Get the IddFile's IddFileType. */
  IddFileType iddFileType() const;

  /** Return the names of all sections. */
  StringVector sectionNames() const;

  /** Get a section by name */
  IdfObjectVector section(const std::string& section) const;

  //@}
  /** @name Queries */
  //@{

  /** Return the number of sections in the ImfFile */
  unsigned numSections() const;

  //@}
  /** @name Serialization */
  //@{

  /// load file from std::istream. assumes objects are in sub-idd file fileType of IddFactory.
  static boost::optional<ImfFile> load(std::istream& is,IddFileType iddFileType);

  /// load file from std::istream. use user-supplied IddFile.
  static boost::optional<ImfFile> load(std::istream& is,const IddFile& iddFile);

  /// load from file path
  static boost::optional<ImfFile> load(const openstudio::path& p, IddFileType iddFileType);

  /// load from file path
  static boost::optional<ImfFile> load(const openstudio::path& p, const IddFile& iddFile);

  /// print file to std::stream
  std::ostream& print(std::ostream& os) const;

  /** Save IddFile to path. Will construct parent folder, but no further up the chain. Will 
   *  only overwrite an existing file if overwrite==true. If no extension is provided will use 
   *  'imf'. */
  bool save(const openstudio::path& p, bool overwrite=false);

  //@}

 private:

  // store all idf objects in a map by section
  typedef std::map<std::string, IdfObjectVector> SectionMapType;
  SectionMapType m_sectionMap;
  IddFileAndFactoryWrapper m_iddFileAndFactoryWrapper;

  bool m_load(std::istream& is);

  // configure logging
  REGISTER_LOGGER("utilities.idf.ImfFile");
};

/// optional imf file
typedef boost::optional<ImfFile> OptionalImfFile;

/// vector of imf file
typedef std::vector<ImfFile> ImfFileVector;

// ostream operator<<
UTILITIES_API std::ostream& operator<<(std::ostream& os, const ImfFile& imfFile);

} // openstudio

#endif //UTILITIES_IDF_IMFFILE_HPP
