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

#ifndef RUNMANAGER_LIB_CONFIGURATION_HPP
#define RUNMANAGER_LIB_CONFIGURATION_HPP

#include <string>
#include <QTimer>
#include <QItemDelegate>
#include <boost/optional.hpp>
#include "RunManager.hpp"
#include <runmanager/lib/ui_Configuration.h>
#include "Job.hpp"
#include <QStandardItemModel>

class QFileDialog;

namespace openstudio {
namespace runmanager {

  /// Editor delegate used during editing of the ToolInfo version number
  class SpinBoxDelegate : public QItemDelegate
  {
    Q_OBJECT

    public:
      SpinBoxDelegate(QObject *parent = nullptr);
      virtual ~SpinBoxDelegate() {}

      QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
          const QModelIndex &index) const;

      void setEditorData(QWidget *editor, const QModelIndex &index) const;
      void setModelData(QWidget *editor, QAbstractItemModel *model,
          const QModelIndex &index) const;

      void updateEditorGeometry(QWidget *editor,
          const QStyleOptionViewItem &option, const QModelIndex &index) const;
  };

  /// Editor delegate used during editing of the ToolInfo name
  class ToolNameDelegate : public QItemDelegate
  {
    Q_OBJECT

    public:
      ToolNameDelegate(QObject *parent = nullptr);
      virtual ~ToolNameDelegate() {}

      QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
          const QModelIndex &index) const;

      void setEditorData(QWidget *editor, const QModelIndex &index) const;
      void setModelData(QWidget *editor, QAbstractItemModel *model,
          const QModelIndex &index) const;

      void updateEditorGeometry(QWidget *editor,
          const QStyleOptionViewItem &option, const QModelIndex &index) const;
  };


  //! QDialog based user interface for setting RunManager configuration options. 
  //! \sa http://doc.qt.nokia.com/qdialog.html
  class Configuration : public QDialog
  {
    Q_OBJECT

  public:
    //! Create a Configuration dialog for setting ConfigOptions
    Configuration(QWidget *parent = nullptr, 
                  Qt::WindowFlags flags = Qt::WindowFlags(Qt::Dialog | Qt::WindowTitleHint), 
                  const ConfigOptions &co = ConfigOptions());
    virtual ~Configuration();

    //! Returns the last modified version of the ConfigOptions object. 
    //! This return value should only be used if the Configuration exec() or
    //! result() methods return QDialog::Accepted - indicating that the user
    //! did not cancel out of the dialog.
    //! \return The ConfigOptions object as it was modified by the user
    ConfigOptions getConfigOptions();

  private:
    //! Used for QDesigner classes.
    Ui::Configuration ui;
    QStandardItemModel m_model;

    void addToolLocation(const std::pair<ToolVersion, ToolLocationInfo> &tool);

  private slots:
    //! Called when path search button is pressed for default IDFLocation
    void updateIDFLocation();

    //! Called when path search button is pressed for default EPWLocation
    void updateEPWLocation();

    //! Called when path search button is pressed for output location
    void updateOutputLocation();

    //! Called when remove tool button is pressed
    void removeSelectedTool();

    //! Called when the add tool button is pressed
    void addTool();

    //! Called when the state of the in place checkbox is changed
    void inPlaceStateChanged(int t_newstate);

  };


} // runmanager namespace
} // openstudio namespace


#endif // RUNMANAGER_LIB_CONFIGURATION_HPP
