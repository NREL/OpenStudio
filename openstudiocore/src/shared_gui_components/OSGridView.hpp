/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef SHAREDGUICOMPONENTS_OSGRIDVIEW_HPP
#define SHAREDGUICOMPONENTS_OSGRIDVIEW_HPP

#include <QTimer>
#include <QWidget>

#include "../openstudio_lib/OSItem.hpp"

#include "../model/ModelObject.hpp"

class QGridLayout;
class QHideEvent;
class QVBoxLayout;
class QLabel;
class QShowEvent;
class QString;
class QLayoutItem;

namespace openstudio{

class ModelSubTabView;
class OSCollapsibleView;
class OSDropZone;
class OSGridController;
class OSItem;

class OSGridView : public QWidget
{
  Q_OBJECT

public:

  OSGridView(OSGridController * gridController,
    const QString & headerText,
    const QString & dropZoneText,
    bool useHeader,
    QWidget * parent = nullptr);

  virtual ~OSGridView() {};

  // return the QLayoutItem at a particular partition, accounting for multiple grid layouts
  QLayoutItem * itemAtPosition(int row, int column);

  OSDropZone * m_dropZone;

  virtual ModelSubTabView * modelSubTabView();

  void requestRemoveRow(int row);

  void requestAddRow(int row);

  QVBoxLayout * m_contentLayout;

protected:

  virtual void hideEvent(QHideEvent * event) override;

  virtual void showEvent(QShowEvent * event) override;

signals:

  void dropZoneItemClicked(OSItem* item);

  void gridRowSelectionChanged(int checkState);

public slots:

  void onSelectionCleared();

  void refreshAll();

  void requestRefreshAll();

  void requestRefreshGrid();

private slots:

  void deleteAll();

  void addWidget(int row, int column);

  void selectCategory(int index);

  void doRefresh();

  void doRowSelect();

  void selectRowDeterminedByModelSubTabView();

private:

  enum QueueType
  {
    AddRow,
    RemoveRow,
    RefreshRow,
    RefreshGrid,
    RefreshAll
  };

  // construct a grid layout to our specs
  QGridLayout * makeGridLayout();

  // Add a widget, adding a new layout if necessary
  void addWidget(QWidget *w, int row, int column);

  void setGridController(OSGridController * gridController);

  static const int ROWS_PER_LAYOUT = 100;

  std::vector<QGridLayout *> m_gridLayouts;

  OSCollapsibleView * m_CollapsibleView;

  OSGridController * m_gridController;

  std::vector<QueueType> m_queueRequests;

  QTimer m_timer;

  int m_rowToAdd = -1;

  int m_rowToRemove = -1;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSGRIDVIEW_HPP
