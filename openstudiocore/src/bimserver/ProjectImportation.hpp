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

#ifndef BIMSERVER_PROJECTIMPORTATION_HPP
#define BIMSERVER_PROJECTIMPORTATION_HPP

#include "BIMserverAPI.hpp"
#include "BIMserverConnection.hpp"

#include "../model/Model.hpp"

#include <QDialog>
#include <QListWidget>
#include <QStatusBar>
#include <QSettings>
#include <QEventLoop>

namespace openstudio {
namespace bimserver {

		/// This shows a input dialog to gather project id for import
		class BIMSERVER_API ProjectImportation: public QDialog
		{
			Q_OBJECT

		public:

			/// Default constructor
			ProjectImportation(QWidget *parent);

			/// Start importing IFC workflow
			boost::optional<model::Model> run();

			/// Virtual destructor
			~ProjectImportation();

		signals:
			/// OSM String is retrieved.
			void finished();

		public slots:

			/// Takes projectList from BIMserverConnection and prints out projects
			void processProjectList(QStringList projectList);
			void processIFCList(QStringList ifcList);
			void processSucessCases(QString sucessCase);
			void processFailureCases(QString failureCase);
			void processOSMRetrieved(QString osmString);

		private:
			BIMserverConnection *m_bimserverConnection;
			QSettings *m_settings;


			QString     m_proID;
			QString     m_ifcID;
			QListWidget *m_proList;
			QListWidget *m_ifcList;
			QStatusBar	*m_statusBar;
			QEventLoop  *m_waitForOSM;
			QString     m_OSM;

		private slots:

		 /// What to do when the user clicks on the okButton
			void okButton_clicked();
			void newButton_clicked();
			void loadButton_clicked();
			void selectButton_clicked();
			void settingButton_clicked();
		};

} // bimserver
} // openstudio

#endif // BIMSERVER_PROJECTIMPORTATION_HPP 
