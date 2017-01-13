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

#ifndef OPENSTUDIO_MAINTABVIEW_HPP
#define OPENSTUDIO_MAINTABVIEW_HPP

#include <QPointer>
#include <QWidget>

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include <vector>

class QLabel;
class QPushButton;
class QStackedWidget;
class QVBoxLayout;

namespace openstudio {

class OSViewSwitcher;

class MainTabView : public QWidget, public Nano::Observer
{
  Q_OBJECT

public:

  enum TabType {
    MAIN_TAB,
    SUB_TAB,
    GRIDVIEW_SUB_TAB
  };

  MainTabView(const QString & tabLabel, TabType tabType, QWidget * parent = nullptr);

  virtual ~MainTabView();

  void setTabType(TabType tabTyp);

  ///! Use this method only if your tab will *NOT* have sub tabs
  bool addTabWidget(QWidget * widget);

  ///! Use this method only if your tab will have sub tabs
  bool addSubTab(const QString & subTabLabel, int id);

  void setSubTab(QWidget * widget);

  // Returns the id of the current sub tab.
  // Returns -1 if there are no sub tabs.
  //int subTabId() const;

  // Returns the index of the current sub tab.
  // Returns -1 if there are no sub tabs.
  //int subTabIndex() const;

  // Public method for setting the current sub tab.
  bool selectSubTabByIndex(int index);

  QPointer<OSViewSwitcher> m_editView;

protected:

  void setCurrentIndex(int index);
  void setCurrentWidget(QWidget * widget);
  void paintEvent( QPaintEvent * event ) override;
  void resizeEvent( QResizeEvent * event ) override;

private:

  QLabel * m_tabLabel = nullptr;
  QWidget * m_mainWidget = nullptr;
  QWidget * m_currentInnerWidget = nullptr;
  QVBoxLayout * m_innerLayout = nullptr;

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
