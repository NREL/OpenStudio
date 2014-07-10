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

#ifndef RUNMANAGER_LIB_FILEINFO_HPP
#define RUNMANAGER_LIB_FILEINFO_HPP

#include "RunManagerAPI.hpp"

#include <string>
#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/core/Path.hpp"
#include <QMetaType>
#include <QRegExp>
#include <QUrl>
#include <algorithm>

namespace openstudio {
namespace runmanager {

/// Basic file information class used by runmanager for handling tracking of which
/// input and output files to and from Job objects
struct RUNMANAGER_API FileInfo {

  /// Construct a FileInfo object
  /// \param[in] t_filename The short file name of the file
  /// \param[in] t_lastModified The last modified time of the file
  /// \param[in] t_key The "key" by which this file is known. For example, an in.idf for 
  ///              processing might be known as "idf" for other idf based tools to find it
  /// \param[in] t_fullPath The full path of the file on the local disk. If this represents
  ///                   a file on a remote disk, this item might not provided.
  /// \param[in] t_exists   True if the file exists, false if it represents a (recently)
  ///                   removed file.
  FileInfo(const std::string &t_filename, 
           const openstudio::DateTime &t_lastModified,
           const std::string &t_key,
           const openstudio::path &t_fullPath = openstudio::path(), 
           bool t_exists = true);

  /// Construct a FileInfo from a openstudio::path
  /// \param[in] t_path Full path of file
  /// \param[in] t_key Key to use for this FileInfo
  FileInfo(const openstudio::path &t_path, const std::string &t_key);

  FileInfo();

  FileInfo complete(const openstudio::path &t_basePath) const;  

  bool operator<(const FileInfo &fi) const;

  bool operator==(const FileInfo &t_rhs) const;

  bool operator!=(const FileInfo &t_rhs) const;

  /// Adds a "requiredFile" for tracking for this FileInfo object
  /// 
  /// When a FileInfo is used as input to a Job, the required files attached to
  /// the given FileInfo are brought along automatically. They (or references to them)
  /// are placed in the working directory that the Job uses during execution.
  ///
  /// \param[in] t_url Url of required file
  /// \param[in] t_target Target filename of required file
  /// 
  /// \todo implement handling of remote (ftp/http) required files.
  void addRequiredFile(const QUrl &t_url, const openstudio::path &t_target);

  /// Adds a required file with the target path being the filename of the passed in url
  ///
  /// \param[in] t_url Url of requried file
  ///
  /// \sa FileInfo::addRequiredFile(const QUrl &, const openstudio::path &)
  void addRequiredFile(const QUrl &t_url);

  /// Adds a required file with the given target name
  ///
  /// \param[in] t_location Path of the required file. Converted to a QUrl internally
  /// \param[in] t_target Target filename of required file
  ///
  /// \sa FileInfo::addRequiredFile(const QUrl &, const openstudio::path &)
  void addRequiredFile(const openstudio::path &t_location, const openstudio::path &t_target);

  /// Adds a required file with the target path being the filename of the passed in path
  ///
  /// \param[in] t_location Path of the required file. Converted to a QUrl internally
  ///
  /// \sa FileInfo::addRequiredFile(const QUrl &, const openstudio::path &)
  void addRequiredFile(const openstudio::path &t_location);

  /// Returns the details of a required file attached to this FileInfo if it exists
  /// 
  /// The lookup is performed on the target location of the file.
  ///
  /// \throws std::runtime_error if the required file could not be found
  std::pair<QUrl, openstudio::path> getRequiredFile(const openstudio::path &filename);


  /// Prepends a path onto the requiredFiles 
  void prependRequiredFilePath(const openstudio::path &t_path);

  bool hasRequiredFile(const openstudio::path &filename);

  openstudio::path fullPath;         ///< Full local filename
  std::string filename;              ///< Short filename
  bool exists;                       ///< True if the file exists, false if it was removed
  openstudio::DateTime lastModified; ///< Date of last file modification
  std::string key;                   ///< A key identifying the type / usage of this file

  /// Files required by this FileInfo object
  std::vector<std::pair<QUrl, openstudio::path> > requiredFiles;

 private:
  REGISTER_LOGGER("openstudio.runmanager.FileInfo");
};


/// Represents a collection of FileInfo objects
///
/// A Files collection always maintains the original order of the FileInfo objects
/// it was created from and were subsequently added to it.
class RUNMANAGER_API Files
{
  public:
    /// Default constructor
    Files()
    {
    }

    /// Constructs a Files object from a vector of FileInfo objects.
    explicit Files(const std::vector<FileInfo> &t_files)
      : m_files(t_files)
    {}

    Files complete(const openstudio::path &t_basePath) const
    {
      std::vector<FileInfo> files;

      for (const auto & file : m_files)
      {
        files.push_back(file.complete(t_basePath));
      }

      return Files(files);
    }


    bool operator==(const Files &t_rhs) const;

    /// Adds a new FileInfo object on to this collection
    void append(const FileInfo &t_fi)
    {
      m_files.push_back(t_fi);
    }

    /// Adds all of the FileInfo objects in the given Files object to this one
    void append(const Files &t_files)
    {
      append(t_files.m_files.begin(), t_files.m_files.end());
    }

