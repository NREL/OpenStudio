/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/document/Table.hpp>
#include <utilities/document/Table_Impl.hpp>
#include <utilities/document/DocumentRegex.hpp>

#include <utilities/units/UnitFactory.hpp>
#include <utilities/units/QuantityFactory.hpp>
#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/OSQuantityVector.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Containers.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/iostreams/filter/newline.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/regex.hpp>

namespace openstudio {

namespace detail {

  // CONSTRUCTORS

  Table_Impl::Table_Impl() :
      m_nRows(0), m_nCols(0),
      m_nHead(0), m_nFoot(0), m_nLeft(0), m_nRight(0) {}

  DocumentElement Table_Impl::clone() const {
    boost::shared_ptr<Table_Impl> impl(new Table_Impl(*this));
    return Table(impl).cast<DocumentElement>();
  }

  std::string Table_Impl::title() const {
    return m_title;
  }

  std::string Table_Impl::caption() const {
    return m_caption;
  }

  TableElement Table_Impl::element(unsigned rowIndex,unsigned colIndex) const {
    OS_ASSERT(rowIndex < m_nRows);
    OS_ASSERT(colIndex < m_nCols);
    return m_table[rowIndex][colIndex];
  }

  std::vector<TableElement>& Table_Impl::operator[](unsigned rowIndex) {
    OS_ASSERT(rowIndex < m_nRows);
    return m_table[rowIndex];
  }

  const std::vector<TableElement>& Table_Impl::operator[](unsigned rowIndex) const {
    OS_ASSERT(rowIndex < m_nRows);
    return m_table[rowIndex];
  }

  // SETTERS

  void Table_Impl::setTitle(const std::string& title) {
    m_title = title;
  }

  void Table_Impl::setCaption(const std::string& caption) {
    m_caption = caption;
  }

  void Table_Impl::setNHead(unsigned nH) {
    if (nH <= m_nRows - m_nFoot) { m_nHead = nH; }
    else {
      LOG(Warn,"Asked to set the number of header rows to " << nH << ", but there are only "
          << m_nRows - m_nFoot << " rows available to the header. Setting the number of header "
          << "rows to " << m_nRows - m_nFoot << ".");
      m_nHead = m_nRows - m_nFoot;
    }
  }

  void Table_Impl::setNFoot(unsigned nF) {
    if (nF <= m_nRows - m_nHead) { m_nFoot = nF; }
    else {
      LOG(Warn,"Asked to set the number of footer rows to " << nF << ", but there are only "
          << m_nRows - m_nHead << " rows available to the footer. Setting the number of footer "
          << "rows to " << m_nRows - m_nHead << ".");
      m_nFoot = m_nRows - m_nHead;
    }
  }

  void Table_Impl::setNLeft(unsigned nL) {
    if (nL <= m_nCols - m_nRight) { m_nLeft = nL; }
    else {
      LOG(Warn,"Asked to set the number of left-side columns to " << nL << ", but there are only "
          << m_nCols - m_nRight << " columns available to the left side. Setting the number of left-side "
          << "columns to " << m_nCols - m_nRight << ".");
      m_nLeft = m_nCols - m_nRight;
    }
  }

  void Table_Impl::setNRight(unsigned nR) {
    if (nR <= m_nCols - m_nLeft) { m_nRight = nR; }
    else {
      LOG(Warn,"Asked to set the number of right-side columns to " << nR << ", but there are only "
          << m_nCols - m_nLeft << " columns available to the right side. Setting the number of right-side "
          << "columns to " << m_nCols - m_nLeft << ".");
      m_nRight = m_nCols - m_nLeft;
    }
  }

  bool Table_Impl::setElement(unsigned rowIndex, unsigned colIndex, const TableElement& value) {
    if (!isRowIndex(rowIndex)) { return false; }
    if (!isColIndex(colIndex)) { return false; }
    m_table[rowIndex][colIndex] = value;
    return true;
  }

  void Table_Impl::appendRow(const std::vector<TableElement>& row) {
    // add columns to existing rows, if necessary
    if (row.size() > m_nCols) {
      m_nCols = row.size();
      BOOST_FOREACH(TableRow& row, m_table) {
        if (row.size() < m_nCols) { row.resize(m_nCols); }
      }
    }

    // add columns to new row, if necessary
    TableElementVector wRow(row);
    if (row.size() < m_nCols) {
      wRow.resize(m_nCols);
    }

    // add row
    ++m_nRows;
    m_table.push_back(wRow);

  }

  void Table_Impl::appendRow(const std::vector<std::string>& row) {
    appendRow(row,TableLoadOptions());
  }

  void Table_Impl::appendRow(const std::vector<std::string>& row,
                             const TableLoadOptions& options)
  {
    TableElementVector teRow;
    BOOST_FOREACH(const std::string& element,row) {
      teRow.push_back(TableElement(element,options));
    }

    appendRow(teRow);
  }

