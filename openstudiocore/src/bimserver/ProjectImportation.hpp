#ifndef BIMSERVER_PROJECTIMPORTATION_HPP
#define BIMSERVER_PROJECTIMPORTATION_HPP

#include "BIMserverAPI.hpp"
#include "BIMserverConnection.hpp"

#include <QDialog>
#include <QListWidget>
#include <QStatusBar>
#include <QSettings>

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
			void run();

			/// Virtual destructor
			~ProjectImportation();

		public slots:

			/// Takes projectList from BIMserverConnection and prints out projects
			void processProjectList(QStringList projectList);
			void processIFCList(QStringList ifcList);
			void processSucessCases(QString sucessCase);
			void processFailureCases(QString failureCase);
      void processOSMRetrieved(QString osmString);

		signals:

			void osmRetrieved(QString osmString);

		private:
			BIMserverConnection *m_bimserverConnection;
			QSettings *m_settings;


			QString     m_proID;
			QString     m_ifcID;
			QListWidget *m_proList;
			QListWidget *m_ifcList;
			QStatusBar	*m_statusBar;

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
