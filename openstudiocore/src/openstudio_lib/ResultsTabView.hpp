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

#ifndef OPENSTUDIO_RESULTSTABVIEW_HPP
#define OPENSTUDIO_RESULTSTABVIEW_HPP

#include "MainTabView.hpp"

#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/units/Unit.hpp"

#include "../runmanager/lib/RunManager.hpp"

#include <QWidget>
#if QT_VERSION >= 0x050400
#include <QWebEngineView>
#else
#include <QWebView>
#endif

class QComboBox;
class QPushButton;

namespace openstudio {

  // main widget

  class ResultsView : public QWidget
  {
    Q_OBJECT;

    public:
      ResultsView(QWidget *t_parent = nullptr);
      virtual ~ResultsView();
      void searchForExistingResults(const openstudio::path &t_runDir);

    public slots:
      void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceResultsPath);
      void onUnitSystemChange(bool t_isIP);
      void treeChanged(const openstudio::UUID &t_uuid);

    private slots:
      void openResultsViewerClicked();
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

#if QT_VERSION >= 0x050400
      QWebEngineView * m_view;
#else
      QWebView * m_view;
#endif
      QComboBox * m_comboBox;
  };

  class ResultsTabView : public MainTabView
  {
    Q_OBJECT;

    public:

      ResultsTabView(const QString & tabLabel,
        TabType tabType,
        QWidget * parent = nullptr);
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

} // openstudio

#endif // OPENSTUDIO_RESULTSTABVIEW_HPP