  void Table_Impl::appendRow(const std::vector<double>& row) {
    TableElementVector teRow;
    BOOST_FOREACH(double element,row) {
      teRow.push_back(TableElement(element));
    }

    appendRow(teRow);
  }

  void Table_Impl::appendRow(const std::vector<int>& row) {
    TableElementVector teRow;
    BOOST_FOREACH(int element,row) {
      teRow.push_back(TableElement(element));
    }

    appendRow(teRow);
  }

  void Table_Impl::appendRow(const std::vector<bool>& row) {
    TableElementVector teRow;
    BOOST_FOREACH(bool element,row) {
      teRow.push_back(TableElement(element));
    }

    appendRow(teRow);
  }

  void Table_Impl::trim() {
    trim(Table::B);
    trim(Table::R);
    trim(Table::T);
    trim(Table::L);
  }

  void Table_Impl::trim(Table::Edge edge) {

    unsigned num;

    if (edge == Table::T) {
      num = nEmptyRows(Table::T);
      if (num > 0) {
        std::vector<TableRow>::iterator itEraseEnd = m_table.begin();
        for (unsigned i = 0; i < num; ++i, ++itEraseEnd);
        m_table.erase(m_table.begin(),itEraseEnd);
      }
      OS_ASSERT(m_table.size() == m_nRows - num);
      m_nRows = m_table.size();
    }

    if (edge == Table::B) {
      num = nEmptyRows(Table::B);
      if (num > 0) {
        m_table.resize(m_nRows - num);
        m_nRows = m_table.size();
      }
    }

    if (edge == Table::L) {
      num = nEmptyCols(Table::L);
      if (num > 0) {
        BOOST_FOREACH(TableRow& row, m_table) {
          TableRow::iterator itEraseEnd = row.begin();
          for (unsigned i = 0; i < num; ++i, ++itEraseEnd);
          row.erase(row.begin(),itEraseEnd);
          OS_ASSERT(row.size() == m_nCols - num);
        }
        m_nCols -= num;
      }
    }

    if (edge == Table::R) {
      num = nEmptyCols(Table::R);
      if (num > 0) {
        BOOST_FOREACH(TableRow& row, m_table) {
          row.resize(m_nCols - num);
        }
        m_nCols -= num;
      }
    }

  }

  bool Table_Impl::convertUnits(unsigned index,
                                Table::Format tableSection,
                                UnitSystem targetSystem,
                                const boost::optional<int>& scaleExponent)
  {
    // use Table_Impl::units to figure determine current units
    std::pair<std::string,unsigned> unitsResult = getUnits(index,tableSection);

    // unable to determine current units, return false to indicate that no action was taken
    if (unitsResult.first.empty()) {
      return false;
    }
    // check for trivial completion
    if (getSystem(unitsResult.first) == targetSystem) {
      return true;
    }

    return convertUnits(index,
                        tableSection,
                        unitsResult.second,
                        unitsResult.first,
                        targetSystem,
                        boost::none,
                        scaleExponent);
  }

  bool Table_Impl::convertUnits(unsigned index,
                                Table::Format tableSection,
                                unsigned cell,
                                UnitSystem targetSystem,
                                const boost::optional<int>& scaleExponent)
  {
    // use Table_Impl::units to figure determine current units
    std::string unitString = units(index,tableSection,cell);

    // unable to determine current units, return false to indicate that no action was taken
    if (unitString.empty()) {
      return false;
    }
    // check for trivial completion
    if (getSystem(unitString) == targetSystem) {
      return true;
    }

    return convertUnits(index,
                        tableSection,
                        cell,
                        unitString,
                        targetSystem,
                        boost::none,
                        scaleExponent);
  }

  bool Table_Impl::convertUnits(unsigned index,Table::Format tableSection,const Unit& targetUnits)
  {
    // use Table_Impl::units to figure determine current units
    std::pair<std::string,unsigned> unitsResult = getUnits(index,tableSection);

    // unable to determine current units, return false to indicate that no action was taken
    if (unitsResult.first.empty()) {
      return false;
    }

    return convertUnits(index,
                        tableSection,
                        unitsResult.second,
                        unitsResult.first,
                        boost::none,
                        targetUnits);
  }

  bool Table_Impl::convertUnits(unsigned index,
                                Table::Format tableSection,
                                unsigned cell,
                                const Unit &targetUnits)
  {
    // use Table_Impl::units to figure determine current units
    std::string unitString = units(index,tableSection,cell);

    // unable to determine current units, return false to indicate that no action was taken
    if (unitString.empty()) {
      return false;
    }

    return convertUnits(index,
                        tableSection,
                        cell,
                        unitString,
                        boost::none,
                        targetUnits);
  }

  // QUERIES

  unsigned Table_Impl::nRows() const {
    OS_ASSERT(m_nRows == m_table.size());
    return m_nRows;
  }

  unsigned Table_Impl::nCols() const {
    return m_nCols;
  }

  unsigned Table_Impl::nHead() const {
    return m_nHead;
  }

  unsigned Table_Impl::nFoot() const {
    return m_nFoot;
  }

