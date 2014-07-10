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

#include "TimeDependentValuationFile.hpp"

#include "../units/QuantityRegex.hpp"
#include "../units/UnitFactory.hpp"
#include "../units/QuantityFactory.hpp"
#include "../units/QuantityConverter.hpp"
#include "../units/SIUnit.hpp"

#include "../core/Checksum.hpp"
#include "../core/Containers.hpp"
#include "../core/Assert.hpp"
#include "../core/StringHelpers.hpp"

#include <boost/regex.hpp>

#include <QFile>
#include <QString>
#include <QStringList>


namespace openstudio {

openstudio::path TimeDependentValuationFile::path() const {
  return m_path; 
}

std::string TimeDependentValuationFile::name() const {
  std::string result;

  OS_ASSERT(m_table.size() > 0);
  if (m_table[0].size() > 0){
    result = m_table[0][0];
  }

  return result;
}

std::string TimeDependentValuationFile::description() const {
  std::string result;

  OS_ASSERT(m_table.size() > 0);
  if (m_table[0].size() > 1){
    result = m_table[0][1];
  }

  return result;
}

boost::optional<double> TimeDependentValuationFile::nominalCommercialCostOfEnergy() const {
  LOG(Debug,"Extracting nominalCommercialCostOfEnergy.");

  OS_ASSERT(m_table.size() > 0);
  if (m_table[0].size() > 2){
    std::string cell = m_table[0][2];
    boost::optional<Quantity> oq = mf_extractAndConvertNominalCostOfEnergy(cell);
    if (oq) { 
      return oq->value(); 
    }
  }

  return boost::none;
}

boost::optional<double> TimeDependentValuationFile::nominalResidentialCostOfEnergy() const {
  LOG(Debug,"Extracting nominalResidentialCostOfEnergy.");

  OS_ASSERT(m_table.size() > 0);
  if (m_table[0].size() > 3){
    std::string cell = m_table[0][3];
    boost::optional<Quantity> oq = mf_extractAndConvertNominalCostOfEnergy(cell);
    if (oq) {
      return oq->value();
    }
  }

  return boost::none;
}

std::string TimeDependentValuationFile::checksum() const {
  return m_checksum;
}

FuelType TimeDependentValuationFile::fuelType(unsigned columnIndex) const {
  OptionalFuelType oft;
  
  if (columnIndex >= numColumns()) {
    LOG_AND_THROW("Cannot return fuelType for column " << columnIndex 
        << ", because the time dependent valuation file at '" << toString(m_path) 
        << "' only has " << numColumns() << " columns.");
  }

  std::string cell;
  OS_ASSERT(m_table.size() > 1);
  if (columnIndex < m_table[1].size()) {
    cell = m_table[1][columnIndex];
  }
  if (cell.empty() && columnIndex > 0 && columnIndex-1 < m_table[1].size()) {
    cell = m_table[1][columnIndex-1];
  }
  if (cell.empty()) {
    LOG_AND_THROW("Unexpected format in time dependent valuation file at '" << toString(m_path)
        << "'. Could not determine FuelType for column " << columnIndex << ".");
  }
  cell = toCamelCase(cell);  
  try {
    oft = FuelType(cell);
  }
  catch (...) {}
  if (!oft) {
    // try pseudonyms
    if (istringEqual(cell,"Electric")) { 
      oft = FuelType(FuelType::Electricity); 
    }
  }
  if (!oft) {
    LOG_AND_THROW("Unexpected format in time dependent valuation file at '" << toString(m_path)
        << "'. Could not determine FuelType for column " << columnIndex << " from cell contents '"
        << cell << "'.");
  }
  return *oft;
}

BuildingSector TimeDependentValuationFile::buildingSector(unsigned columnIndex) const {
  OptionalBuildingSector obs;

  if (columnIndex >= numColumns()) {
    LOG_AND_THROW("Cannot return buildingSector for column " << columnIndex 
        << ", because the time dependent valuation file at '" << toString(m_path) 
        << "' only has " << numColumns() << " columns.");
  }

  OS_ASSERT(m_table.size() > 1);
  std::string cell;
  if (columnIndex < m_table[2].size()) {
    cell = m_table[2][columnIndex];
  }
  if (cell.empty()) {
    LOG_AND_THROW("Unexpected format in time dependent valuation file at '" << toString(m_path)
        << "'. Could not determine BuildingSector for column " << columnIndex << ".");
  }
  cell = toCamelCase(cell);
  try {
    obs = BuildingSector(cell);
  }
  catch (...) {}
  if (!obs) {
    // try pseudonyms
    if (istringEqual(cell,"Res")) { 
      obs = BuildingSector(BuildingSector::Residential); 
    }
    else if (istringEqual(cell,"NonRes")) { 
      obs = BuildingSector(BuildingSector::Commercial); 
    }
    else if (istringEqual(cell,"Non-Residential")) { 
      obs = BuildingSector(BuildingSector::Commercial); 
    }
  }
  if (!obs) {
    LOG_AND_THROW("Unexpected format in time dependent valuation file at '" << toString(m_path)
        << "'. Could not determine BuildingSector for column " << columnIndex << " from cell contents '"
        << cell << "'.");
  }
  return *obs;
}

std::string TimeDependentValuationFile::units(unsigned columnIndex) const {
  OS_ASSERT(m_table.size() > 3);
  std::string result;
  if (columnIndex < m_table[3].size()){
    result = m_table[3][columnIndex];
  }
  return result;
}

std::vector<double> TimeDependentValuationFile::values(unsigned columnIndex) const {
  if (columnIndex >= numColumns()) {
    LOG_AND_THROW("Cannot return values for column " << columnIndex << ", because the time "
        << "dependent valuation file at '" << toString(m_path) << "' only has " << numColumns() 
        << " columns.");
  }
  if (m_table.size() - 4 != 8760u) {
    LOG_AND_THROW("Unexpected format in time dependent valuation file at '" << toString(m_path)
        << "'. Expected 8760 hourly values, one hour per row in body of table, but body of table contains " 
        << m_table.size() - 4 << " values.");
  }
  DoubleVector result;
  for (unsigned i = 4; i < 8764u; ++i) {
    std::string cell;
    if (columnIndex < m_table[i].size()){
      cell = m_table[i][columnIndex];
      bool ok;
      double value = toQString(cell).toDouble(&ok);
      if (!ok) {
        LOG_AND_THROW("Unexpected format in time dependent valuation file at '" << toString(m_path)
          << "'. Could not covert element at [" << i << "][" << columnIndex << "],'" << cell
          << "', to double value.");
      }
      result.push_back(value);
    }
  }
  OS_ASSERT(result.size() == 8760u);
  return result;
}

std::vector<Quantity> TimeDependentValuationFile::quantities(unsigned columnIndex) const {
  std::string unitString = units(columnIndex);
  DoubleVector vals = values(columnIndex);
  QuantityVector result;
  for (double val : vals) {
    Quantity q;
    if (result.empty()) {
      q = createQuantity(val,unitString).get();
    }
    else { q = result.back(); }
    q.setValue(val);
    result.push_back(q);
  }
  OS_ASSERT(result.size() == 8760u);
  return result;
}

boost::optional<TimeDependentValuationFile> TimeDependentValuationFile::convertUnits(
    UnitSystem targetSystem) const
{
  if (system() == targetSystem) {
    return *this; 
  }
  return boost::none;
  /*
  std::vector<std::vector<std::string> > result = m_table; // copy table to return if successful

  OS_ASSERT(m_table.size() > 0);

  // try to convert nominal cost of energy
  if (numColumns() > 2) {
    OS_ASSERT(m_table[0].size() > 2);
    std::string cell = result[0][2]; // commercial
    std::string newCell = mf_convertNominalCostOfEnergyInPlace(cell,targetSystem);
    if (newCell.empty()) {
      LOG(Info,"Unable to convert units of nominal commercial cost of energy, '" << cell << "', in "
          << "time dependent valuation file '" << toString(m_path) << "'.");
    }
    result[0][2] = TableElement(newCell);

    if (numColumns > 3) {
      OS_ASSERT(m_table[0].size() > 3);
      cell = result[0][3].toString(); // residential
      newCell = mf_convertNominalCostOfEnergyInPlace(cell,targetSystem);
      if (newCell.empty()) {
        LOG(Info,"Unable to convert units of nominal residential cost of energy, '" << cell << "', in "
            << "time dependent valuation file '" << toString(m_path) << "'.");
      }
      result[0][3] = TableElement(newCell);
    }
  }

  // provide for nice units string in dimensionless case
  SIUnit siEnergyUnits = createSIEnergy();
  Quantity energy(1.0,siEnergyUnits);
  energy = convert(energy,targetSystem).get();
  std::stringstream ss;
  if (energy.prettyUnitsString(false).empty()) {
    ss << energy.standardUnitsString() << "/" << energy.standardUnitsString();
  }
  else {
    ss << energy.prettyUnitsString() << "/" << energy.prettyUnitsString();
  }

  // try to convert columns
  for (unsigned i = 0, n = result.nCols(); i < n; ++i) {
    std::string unitString = units(i);
    OS_ASSERT(!unitString.empty());
    bool ok = result.convertUnits(i,Table::HEAD,3u,targetSystem,OptionalInt(0));
    if (!ok) {
      LOG(Info,"Unable to convert units of column " << i << " in time dependent valuation file "
          << "'" << toString(m_path) << "'.");
      return boost::none;
    }
    // if no unit string in the result, set J/J or similar
    unitString = result.units(i,Table::HEAD,3u);
    if (unitString.empty()) {
      result[3][i] = TableElement(ss.str());
    }
  }

  return TimeDependentValuationFile(result);
  */
}

unsigned TimeDependentValuationFile::numColumns() const {
  unsigned result = 0;
  if (m_table.size() > 2){
    result = m_table[2].size();
  }
  return result;
}

UnitSystem TimeDependentValuationFile::system() const {
  bool foundASystem(false);
  UnitSystem result;
  /*
  // energy costs
  if (m_table.nCols() > 2) {
    std::string cell = print(m_table[0][2]); // commercial cost of energy
    OptionalQuantity oq = mf_extractNominalCostOfEnergy(cell);
    if (oq) {
      result = oq->system();
      foundASystem = true;
    }
    if (m_table.nCols() > 3) {
      cell = print(m_table[0][3]); // residential cost of energy
      oq = mf_extractNominalCostOfEnergy(cell);
      if (oq) {
        if (foundASystem) {
          if (oq->system() != result) {
            return UnitSystem::Mixed;
          }
        }
        else {
          result = oq->system();
          foundASystem = true;
        }
      }
    }
  }

  // column units
  for (unsigned i = 0, n = numColumns(); i < n; ++i) {
    std::string unitString = units(i);
    if (!unitString.empty()) {
      OptionalUnit unit = UnitFactory::instance().createUnit(unitString);
      if (!unit) { return UnitSystem::Mixed; }
      if (foundASystem) {
        if (unit->system() != result) { return UnitSystem::Mixed; }
      }
      else {
        result = unit->system();
        foundASystem = true;
      }
    }
  }
  */
  return result;
}

boost::optional<TimeDependentValuationFile> TimeDependentValuationFile::load(
    const openstudio::path& p)
{
  LOG(Debug,"Loading TimeDependentValuationFile from '" << toString(p) << "'.");

  // load table
  std::vector<std::vector<std::string> > candidateTable;
  QFile f(toQString(p));
  if (f.open(QIODevice::ReadOnly)){
    //file opened successfully
    QTextStream in(&f);
    while (!in.atEnd()){
      QString line = in.readLine();

      // DLM: this does not work for cells with commas in quotes
      // Climate zone 13 with externalities,"Created April 18, 2006",Nominal nonres $0.145972/kBtu,Nominal res $0.164171/kBtu,,
      // QStringList line = in.readLine().split(',', QString::KeepEmptyParts);

      bool inside = (line.at(0) == QString("\"")); //true if the first character is "
      QStringList tmpList = line.split(QRegExp("\""), QString::KeepEmptyParts); // Split by " and make sure you don't have an empty string at the beginning
      QString escapedLine;
      QString comma(",");
      QString escapeSeq("&#45");
      for (QString s: tmpList) {
        if (inside) { // If 's' is inside quotes ...
          escapedLine.append(s.replace(comma, escapeSeq)); // escape commas and add to the string
        } else { // If 's' is outside quotes ...
          escapedLine.append(s); // add to the string
        }
        inside = !inside;
      }

      QStringList csvList = escapedLine.split(",", QString::KeepEmptyParts);

      std::vector<std::string> row;
      for (QString cell : csvList){
        row.push_back(toString(cell.replace(escapeSeq, comma)));
      }
      candidateTable.push_back(row);
    }
    f.close();
  }

  if (candidateTable.size() != 8764u) {
    LOG(Error,"Table loaded from '" << toString(p) << "' for construction of "
        << " TimeDependentValuationFile has " << candidateTable.size() << " rows. "
        << "Was expecting 8764.");
    return boost::none;
  }

  // construct file object
  TimeDependentValuationFile candidateFile(candidateTable,p);
  
  // check that contents are ok
  for (unsigned i = 0, n = candidateFile.numColumns(); i < n; ++i) {
    // must be able to determine FuelType, BuildingSector, and energy/energy units.
    std::string unitString;
    try {
       candidateFile.fuelType(i); //FuelType ft =
       candidateFile.buildingSector(i); //BuildingSector bs = 
       candidateFile.units(i); //unitString = 
    }
    catch (...) {
      LOG(Error,"Unexpected format in candidate time dependent valuation file '" << toString(p) 
          << "'. Unable to determine fuel type, building sector, or units for column " << i << ".");
      return boost::none;
    }    
    Quantity q = QuantityFactory::instance().createQuantity(1.0,unitString).get();
    Quantity qTestResult = convert(q,UnitSystem(UnitSystem::SI)).get();
    if (qTestResult.standardUnitsString(false) != "") {
      LOG(Error,"Unexpected units in column " << i << " of candidate time dependent valuation "
          << "file '" << toString(p) << "'. Conversion to SI resulted in units " 
          << qTestResult.standardUnitsString(false) << ". Expected result to be dimensionless.");
      return boost::none;
    }
    q *= Quantity(1.0,SIUnit(SIExpnt(1,2,-2))); // multiply by Joules
    qTestResult = convert(q,UnitSystem(UnitSystem::SI)).get();
    if (qTestResult.prettyUnitsString(false) != "J") {
      LOG(Error,"Unexpected units in column " << i << " of candidate time dependent valuation "
          << "file '" << toString(p) << "'. Conversion to SI of units times '1.0 J' resulted in "
          << "units " << qTestResult.standardUnitsString(false) << ". Expected result to have units of "
          << "J (kg*m^2/s^2).");
      return boost::none;
    }
  }

  return candidateFile;
}

bool TimeDependentValuationFile::save(const openstudio::path& p, bool overwrite) {

  if (!overwrite && QFile::exists(toQString(p))){
    return false;
  }

  QFile file(toQString(p));
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
    return false;
  }

