/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_CSVFILE_HPP
#define UTILITIES_FILETYPES_CSVFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Vector.hpp"

namespace openstudio {

class Variant;
class TimeSeries;
class DateTime;

namespace detail {
  class CSVFile_Impl;
}

/** Class for reading and writing CSV files. */
class UTILITIES_API CSVFile
{
 public:
  /** Create a new, empty CSVFile. */
  CSVFile();

  /** Constructor with string, will throw if string is not a CSVFile. */
  CSVFile(const std::string& s);

  /** Constructor with path, will throw if path does not exist or file is incorrect. */
  CSVFile(const openstudio::path& p);

  /** Clones this CSVFile into a separate one. */
  CSVFile clone() const;

  /** Attempt to load a CSVFile from string */
  static boost::optional<CSVFile> load(const std::string& s);

  /** Attempt to load a CSVFile from path */
  static boost::optional<CSVFile> load(const openstudio::path& p);

  /** Get the CSVFile as a string. */
  std::string string() const;

  /** Saves this file to the current location, fails if no path is set. */
  bool save() const;

  /** Saves this file to a new location. Updates the CSVFile path. */
  // cppcheck-suppress functionConst
  bool saveAs(const openstudio::path& p);

  /** Returns the absolute path this CSVFile was loaded from or saved to, empty for new CSVFile. */
  boost::optional<openstudio::path> path() const;

  /** Sets the absolute path for this CSVFile. */
  // cppcheck-suppress functionConst
  bool setPath(const openstudio::path& path);

  /** Resets the absolute path for this CSVFile. */
  // cppcheck-suppress functionConst
  void resetPath();

  /** Returns number of columns, this is the maximum number of columns across all rows. */
  unsigned numColumns() const;

  /** Returns number of rows. */
  unsigned numRows() const;

  /** Get all rows. */
  std::vector<std::vector<Variant>> rows() const;

  /** Add a row. */
  // cppcheck-suppress functionConst
  void addRow(const std::vector<Variant>& row);

  /** Set all rows. */
  // cppcheck-suppress functionConst
  void setRows(const std::vector<std::vector<Variant>>& rows);

  /** Clear all rows. */
  // cppcheck-suppress functionConst
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

 protected:
  // get the impl
  template <typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  friend class detail::CSVFile_Impl;

  /** Protected constructor from impl. */
  CSVFile(std::shared_ptr<detail::CSVFile_Impl> impl);

 private:
  // configure logging
  REGISTER_LOGGER("openstudio.CSVFile");

  // pointer to implementation
  std::shared_ptr<detail::CSVFile_Impl> m_impl;
};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const CSVFile& csvFile);

}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_CSVFILE_HPP
