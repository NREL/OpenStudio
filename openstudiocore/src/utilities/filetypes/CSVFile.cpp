/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CSVFile.hpp"
#include "CSVFile_Impl.hpp"

#include "../core/Assert.hpp"
#include "../core/PathHelpers.hpp"
#include "../core/Checksum.hpp"
#include "../data/Variant.hpp"
#include "../time/DateTime.hpp"

#include <iostream>

namespace openstudio{
namespace detail{

  CSVFile_Impl::CSVFile_Impl()
  : m_numColumns(0)
  {
  }

  CSVFile_Impl::CSVFile_Impl(const std::string& s)
  {
    std::istringstream ss(s);

    // will throw on error
    m_rows = parseRows(ss);
      
    setNumColumns();
  }

  CSVFile_Impl::CSVFile_Impl(const openstudio::path& p)
  {
    if (!boost::filesystem::exists(p) || !boost::filesystem::is_regular_file(p)){
      LOG_AND_THROW("Path '" << p << "' is not a CSVFile file");
    }

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(p));

    // will throw on error
    m_rows = parseRows(ifs);
    
    m_path = p;
    setNumColumns();
  }

  CSVFile CSVFile_Impl::clone() const
  {
    CSVFile result(this->string());
    if (m_path) {
      result.setPath(*m_path);
    }

    return result;
  }

  std::string CSVFile_Impl::string() const
  {
    std::string result;
    return result;
  }

  bool CSVFile_Impl::save() const
  {
    boost::optional<openstudio::path> p = path();
    if (!p){
      return false;
    }

    if (makeParentFolder(*p)) {
      std::ofstream outFile(openstudio::toSystemFilename(*p));

      if (outFile) {
        try {
          outFile << string();
          outFile.close();
          return true;
        } catch (...) {
          LOG(Error, "Unable to write file to path '" << toString(*p) << "'.");
          return false;
        }
      }
    }

    LOG(Error, "Unable to write file to path '" << toString(*p) << "', because parent directory "
        << "could not be created.");

    return false;
  }

  bool CSVFile_Impl::saveAs(const openstudio::path& p)
  {
    setPath(p);
    return save();
  }

  boost::optional<openstudio::path> CSVFile_Impl::path() const
  {
    return m_path;
  }

  bool CSVFile_Impl::setPath(const openstudio::path& path) 
  {
    m_path = path;
    return true;
  }

  void CSVFile_Impl::resetPath() 
  {
    m_path.reset();
  }

  unsigned CSVFile_Impl::numColumns() const 
  {
    return m_numColumns;
  }

  unsigned CSVFile_Impl::numRows() const 
  {
    return m_rows.size();
  }

  std::vector<std::vector<Variant> > CSVFile_Impl::rows() const 
  {
    return m_rows;
  }

  void CSVFile_Impl::addRow(const std::vector<Variant>& row) 
  {
    m_rows.push_back(row);
    m_numColumns = std::max<unsigned>(m_numColumns, row.size());
  }

  void CSVFile_Impl::setRows(const std::vector<std::vector<Variant> >& rows) 
  {
    m_rows = rows;
    setNumColumns();
  }

  void CSVFile_Impl::clear() 
  {
    m_rows.clear();
    m_path.reset();
    m_numColumns = 0;
  }

  // throws on error
  std::vector<std::vector<Variant> > CSVFile_Impl::parseRows(const std::istream& input) 
  {
    return std::vector<std::vector<Variant> >();
  }

  void CSVFile_Impl::setNumColumns() 
  {
    m_numColumns = 0;
    for (const auto& row : m_rows) {
      m_numColumns = std::max<unsigned>(m_numColumns, row.size());
    }
  }

} // detail

CSVFile::CSVFile()
  : m_impl(std::shared_ptr<detail::CSVFile_Impl>(new detail::CSVFile_Impl()))
{}

CSVFile::CSVFile(const std::string& s)
  : m_impl(std::shared_ptr<detail::CSVFile_Impl>(new detail::CSVFile_Impl(s)))
{}

CSVFile::CSVFile(const openstudio::path& p)
  : m_impl(std::shared_ptr<detail::CSVFile_Impl>(new detail::CSVFile_Impl(p)))
{}

CSVFile::CSVFile(std::shared_ptr<detail::CSVFile_Impl> impl)
  : m_impl(impl)
{}

CSVFile CSVFile::clone() const
{
  return getImpl<detail::CSVFile_Impl>()->clone();
}

boost::optional<CSVFile> CSVFile::load(const std::string& s)
{
  boost::optional<CSVFile> result;
  try {
    result = CSVFile(s);
  } catch (const std::exception&){
  }
  return result;
}

boost::optional<CSVFile> CSVFile::load(const openstudio::path& p)
{
  boost::optional<CSVFile> result;
  try {
    result = CSVFile(p);
  } catch (const std::exception&){
  }
  return result;
}

std::string CSVFile::string() const
{
  return getImpl<detail::CSVFile_Impl>()->string();
}

bool CSVFile::save() const
{
  return getImpl<detail::CSVFile_Impl>()->save();
}

bool CSVFile::saveAs(const openstudio::path& p)
{
  return getImpl<detail::CSVFile_Impl>()->saveAs(p);
}

boost::optional<openstudio::path> CSVFile::path() const 
{
  return getImpl<detail::CSVFile_Impl>()->path();
}

bool CSVFile::setPath(const openstudio::path& path) 
{
  return getImpl<detail::CSVFile_Impl>()->setPath(path);
}

void CSVFile::resetPath() 
{
  getImpl<detail::CSVFile_Impl>()->resetPath();
}


unsigned CSVFile::numColumns() const 
{
  return getImpl<detail::CSVFile_Impl>()->numColumns();
}

unsigned CSVFile::numRows() const 
{
  return getImpl<detail::CSVFile_Impl>()->numRows();
}

std::vector<std::vector<Variant> > CSVFile::rows() const 
{
  return getImpl<detail::CSVFile_Impl>()->rows();
}

void CSVFile::addRow(const std::vector<Variant>& row) 
{
  getImpl<detail::CSVFile_Impl>()->addRow(row);
}

void CSVFile::setRows(const std::vector<std::vector<Variant> >& rows) 
{
  getImpl<detail::CSVFile_Impl>()->setRows(rows);
}

void CSVFile::clear() 
{
  getImpl<detail::CSVFile_Impl>()->clear();
}


std::ostream& operator<<(std::ostream& os, const CSVFile& CSVFile)
{
  os << CSVFile.string();
  return os;
}

} // openstudio
