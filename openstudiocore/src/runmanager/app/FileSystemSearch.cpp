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

#include "FileSystemSearch.hpp"
#include "../../utilities/core/Application.hpp"

namespace openstudio {
namespace runmanager {

  namespace detail {
    void FileSystemSearchThread::run()
    {
      m_fileBuilder();
    }
  }


  FileSystemSearch::FileSystemSearch(const openstudio::path &t_rootPath, bool t_recursive,
      const std::string &t_fileExtension,
      const QRegExp &t_regex)
    : m_rootPath(t_rootPath), m_recursive(t_recursive), m_fileExtension(t_fileExtension),
      m_regex(t_regex), m_header(new QStandardItem())
  {
    updateModel();

    m_header->setEditable(true);
    m_header->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    m_model.setHorizontalHeaderItem(0, m_header);
  }



  bool FileSystemSearch::recursive() const
  {
    return m_recursive;
  }
 
  void FileSystemSearch::setRecursive(bool t_recursive)
  {
    if (m_recursive != t_recursive)
    {
      m_recursive = t_recursive;
      updateModel();
    }
  }

  openstudio::path FileSystemSearch::rootPath() const
  {
    return m_rootPath;
  }

  void FileSystemSearch::setRootPath(const openstudio::path &t_rootPath)
  {
    if (m_rootPath != t_rootPath)
    {

      m_rootPath = t_rootPath;
      emit rootPathChanged(m_rootPath);
      updateModel();
    }
  }

  QRegExp FileSystemSearch::regex() const
  {
    return m_regex;
  }

  void FileSystemSearch::setRegex(const QRegExp &t_regex)
  {
    if (m_regex != t_regex)
    {
      m_regex = t_regex;
      LOG(Debug, "New regex " << toString(m_regex.pattern()));
      updateModel();
    }
  }

  std::string FileSystemSearch::fileExtension() const
  {
    return m_fileExtension;
  }

  void FileSystemSearch::threadFinished()
  {
    emit searchComplete();
  }

  void FileSystemSearch::setFileExtension(const std::string &t_fileExtension)
  {
    if (m_fileExtension != t_fileExtension)
    {
      m_fileExtension = t_fileExtension;
      updateModel();
    }
  }

  void FileSystemSearch::fileFoundSlot(const QString &filestring)
  {
    //LOG(Debug, "FileFoundSlot " << toString(filestring) << " thread: " << QThread::currentThreadId());

    if (sender() && sender() == m_thread.get())
    {

      if (m_model.findItems(filestring).empty())
      {

        // Make sure the signal is coming from the thread we expect it to, and it's not just queued up
        auto qsi = new QStandardItem(filestring);
        qsi->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        qsi->setCheckable(true);
        qsi->setCheckState(Qt::Unchecked);


        m_model.appendRow(qsi);
        m_model.sort(0);
      }
    }
  }

  void FileSystemSearch::selectAll()
  {
    for (int i = 0; i < m_model.rowCount(); ++i)
    {
      m_model.item(i)->setCheckState(Qt::Checked);
    }
  }

  void FileSystemSearch::select(const QModelIndexList &qmi)
  {
    for (QModelIndexList::const_iterator itr = qmi.begin();
         itr != qmi.end();
         ++itr)
    {
      m_model.itemFromIndex(*itr)->setCheckState(Qt::Checked);
    }

  }

  void FileSystemSearch::selectNone()
  {
    for (int i = 0; i < m_model.rowCount(); ++i)
    {
      m_model.item(i)->setCheckState(Qt::Unchecked);
    }
  }

  void FileSystemSearch::invertSelection()
  {
    for (int i = 0; i < m_model.rowCount(); ++i)
    {
      QStandardItem *item = m_model.item(i);
      item->setCheckState(item->checkState()==Qt::Checked?Qt::Unchecked:Qt::Checked);
    }
  }