  unsigned Table_Impl::nLeft() const {
    return m_nLeft;
  }

  unsigned Table_Impl::nRight() const {
    return m_nRight;
  }

  bool Table_Impl::isRowIndex(unsigned index) const {
    return (index < m_nRows);
  }

  bool Table_Impl::isRowIndex(unsigned index,Table::Format tableSection) const {
    if (!isRowIndex(index)) { return false; }
    if (tableSection == Table::BODY) {
      if ((m_nLeft + m_nRight >= m_nCols) || (m_nHead + m_nFoot >= m_nRows)) { return false; }
      return ((index >= m_nHead) && (index < (m_nRows - m_nFoot)));
    }
    if (tableSection == Table::HEAD) {
      return (index < m_nHead);
    }
    if (tableSection == Table::FOOT) {
      return (index >= (m_nRows - m_nFoot));
    }
    if (tableSection == Table::LEFT) {
      if (m_nLeft == 0) { return false; }
      return ((index >= m_nHead) && (index < (m_nRows - m_nFoot)));
    }
    if (tableSection == Table::RIGHT) {
      if (m_nRight == 0) { return false; }
      return ((index >= m_nHead) && (index < (m_nRows - m_nFoot)));
    }
    LOG(Error,"Unknown Table::Format.");
    return false;
  }

  bool Table_Impl::isColIndex(unsigned index) const {
    return (index < m_nCols);
  }

  bool Table_Impl::isColIndex(unsigned index,Table::Format tableSection) const {
    if (!isColIndex(index)) { return false; }
    if (tableSection == Table::BODY) {
      if ((m_nLeft + m_nRight >= m_nCols) || (m_nHead + m_nFoot >= m_nRows)) { return false; }
      return ((index >= m_nLeft) && (index < (m_nCols - m_nRight)));
    }
    if (tableSection == Table::HEAD) {
      return true; // header spans whole table
    }
    if (tableSection == Table::FOOT) {
      return true; // header spans whole table
    }
    if (tableSection == Table::LEFT) {
      return (index < m_nLeft);
    }
    if (tableSection == Table::RIGHT) {
      return (index >= (m_nCols - m_nRight));
    }
    LOG(Error,"Unknown Table::Format.");
    return false;
  }

  bool Table_Impl::isEmptyRow(unsigned index) const {
    if (index >= m_nRows) { return false; }
    OS_ASSERT(index < m_table.size());

    bool allEmpty = true;
    BOOST_FOREACH(const TableElement& e, m_table[index]) {
      allEmpty = allEmpty && e.empty();
      if (!allEmpty) { break; }
    }

    return allEmpty;
  }

  bool Table_Impl::isEmptyCol(unsigned index) const {
    if (index >= m_nCols) { return false; }

    bool allEmpty = true;
    BOOST_FOREACH(const TableRow& row, m_table) {
      OS_ASSERT(index < row.size());
      allEmpty = allEmpty && row[index].empty();
      if (!allEmpty) {
        break;
      }
    }
    return allEmpty;
  }

  unsigned Table_Impl::nEmptyRows(Table::Edge edge) const {
    unsigned nRows = 0;

    if (edge == Table::T) {
      for (unsigned i = 0; i < m_nRows; ++i) {
        if (isEmptyRow(i)) { ++nRows; }
        else { break; }
      }
    }

    if (edge == Table::B) {
      for (int i = m_nRows-1; i >= 0; --i) {
        if (isEmptyRow(i)) { ++nRows; }
        else { break; }
      }
    }

    return nRows;
  }

  unsigned Table_Impl::nEmptyCols(Table::Edge edge) const {
    unsigned nCols = 0;

    if (edge == Table::L) {
      for (unsigned i = 0; i < m_nCols; ++i) {
        if (isEmptyCol(i)) { ++nCols; }
        else { break; }
      }
    }

    if (edge == Table::R) {
      for (int i = m_nCols-1; i >= 0; --i) {
        if (isEmptyCol(i)) { ++nCols; }
        else { break; }
      }
    }

    return nCols;
  }

  std::string Table_Impl::units(unsigned index, Table::Format tableSection) const {
    std::pair<std::string,unsigned> fullResult = getUnits(index,tableSection);
    return fullResult.first;
  }

  std::string Table_Impl::units(unsigned index, Table::Format tableSection, unsigned cell) const {
    std::string result;
    std::pair<unsigned,unsigned> ij = toRowColumnIndices(index,tableSection,cell);

    std::stringstream ss;
    ss << element(ij.first,ij.second);
    std::string text = ss.str();
    result = extractUnitString(text);
    result = convertToStandardForm(result);
    return result;
  }

  // PRINTING

  std::string Table_Impl::print(DocumentFormat fmt) const {
    if (fmt == DocumentFormat::COUT) { return printCOUT(); }
    if (fmt == DocumentFormat::LaTeX) { return printLaTeX(); }
    if (fmt == DocumentFormat::XHTML) { return printXHTML(); }
    return std::string();
  }

