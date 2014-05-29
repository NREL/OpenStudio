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

#ifndef OPENSTUDIO_RESULTSTABVIEW_HPP
#define OPENSTUDIO_RESULTSTABVIEW_HPP

#include "MainTabView.hpp"

#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/units/Unit.hpp"

#include "../runmanager/lib/RunManager.hpp"

#include <QWidget>
#include <QWebView>

class QComboBox;
class QPushButton;

namespace openstudio {

  // main widget

  class ResultsView : public QWidget
  {
    Q_OBJECT;

    public:
      ResultsView(QWidget *t_parent = 0);
      virtual ~ResultsView();
      void searchForExistingResults(const openstudio::path &t_runDir);

    public slots:
      void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceResultsPath);
      void onUnitSystemChange(bool t_isIP);

    private slots:
      void openResultsViewerClicked();
      void treeChanged(const openstudio::UUID &t_uuid);
      void comboBoxChanged(int index);

    private:
      REGISTER_LOGGER("openstudio::ResultsView");
      openstudio::runmanager::RunManager runManager();
      void populateComboBox(std::vector<openstudio::path> reports);

      bool m_isIP;

      // utility bill results
      QLabel * m_reportLabel;
      QPushButton * m_openResultsViewerBtn;
      
      openstudio::path m_sqlFilePath;
      openstudio::path m_radianceResultsPath;

      QWebView * m_view;
      QComboBox * m_comboBox;
  };

  class ResultsTabView : public MainTabView
  {
    Q_OBJECT;

    public:

      ResultsTabView(const QString & tabLabel,
          bool hasSubTabs,
          QWidget * parent = 0);
      virtual ~ResultsTabView() {}
      void searchForExistingResults(const openstudio::path &t_runDir);

    public slots:
      void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceResultsPath);
      void onUnitSystemChange(bool t_isIP);

    signals:

      void removeResultClicked();

      void importResultClicked();

      void treeChanged(const openstudio::UUID &t_uuid);

    private:
      ResultsView * m_resultsView;
      REGISTER_LOGGER("openstudio::ResultsTabView");
  };

  class ResultsWebView : public QWebView
  {
    Q_OBJECT;

    public:

      ResultsWebView(QWidget * parent = 0);
      
      QSize sizeHint() const;
  };

} // openstudio

#endif // OPENSTUDIO_RESULTSTABVIEW_HPP
