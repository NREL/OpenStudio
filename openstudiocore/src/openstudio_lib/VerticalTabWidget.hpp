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

#ifndef OPENSTUDIO_VERTICALTABWIDGET_HPP
#define OPENSTUDIO_VERTICALTABWIDGET_HPP

#include <QWidget>
#include <vector>

class QStackedWidget;

class QPushButton;

namespace openstudio {

class VerticalTabWidget : public QWidget
{
  Q_OBJECT

  public:

  VerticalTabWidget(QWidget * parent = 0);

  virtual ~VerticalTabWidget() {}

  void addTab( QWidget * widget,
               int id,
               QString toolTip,
               const QString & selectedImagePath,
               const QString & unSelectedImagePath );

  void deleteAllTabs();

  int verticalTabIndex();

  QWidget* verticalTabWidgetByIndex(int index);

  signals:

  void tabSelected(int id);

  public slots:

  void setCurrentId(int id);

  void setCurrentIndex(int index);

  void setCurrentWidget(QWidget * widget);

  void nextTab();

  void previousTab();

  private slots:

  void select();

  private:

  QStackedWidget * m_pageStack;

  QWidget * m_tabBar;

  std::vector<QString> m_selectedPixmaps;

  std::vector<QString> m_unSelectedPixmaps;

  std::vector<QPushButton *> m_tabButtons; 

  std::vector<int> m_ids;

  int currentIndex;
};

} // namespace openstudio

#endif // OPENSTUDIO_VERTICALTABWIDGET_HPP