  std::string Table_Impl::print(TableFormat fmt) const {
    if (fmt == TableFormat::CSV) { return printCSV(); }
    if (fmt == TableFormat::JSON) { return printJSON(); }
    return std::string();
  }

  std::ostream& Table_Impl::printToStream(std::ostream& os, DocumentFormat fmt) const {
    os << print(fmt);
    return os;
  }

  std::ostream& Table_Impl::printToStream(std::ostream& os, TableFormat fmt) const {
    os << print(fmt);
    return os;
  }

  bool Table_Impl::save(const openstudio::path& p, bool overwrite) {
    // default is csv
    std::string expectedExtension("csv");
    std::string pext = toString(boost::filesystem::extension(p));
    if (!pext.empty()) {
      // remove '.'
      pext = std::string(++pext.begin(),pext.end());
    }
    if (pext == tableFileExtension()) {
      expectedExtension = pext; // boost serialization
    }

    // complete path
    path wp = completePathToFile(p,path(),expectedExtension,false);
    if (!wp.empty() && (overwrite == false)) {
      LOG(Info,"Table save method failed because instructed not to overwrite path '"
          << toString(p) << "'.");
      return false;
    }

    if (makeParentFolder(p)) {
      boost::filesystem::ofstream outFile(p);
      if (outFile) {
        if (expectedExtension == tableFileExtension()) {
          try {
            boost::archive::text_oarchive out(outFile);
            Table publicSelf = getObject<Table>();
            out << boost::serialization::make_nvp("table",publicSelf);
            outFile.close();
            return true;
          }
          catch (boost::archive::archive_exception e) {
            LOG(Error,"Boost archive exception while writing Table out to boost serialization text ("
                << tableFileExtension() << "). Code " << e.code << ". Message: " << e.what());
          }
          catch (...) {
            LOG(Error,"Error serializing Table to boost serialization text ("
                << tableFileExtension() << ").");
          }
          return false;
        }
        else {
          try {
            outFile << print(TableFormat(TableFormat::CSV));
            outFile.close();
            return true;
          }
          catch (...) {
            LOG(Error,"Unable to write Table to path '" << toString(p) << "'.");
            return false;
          }
        } // if expectedExtension
      } // if outFile
    } // if parent folder

    LOG(Error,"Unable to write Table to path '" << toString(p)
        << "', because parent directory could not be created.");
    return false;
  }

  // PRIVATE

  std::pair<unsigned,unsigned> Table_Impl::toRowColumnIndices(unsigned index,
                                                            Table::Format tableSection,
                                                            unsigned cell) const
  {
    std::pair<unsigned,unsigned> ij;
    if ((tableSection == Table::HEAD) && isColIndex(index)) {
      ij.first = cell;
      ij.second = index;
    }
    if ((tableSection == Table::FOOT) && isColIndex(index)) {
      ij.first = (m_nRows - m_nFoot) + cell;
      ij.second = index;
    }
    if ((tableSection == Table::LEFT) && isRowIndex(index)) {
      ij.first = index;
      ij.second = cell;
    }
    if ((tableSection == Table::RIGHT) && isRowIndex(index)) {
      ij.first = index;
      ij.second = (m_nCols - m_nRight) + cell;
    }
    return ij;
  }

  std::pair<std::string,unsigned> Table_Impl::getUnits(unsigned index,Table::Format tableSection) const {
    std::pair<std::string,unsigned> result(std::string(),0);
    TableElementVector elements;
    if ((tableSection == Table::HEAD) && isColIndex(index)) {
      for (unsigned i = 0; i < m_nHead; ++i) {
        elements.push_back(element(i,index));
      }
    }
    if ((tableSection == Table::FOOT) && isColIndex(index)) {
      for (unsigned i = m_nRows - m_nFoot; i < m_nRows; ++i) {
        elements.push_back(element(i,index));
      }
    }
    if ((tableSection == Table::LEFT) && isRowIndex(index)) {
      for (unsigned j = 0; j < m_nLeft; ++j) {
        elements.push_back(element(index,j));
      }
    }
    if ((tableSection == Table::RIGHT) && isRowIndex(index)) {
      for (unsigned j = m_nCols - m_nRight; j < m_nCols; ++j) {
        elements.push_back(element(index,j));
      }
    }
    BOOST_FOREACH(const TableElement& e, elements) {
      std::stringstream ss;
      ss << e;
      std::string text = ss.str();
      result.first = extractUnitString(text);
      result.first = convertToStandardForm(result.first);
      if (!result.first.empty() && containsRegisteredBaseUnit(result.first)) {
        break;
      }
      ++result.second;
    }
    return result;
  }

