/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_CSVFILE_IMPL_HPP
#define UTILITIES_FILETYPES_CSVFILE_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Vector.hpp"

namespace openstudio {

class CSVFile;
class Variant;
class DateTime;

namespace detail {

  class UTILITIES_API CSVFile_Impl
  {
   public:
    CSVFile_Impl();

    CSVFile_Impl(const std::string& s);

    CSVFile_Impl(const openstudio::path& p);

    CSVFile clone() const;

    std::string string() const;

    // cppcheck-suppress functionStatic
    bool save() const;

    bool saveAs(const openstudio::path& p);

    boost::optional<openstudio::path> path() const;

    bool setPath(const openstudio::path& path);

    void resetPath();

    unsigned numColumns() const;

    unsigned numRows() const;

    std::vector<std::vector<Variant>> rows() const;

    void addRow(const std::vector<Variant>& row);

    void setRows(const std::vector<std::vector<Variant>>& rows);

    void clear();

    /** Add a column of std::vector<DateTime>, returns column index (first column is index 0). */
    unsigned addColumn(const std::vector<DateTime>& dateTimes);

    /** Add a column of values in a Vector, returns column index (first column is index 0). */
    unsigned addColumn(const Vector& values);

    /** Add a column of values in a std::vector<double>, returns column index (first column is index 0). */
    unsigned addColumn(const std::vector<double>& values);

    /** Add a column of values in a std::vector<std::string>, returns column index (first column is index 0). */
    unsigned addColumn(const std::vector<std::string>& values);

    /** Get column of DateTime values (first column is index 0). Empty vector is returned if any cell is not a valid DateTime or if column index is invalid.*/
    std::vector<DateTime> getColumnAsDateTimes(unsigned columnIndex) const;

    /** Get column as a Vector (first column is index 0). Empty vector is returned if any cell is not a valid number or if column index is invalid.*/
    std::vector<double> getColumnAsDoubleVector(unsigned columnIndex) const;

    /** Get column as a Vector (first column is index 0). Numeric cells will be converted to strings. Empty vector is returned if column index is invalid.*/
    std::vector<std::string> getColumnAsStringVector(unsigned columnIndex) const;

   private:
    REGISTER_LOGGER("openstudio.CSVFile");

    // throws on error
    static std::vector<std::vector<Variant>> parseRows(std::istream& input);

    void assignNumColumns();

    void ensureNumRows(unsigned numRows);

    void padRows();

    boost::optional<openstudio::path> m_path;
    unsigned m_numColumns;
    std::vector<std::vector<Variant>> m_rows;
  };

}  // namespace detail
}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_CSVFILE_IMPL_HPP
