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

#include "RunTabView.hpp"

#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include <OpenStudio.hxx>

#include "../model/FileOperations.hpp"
#include "../model/DaylightingControl.hpp"
#include "../model/DaylightingControl_Impl.hpp"
#include "../model/GlareSensor.hpp"
#include "../model/GlareSensor_Impl.hpp"
#include "../model/IlluminanceMap.hpp"
#include "../model/IlluminanceMap_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/UtilityBill.hpp"
#include "../model/UtilityBill_Impl.hpp"

//#include "../runmanager/lib/JobStatusWidget.hpp"
//#include "../runmanager/lib/RubyJobUtils.hpp"
//#include "../runmanager/lib/RunManager.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/core/Assert.hpp"

#include "../shared_gui_components/WorkflowTools.hpp"

#include <boost/filesystem.hpp>

#include "../energyplus/ForwardTranslator.hpp"
#include "../bec/ForwardTranslator.hpp"

#include "../model/Photovoltaic.hpp"
#include "../model/PhotovoltaicThermal.hpp"
#include "../model/Photovoltaic_Impl.hpp"
#include "../model/PhotovoltaicThermal_Impl.hpp"
#include "benchmarkdialog.hpp"

enum PVReportMode { PVReportMode_OPENSTUDIO, PVReportMode_BEC, PVReportMode_ENERGYPLUS};

#include "../energyplus/ForwardTranslator.hpp"

#include <QButtonGroup>
#include <QDir>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QSysInfo>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QProcess>
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QFileSystemWatcher>
#include <QDesktopServices>
#include <QTcpServer>
#include <QTcpSocket>
#include <QComboBox>

#define STR_Mode_EPLUS "Energy Plus"
#define STR_Mode_BEC "BEC"
#define STR_Mode_EPLUS_BEC "Energy Plus then BEC"


////////////////////////////////////////////////////////////////////////////////
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QInputDialog>
#include <QDateTime>

static double lastPV;
static QString bvName;
static double bvVal;
static double WholeNetEnergyConsumptionPerArea = 0.0;
static QPlainTextEdit* _log = NULL;

static void echo(const QString& msg){
    if(_log){
        _log->appendHtml(msg);
    }
}

static QString doubleToMoney(double val){
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    QString sValue = QString("%L1").arg(val ,12,'d',2);
    while(sValue.endsWith('0'))
        sValue.remove(sValue.length()-1, 1);

    if(sValue.endsWith('.'))
        sValue.append('0');

    return sValue;
}

static QString stringToMoney(const QString& val){
    bool isOK;
    double dval = val.toDouble(&isOK);
    if(isOK){
        return doubleToMoney(dval);
    }
    else{
        if(val.indexOf("m^2")){
            QString newVal = val;
            newVal.replace("m^2", "m<sup>2</sup>");
            return newVal;
        }else{
            return val;
        }
    }
}

QString insertSpaceInTag(const QString& tagName){
    QChar ch0 = 'A';
    QString out;
    for(int i=0;i<tagName.length();i++){
        QChar ch = tagName.at(i);
        if(ch.isUpper()){
            if(ch0.isLower()){
                out+= " ";
            }
            else if(ch0.isUpper() && i!=0){
                int i1=i+1;
                if(i1 < tagName.length()){
                    QChar ch1 = tagName.at(i1);
                    if(ch1.isLower())
                        out += " ";
                }
            }
        }
        ch0 = ch;
        out += ch;
    }
    return out;
}

QString Bold(const QString& text){
    return QString("<b>%1%2").arg(text).arg("</b>");
}

QString ParagraphBold(const QString& text){
    return QString("<p><b>%1%2").arg(text).arg("</b></p>");
}

QString PTag(const QString& text){
    return QString("<p>%1</p>").arg(text);
}

QString hn(size_t id, const QString& text){
    QString hx = QString("h") + QString::number(id);
    return QString("<%1>%2</%1>\n").arg(hx).arg(text);
}

static int levelsub=-1;

QString doHorizontalTable(QDomNode& root, QDomNode &node, int& level){

    level = 0;

    QString table =
        "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n"
        "<tbody>\n";

    QString row1="<tr>", row2="<tr>";

    while(!node.isNull()) {
        QDomElement e = node.toElement();
        if(e.tagName() == "WholeNetEnergyConsumptionPerArea"){
            bool isOK;
            WholeNetEnergyConsumptionPerArea = e.text().toDouble(&isOK);
            if(!isOK){
                WholeNetEnergyConsumptionPerArea = 0;
            }
        }
        row1 += QString("<td align=\"right\" valign=\"top\"><b>%1</b></td>").arg(insertSpaceInTag(e.tagName()));
        row2 += QString("<td align=\"right\" valign=\"top\">%1</td>").arg(stringToMoney(e.text()));
        node = node.nextSibling();
    }
    row1 += "</tr>\n";
    row2 += "</tr>\n";

    table += row1;
    table += row2;

    level++;

    QDomElement re = root.toElement();
    QDomNode tmproot = root.nextSibling();
    while(1) {
        QDomElement enext = tmproot.toElement();
        if(re.tagName() == enext.tagName()){
            level++;
            node = tmproot.firstChild();
            QString rown="<tr>";
            while(!node.isNull()) {
                QDomElement e = node.toElement();
                if(!e.text().isEmpty()){
                    rown += QString("<td align=\"right\" valign=\"top\">%1</td>").arg(stringToMoney(e.text()));
                }
                node = node.nextSibling();
            }
            table += rown;
            tmproot = tmproot.nextSibling();
            levelsub = level-1;
        }
        else{
            root = tmproot.toElement();
            break;
        }
    }
    table += "</tbody>\n"
             "</table>\n";
    return table;
}

QString escapeTitle;

static QMap<QString, int> getTableNamesValues() {
    QMap<QString, int>map;
	map.insert("LightingSystemPerformance", 1);
    map.insert("LightingSystemByFloor", 1);
    map.insert("LightingSystemByZone", 1);
    return map;
}

static const QMap<QString, int> tableNames = getTableNamesValues();

QDomElement findElementOnPath(QDomNode& root, const QStringList& paths){
    QDomElement targetNode;
    for (int i = 0; i < paths.size(); ++i) {
        if(i==0){
            targetNode = root.firstChildElement(paths.at(0));
        }
        else{
            targetNode = targetNode.firstChildElement(paths.at(i));
        }

        if(targetNode.isNull())
            break;
    }
    return targetNode;
}

class IDoTable
{
public:
    virtual QString process(QDomNode& root, const QStringList& fullpaths)=0;
};

class doTableArrayOnPath : public IDoTable
{
public:
    doTableArrayOnPath() {}
    double TransparentComponentAreaSUM;
    double ComponentAreaComponentAreaSUM;
    void resetSUM(){
        TransparentComponentAreaSUM = 0.0f;
        ComponentAreaComponentAreaSUM = 0.0f;
    }
    double toWWR(){
        if(ComponentAreaComponentAreaSUM < 0.00000000001)
            return 0.0;
        //echo(QString("----------- TransparentComponentAreaSUM / ComponentAreaComponentAreaSUM : %1/%2").arg(TransparentComponentAreaSUM).arg(ComponentAreaComponentAreaSUM));
        return TransparentComponentAreaSUM / ComponentAreaComponentAreaSUM;
    }