    /// Returns a vector of the accumulated FileInfo objects
    const std::vector<FileInfo> &files() const
    {
      return m_files;
    }

    /// Returns a vector of the accumulated FileInfo objects
    std::vector<FileInfo> &files()
    {
      return m_files;
    }

    /// Returns the last FileInfo object in the collection with the matching keyname
    ///
    /// const version
    /// 
    /// \param[in] t_key Key to look up
    const FileInfo &getLastByKey(const std::string &t_key) const;

    /// Returns the last FileInfo object in the collection that matches the given regular expression
    ///
    /// Const version. Searches are case insensitive
    /// 
    /// \param[in] t_regex RegEx to look up
    ///
    /// \sa http://doc.qt.nokia.com/qregexp.html
    const FileInfo &getLastByRegex(const std::string &t_regex) const;

    /// Returns all FileInfo objects that match the given regular expression as a new Files object
    ///
    /// Searches are case insensitive
    /// 
    /// \param[in] t_regex RegEx to look up
    ///
    /// \sa http://doc.qt.nokia.com/qregexp.html
    Files getAllByRegex(const std::string &t_regex) const;

    /// Returns all FileInfo objects that match the given key as a new Files object
    ///
    /// \param[in] t_key key to lookup
    Files getAllByKey(const std::string &t_key) const;

    /// Returns last FileInfo object matching the given filename
    ///
    /// Const version
    ///
    /// \param[in] t_filename Filename to match
    const FileInfo &getLastByFilename(const std::string &t_filename) const;

    /// Returns all FileInfo objects matching the given filename as a new Files object
    ///
    /// \param[in] t_filename Filename to match
    Files getAllByFilename(const std::string &t_filename) const;

    /// Returns the last FileInfo object in the collection that matches the given regular expression
    ///
    /// non-Const version. Searches are case insensitive
    /// 
    /// \param[in] t_regex RegEx to look up
    ///
    /// \sa http://doc.qt.nokia.com/qregexp.html
    FileInfo &getLastByRegex(const std::string &t_regex);

    /// Returns the last FileInfo object in the collection that matches the given extension
    ///
    /// const version
    ///
    /// \param[in] t_ext Extension to match
    const FileInfo &getLastByExtension(const std::string &t_ext) const;

    /// Returns all FileInfo objects in the collection that match the given extension as a new Files object
    ///
    /// \param[in] t_ext Extension to match
    Files getAllByExtension(const std::string &t_ext) const;

    /// Generic templated function for appending a collection of FileInfo objects to this 
    template<typename InItr>
      void append(const InItr &begin, const InItr &end)
      {
        m_files.insert(m_files.end(), begin, end);
      }

    /// Returns the last FileInfo object in the collection with the matching keyname
    ///
    /// non-const version
    /// 
    /// \param[in] t_key Key to look up
    FileInfo &getLastByKey(const std::string &t_key);

    /// Returns last FileInfo object matching the given filename
    ///
    /// non-Const version
    ///
    /// \param[in] t_filename Filename to match
    FileInfo &getLastByFilename(const std::string &t_filename);

    /// Returns the last FileInfo object in the collection that matches the given extension
    ///
    /// non-const version
    ///
    /// \param[in] t_ext Extension to match
    FileInfo &getLastByExtension(const std::string &t_ext);

  private:
    static bool keyCompare(const std::string &t_key, const FileInfo &fi)
    {
      return fi.key == t_key;
    }

    static bool extensionCompare(const std::string &t_ext, const FileInfo &fi)
    {
      std::string ext = ( (!t_ext.empty()&&t_ext[0]!='.')?".":"") + t_ext;
      return boost::filesystem::path(fi.filename).extension() == ext;
    }

    static bool regexCompare(const QRegExp &t_regex, const FileInfo &fi)
    {
      return t_regex.exactMatch(toQString(fi.filename));
    }

    static bool filenameCompare(const std::string &t_filename, const FileInfo &fi)
    {
      return fi.filename == t_filename;
    }

    const FileInfo &getLast(const std::function<bool (const FileInfo &)> &f) const
    {
      auto itr = std::find_if(m_files.rbegin(), m_files.rend(), f);

      if (itr != m_files.rend())
      {
        return *itr;
      }

      throw std::runtime_error("FileInfo not found");
    }

    FileInfo &getLast(const std::function<bool (const FileInfo &)> &f) 
    {
      auto itr = std::find_if(m_files.rbegin(), m_files.rend(), f);

      if (itr != m_files.rend())
      {
        return *itr;
      }

      throw std::runtime_error("FileInfo not found");
    }

    Files getAll(const std::function<bool (const FileInfo &)> &f) const
    {
      auto itr = m_files.begin();
      auto end = m_files.end();

      Files ret;

      while (itr != end)
      {
        if (f(*itr))
        {
          ret.append(*itr);
        }
        ++itr;
      }

      return ret;
    }

    std::vector<FileInfo> m_files;

};

} // runmanager
} // openstudio


Q_DECLARE_METATYPE(openstudio::runmanager::FileInfo);

#endif // RUNMANAGER_LIB_FILEINFO_HPP