  bool Table_Impl::convertUnits(unsigned index,
                                Table::Format tableSection,
                                unsigned cell,
                                const std::string& unitString,
                                boost::optional<UnitSystem> targetSystem,
                                boost::optional<Unit> targetUnits,
                                const boost::optional<int>& scaleExponent)
  {
    // determine table elements that need converting
    UnsignedVector is,js;
    if ((tableSection == Table::HEAD) || (tableSection == Table::FOOT)) {
      // column
      for (unsigned i = m_nHead, n = m_nRows - m_nFoot; i < n; ++i) {
        // ignore null entries
        if (!element(i,index).empty()) {
          is.push_back(i);
        }
      }
      if (is.empty()) {
        return false;
      }
      js.push_back(index);
    }
    else {
      // row
      is.push_back(index);
      for (unsigned j = m_nLeft, n = m_nCols - m_nRight; j < n; ++j) {
        if (!element(index,j).empty()) {
          js.push_back(j);
        }
      }
      if (js.empty()) {
        return false;
      }
    }

    // get quantities to be converted and convert them
    DoubleVector originalValues;
    BOOST_FOREACH(unsigned i, is) {
      BOOST_FOREACH(unsigned j, js) {
        openstudio::TableElement e = element(i, j);
        OptionalDouble d;
        if (e.isDouble() || e.isInt()) {
          d = e.toDouble();
        }
        else {
          LOG(Info,"Unable to return " << e << " as a double.");
          return false;
        }
        originalValues.push_back(*d);
      }
    }
    OptionalUnit originalUnit = createUnit(unitString);
    if (!originalUnit) {
      LOG(Info,"Unable to create unit from " << unitString << ".");
      return false;
    }
    OSQuantityVector originalQuantities(*originalUnit,originalValues);
    OSQuantityVector newQuantities;
    if (targetSystem) {
      newQuantities = convert(originalQuantities,*targetSystem);
    }
    else {
      OS_ASSERT(targetUnits);
      newQuantities = convert(originalQuantities,*targetUnits);
    }
    if (newQuantities.size() != originalQuantities.size()) {
      if (targetSystem) {
        LOG(Info,"Unable to convert " << originalUnit.get() << " to target system "
            << targetSystem->valueDescription() << ".");
      }
      else {
        LOG(Info,"Unable to convert " << originalUnit.get() << " to target units "
            << *targetUnits << ".");
      }
      return false;
    }

    // set unit string
    std::stringstream ss;
    ss << newQuantities.units();
    replaceUnitString(index,tableSection,cell,ss.str());
    // set values
    unsigned vecIndex = 0;
    DoubleVector newValues = newQuantities.values();
    BOOST_FOREACH(unsigned i, is) {
      BOOST_FOREACH(unsigned j, js) {
        (*this)[i][j] = newValues[vecIndex];
        ++vecIndex;
      }
    }
    return true;
  }

  void Table_Impl::replaceUnitString(unsigned index,
                                     Table::Format tableSection,
                                     unsigned cell,
                                     const std::string& newUnitString) {
    std::string result;
    std::pair<unsigned,unsigned> ij = toRowColumnIndices(index,tableSection,cell);
    std::stringstream ss;
    ss << element(ij.first,ij.second);
    std::string newText = openstudio::replaceUnitString(ss.str(),newUnitString);
    (*this)[ij.first][ij.second] = newText;
  }

  std::string Table_Impl::printCOUT() const {
    LOG(Warn,"COUT tables have not been implemented.");
    return std::string();
  }

  std::string Table_Impl::printLaTeX() const {
    std::string wTitle = formatText(m_title,DocumentFormat(DocumentFormat::LaTeX));
    std::string wCaption = formatText(m_caption,DocumentFormat(DocumentFormat::LaTeX));

    std::stringstream ss;
    ss << std::endl << "\\begin{table}[htb]" << std::endl;
    if (!m_title.empty()) {
      ss << "\\caption[" << wTitle << "]";
      if (m_caption.empty()) {
        ss << "{" << wTitle << "}" << std::endl;
      }
      else {
        ss << "{" << wCaption << "}" << std::endl;
      }
      ss << "\\label{table:" << wTitle << "}" << std::endl;
    }
    else if (!m_caption.empty()) {
      ss << "\\caption{" << wCaption << "}" << std::endl
         << "\\label{table:" << wCaption << "}" << std::endl;
    }
    ss << "\\begin{center}" << std::endl
       << "\\begin{tabular}{" << columnFormatsLaTeX() << "}" << std::endl;

    BOOST_FOREACH(const TableRow& row,m_table) {
      std::string sep;
      for (unsigned i = 0; i < m_nCols; ++i) {
        if (row[i].isString()) {
          std::string wStr = formatText(row[i].toString(),DocumentFormat(DocumentFormat::LaTeX));
          ss << sep << wStr;
        }
        else {
          ss << sep << row[i];
        }
        sep = std::string(" & ");
      }
      ss << " \\\\" << std::endl;
    }

    ss << "\\end{tabular}" << std::endl
       << "\\end{center}" << std::endl
       << "\\end{table}" << std::endl;

    return ss.str();
  }

