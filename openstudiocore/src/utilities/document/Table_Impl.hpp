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

#ifndef UTILITIES_DOCUMENT_TABLE_IMPL_HPP
#define UTILITIES_DOCUMENT_TABLE_IMPL_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/document/SectionElement_Impl.hpp>
#include <utilities/document/Table.hpp> // needed for enums

#include <utilities/units/Unit.hpp>

namespace openstudio {

namespace detail {

  /** Implementation of Table class. */
  class UTILITIES_API Table_Impl : public SectionElement_Impl {
   public:
    /** @name Constructors */
    //@{

    Table_Impl();

    virtual ~Table_Impl() {}

    virtual DocumentElement clone() const;

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

    void setNHead(unsigned nH);

    void setNFoot(unsigned nF);

    void setNLeft(unsigned nL);

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
    void trim(Table::Edge edge);

    /** Convert row/col index BODY elements to use sys units, if possible. Uses
     *  units(index,tableSection) to determine the current units. */
    bool convertUnits(unsigned index,
                      Table::Format tableSection=Table::HEAD,
                      UnitSystem targetSystem=UnitSystem::SI,
                      const boost::optional<int>& scaleExponent=boost::none);

    /** \overload. Uses units(index,tableSection,cell) to determine the current units. */
    bool convertUnits(unsigned index,
                      Table::Format tableSection,
                      unsigned cell,
                      UnitSystem targetSystem,
                      const boost::optional<int>& scaleExponent=boost::none);

    /** Convert row/col index BODY elements to use targetUnits, if possible. Uses
     *  units(index,tableSection) to determine the current units. */
    bool convertUnits(unsigned index,Table::Format tableSection,const Unit& targetUnits);

    /** \overload. Uses units(index,tableSection,cell) to determine the current units. */
    bool convertUnits(unsigned index,
                      Table::Format tableSection,
                      unsigned cell,
                      const Unit &targetUnits);

    //@}
    /** @name Queries */
    //@{

    unsigned nRows() const;

    unsigned nCols() const;

    unsigned nHead() const;

    unsigned nFoot() const;

    unsigned nLeft() const;

    unsigned nRight() const;

    bool isRowIndex(unsigned index) const;

    bool isRowIndex(unsigned index,Table::Format tableSection) const;

    bool isColIndex(unsigned index) const;

    bool isColIndex(unsigned index,Table::Format tableSection) const;

    bool isEmptyRow(unsigned index) const;

    bool isEmptyCol(unsigned index) const;

    unsigned nEmptyRows(Table::Edge edge) const;

    unsigned nEmptyCols(Table::Edge edge) const;

    /** Return the units associated with the given column/row index, as deduced from the text
     *  in tableSection associated with that index. For instance, if nHead = 1, and isColIndex(3,BODY),
     *  will look in element(0,3) for a unit string, and return it if found. */
    std::string units(unsigned index, Table::Format tableSection=Table::HEAD) const;

    /** This overload is provided to give the user more control over which cell the unit string
     *  is extracted from, for use when the user has that precise knowledge. Cell is an index
     *  measured from within tableSection, starting from the top or right and staying within the row
     *  or column indicated by index. For instance, if tableSection==HEAD and nHead = 3, then cell
     *  may be 0, 1, or 2, and indicates the row within column index that contains the unit string. */
    std::string units(unsigned index, Table::Format tableSection, unsigned cell) const;

    //@}
    /** @name Printing */
    //@{

    virtual std::string print(DocumentFormat fmt) const;

    std::string print(TableFormat fmt) const;

    virtual std::ostream& printToStream(std::ostream& os, DocumentFormat fmt) const;

    std::ostream& printToStream(std::ostream& os, TableFormat fmt) const;

    //@}
    /** @name Serialization */
    //@{

    /** Save as csv or ost. Default (if no extension is specified) is csv. */
    bool save(const openstudio::path& p, bool overwrite=false);

    //@}
   protected:
    std::string m_title;   // short caption/summary
    std::string m_caption; // full caption
    std::vector< std::vector<TableElement> > m_table;

    unsigned m_nRows,m_nCols;
    unsigned m_nHead,m_nFoot,m_nLeft,m_nRight;

   private:
    REGISTER_LOGGER("openstudio.Table");

    std::pair<unsigned,unsigned> toRowColumnIndices(unsigned index,
                                                    Table::Format tableSection,
                                                    unsigned cell) const;

    /** Does the work of units(index,tableSection, also returning the cell index of the element
     *  containing the unit string. */
    std::pair<std::string,unsigned> getUnits(unsigned index,Table::Format tableSection) const;

    /** Shared functionality between overloaded methods convertUnits. */
    bool convertUnits(unsigned index,
                      Table::Format tableSection,
                      unsigned cell,
                      const std::string& unitString,
                      boost::optional<UnitSystem> targetSystem,
                      boost::optional<Unit> targetUnits,
                      const boost::optional<int>& scaleExponent=boost::none);

    void replaceUnitString(unsigned index,
                           Table::Format tableSection,
                           unsigned cell,
                           const std::string& newUnitString);

    std::string printCOUT() const;
    std::string printLaTeX() const;
    std::string printXHTML() const;

    std::string printCSV() const;
    std::string printJSON() const;
    std::string printJSONArray(int iBegin,int iEnd, int jBegin, int jEnd) const;

    std::string columnFormatsLaTeX() const;

    // serialization support
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
      ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SectionElement_Impl);
      ar & BOOST_SERIALIZATION_NVP(m_title);
      ar & BOOST_SERIALIZATION_NVP(m_caption);
      ar & BOOST_SERIALIZATION_NVP(m_table);
      ar & BOOST_SERIALIZATION_NVP(m_nRows);
      ar & BOOST_SERIALIZATION_NVP(m_nCols);
      ar & BOOST_SERIALIZATION_NVP(m_nHead);
      ar & BOOST_SERIALIZATION_NVP(m_nFoot);
      ar & BOOST_SERIALIZATION_NVP(m_nLeft);
      ar & BOOST_SERIALIZATION_NVP(m_nRight);
    }
  };

} // detail

} // openstudio

#endif // UTILITIES_DOCUMENT_TABLE_IMPL_HPP