  QTextStream out(&file);
  for (const std::vector<std::string>& row : m_table){
    unsigned n = row.size();
    for (unsigned i = 0; i < n; ++i){
      out << row[i];
      if (i < n - 1){
        out << ", ";
      }
    }
    out << "\n";
  }
  file.close();

  return true;
}

TimeDependentValuationFile::TimeDependentValuationFile(const std::vector<std::vector<std::string> >& table,
                                                       const openstudio::path& p) 
  : m_table(table), m_path(p)
{
  if (!m_path.empty()) {
    m_checksum = openstudio::checksum(m_path);
  }
}

boost::optional<Quantity> TimeDependentValuationFile::mf_extractNominalCostOfEnergy(
    const std::string& cellContents) const
{
  OptionalQuantity result;

  std::stringstream ss;
  ss << "[^\\$]*\\$(" << regexQuantity().str() << ")";
  boost::regex re(ss.str());
  boost::smatch matches;
  if (boost::regex_search(cellContents,matches,re)) {  
    std::string quantityString(matches[1].first,matches[1].second);
    Quantity candidate = createQuantity(quantityString).get();
    if (candidate.value() > 0.0) {
      result = candidate;
    }
  } 
  return result;
}

boost::optional<Quantity> TimeDependentValuationFile::mf_extractAndConvertNominalCostOfEnergy(
    const std::string& cellContents,UnitSystem targetSystem) const
{
  OptionalQuantity result;

  OptionalQuantity intermediate = mf_extractNominalCostOfEnergy(cellContents);
  if (intermediate) {
    try {
      Quantity candidate = convert(*intermediate,UnitSystem(UnitSystem::SI)).get();
      candidate.setScale(0);
      if (candidate.prettyUnitsString() == "1/J") {
        result = candidate;
      }
      else {
        LOG(Error,"Unexpected units found in time dependent valuation file '" 
            << toString(m_path) << "' nominal cost of energy. Extracted $" << *intermediate 
            << " and after conversion, wound up with $" << candidate 
            << ". Was expecting to get to $/J.");
      }
    }
    catch (...) {
      LOG(Error,"Unable to convert nominal cost of energy in time dependent valuation file '" 
          << toString(m_path) << "' to SI units. Extracted $" << *intermediate 
          << "as the quantity to be converted.");
    }
  }
  return result;
}

