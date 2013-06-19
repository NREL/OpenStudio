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

#ifndef UTILITIES_DOCUMENT_TABLE_HPP
#define UTILITIES_DOCUMENT_TABLE_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/document/SectionElement.hpp>
#include <utilities/document/TableElement.hpp>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Path.hpp>
#include <boost/filesystem/fstream.hpp>

namespace openstudio {

namespace detail {
  class Table_Impl;
}

/** Table is a SectionElement that provides support for Table creation, serialization,
 *  and de-serialization. Tables can be serialized to their own internal format, HTML, LaTeX,
 *  CSV or JSON. Tables can be de-serialized from their own internal format and CSV. */
class UTILITIES_API Table : public SectionElement {
 public:

  enum Format { BODY, HEAD, FOOT, LEFT, RIGHT };
  enum Edge { T, B, L, R};

  /** @name Constructors */
  //@{

  Table();

  virtual ~Table() {}

  //@}
  /** @name Getters */
  //@{

  std::string title() const;

  std::string caption() const;

  TableElement element(unsigned rowIndex,unsigned colIndex) const;

  std::vector<TableElement>& operator[](unsigned rowIndex);

  const std::vector<TableElement>& operator[](unsigned rowIndex) const;

  //@}
  /** @name Setters */
  //@{

  void setTitle(const std::string& title);

  void setCaption(const std::string& caption);

  /** Set the number of rows in the top (header) section of the table. */
  void setNHead(unsigned nH);

  /** Set the number of rows in the bottom (footer) section of the table. */
  void setNFoot(unsigned nF);

  /** Set the number of columns on the left side of the table that contain labels
   *  rather than data. */
  void setNLeft(unsigned nL);

  /** Set the number of columns on the right side of the table that contain labels
   *  rather than data. */
  void setNRight(unsigned nR);

  bool setElement(unsigned rowIndex, unsigned colIndex, const TableElement& value);

  void appendRow(const std::vector<TableElement>& row);

  void appendRow(const std::vector<std::string>& row);

  void appendRow(const std::vector<std::string>& row, const TableLoadOptions& options);

  void appendRow(const std::vector<double>& row);

  void appendRow(const std::vector<int>& row);

  void appendRow(const std::vector<bool>& row);

  /** Deletes empty rows and columns at the edges of the table.  */
  void trim();

  /** For all arguments except BODY, deletes empty rows or columns corresponding to the
   *  part of the table indicated by area. */
  void trim(Edge edge);

  /** Convert row/col index BODY elements to use targetSystem units, if possible. Uses
   *  units(index,tableSection) to determine the current units. */
  bool convertUnits(unsigned index,
                    Format tableSection=HEAD,
                    UnitSystem targetSystem=UnitSystem::SI,
                    const boost::optional<int>& scaleExponent=boost::none);

  /** \overload. Uses units(index,tableSection,cell) to determine the current units. */
  bool convertUnits(unsigned index,
                    Format tableSection,
                    unsigned cell,
                    UnitSystem targetSystem,
                    const boost::optional<int>& scaleExponent=boost::none);

  /** Convert row/col index BODY elements to use targetUnits, if possible. Uses
   *  units(index,tableSection) to determine the current units. */
  bool convertUnits(unsigned index,Format tableSection,const Unit& targetUnits);

  /** \overload. Uses units(index,tableSection,cell) to determine the current units. */
  bool convertUnits(unsigned index,
                    Format tableSection,
                    unsigned cell,
                    const Unit &targetUnits);

  //@}
  /** @name Queries */
  //@{

  /** Returns the total number of rows in the Table. */
  unsigned nRows() const;

  /** Returns the total number of columns in the Table. */
  unsigned nCols() const;

  /** Returns the number of rows in the top (header) section of the Table. */
  unsigned nHead() const;

  /** Returns the number of rows in the bottom (footer) section of the Table. */
  unsigned nFoot() const;

  /** Returns the number of columns in the left section of the Table. */
  unsigned nLeft() const;

  /** Returns the number of columns in the right section of the Table. */
  unsigned nRight() const;

  /** Returns true if index is < nRows(). */
  bool isRowIndex(unsigned index) const;

  /** Returns true if index is valid for tableSection. */
  bool isRowIndex(unsigned index,Format tableSection) const;

  /** Returns true if index is < nCols(). */
  bool isColIndex(unsigned index) const;

  /** Returns true if index is valid for tableSection. */
  bool isColIndex(unsigned index,Format tableSection) const;

  /** Returns true if the row at index exists and only contains .empty()
   *  \link TableElement TableElements\endlink. */
  bool isEmptyRow(unsigned index) const;

  /** Returns true if the column at index exists and only contains .empty()
   *  \link TableElement TableElements\endlink. */
  bool isEmptyCol(unsigned index) const;

  /** Returns the number of contiguous empty rows starting from edge. Always returns
   *  0 for edge == Table::L or Table::R. */
  unsigned nEmptyRows(Edge edge) const;

  /** Returns the number of contiguous empty columns starting from edge. Always returns
   *  0 for edge == Table::T or Table::B. */
  unsigned nEmptyCols(Edge edge) const;

  /** Return the units associated with the given column/row index, as deduced from the text
   *  in tableSection associated with that index. For instance, if nHead = 1, and isColIndex(3,BODY),
   *  will look in element(0,3) for a unit string, and return it if found. */
  std::string units(unsigned index, Format tableSection=HEAD) const;

  /** This overload is provided to give the user more control over which cell the unit string
   *  is extracted from, for use when the user has that precise knowledge. Cell is an index
   *  measured from within tableSection, starting from the top or right and staying within the row
   *  or column indicated by index. For instance, if tableSection==HEAD and nHead = 3, then cell
   *  may be 0, 1, or 2, and indicates the row within column index that contains the unit string. */
  std::string units(unsigned index, Format tableSection, unsigned cell) const;

  //@}
  /** @name Printing */
  //@{

  /** Prints this table in fmt. */
  std::string print(TableFormat fmt) const;

  /** Prints this table to os in fmt. */
  std::ostream& printToStream(std::ostream& os, TableFormat fmt) const;

  //@}
  /** @name Serialization */
  //@{

  // TODO: You should also be able to save to .json.
  /** Save as csv or ost. Default (if no extension is specified) is csv. */
  bool save(const openstudio::path& p, bool overwrite=false);

  // TODO: Make this return boost::optional<Table>. Get really strange compiler error with any
  // instantiation of boost::optional<Table>. Or maybe this will go away with VS/boost upgrade(s).
  /** Load from csv or ost. Default (if no extension is specified) is csv. */
  static Table load(const openstudio::path& p,
                    const TableLoadOptions& options=TableLoadOptions());

  /** Load csv formatted string into Table. */
  static Table load(const std::string& str,
                    const TableLoadOptions& options=TableLoadOptions());

  //@}
 protected:
  typedef detail::Table_Impl ImplType;

  friend class DocumentElement;
  friend class detail::DocumentElement_Impl;
  friend class detail::Table_Impl;

  Table(boost::shared_ptr<detail::Table_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.Table");

  static Table loadFromCsv(std::istream& is,
                           const TableLoadOptions& options);

  static Table loadFromBoostTextSerialization(boost::filesystem::ifstream& is);

  // serialization support
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SectionElement);
  }
};

/** WARNING: DO NOT USE FOR NOW. CAUSES STRANGE VS/BOOST COMPILER ERROR. \relates Table */
typedef boost::optional<Table> OptionalTable;

/** \relates Table */
typedef std::vector<Table> TableVector;

} // openstudio

#endif // UTILITIES_DOCUMENT_TABLE_HPP