    QString process(QDomNode& root, const QStringList& fullpaths){
        bool initHeader = false;
        QString arrayTags = fullpaths.at(fullpaths.size()-1);

        QStringList paths;
        for (int idx = 0; idx < fullpaths.size()-1; ++idx) {
            paths.append(fullpaths.at(idx));
        }
        QDomElement targetNode = findElementOnPath(root, paths);
        QString out;
        if(targetNode.isNull()){
            out = "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">"
                            "<tbody>"
                            "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>"
                            "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>"
                            "</tbody>"
                            "</table>";
        }else{
            QString rowData;
            QString headerData;
            QDomNodeList rawNodes = targetNode.childNodes();
            if(rawNodes.size()>0){
                for (int rit = 0; rit < rawNodes.size(); ++rit) {
                    QDomNode nd = rawNodes.at(rit);
                    QDomElement erow = nd.toElement();

                    if(erow.tagName()!=arrayTags)
                        continue;

                    rowData.append("<tr>");
                    QDomNodeList colNodes = erow.childNodes();
                    for (int cit = 0; cit < colNodes.size(); ++cit) {

                        QDomNode nd = colNodes.at(cit);
                        QDomElement ecol = nd.toElement();

                        if(ecol.tagName().endsWith("Unit"))
                            continue;

                        if(ecol.tagName() == "TransparentComponentArea"){
                            TransparentComponentAreaSUM += ecol.text().toDouble();
                            //echo(QString("++++++++++++++++++++++++++++TransparentComponentAreaSUM [%1]:%2").arg(ecol.text()).arg(QString::number(TransparentComponentAreaSUM)));
                        }
                        if(ecol.tagName() == "ComponentAreaComponentArea"){
                            ComponentAreaComponentAreaSUM += ecol.text().toDouble();
                            //echo(QString("*****************************ComponentAreaComponentAreaSUM [%1]:%2").arg(ecol.text()).arg(QString::number(ComponentAreaComponentAreaSUM)));
                        }

                        if(!initHeader){
                            QString header = insertSpaceInTag(ecol.tagName());
                            QDomElement ecolnext = ecol.nextSiblingElement();
                            if(ecolnext.tagName().endsWith("Unit")){
                                headerData.append(QString("<td>%1(%2)</td>\n").arg(header).arg(ecolnext.text()));
                            }else{
                                headerData.append(QString("<td>%1</td>\n").arg(header));
                            }
                        }
                        rowData.append(QString("<td>%1</td>\n").arg(stringToMoney(ecol.text())));
                    }
                    initHeader = true;
                    rowData.append("</tr>\n");
                }

                if(rowData.isEmpty()&&headerData.isEmpty()){
                    out = "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n"
                                    "<tbody>\n"
                                    "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n"
                                    "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n"
                                    "</tbody>\n"
                                    "</table>\n";
                }else{
                    out = QString("<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n"
                          "<tbody>\n"
                          "%1\n"
                          "%2\n"
                          "</tbody>\n"
                          "</table>\n").arg(headerData).arg(rowData);
                }
                return out;
            }else{
                out = "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n"
                                "<tbody>\n"
                                "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n"
                                "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n"
                                "</tbody>\n"
                                "</table>\n";
                return out;
            }
        }
        return out;
    }
};

class doVTableOnPath : public IDoTable
{
public:
    doVTableOnPath() {}
    QString process(QDomNode& root, const QStringList& paths){
        QDomElement targetNode = findElementOnPath(root, paths);
        QString out;
        if(targetNode.isNull()){
            out = "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">"
                            "<tbody>"
                            "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>"
                            "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>"
                            "</tbody>"
                            "</table>";
        }else{
            out += "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n<tbody>\n";
            QDomNodeList nodes = targetNode.childNodes();
            if(nodes.size()>0){
                for (int idx = 0; idx < nodes.size(); ++idx) {
                    QDomNode nd = nodes.at(idx);
                    QDomElement element = nd.toElement();

                    if(element.tagName().endsWith("Unit"))
                        continue;

                    QString header = insertSpaceInTag(element.tagName());

                    QDomElement ecolnext = element.nextSiblingElement();
                    if(ecolnext.tagName().endsWith("Unit")){
                        out.append(QString("<tr>\n<td>%1(%2)</td><td>%3</td>\n</tr>\n")
                                   .arg(header)
                                   .arg(ecolnext.text())
                                   .arg(stringToMoney(element.text())));
                    }else{
                        out.append(QString("<tr>\n<td>%1</td><td>%2</td>\n</tr>\n")
                                   .arg(header)
                                   .arg(stringToMoney(element.text())));
                    }
                }
            }else{
                out += "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n";
                out += "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n";
            }
            out += "</tbody>\n</table>\n";
        }
        return out;
    }
};

class doTableOnPath : public IDoTable
{
public:
    doTableOnPath() {}
	double WWR;
    QString process(QDomNode& root, const QStringList& paths){
        QDomElement targetNode = findElementOnPath(root, paths);
        QString out;
        bool isOnBuildingOTTVAC = false;
        if(targetNode.isNull()){
            out = "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">"
                            "<tbody>"
                            "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>"
                            "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>"
                            "</tbody>"
                            "</table>";
        }else{
            QString row2;
            out += "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n<tbody>\n";
            QDomNodeList nodes = targetNode.childNodes();
            if(nodes.size()>0){
                out.append("<tr>");
                row2.append("<tr>");
                for (int idx = 0; idx < nodes.size(); ++idx) {
                    QDomNode nd = nodes.at(idx);
                    QDomElement element = nd.toElement();

                    if(element.tagName() == "WholeNetEnergyConsumptionPerArea"){
                        bool isOK = false;
                        WholeNetEnergyConsumptionPerArea = element.text().toDouble(&isOK);
                        if(!isOK){
                            WholeNetEnergyConsumptionPerArea = 0;
                        }
                    }

                    if(element.tagName() == "BuildingOTTVAC"){
                        bool isOK = false;
                        isOnBuildingOTTVAC = true;
                        WholeNetEnergyConsumptionPerArea = element.text().toDouble(&isOK);
                        if(!isOK){
                            WholeNetEnergyConsumptionPerArea = 0;
                        }
                    }

                    if(element.tagName().endsWith("Unit"))
                        continue;

                    QString header = insertSpaceInTag(element.tagName());
                    QDomElement ecolnext = element.nextSiblingElement();
                    if(ecolnext.tagName().endsWith("Unit")){
                        out.append(QString("<td>%1(%2)</td>\n").arg(header).arg(ecolnext.text()));
                    }else{
                        out.append(QString("<td>%1</td>\n").arg(header));
                    }

                    row2.append(QString("<td>%1</td>\n").arg(stringToMoney(element.text())));
                }
                if (isOnBuildingOTTVAC) {
					out.append("<td>WWR</td>\n");
                    row2.append(QString("<td>%1</td>\n").arg(QString::number(WWR, 'f', 2)));
				}
                row2.append("</tr>\n");
                out.append("</tr>\n");
                out.append(row2);
            }else{
                out += "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n";
                out += "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>\n";
            }
            out += "</tbody>\n</table>\n";
        }
        return out;
    }
};

class TableDataList
{
public:
    enum TABLETYPE { TABLE=0, VTABLE, ARRAYTABLE };
    doTableOnPath* _doTableOnPath;
    doVTableOnPath *_doVTableOnPath;
    doTableArrayOnPath *_doTableArrayOnPath;
    TableDataList(QDomNode root) {
        this->root = root;
        _doTableOnPath = new doTableOnPath;
        _doVTableOnPath = new doVTableOnPath;
        _doTableArrayOnPath = new doTableArrayOnPath;
        tableProcesser.insert(TABLE, _doTableOnPath);
        tableProcesser.insert(VTABLE, _doVTableOnPath);
        tableProcesser.insert(ARRAYTABLE, _doTableArrayOnPath);
    }

    ~TableDataList(){
        QHash<TABLETYPE, IDoTable*>::iterator it = tableProcesser.begin();
        while (it!=tableProcesser.end()) {
            delete it.value();
            it++;
        }
        tableProcesser.clear();
    }

    void append(const QStringList& path, const QString& title, TABLETYPE type){
        sls.append(path);
        titles.append(title);
        tableTypes.append(type);
    }