  void FileSystemSearch::updateModel()
  {
    openstudio::path root(m_rootPath);

    if (!root.empty())
    {
      openstudio::path::const_iterator itr = root.end();
      --itr;
      if (itr != root.begin() && (itr->empty() || *itr == toPath(".")))
      {
        root = root.parent_path();
      }
    }


    if (m_thread)
    {
      if (m_thread->isRunning())
      {
        LOG(Info, "FileSystemSearchThread is running, interrupting");
      }

      m_thread->disconnect();

      m_thread->cancel();
      m_thread->quit();
      m_thread->wait();

      //Can we eat all of the pending messages from the thread before we continue??
//      openstudio::Application::instance().processEvents();

      m_thread.reset();

      LOG(Info, "FileSystemSearchThread completed");
    }


    if (!m_regex.isValid())
    {
      m_header->setText("Error in search pattern");
      emit errorInRegex(m_regex.errorString());
    } else {
      emit errorInRegex("");
      QString p = toQString(m_rootPath.native());
      QString slash = toQString(boost::filesystem::path("/").make_preferred().native());
      if (!p.endsWith(slash))
      {
        p += slash;
      }

      m_header->setText(p);
    }

    try {
      if (m_rootPath.empty() || !boost::filesystem::exists(m_rootPath) || !boost::filesystem::is_directory(m_rootPath))
      {
        return;
      }
    } catch (const std::exception &) {
      m_model.removeRows(0, m_model.rowCount());
      return;
    }


    if (!m_regex.isValid())
    {
      return;
    }
   
    if (m_recursive)
    {
      typedef boost::filesystem::recursive_directory_iterator diritr;

      try {
        diritr begin(root);
        diritr end;
        m_thread = std::shared_ptr<detail::FileSystemSearchThread>(new detail::FileSystemSearchThread(
              root, begin, end, m_regex, m_fileExtension));
      } catch (const boost::filesystem::filesystem_error &e) {
        LOG(Info, "Error browsing path: " << toString(root) << ": " << e.what());
        return;
      }

    } else {
      typedef boost::filesystem::directory_iterator diritr;

      try {
        diritr begin(root);
        diritr end;
        m_thread = std::shared_ptr<detail::FileSystemSearchThread>(new detail::FileSystemSearchThread(
              root, begin, end, m_regex, m_fileExtension));
      } catch (const boost::filesystem::filesystem_error &e) {
        LOG(Info, "Error browsing path: " << toString(root) << ": " << e.what());
        return;
      }

    }

    connect(m_thread.get(), SIGNAL(fileFound(const QString &)), this, SLOT(fileFoundSlot(const QString &)));
    connect(m_thread.get(), SIGNAL(finished()), this, SLOT(threadFinished()));
    emit searchStarted();
    m_model.removeRows(0, m_model.rowCount());
    m_thread->start();

  }

  std::vector<openstudio::path> FileSystemSearch::selectedFiles()
  {
    std::vector<openstudio::path> results;

    int rowCount = m_model.rowCount();
    
    for (int i = 0; i < rowCount; ++i)
    {
      QStandardItem *qsi = m_model.item(i);
      if (qsi->checkState() == Qt::Checked)
      {
        results.push_back(m_rootPath / toPath(qsi->text()));
      }
    }

    return results;
  }

  std::vector<openstudio::path> FileSystemSearch::foundFiles()
  {
    std::vector<openstudio::path> results;

    int rowCount = m_model.rowCount();
    
    for (int i = 0; i < rowCount; ++i)
    {
      QStandardItem *qsi = m_model.item(i);
      results.push_back(m_rootPath / toPath(qsi->text()));
    }

    return results;
  }

  QAbstractItemModel *FileSystemSearch::getQItemModel() 
  {
    return &m_model;
  }

  openstudio::path FileSystemSearch::getFile(const QModelIndex &idx)
  {
    QStandardItem *qsi = m_model.itemFromIndex(idx);
    if (qsi)
    {
      return m_rootPath / toPath(qsi->text());
    }

    throw std::out_of_range("Not a valid item");
  }


}
}