std::string TimeDependentValuationFile::mf_convertNominalCostOfEnergyInPlace(
    const std::string& cellContents,UnitSystem targetSystem) const
{
  std::string result;
  std::stringstream ss;

  ss << "[^\\$]*\\$(" << regexQuantity().str() << ")";
  boost::regex re(ss.str());
  boost::smatch matches;
  if (boost::regex_search(cellContents,matches,re)) {  
    std::string quantityString(matches[1].first,matches[1].second);
    Quantity original = createQuantity(quantityString).get();
    if (original.value() > 0.0) {
      try {
        Quantity converted = convert(original,targetSystem).get();
        int scaleExponent(0);
        if (targetSystem == UnitSystem::SI) {
          scaleExponent = -6;
        }
        else if (targetSystem == UnitSystem::BTU) {
          scaleExponent = -3;
        }
        converted.setScale(scaleExponent);
        ss.str("");
        ss << converted;
        result = cellContents;
        result.replace(int(matches[1].first - matches[0].first),quantityString.size(),ss.str());
      }
      catch(...) {
        LOG(Error,"Unable to convert and replace units in nominal cost of energy cell with contents '" 
            << cellContents << "' in time dependent valuation file '" << toString(m_path) << "'.");
      }
    }
  } 
  return result;
}

} // openstudio 
