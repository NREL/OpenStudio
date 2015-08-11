/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_MAINTABVIEW_HPP
#define OPENSTUDIO_MAINTABVIEW_HPP

#include <QWidget>
#include <vector>

class QStackedWidget;
class QPushButton;
class QLabel;

namespace openstudio {

class MainTabView : public QWidget
{
  Q_OBJECT

public:

  enum TabType {
    MAIN_TAB,
    SUB_TAB,
    GRIDVIEW_SUB_TAB
  };

  MainTabView(const QString & tabLabel, TabType tabType, QWidget * parent = nullptr);

  virtual ~MainTabView() {}

  void setTabType(TabType tabTyp);

  ///! Use this method only if your tab will *NOT* have sub tabs
  bool addTabWidget(QWidget * widget);

  ///! Use this method only if your tab will have sub tabs
  bool addSubTab(const QString & subTabLabel, QWidget * widget, int id);

  // Returns the id of the current sub tab.
  // Returns -1 if there are no sub tabs.
  int subTabId() const;

  // Returns the index of the current sub tab.
  // Returns -1 if there are no sub tabs.
  int subTabIndex() const;

  // Public method for setting the current sub tab.
  bool selectSubTabByIndex(int index);

protected:

  void setCurrentIndex(int index);
  void setCurrentWidget(QWidget * widget);
  void paintEvent( QPaintEvent * event ) override;
  void resizeEvent( QResizeEvent * event ) override;

private:

  QLabel * m_tabLabel;
  QStackedWidget * m_stackedWidget;
  QWidget * m_mainWidget;

  std::vector<QString> m_selectedPixmaps;
  std::vector<QString> m_neighborSelectedPixmaps;
  std::vector<QString> m_unSelectedPixmaps;
  std::vector<QPushButton *> m_tabButtons;
  std::vector<int> m_ids;

  TabType m_tabType;

signals:

  void tabSelected(int id);
  void toggleUnitsClicked(bool displayIP);

private slots:

  void select();

};

} // namespace openstudio

#endif // OPENSTUDIO_MAINTABVIEW_HPP
