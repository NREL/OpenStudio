/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