    QString toString(){
        _doTableArrayOnPath->resetSUM();
        _doTableArrayOnPath->process(root, QStringList()<<"EnvelopeSystem" << "BuildingOTTVwall" << "TransparentComponentWall" << "TransparentComponentByWall");
        _doTableArrayOnPath->process(root, QStringList()<<"EnvelopeSystem" << "BuildingOTTVwall" << "ComponentAreaWall" << "ComponentAreaPerWall");
        _doTableOnPath->WWR = _doTableArrayOnPath->toWWR();
        QString out;
        for (int idx = 0; idx < sls.size(); ++idx) {
            QStringList path = sls.at(idx);
            QString title = titles.at(idx);
            out += ParagraphBold(title);
            TABLETYPE type = tableTypes.at(idx);
            if(tableTypes.contains(type)){
                out.append(tableProcesser[type]->process(root, path));
            }
        }
        return out;
    }

    void clean(){
        sls.clear();
        titles.clear();
        tableTypes.clear();
    }

private:
    QList<QStringList> sls;
    QList<QString> titles;
    QList<TABLETYPE> tableTypes;
    QHash<TABLETYPE, IDoTable*> tableProcesser;
    QDomNode root;
};

static QString doTableV2(QDomNode& root){
    WholeNetEnergyConsumptionPerArea = 0.0;
    TableDataList tables(root);
    tables.append(QStringList()<<"BuildingInfo", "Building Info", TableDataList::TABLE);
    tables.append(QStringList()<<"EnvelopeSystem"<<"BuildingOTTVReport", insertSpaceInTag("BuildingOTTVReport"), TableDataList::TABLE);
    //tables.append(QStringList()<<"EnvelopeSystem"<<"BuildingOTTVReport", "Building Info V", TableDataList::TABLE);
    tables.append(QStringList()<<"EnvelopeSystem" << "BuildingOTTVwall" << "TotalWallOTTVReport", insertSpaceInTag("TotalWallOTTVReport"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"EnvelopeSystem" << "BuildingOTTVwall" << "WallOTTVBySection" << "WallOTTVSection", insertSpaceInTag("WallOTTVSection"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"EnvelopeSystem" << "BuildingOTTVwall" << "OpaqueComponentWall" << "OpaqueComponentByWall", insertSpaceInTag("OpaqueComponentByWall"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"EnvelopeSystem" << "BuildingOTTVwall" << "TransparentComponentWall" << "TransparentComponentByWall", insertSpaceInTag("TransparentComponentByWall"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"EnvelopeSystem" << "BuildingOTTVwall" << "ComponentAreaWall" << "ComponentAreaPerWall", insertSpaceInTag("ComponentAreaPerWall"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"LightingSystem" << "LightingSystemPerformance", insertSpaceInTag("LightingSystemPerformance"), TableDataList::TABLE);
    tables.append(QStringList()<<"LightingSystem" << "LightingSystemFloor" << "LightingSystemByFloor", insertSpaceInTag("LightingSystemByFloor"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"LightingSystem" << "LightingSystemZone" << "LightingSystemByZone", insertSpaceInTag("LightingSystemByZone"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"DXAirUnitSystem" << "DXAirUnit", insertSpaceInTag("DXAirUnit"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"CentralACSystem" << "WaterChilledReport", insertSpaceInTag("WaterChillerReport"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"CentralACSystem" << "CentralACOtherEQReport", insertSpaceInTag("CentralACOtherEQReport"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"CentralACSystem" << "CentralACEQList", insertSpaceInTag("CentralACEQList"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"PVSystem" << "PVSys", insertSpaceInTag("PVSystem"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"WholeBuildingEnergy"<<"WholeBuildingEnergyReport", insertSpaceInTag("WholeBuildingEnergyReport"), TableDataList::TABLE);
    tables.append(QStringList()<<"WholeBuildingEnergy" << "WholeBuildingEnergyReportFloor" << "WholeBuildingEnergyReportByFloor", insertSpaceInTag("WholeBuildingEnergyReportByFloor"), TableDataList::ARRAYTABLE);
    tables.append(QStringList()<<"WholeBuildingEnergy" << "WholeBuildingEnergyReportZone" << "WholeBuildingEnergyReportByZone", insertSpaceInTag("WholeBuildingEnergyReportByZone"), TableDataList::ARRAYTABLE);
    return tables.toString();
}

void doTable(const QString &title, QDomNode& root, QFile& file, int level){

    QHash<QString, bool> h1;
    h1["WholeBuildingEnergy"] = true;

    //if(level == 7)return;
    QDomNode node = root.firstChild();
    QDomElement elm = node.toElement();
    QDomElement fe = elm.firstChildElement();

    if(escapeTitle == title && !escapeTitle.isEmpty())
        return;
    else
        escapeTitle.clear();

    if(tableNames.contains(title)){
        int mylevel=0;
        QString table = doHorizontalTable(root, node, mylevel);
        file.write(table.toStdString().c_str());
        escapeTitle = title;
        return;
    }

    if(fe.isNull()){
        QString out = ParagraphBold(insertSpaceInTag(title));
        file.write(out.toStdString().c_str());
    }
    else{
        int ihn = fmin(level+1, 3);
        QString out = hn(ihn, insertSpaceInTag(title));
        file.write(out.toStdString().c_str());
    }

    //qDebug() << "------------------\n" << title << ":" << root.toElement().tagName() << ", " << level;

    while(!node.isNull()) {
        elm = node.toElement();
        { //AUTOMATIC GENERATE TABLE IN OTHER IS HARDCODE.
            fe = elm.firstChildElement();
            QDomElement fenx = elm.nextSibling().firstChildElement();

            if(fe.isNull()){
                if(level == 0){
                    QString table = ParagraphBold(insertSpaceInTag(elm.tagName()));
                    table +=    "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">"
                                "<tbody>"
                                "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>"
                                "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>"
                                "</tbody>"
                                "</table>";
                    file.write(table.toStdString().c_str());
                }
                else if(fenx.tagName() != fe.tagName() && fe.tagName().isNull()){
                    QString table = ParagraphBold(insertSpaceInTag(elm.tagName()));
                    file.write(table.toStdString().c_str());
                    escapeTitle = title;
                }
                else{
                    int mylevel=0;
                    //QString table = Bold(insertSpaceInTag(elm.tagName()));
					QString table = doHorizontalTable(root, node, mylevel);
                    file.write(table.toStdString().c_str());
                    escapeTitle = title;
                    return;
                }
            }
            else{
                doTable(elm.tagName(), elm, file, level+1);
            }
        }
        if(!node.isNull())
            node = node.nextSibling();
        else
            break;
    }
}

static openstudio::path binResourcesPath()
{
    if (openstudio::applicationIsRunningFromBuildDirectory())
    {
      return openstudio::getApplicationSourceDirectory() / openstudio::toPath("src/openstudio_app/Resources");
    }
    else
    {
      return openstudio::getApplicationDirectory() / openstudio::toPath("../Resources");
    }
}

static QDomElement createTagWithText(QDomDocument* doc, QDomElement &parent, const QString &tag, const QString &text)
{
    QDomElement elm = doc->createElement(tag);
    if(!text.isEmpty())
        elm.appendChild(doc->createTextNode(text.toUtf8()));

    parent.appendChild(elm);
    return elm;
}

static bool doBecReport(const QString &path, QString& outpath, QString &err, float wwr_total){
    QDomDocument doc("becreport");

    QFileInfo fi(path);
    QString output = fi.absolutePath()+"/report.html";
    outpath = output;
    QFile file(output);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(
"<meta charset=\"UTF-8\">"
"<html>\n"
"<head>\n"
"<title>BEC Report</title>\n"
"</head>\n"
"<body>\n"
"<style>\n\
table {\n\
border-width: 1px;\n\
border-spacing: 0px;\n\
border-style: solid;\n\
border-color: green;\n\
border-collapse: separate;\n\
background-color: white;\n\
}\n\
h1 {\n\
background-color: #fbd4b3;\n\
}\n\
h2 {\n\
background-color: #fbd4b3;\n\
}\n\
h3 {\n\
background-color: #fbd4b3;\n\
}\n\
h4 {\n\
background-color: #fbd4b3;\n\
}\n\
p {\n\
background-color: #fbd4b3;\n\
}\n\
table th {\n\
border-width: 1px;\n\
padding: 1px;\n\
border-style: solid;\n\
border-color: black;\n\
background-color: white;\n\
-moz-border-radius: ;\n\
}\n\
\n\
table tr:first-child td {\n\
background-color: #b8dce8;\n\
text-align: center;\n\
}\n\
\n\
table td {\n\
border-width: 1px;\n\
padding: 1px;\n\
border-style: solid;\n\
border-color: black;\n\
-moz-border-radius: ;\n\
}\n\
\n\
table tr:nth-child(even){\n\
background-color: #ddd8c4;\n\
}\n\
\n\
table tr:hover {\n\
background-color: #ffff99;\n\
}\n\
</style>\n");

    }
    else{
        err = "Can't create report name " +output;
        return false;
    }

    QFile xmlfile(path);
    if (!xmlfile.open(QIODevice::ReadOnly))
        return false;
    if (!doc.setContent(&xmlfile)) {
        xmlfile.close();
        err = "Can't set content xml file.";
        return false;
    }
    xmlfile.close();

    QDomElement docElem = doc.documentElement();
    //doTable(docElem.tagName(), docElem, file, 0);

    QString tables = doTableV2(docElem);
    file.write(tables.toUtf8());

    //BENCHMARK
    QString pass = "Failed";
    if(WholeNetEnergyConsumptionPerArea<bvVal){
        pass = "Passed";
    }

    QString bvTable = QString("<p><b>Analysis result</b></p>\n"
                            "<table id=\"%1\" border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n"
                            "  <tbody>\n"
                            "  <tr>\n"
                            "    <td align=\"center\" valign=\"top\"></td>\n"
                            "    <td align=\"center\" valign=\"top\">Type</td>\n"
                            "    <td align=\"center\" valign=\"top\">Ref Sector/Floor Area[kWh/m2]<sup>2</sup>]</td>\n"
                            "    <td align=\"center\" valign=\"top\">Result[kWh/m<sup>2</sup>]</td>\n"
                            "  <tr>\n"
                            "    <td align=\"right\" valign=\"top\">Benchmark</td>\n"
                            "    <td align=\"right\" valign=\"top\">%2</td>\n"
                            "    <td align=\"right\" valign=\"top\">%3</td>\n"
                            "    <td align=\"right\" valign=\"top\">%4</td>\n"
                            "  </tr>\n"
                            "</tbody></table><br><br>\n</body>")
            .arg("bv_table")
            .arg(bvName)
            .arg(doubleToMoney(bvVal))
            .arg(doubleToMoney(WholeNetEnergyConsumptionPerArea))
            .arg(pass);

    file.write(bvTable.toUtf8());

    QDate date = QDateTime::currentDateTime().date();
    int cryear, month, day;
    date.getDate(&cryear, &month, &day);
    cryear += 543;
    QLocale locale  = QLocale(QLocale::Thai, QLocale::Thailand); // set the locale you want here
    QString thaidate = locale.toString(date, "d MMMM");

    QString sign = QString("<br><div style=\"text-align: center;padding-bottom: 50px;padding-right: 50px;float: right;\"><br>\n"
                         "----- ----- ----- ----- ----- ----- ----- ----- -----<br><br>\n"
                         "(..... ..... ..... ..... ..... ..... ..... ..... ..... ..... .....)<br>\n"
                         "ผู้รับรองการประเมิน<br>\n"
                         "%1 %2\n"
                         "</div>\n").arg(thaidate).arg(cryear);
    file.write(sign.toUtf8());
    file.write("</body>\n"
               "</html>\n");

    file.close();
    return true;
}
////////////////////////////////////////////////////////////////////////////////

namespace openstudio {

RunTabView::RunTabView(const model::Model & model,
  QWidget * parent)
  : MainTabView("Run Simulation", MainTabView::MAIN_TAB, parent),
    m_runView(new RunView())
{
  addTabWidget(m_runView);
}

RunView::RunView()
  : QWidget(), m_runSocket(nullptr)
{
  auto mainLayout = new QGridLayout();
  mainLayout->setContentsMargins(10,10,10,10);
  mainLayout->setSpacing(5);
  setLayout(mainLayout);

  m_playButton = new QToolButton();
  m_playButton->setText("Run");
  m_playButton->setCheckable(true);
  m_playButton->setChecked(false);
  QIcon playbuttonicon(QPixmap(":/images/run_simulation_button.png"));
  playbuttonicon.addPixmap(QPixmap(":/images/run_simulation_button.png"), QIcon::Normal, QIcon::Off);
  playbuttonicon.addPixmap(QPixmap(":/images/cancel_simulation_button.png"), QIcon::Normal, QIcon::On);
  m_playButton->setStyleSheet("QToolButton { background:transparent; font: bold; }");
  m_playButton->setIconSize(QSize(35,35));
  m_playButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_playButton->setIcon(playbuttonicon);
  m_playButton->setLayoutDirection(Qt::RightToLeft);
  m_playButton->setStyleSheet("QAbstractButton:!hover { border: none; }");
  
  mainLayout->addWidget(m_playButton, 0, 0);
  connect(m_playButton, &QToolButton::clicked, this, &RunView::playButtonClicked);
  
  // Progress bar area
  m_progressBar = new QProgressBar();
  m_progressBar->setMaximum(State::complete);
  
  auto progressbarlayout = new QVBoxLayout();
  progressbarlayout->addWidget(m_progressBar);
  mainLayout->addLayout(progressbarlayout, 0, 1);

  m_runMode = new QComboBox();
  mainLayout->addWidget(m_runMode, 0, 2);
  m_runMode->addItem(STR_Mode_EPLUS);
  m_runMode->addItem(STR_Mode_BEC);
  m_runMode->addItem(STR_Mode_EPLUS_BEC);

  m_openSimDirButton = new QPushButton();
  m_openSimDirButton->setText("Show Simulation");
  m_openSimDirButton->setFlat(true);
  m_openSimDirButton->setObjectName("StandardGrayButton");
  connect(m_openSimDirButton, &QPushButton::clicked, this, &RunView::onOpenSimDirClicked);
  mainLayout->addWidget(m_openSimDirButton,0, 3);

  m_textInfo = new QTextEdit();
  m_textInfo->setReadOnly(true);
  mainLayout->addWidget(m_textInfo,1,0,1,3);

  m_runProcess = new QProcess(this);
  connect(m_runProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &RunView::onRunProcessFinished);

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

  auto energyPlusExePath = getEnergyPlusExecutable();
  if (!energyPlusExePath.empty()){
    env.insert("ENERGYPLUS_EXE_PATH", toQString(energyPlusExePath));
  }

  auto radianceDirectory = getRadianceDirectory();
  if (!radianceDirectory.empty()){
    env.insert("OS_RAYPATH", toQString(radianceDirectory));
  }

  auto perlExecutablePath = getPerlExecutable();
  if (!perlExecutablePath.empty()){
    env.insert("PERL_EXE_PATH", toQString(perlExecutablePath));
  }

  m_runProcess->setProcessEnvironment(env);

  m_runTcpServer = new QTcpServer();
  m_runTcpServer->listen();
  connect(m_runTcpServer, &QTcpServer::newConnection, this, &RunView::onNewConnection);
}

void RunView::onOpenSimDirClicked()
{
  std::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();
  QString url = QString::fromStdString((resourcePath(toPath(osdocument->savePath())) / "run").string());
  QUrl qurl = QUrl::fromLocalFile(url);
  if( ! QDesktopServices::openUrl(qurl) ) {
    QMessageBox::critical(this, "Unable to open simulation", "Please save the OpenStudio Model to view the simulation.");
  }
}

void RunView::doFinish()
{
    LOG(Debug, "run finished");
    m_playButton->setChecked(false);
    m_state = State::stopped;
    m_progressBar->setValue(State::complete);

    std::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();
    osdocument->save();
    osdocument->enableTabsAfterRun();
    m_openSimDirButton->setEnabled(true);

    if (m_runSocket){
      delete m_runSocket;
    }
    m_runSocket = nullptr;
}

void RunView::onRunProcessFinished(int exitCode, QProcess::ExitStatus status)
{
  if(runmode == RUN_ENERGY_BEC)
  {
    runmode = RUN_BEC;
    playButtonClicked00(true, runmode);
  }
  else if(runmode == RUN_BEC){
    becFinished();
    doFinish();
  }
  else{
      doFinish();
  }
}

openstudio::path RunView::resourcePath(const openstudio::path & osmPath) const
{
  auto baseName = osmPath.stem();
  auto parentPath = osmPath.parent_path();
  auto resourcePath = parentPath / baseName;
  return resourcePath; 
}

void RunView::playButtonClicked00(bool t_checked, RunView::RUNMODE runmode)
{
    std::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();

    if (t_checked) {
      // run

      if(osdocument->modified())
      {
        osdocument->save();
        // save dialog was canceled
        if(osdocument->modified()) {
          m_playButton->setChecked(false);
          return;
        }
      }

      QStringList paths;
      paths << QCoreApplication::applicationDirPath();
      auto openstudioExePath = QStandardPaths::findExecutable("openstudio", paths);

      // run in save dir
      //auto basePath = resourcePath(toPath(osdocument->savePath()));

      // run in temp dir
      auto basePath = toPath(osdocument->modelTempDir()) / toPath("resources");

      auto workflowPath = basePath / "workflow.osw";
      auto stdoutPath = basePath / "stdout";
      auto stderrPath = basePath / "stderr";

      OS_ASSERT(exists(workflowPath));

      auto workflowJSONPath = QString::fromStdString(workflowPath.string());

      osdocument->disableTabsDuringRun();
      m_openSimDirButton->setEnabled(false);

      if (exists(stdoutPath)){
        remove(stdoutPath);
      }
      if (exists(stderrPath)){
        remove(stderrPath);
      }

      m_progressBar->setValue(0);
      m_state = State::stopped;
      m_textInfo->clear();
      m_runProcess->setStandardOutputFile( toQString(stdoutPath) );
      m_runProcess->setStandardErrorFile( toQString(stderrPath) );

      if(runmode == RUN_ENERGY || runmode == RUN_ENERGY_BEC){
          QStringList arguments;
          arguments << "run" << "-s" << QString::number(m_runTcpServer->serverPort()) << "-w" << workflowJSONPath;

          LOG(Debug, "run exe" << openstudioExePath.toStdString());
          LOG(Debug, "run arguments" << arguments.join(";").toStdString());
          m_runProcess->start(openstudioExePath, arguments);
      }
      else
      {
          //TODO:RUN bec here.

          LOG(Debug, "RUN BEC");
          logH1Text("RUN BEC");

          m_tempFolder = toPath(osdocument->modelTempDir());
          QString outpath = (m_tempFolder/"resources").string().c_str();
          if(!outpath.isEmpty()){

              //GEN INPUT
              QString filePath;
              QString err;
              outpath += "/run/9-BEC-0/";
              logNormalText(QString("Temp output : '%1'").arg(outpath));

              QDir dir(outpath);
              if (!dir.exists()) {
                  dir.mkpath(".");
              }

              //////////////////////////
              QFile file(outpath+"../5-EnergyPlus-0/eplustbl.htm");
              sunlits.clear();
              wwr_totoal = 0.0f;
              if (file.open(QIODevice::ReadOnly | QIODevice::Text))
              {
                  logH2Text(QString("Start read eblustbl.htm<"));
                  QTextStream in(&file);
                  QString str = in.readAll();
                  QString gwstr = str;
                  int start = str.indexOf("<b>Sunlit Fraction</b>");
                  int table_start_idx = str.indexOf("<table", start);
                  int table_end_idx = str.indexOf("</table>", table_start_idx+6);

                  logNormalText(QString("table sidx:%1->%2:%3").arg(start).arg(table_start_idx).arg(table_end_idx));

                  str = str.mid(table_start_idx, table_end_idx-table_start_idx);
                  str = str.replace("\r", "");
                  str = str.replace("\n", "");
                  str = str.replace(QRegExp("<table[^>]*>"), "");
                  str = str.replace("</table>", "");
                  str = str.replace(QRegExp("<td[^>]*>"), "");
                  str = str.replace(QRegExp("<tr[^>]*>"), "");

                  QStringList rowls = str.split("</tr>");

                  logH2Text("Sunlit Fraction");
                  for (int irow = 1; irow < rowls.size(); ++irow) {
                      QStringList cols = rowls.at(irow).split("</td>");

                      if(cols.size()<1)
                          break;

                      QString sfname = cols.at(0);
                      QString msg = sfname+":";
                      for (int icol = 1; icol < cols.size(); ++icol) {
                          double num = cols.at(icol).trimmed().toDouble();
                          sunlits[sfname.toUpper().trimmed()].append(num);
                          msg += QString::number(num)+",";
                      }
                      logNormalText(QString("%1\n").arg(msg));
                  }
                  logH2Text(QString("SF:%1").arg(QString::number(sunlits.size())));

                  //Read Conditioned Window-Wall Ratio --> Gross Window-Wall Ratio [%] --> <td align="right">       35.20</td>
                  int idx_of_wwrAll = gwstr.indexOf("Conditioned Window-Wall Ratio");
                  int idx_of_wwrAll_end = -1;
                  if (idx_of_wwrAll > -1){
                      idx_of_wwrAll = gwstr.indexOf("Gross Window-Wall Ratio [%]", idx_of_wwrAll);
                      const QString tdBegin("<td align=\"right\"> ");
                      const QString tdEnd("</td>");
                      idx_of_wwrAll = gwstr.indexOf(tdBegin, idx_of_wwrAll);
                      if (idx_of_wwrAll > -1){
                          idx_of_wwrAll += tdBegin.length();
                          idx_of_wwrAll_end = gwstr.indexOf(tdEnd, idx_of_wwrAll);
                          logNormalText(QString("idx0:%1, idx1:%2").arg(idx_of_wwrAll).arg(idx_of_wwrAll_end));
                          if (idx_of_wwrAll_end > -1){
                              gwstr = gwstr.mid(idx_of_wwrAll, idx_of_wwrAll_end - idx_of_wwrAll);
                              logNormalText(QString("gwstr:[%1]").arg(gwstr));
                              gwstr = gwstr.trimmed();
                              wwr_totoal = gwstr.toFloat();
                              wwr_totoal = wwr_totoal/100.0f;
                              logH2Text(QString("wwr_totoal:[%1]").arg(wwr_totoal));
                              logH2Text(QString("wwr totoal:%1").arg(gwstr));
                          }
                      }
                  }
              }else{
                  //logErrorText(QString("<font color=\"red\">ERROR:Can't read eblustbl.htm</font>"));
                  logErrorText(QString("ERROR:Can't read eblustbl.htm"));
              }
              ///////////////////////////

              becoutputPath = outpath+"output.xml";

              bool success = doBecInput(outpath+"input.xml", osdocument->model(), filePath, err);

              if(!err.isEmpty())
                  logErrorText(err);

              if(!success){
                  osdocument->enableTabsAfterRun();
                  onRunProcessFinished(1, QProcess::NormalExit);

                  updatePVInfile();
                  //m_canceling = false;
                  logErrorText(QString("Can't generate bec input files"));
                  return;
              }
              else
              {
                  logNormalText("Call newBEC.exe.");
                  callRealBEC(outpath);
              }
          }
      }

    } else {
      // stop running
      LOG(Debug, "Kill Simulation");
      m_runProcess->kill();
    }
}

void RunView::becFinished()
{
    logH2Text("BEC Finished.");
    logNormalText("Generate Report.");
    QString outpath, err;

    if(!doBecReport(becoutputPath, outpath, err, wwr_totoal)){
        logErrorText("Error BEC Report.");
        logErrorText(err);
    }
    else{
        //std::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();
        logH1Text(QString("Generate bec report %1 complete.").arg(outpath));
        m_playButton->setChecked(false);
        updatePVInfile();
    }

    if(m_progressBar->value()!=50)
        m_progressBar->setValue(100);
}

void RunView::playButtonClicked(bool t_checked)
{
    LOG(Debug, "playButtonClicked " << t_checked);

    if(m_runMode->currentText() == STR_Mode_EPLUS)
        runmode = RUN_ENERGY;
    else if(m_runMode->currentText() == STR_Mode_BEC)
        runmode = RUN_BEC;
    else
        runmode = RUN_ENERGY_BEC;

    playButtonClicked00(t_checked, runmode);

}

void RunView::onNewConnection()
{
    m_runSocket = m_runTcpServer->nextPendingConnection();
    connect(m_runSocket,&QTcpSocket::readyRead,this,&RunView::onRunDataReady);
}

void RunView::onRunDataReady()
{
    auto appendErrorText = [&](const QString & text) {
        m_textInfo->setTextColor(Qt::red);
        m_textInfo->setFontPointSize(18);
        m_textInfo->append(text);
    };

    auto appendNormalText = [&](const QString & text) {
        m_textInfo->setTextColor(Qt::black);
        m_textInfo->setFontPointSize(12);
        m_textInfo->append(text);
    };

    auto appendH1Text = [&](const QString & text) {
    m_textInfo->setTextColor(Qt::black);
    m_textInfo->setFontPointSize(18);
    m_textInfo->append(text);
  };

  auto appendH2Text = [&](const QString & text) {
    m_textInfo->setTextColor(Qt::black);
    m_textInfo->setFontPointSize(15);
    m_textInfo->append(text);
  };

  QString data = m_runSocket->readAll();
  QStringList lines = data.split("\n");

  for (const auto& line: lines){
    //std::cout << data.toStdString() << std::endl;

    QString trimmedLine = line.trimmed();

    // DLM: coordinate with openstudio-workflow-gem\lib\openstudio\workflow\adapters\output\socket.rb
    if (trimmedLine.isEmpty()){
      continue;
    } else if (QString::compare(trimmedLine, "Starting state initialization", Qt::CaseInsensitive) == 0) {
      appendH1Text("Initializing workflow.");
      m_state = State::initialization;
      m_progressBar->setValue(m_state);
    } else if (QString::compare(trimmedLine, "Started", Qt::CaseInsensitive) == 0) {
      // no-op
    } else if (QString::compare(trimmedLine, "Returned from state initialization", Qt::CaseInsensitive) == 0) {
      // no-op
    } else if (QString::compare(trimmedLine, "Starting state os_measures", Qt::CaseInsensitive) == 0) {
      appendH1Text("Processing OpenStudio Measures.");
      m_state = State::os_measures;
      m_progressBar->setValue(m_state);
    } else if (QString::compare(trimmedLine, "Returned from state os_measures", Qt::CaseInsensitive) == 0) {
      // no-op
    } else if (QString::compare(trimmedLine, "Starting state translator", Qt::CaseInsensitive) == 0) {
      appendH1Text("Translating the OpenStudio Model to EnergyPlus.");
      m_state = State::translator;
      m_progressBar->setValue(m_state);
    } else if (QString::compare(trimmedLine, "Returned from state translator", Qt::CaseInsensitive) == 0) {
      // no-op
    } else if (QString::compare(trimmedLine, "Starting state ep_measures", Qt::CaseInsensitive) == 0) {
      appendH1Text("Processing EnergyPlus Measures.");
      m_state = State::ep_measures;
      m_progressBar->setValue(m_state);
    } else if (QString::compare(trimmedLine, "Returned from state ep_measures", Qt::CaseInsensitive) == 0) {
      // no-op
    } else if (QString::compare(trimmedLine, "Starting state preprocess", Qt::CaseInsensitive) == 0) {
      // ignore this state
      m_state = State::preprocess;
      m_progressBar->setValue(m_state);
    } else if (QString::compare(trimmedLine, "Returned from state preprocess", Qt::CaseInsensitive) == 0) {
      // ignore this state
    } else if (QString::compare(trimmedLine, "Starting state simulation", Qt::CaseInsensitive) == 0) {
      appendH1Text("Starting Simulation.");
      m_state = State::simulation;
      m_progressBar->setValue(m_state);
    } else if (QString::compare(trimmedLine, "Returned from state simulation", Qt::CaseInsensitive) == 0) {
      // no-op
    } else if (QString::compare(trimmedLine, "Starting state reporting_measures", Qt::CaseInsensitive) == 0) {
      appendH1Text("Processing Reporting Measures.");
      m_state = State::reporting_measures;
      m_progressBar->setValue(m_state);
    } else if (QString::compare(trimmedLine, "Returned from state reporting_measures", Qt::CaseInsensitive) == 0) {
      // no-op
    } else if (QString::compare(trimmedLine, "Starting state postprocess", Qt::CaseInsensitive) == 0) {
      appendH1Text("Gathering Reports.");
      m_state = State::postprocess;
      m_progressBar->setValue(m_state);
    } else if (QString::compare(trimmedLine, "Returned from state postprocess", Qt::CaseInsensitive) == 0) {
      // no-op
    } else if (QString::compare(trimmedLine, "Failure", Qt::CaseInsensitive) == 0) {
      appendErrorText("Failed.");
    } else if (QString::compare(trimmedLine, "Complete", Qt::CaseInsensitive) == 0) {
      appendH1Text("Completed.");
    } else if (trimmedLine.startsWith("Applying", Qt::CaseInsensitive)) {
      appendH2Text(line);
    } else if (trimmedLine.startsWith("Applied", Qt::CaseInsensitive)) {
      // no-op
    } else{
      appendNormalText(line);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NOTE: BEC CODE
bool RunView::doBecInput(const QString &path, const model::Model &model, QString &outpath, QString &err){
    QString output = path;

    outpath = output;

    bec::ForwardTranslator trans;
    logNormalText(QString("Create input.xml at %1").arg(path));

    std::string bvn;
    bool success = trans.modelTobec(model, path.toStdString().c_str(), NULL, &bvn, &sunlits, wwr_totoal);
    bvName = bvn.c_str();

    std::string bvsdefault = binResourcesPath().string() + "/" + "default_building_standard.bvs";
    BenchmarkDialog* bmdlg = new BenchmarkDialog(bvsdefault.c_str(), this);
    bvVal = bmdlg->getValueByName(bvName);
    bmdlg->accept();

    std::vector<LogMessage> translatorErrors = trans.errors();
    //std::vector<LogMessage> translatorWarnings = trans.warnings();

    for( std::vector<LogMessage>::iterator it = translatorErrors.begin();
         it < translatorErrors.end();
         ++it )
    {
        err.append(QString::fromStdString(it->logMessage()));
        err.append("\n");
    }
    //translatorWarnings = trans.warnings();

    logNormalText(QString("Create input.xml at %1 success").arg(path));
    return success;
}

double RunView::getPV(openstudio::model::Model* model)
{
    double res = 0.0;
    std::vector<model::Photovoltaic> pvs = model->getModelObjects<model::Photovoltaic>();
    for (model::Photovoltaic& pv : pvs){
        res += pv.calculatePV();
    }

    std::vector<model::PhotovoltaicThermal> pvst = model->getModelObjects<model::PhotovoltaicThermal>();
    for (model::PhotovoltaicThermal& pv : pvst){
        res += pv.calculatePV();
    }
    return res;
}

static double sumArrayStringOfDouble(const QString& arrayStr, int begin){
    //qDebug() << QStringRef(&arrayStr, begin, 5).toString();
    QString res;
    double out=0;
    for(int i=begin;i<arrayStr.size();i++){
        QChar ch = arrayStr.at(i);
        if(ch.isNumber()){
            res.append(ch);
        }
        else if(ch == '.'){
            res.append(ch);
        }
        else if(ch==','){
            out += res.toDouble();
            //qDebug() << "in out sum: " << out << ", res:" <<res ;
            res.clear();
        }
        else if(ch==']'){
            out += res.toDouble();
            //qDebug() << "in out sum: " << out << ", res:" <<res ;
            res.clear();
            break;
        }
    }
    return out;
}

static double getDouble(const QString& str){
    QString res;
    for(int i=0;i<str.size();i++){
        QChar ch = str.at(i);
        if(ch.isNumber()){
            res.append(ch);
        }
        else if(ch == '.'){
            res.append(ch);
        }
    }
    bool isOk;
    double out = res.toDouble(&isOk);
    if(isOk)
        return out;
    else
        return 0;
}

static double findEnergyPlusPowerTotalkWh(const QString& str){
    const QString key1 = "<b>Site and Source Energy</b><br><br>";
    const QString key2 = "<td align=\"right\">Total Source Energy</td>";
    const QString tdend = "</td>";

    //qDebug() << "==============\n" << str;
    //qDebug() << "++++++++++++++\n" << key1;

    int idx = str.indexOf(key1);
    if(idx<0)
        return 0;

    idx = str.indexOf(key2, idx+key1.size());
    if(idx<0)
        return 0;

    int col2Begin = str.indexOf(tdend, idx+key2.size());
    if(col2Begin<0)
        return 0;

    int col2End = str.indexOf(tdend, col2Begin+tdend.size());
    if(col2End<0)
        return 0;

    QStringRef sub(&str, col2Begin, col2End-col2Begin);
    return getDouble(sub.toString())/3.6f;
}

static double findOpenStudioPowerTotal(const QString& str){
    const QString key0 = "\"Electricity Consumption\":{";

    QStringList sls;
    sls.push_back("\"Heating\":[");
    sls.push_back("\"Cooling\":[");
    sls.push_back("\"Interior Lighting\":[");
    sls.push_back("\"Exterior Lighting\":[");
    sls.push_back("\"Interior Equipment\":[");
    sls.push_back("\"Exterior Equipment\":[");
    sls.push_back("\"Fans\":[");
    sls.push_back("\"Pumps\":[");
    sls.push_back("\"Heat Rejection\":[");
    sls.push_back("\"Humidification\":[");
    sls.push_back("\"Heat Recovery\":[");
    sls.push_back("\"Water Systems\":[");
    sls.push_back("\"Refrigeration\":[");
    sls.push_back("\"Generators\":[");

    double out = 0;
    int begin = str.indexOf(key0);
    if(begin<0)
        return 0;

    begin = begin+key0.size();
    begin = str.indexOf(sls.at(0), begin);
    if(begin<0)
        return out;
    else
        out += sumArrayStringOfDouble(str, begin+sls.at(0).size());

    for(int i=1;i<sls.size();i++){
        begin = begin+sls.at(i-1).size();
        begin = str.indexOf(sls.at(i), begin);
        if(begin<0)
            return out;
        else{
            //qDebug() << QStringRef(&str, begin, 5).toString();
            out += sumArrayStringOfDouble(str, begin+sls.at(i).size());
            //qDebug() << "CURRENT OUT :" << out;
        }
    }
    return out;
}

void RunView::addPVAndBenchmarkToFile(const QString &fileName, int mode)
{
    double pv = lastPV;
    static QString pvid = "_Z_O_axz1d0_j_i_";
    static QString bvid = "_A_w08_B_p3_O_vv";

    QString fn = fileName;
    fn.replace("file:///", "");

    QByteArray fileData;
    QFile file(fn);

    if(!file.open(QIODevice::ReadWrite)){
        file.close();
        return;
    }
    fileData = file.readAll();
    QString text(fileData);

    bool firstPV = text.lastIndexOf(pvid)<0;

    switch (mode) {
    case PVReportMode_OPENSTUDIO:
    {
        const QString css_style = "\n\
table {\n\
border-width: 1px;\n\
border-spacing: 0px;\n\
border-style: solid;\n\
border-color: green;\n\
border-collapse: separate;\n\
background-color: white;\n\
}\n\
table tr:first-child td {\n\
background-color: #b8dce8;\n\
text-align: center;\n\
}\n\
table tr:last-child td {\n\
background-color: #91cf50;\n\
text-align: center;\n\
}\n\
table th {\n\
border-width: 1px;\n\
padding: 1px;\n\
border-style: solid;\n\
border-color: green;\n\
background-color: white;\n\
-moz-border-radius: ;\n\
}\n\
\n\
table th {\n\
background-color: #ffcc97;\n\
text-align: center;\n\
}\n\
\n\
table td {\n\
border-width: 1px;\n\
padding: 1px;\n\
border-style: solid;\n\
border-color: black;\n\
-moz-border-radius: ;\n\
}\n\
\n\
table tr:nth-child(even){\n\
background-color: #ddd8c4;\n\
}\n\
\n\
table tr:hover {\n\
background-color: #ffff99;\n\
}\n\
";
        double val = findOpenStudioPowerTotal(text);
        logNormalText(QString("Power Total:%1, buildingArea:%2").arg(val).arg(buildingArea));
        val = val/buildingArea;
        logNormalText(QString("Power Total/buildingArea=%1").arg(val));
        //PV
        QString table = QString("<h4>Photovoltaic(watt)</h4>\n"
                                "<table id=\"%1\">\n"
                                "	<thead>\n"
                                "		<tr>\n"
                                "			<th>&nbsp;</th>\n"
                                "			<th>watt</th>\n"
                                "		</tr>\n"
                                "	</thead>\n"
                                "	<tbody>\n"
                                "		<tr>\n"
                                "			<td>Photovoltaic</td>\n"
                                "			<td>%2</td>\n"
                                "		</tr>\n"
                                "	</tbody>\n"
                                "</table>\n"
                                "</body>").arg(pvid).arg(QString::number(pv, 'f', 2));
        if(firstPV){
            text.replace(QRegularExpression("body\\s+{\\s+font: 10px sans-serif;\\s+min-width: 1280px;\\s+}"), "");
            text.replace(QRegularExpression("table\\s+{\\s+max-width:800px;\\s+}"), css_style);
            text.replace("table table-striped table-bordered table-condensed", "");
            text.replace("</body>", table);
        }
        else{
            int start = text.lastIndexOf("<h4>Photovoltaic(watt)</h4>\n");
            int count = text.size()-start;
            text.replace(start, count+1, table);
            text.append("\n</html>\n");
        }
        ///////////////////////////////
        //BENCHMARK
        QString pass = "Failed";
        if(val<bvVal){
            pass = "Passed";
        }
        //TODO: CHANGE Value to Project name.
        //TODO: Add <sup>2</sup> to kwh

        std::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();
        QString savePath = osdocument->savePath();
        QFileInfo savePathFile(savePath);
        table = QString("<h4>Benchmark</h4>\n"
                                "<table id=\"%1\">\n"
                                "	<thead>\n"
                                "		<tr>\n"
                                "			<th></th>\n"
                                "			<th>%2</th>\n"
                                "		</tr>\n"
                                "	</thead>\n"
                                "	<tbody>\n"
                                "		<tr>\n"
                                "			<td>Type</td>\n"
                                "			<td>%3</td>\n"
                                "		</tr>\n"
                                "		<tr>\n"
                                "			<td>Standard(kWh/m<sup>2</sup>)</td>\n"
                                "			<td>%4</td>\n"
                                "		</tr>\n"
                                "		<tr>\n"
                                "			<td>Result(kWh/m<sup>2</sup>)</td>\n"
                                "			<td>%5</td>\n"
                                "		</tr>\n"
                                "		<tr>\n"
                                "			<td>Status</td>\n"
                                "			<td>%6</td>\n"
                                "		</tr>\n"
                                "	</tbody>\n"
                                "</table>\n"
                                "</body>")
                .arg(bvid)
                .arg(savePathFile.baseName())
                .arg(bvName)
                .arg(doubleToMoney(bvVal))
                .arg(doubleToMoney(val))
                .arg(pass);

        text.replace("</body>", table);
    }
        break;
    case PVReportMode_ENERGYPLUS:
    {
        const QString css_style = "\n\
table {\n\
border-width: 1px;\n\
border-spacing: 0px;\n\
border-style: solid;\n\
border-color: black;\n\
border-collapse: separate;\n\
background-color: white;\n\
}\n\
table th {\n\
border-width: 1px;\n\
padding: 1px;\n\
border-style: solid;\n\
border-color: black;\n\
background-color: white;\n\
-moz-border-radius: ;\n\
}\n\
\n\
table th {\n\
background-color: #ffcc97;\n\
text-align: center;\n\
}\n\
table tr:first-child td {\n\
background-color: #b8dce8;\n\
text-align: center;\n\
}\n\
table td {\n\
border-width: 1px;\n\
padding: 1px;\n\
border-style: solid;\n\
border-color: black;\n\
-moz-border-radius: ;\n\
}\n\
\n\
table tr:nth-child(even){\n\
background-color: #ddd8c4;\n\
}\n\
\n\
table tr:hover {\n\
background-color: #ffff99;\n\
}\n\
";
        double val = findEnergyPlusPowerTotalkWh(text);
        if(text.indexOf("<meta charset=\"utf-8\">")<0){
            text.replace("<head>", "<head>\n<meta charset=\"utf-8\">");
        }
        QString table = QString("<b>Photovoltaic</b><br><br>\n"
                                "<table id=\"%1\" border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n"
                                "  <tbody>\n"
                                "  <tr><td></td><td align=\"right\">watt</td></tr>\n"
                                "  <tr>\n"
                                "    <td align=\"right\">Photovoltaic(watt)</td>\n"
                                "    <td align=\"right\">%2</td>\n"
                                "  </tr>\n"
                                "</tbody></table><br><br>\n</body>").arg(pvid).arg(QString::number(pv, 'f', 2));

        if(firstPV){
            text.replace("<html>", QString("<html><style type=\"text/css\">")+css_style+"</style>");
            text.replace("</body>", table);
        }
        else{
            int start = text.lastIndexOf("<b>Photovoltaic</b><br><br>");
            int count = text.size()-start;
            text.replace(start, count+1, table);
            text.append("\n</html>\n");
        }

        ///////////////////////////////
        //BENCHMARK
        QString pass = "Failed";
        if(val<bvVal){
            pass = "Passed";
        }
        table.clear();
        table = QString("<b>Benchmark</b><br><br>\n"
                                "<table id=\"%1\" border=\"1\" cellpadding=\"4\" cellspacing=\"0\">\n"
                                "  <tbody>\n"
                                "  <tr>\n"
                                "    <td align=\"center\"></td>\n"
                                "    <td align=\"center\">Type</td>\n"
                                "    <td align=\"center\">Standard[kWh/m<sup>2</sup>]</td>\n"
                                "    <td align=\"center\">Result[kWh/m<sup>2</sup>]</td>\n"
                                "    <td align=\"center\">Status</td>\n"
                                "  <tr>\n"
                                "    <td align=\"right\">Benchmark</td>\n"
                                "    <td align=\"right\">%2</td>\n"
                                "    <td align=\"right\">%3</td>\n"
                                "    <td align=\"right\">%4</td>\n"
                                "    <td align=\"right\">%5</td>\n"
                                "  </tr>\n"
                                "</tbody></table><br><br>\n</body>")
                .arg(bvid)
                .arg(bvName)
                .arg(doubleToMoney(bvVal))
                .arg(doubleToMoney(val))
                .arg(pass);

        text.replace("</body>", table);
    }
        break;
    }
    file.seek(0);
    file.write(text.toUtf8());
    file.flush();
    file.close();
}

void RunView::updatePVInfile()
{
    QString outpath = (m_tempFolder/"resources").string().c_str();
    QString opsReportPath = outpath + "/run/6-UserScript-0/report.html";
    QString eReportPath = outpath + "/run/5-EnergyPlus-0/eplustbl.htm";
    QString becReportPath = outpath + "/run/9-BEC-0/eplustbl.htm";

    QFileInfo opsCheckFile(opsReportPath);
    if (opsCheckFile.exists() && opsCheckFile.isFile()) {
        logNormalText(QString("Update OpenStudio report pv value is %1 at %2").arg(lastPV).arg(opsReportPath));
        addPVAndBenchmarkToFile(opsReportPath, PVReportMode_OPENSTUDIO);
    }

    QFileInfo eReportFile(eReportPath);
    if (eReportFile.exists() && eReportFile.isFile()) {
        logNormalText(QString("Update EnergyPlus report pv value is %1 at %2").arg(lastPV).arg(eReportPath));
        addPVAndBenchmarkToFile(eReportPath, PVReportMode_ENERGYPLUS);
    }

    QFileInfo becReportFile(becReportPath);
    if (becReportFile.exists() && becReportFile.isFile()) {

    }
}

void RunView::callRealBEC(const QString &dir){
    QString outpath = dir;
    outpath.replace("\\", "/");
    if(!outpath.endsWith("//")){
        outpath.append("/");
    }
    openstudio::path path = binResourcesPath();
    std::string program = path.string() + "/newBEC.exe";

    QStringList arguments;
    arguments << outpath;

    logNormalText(QString("Run bec at:%1").arg(program.c_str()));
    m_runProcess->start(program.c_str(), arguments);

    /*
    if(becProcess)
        becProcess->deleteLater();

    becProcess = new QProcess(this);
    becProcess->start(program.c_str(), arguments);

    //BEC SLOT
    connect(becProcess, SIGNAL(error(QProcess::ProcessError))
            , SLOT(becError(QProcess::ProcessError)));
    connect(becProcess, SIGNAL(finished(int, QProcess::ExitStatus))
            , SLOT(becFinished(int, QProcess::ExitStatus)));
    connect(becProcess, SIGNAL(readyReadStandardError())
            , SLOT(becReadyReadStandardError()));
    connect(becProcess, SIGNAL(readyReadStandardOutput())
            , SLOT(becReadyReadStandardOutput()));
    connect(becProcess, SIGNAL(started())
            , SLOT(becStarted()));
    connect(becProcess, SIGNAL(stateChanged(QProcess::ProcessState))
            , SLOT(becStateChanged(QProcess::ProcessState)));
            */
}

void RunView::logErrorText(const QString &text, const QString colorName) {
    if(colorName.isEmpty()){
        m_textInfo->setTextColor(Qt::red);
    }else{
        m_textInfo->setTextColor(QColor(colorName));
    }
    m_textInfo->setFontPointSize(18);
    m_textInfo->append(text);
}

void RunView::logNormalText(const QString &text, const QString colorName) {
    if(colorName.isEmpty()){
        m_textInfo->setTextColor(Qt::black);
    }else{
        m_textInfo->setTextColor(QColor(colorName));
    }
    m_textInfo->setFontPointSize(12);
    m_textInfo->append(text);
}

void RunView::logH1Text(const QString &text, const QString colorName) {
    if(colorName.isEmpty()){
        m_textInfo->setTextColor(Qt::black);
    }else{
        m_textInfo->setTextColor(QColor(colorName));
    }
    m_textInfo->setFontPointSize(18);
    m_textInfo->append(text);
}

void RunView::logH2Text(const QString &text, const QString colorName) {
    if(colorName.isEmpty()){
        m_textInfo->setTextColor(Qt::black);
    }else{
        m_textInfo->setTextColor(QColor(colorName));
    }
    m_textInfo->setFontPointSize(15);
    m_textInfo->append(text);
}



} // openstudio