  std::string Table_Impl::printXHTML() const {
    std::stringstream ss;
    ss << std::endl
       << "<table";
    if (!m_title.empty()) {
      ss << " summary=\"" << m_title << "\"";
    }
    else if (!m_caption.empty()) {
      ss << " summary=\"" << m_caption << "\"";
    }
    ss << ">" << std::endl;
    if (!m_caption.empty()) {
      ss << "<caption>" << m_caption << "</caption>" << std::endl;
    }
    else if (!m_title.empty()) {
      ss << "<caption>" << m_title << "</caption>" << std::endl;
    }

    unsigned i = 0;
    std::string elemStartTag("  <th>");
    std::string elemEndTag("</th>");
    ss << std::boolalpha;
    BOOST_FOREACH(const TableRow& row,m_table) {
      if (static_cast<int>(i) == static_cast<int>(m_nHead) - 1) {
        elemStartTag = "  <th class=\"lastHeaderRow\">";
      }
      if (i == m_nHead) {
        elemStartTag = "  <td>";
        elemEndTag = "</td>";
      }
      ss << "<tr>" << std::endl;
      for (unsigned j = 0; j < m_nCols; ++j) {
        if (row[j].isString()) {
          std::string wStr = formatSuperAndSubscripts(row[j].toString(),DocumentFormat(DocumentFormat::XHTML));
          ss << elemStartTag << wStr << elemEndTag << std::endl;
        }
        else { ss << elemStartTag << row[j] << elemEndTag << std::endl; }
      }
      ss << "</tr>" << std::endl;
      ++i;
    }
    ss << std::noboolalpha;

    ss << "</table>" << std::endl;

    return ss.str();
  }

  std::string Table_Impl::printCSV() const {
    std::stringstream ss;
    BOOST_FOREACH(const TableRow& row,m_table) {
      std::string sep;
      for (unsigned i = 0; i < m_nCols; ++i) {
        std::stringstream tempSS;
        tempSS << row[i];
        if (boost::regex_search(tempSS.str(),boost::regex(","))) {
          ss << sep << "\"" << tempSS.str() << "\"";
        }
        else { ss << sep << tempSS.str(); }
        sep = std::string(",");
      }
      ss << std::endl;
    }
    return ss.str();
  }

  std::string Table_Impl::printJSON() const {
    std::stringstream ss;
    int indent(0);
    ss << "{" << std::endl;
    indent += 2;

    // title start
    std::string titleStr = title();
    if (!titleStr.empty()) {
      ss << std::setw(indent) << " " << "\"" << titleStr << "\": {" << std::endl;
      indent += 2;
    }

    // header
    for (int i = 0, in = nHead(); i < in; ++i) {
      ss << std::setw(indent) << " " << "\"header" << i+1 << "\": ";
      ss << printJSONArray(i,i+1,nLeft(),nCols() - nRight()) << "," << std::endl;
    }

    // units
    StringVector unitStrings;
    bool anyUnits(false);
    for (int j = nLeft(), jn = nCols() - nRight(); j < jn; ++j) {
      std::string unitString = units(j);
      unitStrings.push_back(unitString);
      if (!unitString.empty()) {
        anyUnits = true;
      }
    }
    if (anyUnits) {
      ss << std::setw(indent) << " " << "\"units\": [";
      for (int j = 0, jn = unitStrings.size(); j < jn; ++j) {
        ss << "\"" << unitStrings[j] << "\"";
        if (j < jn-1) {
          ss << ", ";
        }
      }
      ss << "]," << std::endl;
    }

    // data start
    ss << std::setw(indent) << " " << "\"data\": {" << std::endl;
    indent += 2;

    for (int i = nHead(), in = nRows(); i < in; ++i) {

      std::stringstream key;
      bool hasKey(false);
      for (int j = 0, jn = nLeft(); j < jn; ++j) {
        TableElement e = element(i,j);
        if (!e.empty()) {
          if (hasKey) {
            key << "|";
          }
          key << e;
          hasKey = true;
        }
      }
      for (int j = nCols() - nRight(), jn = nCols(); j < jn; ++j) {
        TableElement e = element(i,j);
        if (!e.empty()) {
          if (hasKey) {
            key << "|";
          }
          key << e;
          hasKey = true;
        }
      }

      ss << std::setw(indent) << " ";
      if (hasKey) {
        ss << "\"" << key.str() << "\": ";
      }

      ss << printJSONArray(i,i+1,nLeft(),nCols() - nRight());
      if (i < in - 1) {
        ss << ",";
      }
      ss << std::endl;

    }

    // data end
    indent -= 2;
    ss << std::setw(indent) << " " << "}" << std::endl;

    // title end
    if (!titleStr.empty()) {
      indent -= 2;
      ss << std::setw(indent) << " " << "}" << std::endl;
    }

    ss << "}" << std::endl;
    return ss.str();
  }

  std::string Table_Impl::printJSONArray(int iBegin,int iEnd, int jBegin, int jEnd) const {
    std::stringstream ss;
    ss << "[";
    for (int i = iBegin; i < iEnd; ++i) {
      for (int j = jBegin; j < jEnd; ++j) {
        TableElement e = element(i,j);
        if (e.isString()) {
          ss << "\"" << e << "\"";
        }
        else {
          ss << e;
        }
        if (j < jEnd-1) {
          ss << ", ";
        }
      }
    }
    ss << "]";
    return ss.str();
  }

