/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
          const QModelIndex &index) const override;

      void setEditorData(QWidget *editor, const QModelIndex &index) const override;
      void setModelData(QWidget *editor, QAbstractItemModel *model,
          const QModelIndex &index) const override;

      void updateEditorGeometry(QWidget *editor,
          const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  };

  /// Editor delegate used during editing of the ToolInfo name
  class ToolNameDelegate : public QItemDelegate
  {
    Q_OBJECT

    public:
      ToolNameDelegate(QObject *parent = nullptr);
      virtual ~ToolNameDelegate() {}

      QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
          const QModelIndex &index) const override;

      void setEditorData(QWidget *editor, const QModelIndex &index) const override;
      void setModelData(QWidget *editor, QAbstractItemModel *model,
          const QModelIndex &index) const override;

      void updateEditorGeometry(QWidget *editor,
          const QStyleOptionViewItem &option, const QModelIndex &index) const override;
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
