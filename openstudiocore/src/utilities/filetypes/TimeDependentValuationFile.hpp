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

#ifndef UTILITIES_FILETYPES_TIMEDEPENDENTVALUATIONFILE_HPP
#define UTILITIES_FILETYPES_TIMEDEPENDENTVALUATIONFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "../data/DataEnums.hpp"
#include "../units/Unit.hpp"

#include "../core/Path.hpp"
#include "../core/Logger.hpp"

#include <boost/optional.hpp>

namespace openstudio {

class Quantity;

class UTILITIES_API TimeDependentValuationFile {
 public:
  /** @name Getters */
  //@{

  openstudio::path path() const;

  std::string name() const;

  std::string description() const;

  /** Cost of energy for commercial buildings associated with this file, in $/J. */
  boost::optional<double> nominalCommercialCostOfEnergy() const;

  /** Cost of energy for residential buildings associated with this file, in $/J. */
  boost::optional<double> nominalResidentialCostOfEnergy() const;

  std::string checksum() const;

  FuelType fuelType(unsigned columnIndex) const;

  BuildingSector buildingSector(unsigned columnIndex) const;

  std::string units(unsigned columnIndex) const;

  std::vector<double> values(unsigned columnIndex) const;

  std::vector<Quantity> quantities(unsigned columnIndex) const;

  //@}
  /** @name Setters */
  //@{

  /** Returns a TimeDependentValuationFile with system() == targetSystem, if possible. If the file
   *  is already in the given system, returns a copy of this object. Otherwise, creates a new file 
   *  with empty path(). Save the new file to populate its path. */
  boost::optional<TimeDependentValuationFile> convertUnits(UnitSystem targetSystem=UnitSystem::SI) const;

  //@}
  /** @name Queries */
  //@{

  unsigned numColumns() const;

  UnitSystem system() const;

  //@}
  /** @name Serialization */
  //@{

  /** Load file from path. Only successful if file opens, name can be located, and each data 
   *  column can be identified with a fuel type, a building sector, units, and 8760 real values. 
   *  Does not check for existence/validity of nominal cost of energy. If that is not present or
   *  parsable, the ModelObject will always return boost::none for cost-based TDV values. */
  static boost::optional<TimeDependentValuationFile> load(const openstudio::path& p);

  bool save(const openstudio::path& p, bool overwrite=false);

  //@}
 private:
  std::vector<std::vector<std::string> > m_table;
  openstudio::path m_path;
  std::string m_checksum;

  REGISTER_LOGGER("openstudio.TimeDependentValuationFile");

  TimeDependentValuationFile(const std::vector<std::vector<std::string> >& table,const openstudio::path& p=openstudio::path());

  boost::optional<Quantity> mf_extractNominalCostOfEnergy(const std::string& cellContents) const;

  boost::optional<Quantity> mf_extractAndConvertNominalCostOfEnergy(
      const std::string& cellContents,UnitSystem targetSystem=UnitSystem::SI) const;

  std::string mf_convertNominalCostOfEnergyInPlace(const std::string& cellContents,
                                                   UnitSystem targetSystem) const;
};

/** \relates TimeDependentValuationFile */
typedef boost::optional<TimeDependentValuationFile> OptionalTimeDependentValuationFile;

} // openstudio

#endif // UTILITIES_FILETYPES_TIMEDEPENDENTVALUATIONFILE_HPP