  std::string Table_Impl::columnFormatsLaTeX() const {
    std::stringstream ss;
    ss << "*{" << m_nCols << "}{c}";
    return ss.str();
  }

} // detail

Table::Table()
  : SectionElement(boost::shared_ptr<detail::Table_Impl>(new detail::Table_Impl()))
{}

std::string Table::title() const {
  return getImpl<detail::Table_Impl>()->title();
}

std::string Table::caption() const {
  return getImpl<detail::Table_Impl>()->caption();
}

TableElement Table::element(unsigned rowIndex,unsigned colIndex) const {
  return getImpl<detail::Table_Impl>()->element(rowIndex,colIndex);
}

std::vector<TableElement>& Table::operator[](unsigned rowIndex) {
  return getImpl<detail::Table_Impl>()->operator[](rowIndex);
}

const std::vector<TableElement>& Table::operator[](unsigned rowIndex) const {
  return getImpl<detail::Table_Impl>()->operator[](rowIndex);
}

void Table::setTitle(const std::string& title) {
  getImpl<detail::Table_Impl>()->setTitle(title);
}

void Table::setCaption(const std::string& caption) {
  getImpl<detail::Table_Impl>()->setCaption(caption);
}

void Table::setNHead(unsigned nH) {
  getImpl<detail::Table_Impl>()->setNHead(nH);
}

void Table::setNFoot(unsigned nF) {
  getImpl<detail::Table_Impl>()->setNFoot(nF);
}

void Table::setNLeft(unsigned nL) {
  getImpl<detail::Table_Impl>()->setNLeft(nL);
}

void Table::setNRight(unsigned nR) {
  getImpl<detail::Table_Impl>()->setNRight(nR);
}

bool Table::setElement(unsigned rowIndex, unsigned colIndex, const TableElement& value) {
  return getImpl<detail::Table_Impl>()->setElement(rowIndex,colIndex,value);
}

void Table::appendRow(const std::vector<TableElement>& row) {
  getImpl<detail::Table_Impl>()->appendRow(row);
}

void Table::appendRow(const std::vector<std::string>& row) {
  getImpl<detail::Table_Impl>()->appendRow(row);
}

void Table::appendRow(const std::vector<std::string>& row,
                      const TableLoadOptions& options)
{
  getImpl<detail::Table_Impl>()->appendRow(row,options);
}

void Table::appendRow(const std::vector<double>& row) {
  getImpl<detail::Table_Impl>()->appendRow(row);
}

void Table::appendRow(const std::vector<int>& row) {
  getImpl<detail::Table_Impl>()->appendRow(row);
}

void Table::appendRow(const std::vector<bool>& row) {
  getImpl<detail::Table_Impl>()->appendRow(row);
}

void Table::trim() {
  getImpl<detail::Table_Impl>()->trim();
}

void Table::trim(Edge edge) {
  getImpl<detail::Table_Impl>()->trim(edge);
}

bool Table::convertUnits(unsigned index,
                         Format tableSection,
                         UnitSystem targetSystem,
                         const boost::optional<int>& scaleExponent)
{
  return getImpl<detail::Table_Impl>()->convertUnits(index,tableSection,targetSystem,scaleExponent);
}

bool Table::convertUnits(unsigned index,
                         Format tableSection,
                         unsigned cell,
                         UnitSystem targetSystem,
                         const boost::optional<int>& scaleExponent)
{
  return getImpl<detail::Table_Impl>()->convertUnits(index,tableSection,cell,targetSystem,scaleExponent);
}

bool Table::convertUnits(unsigned index,Format tableSection,const Unit& targetUnits) {
  return getImpl<detail::Table_Impl>()->convertUnits(index,tableSection,targetUnits);
}

bool Table::convertUnits(unsigned index,Format tableSection,unsigned cell,const Unit &targetUnits)
{
  return getImpl<detail::Table_Impl>()->convertUnits(index,tableSection,cell,targetUnits);
}

unsigned Table::nRows() const {
  return getImpl<detail::Table_Impl>()->nRows();
}

unsigned Table::nCols() const {
  return getImpl<detail::Table_Impl>()->nCols();
}

unsigned Table::nHead() const {
  return getImpl<detail::Table_Impl>()->nHead();
}

unsigned Table::nFoot() const {
  return getImpl<detail::Table_Impl>()->nFoot();
}

unsigned Table::nLeft() const {
  return getImpl<detail::Table_Impl>()->nLeft();
}

unsigned Table::nRight() const {
  return getImpl<detail::Table_Impl>()->nRight();
}

bool Table::isRowIndex(unsigned index) const {
  return getImpl<detail::Table_Impl>()->isRowIndex(index);
}

bool Table::isRowIndex(unsigned index,Format tableSection) const {
  return getImpl<detail::Table_Impl>()->isRowIndex(index,tableSection);
}

bool Table::isColIndex(unsigned index) const {
  return getImpl<detail::Table_Impl>()->isColIndex(index);
}

bool Table::isColIndex(unsigned index,Format tableSection) const {
  return getImpl<detail::Table_Impl>()->isColIndex(index,tableSection);
}

bool Table::isEmptyRow(unsigned index) const {
  return getImpl<detail::Table_Impl>()->isEmptyRow(index);
}

bool Table::isEmptyCol(unsigned index) const {
  return getImpl<detail::Table_Impl>()->isEmptyCol(index);
}

unsigned Table::nEmptyRows(Edge edge) const {
  return getImpl<detail::Table_Impl>()->nEmptyRows(edge);
}

unsigned Table::nEmptyCols(Edge edge) const {
  return getImpl<detail::Table_Impl>()->nEmptyCols(edge);
}

std::string Table::units(unsigned index, Format tableSection) const {
  return getImpl<detail::Table_Impl>()->units(index,tableSection);
}

std::string Table::units(unsigned index, Format tableSection, unsigned cell) const {
  return getImpl<detail::Table_Impl>()->units(index,tableSection,cell);
}

std::string Table::print(TableFormat fmt) const {
  return getImpl<detail::Table_Impl>()->print(fmt);
}

std::ostream& Table::printToStream(std::ostream& os, TableFormat fmt) const {
  return getImpl<detail::Table_Impl>()->printToStream(os,fmt);
}

bool Table::save(const openstudio::path& p, bool overwrite) {
  return getImpl<detail::Table_Impl>()->save(p,overwrite);
}

Table Table::load(const openstudio::path& p,const TableLoadOptions& options)
{
  Table result;

  // default is csv
  std::string expectedExtension("csv");
  std::string pext = toString(boost::filesystem::extension(p));
  if (!pext.empty()) {
    // remove '.'
    pext = std::string(++pext.begin(),pext.end());
  }
  if (pext == tableFileExtension()) {
    expectedExtension = pext; // boost serialization
  }

  // open file
  openstudio::path wp = completePathToFile(p,path(),expectedExtension,true);
  boost::filesystem::ifstream inFile(wp);
  if (!inFile) { return result; }

  // load file
  if (expectedExtension == tableFileExtension()) {
    result = loadFromBoostTextSerialization(inFile);
  }
  else {
    result = loadFromCsv(inFile,options);
  }
  return result;
}

Table Table::load(const std::string& str,
                  const TableLoadOptions& options)
{
  std::stringstream is(str);
  return loadFromCsv(is,options);
}

Table::Table(boost::shared_ptr<detail::Table_Impl> impl)
  : SectionElement(impl)
{}

Table Table::loadFromCsv(std::istream& is,
                            const TableLoadOptions& options)
{
  Table result;

  // create table line-by-line
  std::string lineString;
  boost::regex simpleElement("^([^,]*)(,|$)");
  boost::regex complexElement("^\"(\"*[^\"]*\"*)\"(,|$)");
  boost::smatch matches;

  // filter for line endings
  boost::iostreams::filtering_istream filt;
  filt.push(boost::iostreams::newline_filter(boost::iostreams::newline::posix));
  filt.push(is);

  while (std::getline(filt,lineString)) {
    if (lineString == "\r") { continue; }

    std::string elementString;
    StringVector row;

    // special handling for lines containing "
    if (boost::regex_search(lineString,boost::regex("\""))) {
      while (!lineString.empty()) {
        if (boost::regex_search(lineString,matches,complexElement) ||
            boost::regex_search(lineString,matches,simpleElement))
        {
          OS_ASSERT(matches[0].first == lineString.begin());
          elementString = std::string(matches[1].first,matches[1].second);
          row.push_back(elementString);
          std::string temp = std::string(matches[0].second,std::string::const_iterator(lineString.end()));
          lineString = temp;
        }
        else {
          LOG(Warn,"Loading csv table from file, but unable to fully parse a line. The "
              << "remainder of that line is '" << lineString << "'.");
          lineString.clear();
        }
      } // while
    }
    else {
      std::stringstream lineStream(lineString);
      while (std::getline(lineStream,elementString,',')) {
        row.push_back(elementString);
      } // while
    }

    result.appendRow(row,options);
  } // while

  // clean up any empty rows and columns
  result.trim();

  return result;
}

Table Table::loadFromBoostTextSerialization(boost::filesystem::ifstream& is) {
  Table result;
  try {
    boost::archive::text_iarchive in(is);
    Table tmp;
    in >> boost::serialization::make_nvp("table",tmp);
    result = tmp;
  }
  catch (boost::archive::archive_exception e) {
    LOG(Error,"Boost archive exception while loading Table from Text. Code " << e.code << ". Message: " << e.what());
  }
  catch (...) {
    LOG(Error,"Error deserializing object of type 'Table' from boost searialization text file (.ost).");
  }
  return result;
}

} // openstudio
