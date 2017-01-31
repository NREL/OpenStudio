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

#ifndef SHAREDGUICOMPONENTS_OSLISTVIEW_HPP
#define SHAREDGUICOMPONENTS_OSLISTVIEW_HPP

#include "OSListController.hpp"
#include <QWidget>
#include <QSharedPointer>
#include <map>
#include "../utilities/core/Logger.hpp"

class QScrollArea;
class QVBoxLayout;
class QGraphicsObject;

namespace openstudio{

class OSListItem;
class OSItemDelegate;

/** The purpose of OSListView is to display a list of items in a vertical column.
 *  This class's primary responsibility is to visually arrange a list of things.
 *  Data is provided to OSListView by a subclass of OSListController.
 *  For each item provided by the OSListController, OSListView draws a view provided by OSItemDelegate.
 *  It should be uncommon to subclass OSListView.
 */
class OSListView : public QWidget
{
  Q_OBJECT

 public:

  OSListView(bool scrollable = false, QWidget * parent = nullptr);

  virtual ~OSListView() {}

  void setDelegate(QSharedPointer<OSItemDelegate> delegate);

  void setListController(QSharedPointer<OSListController> listController);

  QSharedPointer<OSListController> listController() const;

  void setSpacing(int spacing);

  void setContentsMargins(int left,int top,int right,int bottom);

  void setHorizontalScrollBarAlwaysOn(bool alwaysOn);

  void setVerticalScrollBarAlwaysOn(bool alwaysOn);

 public slots:

  void refreshAllViews();

 protected:

  void paintEvent(QPaintEvent *) override;

 private slots:

  void insertItemView(int i);

  void removeItemView(int i);

  void removePair(QObject * object);

  void refreshItemView(int i);

 private:

  QSharedPointer<OSItemDelegate> m_delegate;

  QSharedPointer<OSListController> m_listController;

  QVBoxLayout * m_mainVLayout;

  // Use this to keep the OSListItem classes around for the life of the widget
  std::map<QObject *,QSharedPointer<OSListItem> > m_widgetItemPairs;

  bool m_scrollable;

  QScrollArea * m_scrollArea;

  REGISTER_LOGGER("openstudio.shared_gui_components.OSListView");
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSLISTVIEW_HPP

