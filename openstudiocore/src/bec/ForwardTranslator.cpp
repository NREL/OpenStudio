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

#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/ElectricEquipment.hpp"
#include "../model/ElectricEquipmentDefinition.hpp"
#include "../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/LightsDefinition_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/OtherEquipment.hpp"
#include "../model/OtherEquipment_Impl.hpp"
#include "../model/OtherEquipmentDefinition.hpp"
#include "../model/OtherEquipmentDefinition_Impl.hpp"
#include "../model/People.hpp"
#include "../model/People_Impl.hpp"
#include "../model/PeopleDefinition.hpp"
#include "../model/PeopleDefinition_Impl.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/GasEquipment_Impl.hpp"
#include "../model/GasEquipmentDefinition.hpp"
#include "../model/GasEquipmentDefinition_Impl.hpp"
#include "../model/Luminaire.hpp"
#include "../model/Luminaire_Impl.hpp"
#include "../model/LuminaireDefinition.hpp"
#include "../model/LuminaireDefinition_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include "../model/WaterUseEquipmentDefinition.hpp"
#include "../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/RenderingColor.hpp"
#include "../model/RenderingColor_Impl.hpp"
#include "../model/BoilerHotWater.hpp"
#include "../model/BoilerHotWater_Impl.hpp"
#include "../model/SimpleGlazing.hpp"
#include "../model/SimpleGlazing_Impl.hpp"
#include "../model/AirGap.hpp"
#include "../model/AirGap_Impl.hpp"
#include "../model/HotWaterEquipment.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/Photovoltaic.hpp"
#include "../model/Photovoltaic_Impl.hpp"
#include "../model/PhotovoltaicThermal.hpp"
#include "../model/PhotovoltaicThermal_Impl.hpp"
#include "../model/Splitter.hpp"
#include "../model/Splitter_Impl.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/CoilHeatingDXSingleSpeed.hpp"
#include "../model/CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../model/CoilHeatingElectric.hpp"
#include "../model/CoilHeatingElectric_Impl.hpp"
#include "../model/FanConstantVolume.hpp"
#include "../model/FanConstantVolume_Impl.hpp"
#include "../model/CoolingTowerSingleSpeed.hpp"
#include "../model/CoolingTowerSingleSpeed_Impl.hpp"
#include "../model/CoolingTowerTwoSpeed.hpp"
#include "../model/CoolingTowerTwoSpeed_Impl.hpp"
#include "../model/CoolingTowerVariableSpeed.hpp"
#include "../model/CoolingTowerVariableSpeed_Impl.hpp"
#include "../model/PumpVariableSpeed.hpp"
#include "../model/PumpVariableSpeed_Impl.hpp"
#include "../model/ChillerElectricEIR.hpp"
#include "../model/ChillerElectricEIR_Impl.hpp"

#include "../model/BuildingStory.hpp"

///////////////////////////////////////////////////////
#include "../model/CoolingTowerVariableSpeed_Impl.hpp"
#include "../model/CoolingTowerVariableSpeed_Impl.hpp"
#include "../model/FanVariableVolume.hpp"
#include "../model/FanVariableVolume_Impl.hpp"
#include "../model/CoilCoolingDXTwoSpeed.hpp"
#include "../model/CoilCoolingDXTwoSpeed_Impl.hpp"
#include "../model/CoilCoolingDXMultiSpeed.hpp"
#include "../model/CoilCoolingDXMultiSpeed_Impl.hpp"
#include "../model/CoilCoolingWater.hpp"
#include "../model/CoilCoolingWater_Impl.hpp"

#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"

#include "../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../model/AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "../model/CoilCoolingDXSingleSpeed.hpp"
#include "../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../model/CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../model/CoilCoolingDXVariableRefrigerantFlow_Impl.hpp"
#include "../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../model/CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../model/CoilCoolingDXSingleSpeed.hpp"
#include "../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../model/RefrigerationCase.hpp"
#include "../model/RefrigerationCase_Impl.hpp"

#include "../model/FanConstantVolume.hpp"
#include "../model/FanConstantVolume_Impl.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/PlantLoop_Impl.hpp"

#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/EulerAngles.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <boost/math/constants/constants.hpp>

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QThread>
#include <QInputDialog>
#include <QDebug>
#include <math.h>
#include <QRegularExpression>
#include <QRegExp>


#include <cmath>
# define M_PI 3.14159265358979323846  /* pi */

enum CALTIME {_06_00=0, _07_00, _08_00, _09_00, _10_00, _11_00, _12_00, _13_00, _14_00, _15_00, _16_00, _17_00, _18_00, TIME_SIZE};
enum CALDATE {cd21_Mar=0, cd22_Jun, cd23_Sep, cd22_Dec, MONTH_SIZE};
enum SUNLITE_FC_COL {FC_MARCH_9AM=0, FC_MARCH_NOON, FC_MARCH_3PM, FC_JUNE_9AM, FC_JUNE_NOON, FC_JUNE_3PM, FC_DEC_9AM, FC_DEC_NOON, FC_DEC_3PM};

QString toStringArray(double* array, size_t maxrow, size_t maxcol){
    QString out;
    for (size_t row = 0; row < maxrow; ++row) {
        for (size_t col = 0; col < maxcol; ++col) {
            int idx = row*maxcol+col;
            //qDebug() << QString("row:%1, col:%2, idx:%3").arg(row).arg(col).arg(idx);
            out.append(QString::number(array[idx]));
            out.append(", ");
        }
        out.append("\n");
    }
    out.append("\n");
    return out;
}

double absd(double val){
    if(val<0.0)
        val = val*-1.0;
    return val;
}

void setval2d(double* array, size_t maxcol, int row, int col, double val){
    array[(row*maxcol)+col] = val;
}

double getval2d(double* array, size_t maxcol, int row, int col){
    return array[(row*maxcol)+col];
}

double rad(double degrees){
    return ( degrees * M_PI ) / 180 ;
}

double calsc(double azimuth, double tilt, QList<double>& sunlit_fraction
    , QString* safu_af=NULL
    , QString* scal5_beam=NULL, QString* scal5_diffue=NULL
    , QString* scal6_beam=NULL, QString* scal6_diffue=NULL){

    if(sunlit_fraction.size()<9){
        return 1.0;
    }

    qDebug() << "--------------sunlit_fraction---------------\n";
    QString ssfs;
    for (int i = 0; i < sunlit_fraction.size(); ++i) {
        ssfs += QString::number(sunlit_fraction.at(i))+",";
    }

    qDebug() << "[" << ssfs << "]";

    const double sun_azimuth[] = {-89.79081568,-113.7169952,-89.40377855,-0.2526566894,
                               -86.60369907,-110.979335,-87.02129226,-64.75112846,
                               -82.54808643,-109.4640409,-83.10886989,-59.94994306,
                               -77.26492041,-109.4117206,-78.18611135,-52.97593149,
                               -68.99680892,-111.8999348,-70.86686641,-42.69075554,
                               -52.32881004,-121.1567293,-57.26338338,-27.54591276,
                               -10.31331604,-159.0154984,-24.66495519,-7.014620915,
                               42.18338726,134.8912916,29.99550998,15.3686876,
                               64.8000326,115.4833634,59.37844888,33.98572993,
                               74.94615016,110.3095054,71.89344656,47.09950791,
                               80.95279801,109.2156129,78.82414669,55.95224768,
                               85.31047509,110.0007506,83.58463374,61.99294854,
                               89.00579293,112.0774302,87.43164219,66.15793962};

    const double sun_altitude[] = {0.00,1.77,0.00,0.00,
                                12.00,15.26,7.86,3.79,
                                26.51,28.94,22.38,16.71,
                                40.86,42.70,36.76,28.88,
                                54.82,56.36,50.82,39.73,
                                67.62,69.49,63.98,48.18,
                                75.67,79.52,73.88,52.58,
                                71.26,75.79,73.13,51.50,
                                59.36,63.62,62.59,45.33,
                                45.65,50.14,49.26,35.80,
                                31.39,36.41,35.15,24.36,
                                16.92,22.67,20.75,11.85,
                                2.37,9.06,6.22,0.00};

    double cosqs[TIME_SIZE*MONTH_SIZE];

    int iloop = TIME_SIZE*MONTH_SIZE;

    for (int i = 0; i < iloop; ++i) {
        cosqs[i] = ((sin(rad(sun_altitude[i]))*(cos(rad(tilt))))
                    +(cos(rad(sun_altitude[i]))*sin(rad(sun_azimuth[i]))*sin(rad(tilt))*sin(rad(azimuth)))
                    +(cos(rad(sun_altitude[i]))*cos(rad(sun_azimuth[i]))*sin(rad(tilt))*cos(rad(azimuth))));
    }

    qDebug() << "--------------cosqs---------------\n" << toStringArray(cosqs, TIME_SIZE, MONTH_SIZE);

    double afu_af[TIME_SIZE*MONTH_SIZE];

    double afu_af_diffue[] = {
        0.00, 1.00, 0.00, 0.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 0.00
    };

    setval2d(afu_af, MONTH_SIZE, _09_00, cd21_Mar, sunlit_fraction[0]);
    setval2d(afu_af, MONTH_SIZE, _12_00, cd21_Mar, sunlit_fraction[1]);
    setval2d(afu_af, MONTH_SIZE, _15_00, cd21_Mar, sunlit_fraction[2]);

    setval2d(afu_af, MONTH_SIZE, _09_00, cd22_Jun, sunlit_fraction[3]);
    setval2d(afu_af, MONTH_SIZE, _12_00, cd22_Jun, sunlit_fraction[4]);
    setval2d(afu_af, MONTH_SIZE, _15_00, cd22_Jun, sunlit_fraction[5]);

    setval2d(afu_af, MONTH_SIZE, _09_00, cd23_Sep, sunlit_fraction[0]);
    setval2d(afu_af, MONTH_SIZE, _12_00, cd23_Sep, sunlit_fraction[1]);
    setval2d(afu_af, MONTH_SIZE, _15_00, cd23_Sep, sunlit_fraction[2]);

    setval2d(afu_af, MONTH_SIZE, _09_00, cd22_Dec, sunlit_fraction[6]);
    setval2d(afu_af, MONTH_SIZE, _12_00, cd22_Dec, sunlit_fraction[7]);
    setval2d(afu_af, MONTH_SIZE, _15_00, cd22_Dec, sunlit_fraction[8]);

    for (int col = 0; col < 4; ++col) {
        setval2d(afu_af, MONTH_SIZE, _06_00, col, getval2d(afu_af, MONTH_SIZE, _09_00, col));//6
        setval2d(afu_af, MONTH_SIZE, _07_00, col, getval2d(afu_af, MONTH_SIZE, _09_00, col));//7
        setval2d(afu_af, MONTH_SIZE, _08_00, col, getval2d(afu_af, MONTH_SIZE, _09_00, col));//8

        double v12 = getval2d(afu_af, MONTH_SIZE, _12_00, col);
        double v9 = getval2d(afu_af, MONTH_SIZE, _09_00, col);
        double v10 = v9-((v9-v12)/3);
        double v11 = v10-(v9-v10);
        setval2d(afu_af, MONTH_SIZE, _10_00, col, v10);//10
        setval2d(afu_af, MONTH_SIZE, _11_00, col, v11);//11

        double v15 = getval2d(afu_af, MONTH_SIZE, _15_00, col);
        double v13 = v12-((v12-v15)/3);
        double v14 = v13-(v12-v13);
        setval2d(afu_af, MONTH_SIZE, _13_00, col, v13);//13
        setval2d(afu_af, MONTH_SIZE, _14_00, col, v14);//14

        setval2d(afu_af, MONTH_SIZE, _16_00, col, v15);//16
        setval2d(afu_af, MONTH_SIZE, _17_00, col, v15);//17
        setval2d(afu_af, MONTH_SIZE, _18_00, col, v15);//18
    }

    setval2d(afu_af, MONTH_SIZE, _06_00, cd21_Mar, 0.0);
    setval2d(afu_af, MONTH_SIZE, _06_00, cd23_Sep, 0.0);

    if(safu_af)
        (*safu_af) = QString("--------------afu_af---------------\n%1").arg(toStringArray(afu_af, TIME_SIZE, MONTH_SIZE));

    double ees_eed_beam[] = {
        0.0, 7.5, 0.0, 0.0,
        68.5, 105.0, 94.4, 64.4,
        185.7, 196.2, 202.3, 270.0,
        290.1, 275.6, 296.2, 454.4,
        374.8, 338.6, 369.9, 603.3,
        433.8, 381.2, 418.3, 704.9,
        463.2, 401.1, 437.9, 751.3,
        461.0, 397.0, 427.6, 738.9,
        427.3, 369.1, 388.0, 668.7,
        364.5, 319.1, 321.7, 546.1,
        276.7, 250.0, 233.5, 380.8,
        170.0, 165.9, 129.2, 185.6,
        51.7, 72.0, 16.1, 0.0
    };


//    double ees_eed_diffue[] = {
//        0.0, 5.6, 0.0, 0.0,
//        44.9, 77.8, 77.1, 19.9,
//        121.6, 145.4, 165.1, 83.5,
//        190.0, 204.3, 241.8, 140.5,
//        245.5, 250.9, 302.0, 186.5,
//        284.1, 282.6, 341.4, 217.9,
//        303.4, 297.3, 357.5, 232.2,
//        301.9, 294.2, 349.0, 228.4,
//        279.8, 273.6, 316.7, 206.7,
//        238.7, 236.5, 262.6, 168.8,
//        181.2, 185.3, 190.6, 117.7,
//        111.3, 123.0, 105.5, 57.4,
//        33.9, 53.3, 13.1, 0.0
//    };

    double cal6_beam[TIME_SIZE*MONTH_SIZE];
    double cal6_diffue[TIME_SIZE*MONTH_SIZE];
    double cal5_beam[TIME_SIZE*MONTH_SIZE];
    double cal5_diffue[TIME_SIZE*MONTH_SIZE];

    double sumcal6 = 0.0;
    for (int i = 0; i < iloop; ++i) {
        cal6_beam[i] = absd(afu_af[i]*cosqs[i]*ees_eed_beam[i]);
        sumcal6 += cal6_beam[i];
    }

    if(scal6_beam)
        (*scal6_beam) = QString("--------------cal6_beam---------------\n%1").arg(toStringArray(cal6_beam, TIME_SIZE, MONTH_SIZE));

    for (int i = 0; i < iloop; ++i) {
        cal6_diffue[i] = absd(afu_af_diffue[i]*(1+cos(tilt))/2);
        sumcal6 += cal6_diffue[i];
    }

    if(scal6_diffue)
        (*scal6_diffue) = QString("--------------cal6_diffue---------------\n%1").arg(toStringArray(cal6_diffue, TIME_SIZE, MONTH_SIZE));

    double sumcal5 = 0.0;
    for (int i = 0; i < iloop; ++i) {
        cal5_beam[i] = absd(ees_eed_beam[i]*cosqs[i]);
        sumcal5 += cal5_beam[i];
    }

    if(scal5_beam)
        (*scal5_beam) = QString("--------------cal5_beam---------------\n%1").arg(toStringArray(cal5_beam, TIME_SIZE, MONTH_SIZE));

    for (int i = 0; i < iloop; ++i) {
        cal5_diffue[i] = absd(cal6_diffue[i]);
        sumcal5 += cal5_diffue[i];
    }

    if(scal5_diffue)
        (*scal5_diffue) = QString("--------------cal5_diffue---------------\n%1").arg(toStringArray(cal5_diffue, TIME_SIZE, MONTH_SIZE));

    return sumcal6/sumcal5;
}

double rtod(double rad){
    return rad*180/3.141592653589793;
}

namespace openstudio {
namespace bec {

ForwardTranslator::ForwardTranslator()
{
    _sunlits = NULL;
	  _wwr_totoal = 0.0f;
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.bec\\.ForwardTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
}

ForwardTranslator::~ForwardTranslator()
{
}

bool ForwardTranslator::modelTobec(const openstudio::model::Model& model
                                   , const openstudio::path& path
                                   , ProgressBar* progressBar
                                   , std::string *bvName
                                   , QHash<QString, QList<double>>* sunlits
								   , float wwr_totoal)
{
    QInputDialog inputBuildingType;
    _sunlits = sunlits;
	  _wwr_totoal = wwr_totoal;
    inputBuildingType.setOption(QInputDialog::UseListViewForComboBoxItems);
    inputBuildingType.setWindowTitle("What is building type.");
    inputBuildingType.setLabelText("Selection:");
    QStringList types;
        types <<"Office building"
        <<"Department store"
        <<"Community building"
        <<"Hotel"
        <<"Condominium"
        <<"Medical Center"
        <<"Educational Institution"
        <<"Service Facility"
        <<"Theater";

    inputBuildingType.setComboBoxItems(types);

    int ret = inputBuildingType.exec();

    if (ret == QDialog::Accepted){

        if (bvName)
            (*bvName) = inputBuildingType.textValue().toStdString();

        m_progressBar = progressBar;
        m_logSink.setThreadId(QThread::currentThread());
        m_logSink.resetStringStream();

        boost::optional<QDomDocument> doc = this->translateModel(model, inputBuildingType.textValue());
        if (!doc){
            return false;
        }

        QFile file(toQString(path));
        if (file.open(QFile::WriteOnly)){
            QTextStream textStream(&file);
            textStream.setCodec("UTF-8");
            textStream << doc->toString(2);
            file.close();
            return true;
        }
        return false;
    }
    return false;
}

std::vector<LogMessage> ForwardTranslator::warnings() const
{
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() == Warn){
            result.push_back(logMessage);
        }
    }

    return result;
}

std::vector<LogMessage> ForwardTranslator::errors() const
{
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() > Warn){
            result.push_back(logMessage);
        }
    }

    return result;
}

QString ForwardTranslator::escapeName(const std::string& name)
{
    QString result = toQString(name);
    result.replace(" ", "_");
    result.replace("(", "_");
    result.replace(")", "_");
    return result;
}

QDomElement ForwardTranslator::createTagWithText(QDomElement &parent, const QString &tag, const QString &text)
{
    QDomElement elm = _doc->createElement(tag);
    if(!text.isEmpty())
        elm.appendChild(_doc->createTextNode(text.toUtf8()));

    parent.appendChild(elm);
    return elm;
}

void ForwardTranslator::doEnvelope(const model::Model &model, QDomElement &parent)
{
    QDomElement Envelope = createTagWithText(parent, "Envelope");


    doMaterial(model, Envelope);

    //TODO:IMPLEMENT ComponentOfSection
    doComponentLoop(model, Envelope);

    //TODO:IMPLEMENT SectionOfWall and Wall
    doSectionOfWall(model, Envelope);

    //TODO:IMPLEMENT Wall
    //doWall(model, parent);
}

void ForwardTranslator::doMaterial(const model::Model &model, QDomElement &parent)
{
    QDomElement Material = createTagWithText(parent, "Material");
    QDomElement OpaqueMaterial = createTagWithText(Material, "OpaqueMaterial");
    QDomElement TransparentMaterial = createTagWithText(Material, "TransparentMaterial");
    QDomElement AirGapMaterial = createTagWithText(Material, "AirGapMaterial");

    QDomElement UNKNOW_MATERIAL;

    std::vector<model::Material> materials = model.getModelObjects<model::Material>();

    for (const model::Material& material : materials){
        if (material.optionalCast<model::StandardOpaqueMaterial>()){

            QDomElement OpaqueMat = createTagWithText(OpaqueMaterial, "OpaqueMat");
            createTagWithText(OpaqueMat, "OpaqueMaterialName", material.name().get().c_str());
            //!- Conductivity{ W / m - K }
            model::StandardOpaqueMaterial m = material.cast<model::StandardOpaqueMaterial>();
            //WARNING: <OpaqueMaterialType>Wall</OpaqueMaterialType> Not in Openstudio.
            createTagWithText(OpaqueMat, "OpaqueMaterialType", "Wall");
            createTagWithText(OpaqueMat, "OpaqueMaterialThermalConductivity", QString::number(m.conductivity()));
            createTagWithText(OpaqueMat, "OpaqueMaterialThermalConductivityUnit", "W/mK");

            //!- Density{ kg / m3 }
            createTagWithText(OpaqueMat, "OpaqueMaterialDensity", QString::number(m.density()));
            createTagWithText(OpaqueMat, "OpaqueMaterialDensityUnit", "kg/m^3");

            //!- Specific Heat{ J / kg - K }
            createTagWithText(OpaqueMat, "OpaqueMaterialSpecificHeat", QString::number(m.specificHeat()));
            createTagWithText(OpaqueMat, "OpaqueMaterialSpecificHeatUnit", "kJ/kgK");
        }
        else if(material.optionalCast<model::SimpleGlazing>())       {
            model::SimpleGlazing glass = material.cast<model::SimpleGlazing>();
            QDomElement TranMat = createTagWithText(TransparentMaterial, "TranMat");
            std::string name = glass.name().get().c_str();
            double vt = glass.visibleTransmittance().get();
            double uf = glass.uFactor();
            double shg = glass.solarHeatGainCoefficient();
            createTagWithText(TranMat, "TransparentMaterialName", name.c_str());
            double thickness = qMax(0.012, glass.thickness());
            createTagWithText(TranMat, "TransparentMaterialThickness", QString::number(thickness));
            createTagWithText(TranMat, "TransparentMaterialThicknessUnit", "m");
            createTagWithText(TranMat, "TransparentMaterialVisibleRayReflectance", "");//WARNING: NOTING
            createTagWithText(TranMat, "TransparentMaterialVisibleRayTransmittance", QString::number(vt));
            createTagWithText(TranMat, "TransparentMaterialSolarEnergyReflectance", "");//WARNING: NOTING
            createTagWithText(TranMat, "TransparentMaterialSolarEnergyTransmittance", "");//WARNING: NOTING
            createTagWithText(TranMat, "TransparentMaterialSolarEnergyAbsorption", "");//WARNING: NOTING
            createTagWithText(TranMat, "TransparentMaterialUvalue", QString::number(uf));
            createTagWithText(TranMat, "TransparentMaterialUvalueUnit", "w/M^2k");
            createTagWithText(TranMat, "TransparentMaterialSHGC", QString::number(shg));//WARNING: NOTING
        }
        else if(material.optionalCast<model::AirGap>()){
            model::AirGap airgap = material.cast<model::AirGap>();
            QDomElement AirGapMat = createTagWithText(AirGapMaterial, "AirGapMat");
            createTagWithText(AirGapMat, "AirGapMaterialName", airgap.name().get().c_str());
            createTagWithText(AirGapMat, "AirGapMaterialType", ""); //WARNING: NOTING
            createTagWithText(AirGapMat, "AirGapMaterialSurfaceType", ""); //WARNING: NOTING
            createTagWithText(AirGapMat, "AirGapMaterialThickness", QString::number(airgap.thickness()));
            createTagWithText(AirGapMat, "AirGapMaterialThicknessUnit", "m");
            createTagWithText(AirGapMat, "AirGapMaterialAirGapResistance", QString::number(airgap.thermalResistance()));
        }
        else{
            if(UNKNOW_MATERIAL.isNull()){
                UNKNOW_MATERIAL = createTagWithText(Material, "UNKNOW_MATERIALS");
            }
            QDomElement UNKNOW = createTagWithText(UNKNOW_MATERIAL, "UNKNOW");
            createTagWithText(UNKNOW, "NAME", material.name().get().c_str());
            createTagWithText(UNKNOW, "TYPE", material.iddObject().name().c_str());
        }
    }
}

void ForwardTranslator::doTransparentMaterial(const model::Model &model, QDomElement &parent)
{
    model::SimpleGlazingVector glazings = model.getModelObjects<model::SimpleGlazing>();
    model::SimpleGlazingVector::iterator it = glazings.begin();

    createTagWithText(parent, "TransparentSize", QString::number(glazings.size()));

    while (it!=glazings.end()) {
        model::SimpleGlazing& glass = (*it);
        QDomElement TranMat = _doc->createElement("TranMat");
        createTagWithText(TranMat, "TransparentMaterialName", glass.name().get().c_str());
        createTagWithText(TranMat, "TransparentMaterialThickness", QString::number(glass.thickness()));
        createTagWithText(TranMat, "TransparentMaterialThicknessUnit", "m");
        createTagWithText(TranMat, "TransparentMaterialVisibleRayReflectance", "1");//WARNING: NOTING
        createTagWithText(TranMat, "TransparentMaterialVisibleRayTransmittance", QString::number(glass.visibleTransmittance().get()));
        createTagWithText(TranMat, "TransparentMaterialSolarEnergyReflectance", "1");//WARNING: NOTING
        createTagWithText(TranMat, "TransparentMaterialSolarEnergyTransmittance", "1");//WARNING: NOTING
        createTagWithText(TranMat, "TransparentMaterialSolarEnergyAbsorption", "1");//WARNING: NOTING
        createTagWithText(TranMat, "TransparentMaterialUvalue", QString::number(glass.uFactor()));
        createTagWithText(TranMat, "TransparentMaterialUvalueUnit", "w/M^2k");
        createTagWithText(TranMat, "TransparentMaterialSHGC", QString::number(glass.solarHeatGainCoefficient()));//WARNING: NOTING
        parent.appendChild(TranMat);
        it++;
    }
}

void ForwardTranslator::doAirGapMaterial(const model::Model &model, QDomElement &parent)
{
    model::AirGapVector airGaps = model.getModelObjects<model::AirGap>();
    model::AirGapVector::iterator it = airGaps.begin();

    createTagWithText(parent, "AirGapMaterialSize", QString::number(airGaps.size()));

    while (it!=airGaps.end()) {
        model::AirGap& airgap = (*it);
        QDomElement TranMat = _doc->createElement("AirGapMat");
        createTagWithText(TranMat, "AirGapMaterialName", airgap.name().get().c_str());
        createTagWithText(TranMat, "AirGapMaterialType", "Roof"); //WARNING: NOTING
        createTagWithText(TranMat, "AirGapMaterialSurfaceType", "????"); //WARNING: NOTING
        createTagWithText(TranMat, "AirGapMaterialThickness", QString::number(airgap.thickness()));//WARNING: NOTING
        createTagWithText(TranMat, "AirGapMaterialThicknessUnit", "m");
        createTagWithText(TranMat, "AirGapMaterialAirGapResistance", QString::number(airgap.thermalResistance()));//WARNING: NOTING
        parent.appendChild(TranMat);
        it++;
    }
}

void ForwardTranslator::doComponentLoop(const model::Model &model, QDomElement &parent)
{
    //Start doComponentOfSection
    QDomElement componentOfSection = createTagWithText(parent, "ComponentOfSection");
    QDomElement OpaqueComponentList = createTagWithText(componentOfSection, "OpaqueComponentList");
    QDomElement OpaqueComponentDetail = createTagWithText(componentOfSection, "OpaqueComponentDetail");
    QDomElement transparentComponentList = createTagWithText(componentOfSection, "TransparentComponentList");
    QHash<QString, int> componentCheck;
    //END doComponentOfSection

    std::vector<openstudio::model::BuildingStory> stories = model.getConcreteModelObjects<openstudio::model::BuildingStory>();

    for (const openstudio::model::BuildingStory& buildingStory : stories) {

        std::vector<openstudio::model::Space> spaces = buildingStory.spaces();
        for (openstudio::model::Space& space : spaces){

            openstudio::model::SurfaceVector surfaces = space.surfaces();
            for (openstudio::model::Surface& surface : surfaces){

                doComponentOfSection(surface
                                     , OpaqueComponentList
                                     , OpaqueComponentDetail
                                     , transparentComponentList
                                     , componentCheck);
            }
        }
    }
}

void ForwardTranslator::doComponentOfSection(openstudio::model::Surface& surface
                                             , QDomElement &OpaqueComponentList
                                             , QDomElement &OpaqueComponentDetail
                                             , QDomElement &transparentComponentList
                                             , QHash<QString, int>& componentCheck)
{
    std::string name = surface.construction().get().name().get();
    std::string stype = surface.surfaceType();
    if(stype == "RoofCeiling"){
        stype = "Roof";
    }

    if(!componentCheck.contains(name.c_str())){
        boost::optional<model::ConstructionBase> scon = surface.construction();
        if (scon){
            boost::optional<model::Construction> construction = scon->optionalCast<model::Construction>();
            if (construction){
                std::vector<model::Material> layers = construction->layers();
                for (size_t i = 0; i < layers.size(); ++i)
                {
                    if (layers[i].optionalCast<model::StandardOpaqueMaterial>()){
                        model::StandardOpaqueMaterial outerMaterial = layers[i].cast<model::StandardOpaqueMaterial>();

                        QDomElement OpaqueDetail = createTagWithText(OpaqueComponentDetail, "OpaqueDetail");
                        createTagWithText(OpaqueDetail, "OpaqueComponentDetailListName", name.c_str());
                        createTagWithText(OpaqueDetail, "OpaqueComponentDetailMaterialName", outerMaterial.name().get().c_str());
                        createTagWithText(OpaqueDetail, "OpaqueComponentDetailThickness", QString::number(outerMaterial.thickness()));
                        //createTagWithText(OpaqueDetail, "IsOpaque", QString(layers[0]->isOpaque()));
                        createTagWithText(OpaqueDetail, "OpaqueComponentDetailThicknessUnit", "m");

                        if(!componentCheck.contains(name.c_str())){
                            componentCheck.insert(name.c_str(), 1);
                            QDomElement OpaqueList = createTagWithText(OpaqueComponentList, "OpaqueList");
                            createTagWithText(OpaqueList, "OpaqueComponentListName", name.c_str());
                            createTagWithText(OpaqueList, "OpaqueComponentListType", stype.c_str());

                            QString colorName = outerMaterial.roughness().c_str();
                            if(colorName.indexOf("Rough")>0){
                                colorName.replace("Rough", " Rough");
                            }else if(colorName.indexOf("Smooth")>0){
                                colorName.replace("Smooth", " Smooth");
                            }

                            double solarAbsoupTance = outerMaterial.solarAbsorptance();

                            createTagWithText(OpaqueList, "OpaqueComponentListOuterSurfaceColor", QString::number(solarAbsoupTance));
                            createTagWithText(OpaqueList, "OpaqueComponentListInnerSurfaceType", "UNKNOW");
                            createTagWithText(OpaqueList, "OpaqueComponentListDescription", "???");
                        }
                    }
                    else if (layers[i].optionalCast<model::SimpleGlazing>()){
                        model::SimpleGlazing glass = layers[i].cast<model::SimpleGlazing>();

                        QDomElement TransparentList = createTagWithText(transparentComponentList, "TransparentList");
                        createTagWithText(TransparentList, "TransparentComponentListName", glass.name().get().c_str());
                        createTagWithText(TransparentList, "TransparentComponentListType", stype.c_str());
                        createTagWithText(TransparentList, "TransparentComponentListSurfaceName", surface.name().get().c_str());
                        createTagWithText(TransparentList, "TransparentComponentListSHGC", QString::number(glass.solarHeatGainCoefficient()));
                        createTagWithText(TransparentList, "TransparentComponentListTransmittance", QString::number(glass.visibleTransmittance().get()));
                        //createTagWithText(TransparentList, "IsOpaque", QString(layers[0]->isOpaque()));
                        createTagWithText(TransparentList, "TransparentComponentListDescription", "???");
                    }
                    else{
                        createTagWithText(OpaqueComponentList, "ERROR", name.c_str());
                    }
                }
            }
        }
    }
    std::vector<model::SubSurface> subSurfaces = surface.subSurfaces();
    std::vector<model::SubSurface>::iterator it = subSurfaces.begin();
    while (it!=subSurfaces.end()) {
        model::SubSurface sub = (*it);
        std::string name = sub.construction().get().name().get();
        std::string stype = sub.subSurfaceType();
        if(!componentCheck.contains(name.c_str())){
            componentCheck.insert(name.c_str(), 1);
            boost::optional<model::ConstructionBase> scon = sub.construction();
            if (scon){
                boost::optional<model::Construction> construction = scon->optionalCast<model::Construction>();
                if (construction){
                    std::vector<model::Material> layers = construction->layers();
                    for (size_t i = 0; i < layers.size(); ++i)
                    {
                        if (layers[i].optionalCast<model::StandardOpaqueMaterial>()){
                            model::StandardOpaqueMaterial outerMaterial = layers[i].cast<model::StandardOpaqueMaterial>();

                            QDomElement OpaqueDetail = createTagWithText(OpaqueComponentDetail, "OpaqueDetail");
                            createTagWithText(OpaqueDetail, "OpaqueComponentDetailListName", name.c_str());
                            createTagWithText(OpaqueDetail, "OpaqueComponentDetailMaterialName", outerMaterial.name().get().c_str());
                            createTagWithText(OpaqueDetail, "OpaqueComponentDetailThickness", QString::number(outerMaterial.thickness()));
                            //createTagWithText(OpaqueDetail, "IsOpaque", QString(layers[i]->isOpaque()));
                            createTagWithText(OpaqueDetail, "OpaqueComponentDetailThicknessUnit", "m");

                            if(!componentCheck.contains(name.c_str())){
                                componentCheck.insert(name.c_str(), 1);
                                QDomElement OpaqueList = createTagWithText(OpaqueComponentList, "OpaqueList");
                                createTagWithText(OpaqueList, "OpaqueComponentListName", name.c_str());
                                createTagWithText(OpaqueList, "OpaqueComponentListType", stype.c_str());

                                QString colorName = outerMaterial.roughness().c_str();
                                if(colorName.indexOf("Rough")>0){
                                    colorName.replace("Rough", " Rough");
                                }else if(colorName.indexOf("Smooth")>0){
                                    colorName.replace("Smooth", " Smooth");
                                }

                                createTagWithText(OpaqueList, "OpaqueComponentListOuterSurfaceColor", colorName);
                                createTagWithText(OpaqueList, "OpaqueComponentListInnerSurfaceType", "UNKNOW");
                                createTagWithText(OpaqueList, "OpaqueComponentListDescription", "???");
                            }
                        }
                        else if (layers[i].optionalCast<model::SimpleGlazing>()){
                            model::SimpleGlazing glass = layers[i].cast<model::SimpleGlazing>();

                            QDomElement TransparentList = createTagWithText(transparentComponentList, "TransparentList");
                            createTagWithText(TransparentList, "TransparentComponentListName", glass.name().get().c_str());
                            createTagWithText(TransparentList, "TransparentComponentListType", stype.c_str());
                            createTagWithText(TransparentList, "TransparentComponentListSHGC", QString::number(glass.solarHeatGainCoefficient()));
                            createTagWithText(TransparentList, "TransparentComponentListTransmittance", QString::number(glass.visibleTransmittance().get()));
                            //createTagWithText(TransparentList, "IsOpaque", QString(layers[i]->isOpaque()));
                            createTagWithText(TransparentList, "TransparentComponentListDescription", "???");
                        }
                        else{
                            createTagWithText(OpaqueComponentList, "ERROR", name.c_str());
                        }
                    }
                }
            }
        }
        it++;
    }
}

void ForwardTranslator::doOpaqueComponentList(const model::Model &model, QDomElement &parent)
{
    QDomElement opaqueComponentList = _doc->createElement("OpaqueComponentList");
    parent.appendChild(opaqueComponentList);
    //      boost::optional<openstudio::model::Building> building = model.building();
    //      if(building){
    //          std::vector<openstudio::model::Space>::iterator it = building->spaces().begin();
    //          while (it!=building->spaces().end()) {
    //              doOpaqueList((*it), doc, opaqueComponentList);
    //              it++;
    //          }
    //      }
}

void ForwardTranslator::doOpaqueComponentDetail(const model::Model &model, QDomElement &parent)
{

}

void ForwardTranslator::doTransparentComponentList(const model::Model &model, QDomElement &parent)
{

}

void ForwardTranslator::doOpaqueList(const openstudio::model::Space &model, QDomElement &parent)
{

}

void ForwardTranslator::doSectionOfWall(const model::Model &model, QDomElement &Envelope)
{
    QDomElement sectionOfWall = createTagWithText(Envelope, "SectionOfWall");
    QDomElement SectionList = createTagWithText(sectionOfWall, "SectionList");
    QDomElement SectionDetail = createTagWithText(sectionOfWall, "SectionDetail");

    QDomElement Wall = createTagWithText(Envelope, "Wall");
    QDomElement WallList = createTagWithText(Wall, "WallList");
    QDomElement WallDetail = createTagWithText(Wall, "WallDetail");

    std::vector<openstudio::model::BuildingStory> stories = model.getConcreteModelObjects<openstudio::model::BuildingStory>();

    for (const openstudio::model::BuildingStory& buildingStory : stories) {

        std::vector<openstudio::model::Space> spaces = buildingStory.spaces();
        for (openstudio::model::Space& space : spaces){
			double directionRelativeToNorth = space.directionofRelativeNorth();
            openstudio::model::SurfaceVector surfaces = space.surfaces();
            for (model::Surface& surface : surfaces){
                if(surface.surfaceType() == "Wall" || surface.surfaceType() == "RoofCeiling"){

                    QString boundCon = surface.outsideBoundaryCondition().c_str();
                    if(boundCon!="Outdoors"){
                        continue;
                    }

                    QString surfaceType;
                    if(surface.surfaceType() == "RoofCeiling"){
                        surfaceType = "Roof";
                    }
                    else
                        surfaceType = "Wall";

                    QDomElement SectionL = createTagWithText(SectionList,"SectionL");
                    createTagWithText(SectionL, "SectionListName", surface.name().get().c_str());
                    createTagWithText(SectionL, "SectionListType", surfaceType);
                    createTagWithText(SectionL, "SectionListDescription", boundCon);

                    QDomElement SectionD = createTagWithText(SectionDetail,"SectionD");
                    createTagWithText(SectionD, "SectionDetailSectionListName", surface.name().get().c_str());

                    QString componentName = surface.construction().get().name().get().c_str();
                    boost::optional<model::ConstructionBase> scon = surface.construction();
                    if (scon){
                        boost::optional<model::Construction> construction = scon->optionalCast<model::Construction>();
                        if (construction){
                            std::vector<model::Material> layers = construction->layers();
                            if (!layers.empty()){
                                if (layers[0].optionalCast<model::SimpleGlazing>()){
                                    model::SimpleGlazing glass = layers[0].cast<model::SimpleGlazing>();
                                    componentName = glass.name().get().c_str();
                                }
                            }
                        }
                    }

                    createTagWithText(SectionD, "SectionDetailComponentName", componentName);
                    createTagWithText(SectionD, "SectionDetailArea", QString::number(surface.netArea()));
                    createTagWithText(SectionD, "SectionDetailAreaUnit", "m^2");

                    QDomElement WallL = createTagWithText(WallList,"WallL");
                    QString sfName = surface.name().get().c_str();
                    createTagWithText(WallL, "WallListName", sfName);
                    createTagWithText(WallL, "WallListType", surfaceType);
					createTagWithText(WallL, "WallListPlanAzimuth", QString::number(rtod(surface.azimuth()) + directionRelativeToNorth));
					createTagWithText(WallL, "WallListInclination", QString::number(rtod(surface.tilt())));
                    createTagWithText(WallL, "WallListDescription", "???");

                    double sc = 1.0;
                    if(_sunlits!=NULL){
                        if(_sunlits->contains(sfName.toUpper().trimmed())){
                            double degree_azimuth = surface.azimuth()*180.0/M_PI;
                            double degree_tilt = surface.tilt()*180.0/M_PI;
                            qDebug() << QString("\n\n\n--- cal sc for : %1 , azimuth:%2, tilt:%3 ---")
                                        .arg(sfName)
                                        .arg(degree_azimuth)
                                        .arg(degree_tilt);
                            sc = calsc(degree_azimuth, degree_tilt, (*_sunlits)[sfName.toUpper().trimmed()]);
                            qDebug() << QString("################################");
                        }else{
                            //TODO:ERROR
                        }
                    }else{
                        //TODO:ERROR
                    }

                    QDomElement WallD = createTagWithText(WallDetail,"WallD");
                    createTagWithText(WallD, "WallDetailWallListName", sfName);
                    createTagWithText(WallD, "WallDetailSectionName", sfName);

                    openstudio::model::SubSurfaceVector subs = surface.subSurfaces();

                    for (openstudio::model::SubSurface& sub : subs){
                        QString ssfName = sub.name().get().c_str();
                        QDomElement SectionL = createTagWithText(SectionList,"SectionL");
                        createTagWithText(SectionL, "SectionListName", ssfName);
                        createTagWithText(SectionL, "SectionListType", surfaceType);
                        QString boundCon = sub.outsideBoundaryCondition().c_str();
                        createTagWithText(SectionL, "SectionListDescription", boundCon);

                        QDomElement SectionD = createTagWithText(SectionDetail,"SectionD");
                        createTagWithText(SectionD, "SectionDetailSectionListName", sfName);

                        QString componentName = sub.construction().get().name().get().c_str();
                        boost::optional<model::ConstructionBase> scon = sub.construction();
                        if (scon){
                            boost::optional<model::Construction> construction = scon->optionalCast<model::Construction>();
                            if (construction){
                                std::vector<model::Material> layers = construction->layers();
                                if (!layers.empty()){
                                    if (layers[0].optionalCast<model::SimpleGlazing>()){
                                        model::SimpleGlazing glass = layers[0].cast<model::SimpleGlazing>();
                                        componentName = glass.name().get().c_str();
                                    }
                                }
                            }
                        }

                        createTagWithText(SectionD, "SectionDetailComponentName", componentName);
                        createTagWithText(SectionD, "SectionDetailArea", QString::number(sub.netArea()));
                        createTagWithText(SectionD, "SectionDetailAreaUnit", "m^2");

                        /////////////
                        QDomElement WallL = createTagWithText(WallList,"WallL");

                        createTagWithText(WallL, "WallListName", ssfName);
                        createTagWithText(WallL, "WallListType", surfaceType);
						createTagWithText(WallL, "WallListPlanAzimuth", QString::number(rtod(sub.azimuth()) + directionRelativeToNorth));
                        createTagWithText(WallL, "WallListInclination", QString::number(rtod(sub.tilt())));
                        createTagWithText(WallL, "WallListDescription", "???");

                        if(_sunlits!=NULL){
                            if(_sunlits->contains(ssfName.toUpper().trimmed())){
                                double degree_azimuth = sub.azimuth()*180.0/M_PI;
                                double degree_tilt = sub.tilt()*180.0/M_PI;
                                qDebug() << QString("\n\n\n--- cal sc for : %1 , azimuth:%2, tilt:%3 ---")
                                            .arg(ssfName)
                                            .arg(degree_azimuth)
                                            .arg(degree_tilt);
                                sc = calsc(degree_azimuth, degree_tilt, (*_sunlits)[ssfName.toUpper().trimmed()]);
                                qDebug() << QString("################################");
                            }else{
                                //TODO:ERROR
                            }
                        }else{
                            //TODO:ERROR
                        }
                        QDomElement WallD = createTagWithText(WallDetail,"WallD");
                        createTagWithText(WallD, "WallDetailWallListName", sfName);
                        createTagWithText(WallD, "WallDetailSectionName", ssfName);
                        createTagWithText(WallD, "WallDetailSC", QString::number(sc));
                    }
                    createTagWithText(WallD, "WallDetailSC", QString::number(sc));
                }
                else{
                    QDomElement SectionUNKNOW = createTagWithText(SectionList,"SectionUNKNOW");
                    createTagWithText(SectionUNKNOW,"SectionUNKNOW_TYPE", surface.surfaceType().c_str());
                    createTagWithText(SectionUNKNOW,"SectionUNKNOW_NAME", surface.name().get().c_str());
                }
            }
        }
    }
}

void ForwardTranslator::doModelLoop(const model::Model &model, QDomElement &becInput)
{
    QDomElement LightingSystem = createTagWithText(becInput, "LightingSystem");
    QDomElement ACSystem = createTagWithText(becInput, "ACSystem");
    QDomElement HotWaterSystem = createTagWithText(becInput, "HotWaterSystem");
    QDomElement OtherEquipment = createTagWithText(becInput, "OtherEquipment");
    doPV(model, becInput);

    doHotWaterSystem(model, HotWaterSystem);
    doACSystem(model, ACSystem);
    //doOtherEquipment(model, OtherEquipment);

    std::vector<openstudio::model::BuildingStory> stories = model.getConcreteModelObjects<openstudio::model::BuildingStory>();

    QHash<QString, bool> light_checkDup;
    QHash<QString, bool> oeq_checkDup;
    for (const openstudio::model::BuildingStory& buildingStory : stories) {
		qDebug() << "++ PROCESS BUILDING STORY : " << buildingStory.name().get().c_str();
        std::vector<openstudio::model::Space> spaces = buildingStory.spaces();
        for (openstudio::model::Space& space : spaces){
			qDebug() << "++ PROCESS BUILDING SPACE : " << space.name().get().c_str();
            doLightingSystem(space, LightingSystem, light_checkDup);
            doOtherEquipment(space, OtherEquipment, oeq_checkDup);

            openstudio::model::SurfaceVector surfaces = space.surfaces();
            for (openstudio::model::Surface& surface : surfaces){

            }
        }
    }

}

void ForwardTranslator::DoLighting(QDomElement &LightingSystem
                        , const std::vector<model::Lights>& lights
                        , QHash<QString, bool>& checkDup)
{
    for (const model::Lights& light : lights){
        QString name = light.lightsDefinition().name().get().c_str();
        if(checkDup.contains(name)){
            continue;
        }
        else{
            checkDup.insert(name, true);
        }
        if(light.lightsDefinition().lightingLevel()){
            QDomElement Lighting = createTagWithText(LightingSystem, "Lighting");
            createTagWithText(Lighting, "LightingSystemName"
                              , light.lightsDefinition().name().get().c_str());
            createTagWithText(Lighting, "LightingSystemPower", QString::number(light.lightsDefinition().lightingLevel().get()));
            createTagWithText(Lighting, "LightingSystemPowerUnit", "watt");
            createTagWithText(Lighting, "LightingSystemDescription", "???");
        }
        else if(light.lightsDefinition().wattsperSpaceFloorArea()){
            QDomElement Lighting = createTagWithText(LightingSystem, "Lighting");
            createTagWithText(Lighting, "LightingSystemName"
                              , light.lightsDefinition().name().get().c_str());
            createTagWithText(Lighting, "LightingSystemPower", QString::number(light.lightsDefinition().wattsperSpaceFloorArea().get()));
            createTagWithText(Lighting, "LightingSystemPowerUnit", "W/m2");
            createTagWithText(Lighting, "LightingSystemDescription", "???");
        }else if(light.lightsDefinition().wattsperPerson()){
            QDomElement Lighting = createTagWithText(LightingSystem, "Lighting");
            createTagWithText(Lighting, "LightingSystemName"
                              , light.lightsDefinition().name().get().c_str());
            createTagWithText(Lighting, "LightingSystemPower", QString::number(light.lightsDefinition().wattsperPerson().get()));
            createTagWithText(Lighting, "LightingSystemPowerUnit", "W/person");
            createTagWithText(Lighting, "LightingSystemDescription", "???");
        }
        else{
            QDomElement Lighting = createTagWithText(LightingSystem, "Lighting_error");
            createTagWithText(Lighting, "LightingSystemName"
                              , light.lightsDefinition().name().get().c_str());
            createTagWithText(Lighting, "LightingSystemPower", "0");
            createTagWithText(Lighting, "LightingSystemPowerUnit", "unknow");
            createTagWithText(Lighting, "LightingSystemDescription", "???");
        }
    }
}
void ForwardTranslator::doLightingSystem(const model::Space &space, QDomElement &LightingSystem, QHash<QString, bool>& checkDup)
{
	if(space.spaceType())
		DoLighting(LightingSystem, space.spaceType().get().lights(), checkDup);
	else
	{
		qDebug() << "DO LIGHTNG SPACE TYPE ERROR AT:" << space.name().get().c_str();
	}
    
	DoLighting(LightingSystem, space.lights(), checkDup);
}

void ForwardTranslator::DoElectricEquipment(std::vector<model::ElectricEquipment>& others, QDomElement &OtherEquipment, QHash<QString, bool>& checkDup){
    for (const model::ElectricEquipment& other : others){
        QString name = other.electricEquipmentDefinition().name().get().c_str();

        if(checkDup.contains(name)){
            continue;
        }
        else{
            checkDup.insert(name, true);
        }

        QString unit = "W";
        QDomElement OtherEQ = createTagWithText(OtherEquipment, "OtherEQ");
        createTagWithText(OtherEQ, "OtherEQName"
                          , name);
        if(other.electricEquipmentDefinition().designLevel()){
            createTagWithText(OtherEQ, "OtherEQPower", QString::number(other.electricEquipmentDefinition().designLevel().get()));
            unit = "watt";
        }else if(other.electricEquipmentDefinition().wattsperPerson()){
            createTagWithText(OtherEQ, "OtherEQPower", QString::number(other.electricEquipmentDefinition().wattsperPerson().get()));
            unit = "W/person";
        }else if(other.electricEquipmentDefinition().wattsperSpaceFloorArea()){
            createTagWithText(OtherEQ, "OtherEQPower", QString::number(other.electricEquipmentDefinition().wattsperSpaceFloorArea().get()));
            unit = "W/m2";
        }
        createTagWithText(OtherEQ, "OtherEQPowerUnit", unit);
        createTagWithText(OtherEQ, "OtherEQDescription", "???");
    }
}

void ForwardTranslator::DoOtherEquipment(std::vector<model::OtherEquipment>& electris, QDomElement &OtherEquipment, QHash<QString, bool>& checkDup){
    for (const model::OtherEquipment& other : electris){
        QString name = other.otherEquipmentDefinition().name().get().c_str();

        if(checkDup.contains(name)){
            continue;
        }
        else{
            checkDup.insert(name, true);
        }

        QString unit;
        QDomElement OtherEQ = createTagWithText(OtherEquipment, "OtherEQ");
        createTagWithText(OtherEQ, "OtherEQName"
                          , name);
        if(other.otherEquipmentDefinition().designLevel()){
            createTagWithText(OtherEQ, "OtherEQPower", QString::number(other.otherEquipmentDefinition().designLevel().get()));
            unit = "watt";
        }else if(other.otherEquipmentDefinition().wattsperPerson()){
            createTagWithText(OtherEQ, "OtherEQPower", QString::number(other.otherEquipmentDefinition().wattsperPerson().get()));
            unit = "W/person";
        }else if(other.otherEquipmentDefinition().wattsperSpaceFloorArea()){
            createTagWithText(OtherEQ, "OtherEQPower", QString::number(other.otherEquipmentDefinition().wattsperSpaceFloorArea().get()));
            unit = "W/m2";
        }
        createTagWithText(OtherEQ, "OtherEQPowerUnit", unit);
        createTagWithText(OtherEQ, "OtherEQDescription", "???");
    }
}

void ForwardTranslator::doOtherEquipment(const model::Space &space, QDomElement &OtherEquipment, QHash<QString, bool>& checkDup)
{
	if (space.spaceType()){
       DoElectricEquipment(space.spaceType().get().electricEquipment(), OtherEquipment, checkDup);
       DoOtherEquipment(space.spaceType().get().otherEquipment(), OtherEquipment, checkDup);
	}
	else
	{
		qDebug() << "OTHER EQUEPMENT ERROR AT SPACE:" << space.name().get().c_str();
	}

    DoElectricEquipment(space.electricEquipment(), OtherEquipment, checkDup);
    DoOtherEquipment(space.otherEquipment(), OtherEquipment, checkDup);
}

bool isSkip(model::ModelObject model){
    if(       model.iddObject().type().value() == IddObjectType::OS_Node
              || model.iddObject().type().value() == IddObjectType::OS_Pipe_Adiabatic
              || model.iddObject().type().value() == IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem
              || model.iddObject().type().value() == IddObjectType::OS_Connector_Splitter
              || model.iddObject().type().value() == IddObjectType::OS_SetpointManager_SingleZone_Reheat
              || model.iddObject().type().value() == IddObjectType::OS_Connector_Mixer
              || model.iddObject().type().value() == IddObjectType::OS_SetpointManager_Scheduled
              || model.iddObject().type().value() == IddObjectType::OS_AirLoopHVAC_ZoneSplitter
              || model.iddObject().type().value() == IddObjectType::OS_AirLoopHVAC_ZoneMixer
              || model.iddObject().type().value() == IddObjectType::OS_Fan_OnOff)
    {
        return true;
    }
    return false;
}

QString ForwardTranslator::GenParantLoopName(const QString &parantLoopName)
{
    const QRegExp rx("(\\d+)");
    QString numstr;
    QString loopName = parantLoopName;
    rx.indexIn(loopName);
    QStringList numls = rx.capturedTexts();
    if(numls.size()>0){
        numstr = numls.at(numls.size()-1);
    }
    loopName = QString("Central A/C ") + numstr;
    loopName = loopName.trimmed();

    return loopName;
}

void ForwardTranslator::doACSystem(const model::Model &model, QDomElement &ACSystem)
{
    QDomElement SplitTypeSystem = createTagWithText(ACSystem, "SplitTypeSystem");
    QDomElement PackagedAirCooledUnit = createTagWithText(ACSystem, "PackagedAirCooledUnit");
    QHash<QString, bool> duplicateEq;
    QHash<QString, bool> subDuplicateEq;
    QDomElement PackagedWaterCooledUnit = createTagWithText(ACSystem, "PackagedWaterCooledUnit");
    QDomElement CentralACSystem = createTagWithText(ACSystem, "CentralACSystem");
    //////////////////////////////////////////////////////////////////////////
    QDomElement CentralACList = createTagWithText(CentralACSystem, "CentralACList");
    std::vector<openstudio::model::AirLoopHVAC> airLoops = model.getConcreteModelObjects<openstudio::model::AirLoopHVAC>();
    std::vector<openstudio::model::PlantLoop> plantLoops = model.getConcreteModelObjects<openstudio::model::PlantLoop>();
    QDomElement CentralACDetail = createTagWithText(CentralACSystem, "CentralACDetail");
    for (model::AirLoopHVAC &airLoop : airLoops)
    {
        std::vector<model::ModelObject> supplys = airLoop.supplyComponents();
        QString listName = airLoop.name().get().c_str();

        if(listName.indexOf("Air Conditioner")>=0)
        {
            double scapacityW = 0;
            double spowerKW = 0;
            double scop = 0;
            for (const model::ModelObject & sup : supplys)
            {
                if (isSkip(sup)){
                    continue;
                }
                if(sup.iddObject().type().value() == openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed)
                {
                    model::CoilCoolingDXSingleSpeed coil = sup.cast<model::CoilCoolingDXSingleSpeed>();
                    double cop=0.0f, capW=0.0f, powerKw=0.0f;
                    takeCoilCoolingDxSinglespeedValues(coil, cop, capW, powerKw);
                    scapacityW += capW;
                    spowerKW += powerKw;
                    scop += cop;
                }
                else if(sup.iddObject().type().value() == openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed)
                {
                    model::CoilCoolingDXTwoSpeed coil = sup.cast<model::CoilCoolingDXTwoSpeed>();
                    double cop=0.0f, capW=0.0f, powerKw=0.0f;
                    takeCoilCoolingDXTwoSpeedValues(coil, cop, capW, powerKw);
                    scapacityW += capW;
                    spowerKW += powerKw;
                    scop += cop;
                }
            }

            QDomElement PackagedAirCooled = createTagWithText(PackagedAirCooledUnit
                                                                      , "PackagedAirCooled");
            createTagWithText(PackagedAirCooled, "PackagedAirCooledName", listName);
            createTagWithText(PackagedAirCooled, "PackagedAirCooledCoolingCapacity", QString::number(scapacityW));
                    createTagWithText(PackagedAirCooled, "PackagedAirCooledCoolingCapacityUnit", "W");
            createTagWithText(PackagedAirCooled, "PackagedAirCooledPower", QString::number(spowerKW));
                    createTagWithText(PackagedAirCooled, "PackagedAirCooledPowerUnit", "kW");
                    createTagWithText(PackagedAirCooled, "PackagedAirCooledDescription", "???");
            createTagWithText(PackagedAirCooled, "PackagedAirCooledCOP", QString::number(scop));
                    createTagWithText(PackagedAirCooled, "PackagedAirCooledkWth", "0");

            continue;
        }
        else if(listName.indexOf("Split Type")>=0)
        {
            double coolcap=0.0;
            for (size_t iloop=0; iloop<supplys.size(); iloop++){
                model::ModelObject & hvac = supplys.at(iloop);
                if(isSkip(hvac)){
                    continue;
                }
                switch(hvac.iddObject().type().value())
                {
                    case openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed :
                    {
                        model::CoilCoolingDXSingleSpeed coil = hvac.cast<model::CoilCoolingDXSingleSpeed>();
                        double cop=0.0f, capW=0.0f, powerKW=0.0f;
                        std::string name = coil.name().get();

                        if(takeCoilCoolingDxSinglespeedValues(coil, cop, capW, powerKW)){
                            QDomElement SplitType = createTagWithText(SplitTypeSystem, "SplitType");

                            createTagWithText(SplitType, "SplitTypeName", name.c_str());
                            createTagWithText(SplitType, "SplitTypeCoolingCapacity"
                                , QString::number(capW));
                            createTagWithText(SplitType, "SplitTypeCoolingCapacityUnit"
                                              , "W");
                            createTagWithText(SplitType, "SplitTypePower"
                                              , QString::number(powerKW));
                            createTagWithText(SplitType, "SplitTypePowerUnit", "W");
                            createTagWithText(SplitType, "SplitTypeDescription", "?");
                            createTagWithText(SplitType, "SplitTypeCOP", QString::number(cop));
                            createTagWithText(SplitType, "SplitTypekWth", "0");

                            if(coil.isRatedTotalCoolingCapacityAutosized()){
                                createTagWithText(SplitType, "AUTOSIZE", "true");
                            }else{
                                createTagWithText(SplitType, "AUTOSIZE", "false");
                            }
                        }else{
                            QDomElement SplitType = createTagWithText(SplitTypeSystem, "SplitType_ERROR");

                            createTagWithText(SplitType, "SplitTypeName", name.c_str());
                            createTagWithText(SplitType, "SplitTypeCoolingCapacity"
                                , QString::number(capW));
                            createTagWithText(SplitType, "SplitTypeCoolingCapacityUnit"
                                              , "W");
                            createTagWithText(SplitType, "SplitTypePower"
                                              , QString::number(powerKW));
                            //createTagWithText(SplitType, "SplitTypePower2"
                            //                  , QString::number(power2.get_value_or(0.0f)));
                            createTagWithText(SplitType, "SplitTypePowerUnit", "W");
                            createTagWithText(SplitType, "SplitTypeDescription", "?");
                            //TODO: CHANGE TO REAL COP
                            createTagWithText(SplitType, "SplitTypeCOP", QString::number(cop));
                            createTagWithText(SplitType, "SplitTypekWth", "0");

                            if(coil.isRatedTotalCoolingCapacityAutosized()){
                                createTagWithText(SplitType, "AUTOSIZE", "true");
                            }else{
                                createTagWithText(SplitType, "AUTOSIZE", "false");
                            }
                        }
                        break;
                    }
                    case openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed:
                    {
                        model::CoilCoolingDXTwoSpeed coil = hvac.cast<model::CoilCoolingDXTwoSpeed>();
                        QDomElement SplitType = createTagWithText(SplitTypeSystem, "SplitType");
                        //////////////////////////////////////////////////
                        std::string name = coil.name().get();
                        double cop=0.0f, capW=0.0f, powerKW=0.0f;
                        takeCoilCoolingDXTwoSpeedValues(coil, cop, capW, powerKW);

                        createTagWithText(SplitType, "SplitTypeName", name.c_str());
                        createTagWithText(SplitType, "SplitTypeCoolingCapacity"
                                          , QString::number(capW));
                        createTagWithText(SplitType, "SplitTypeCoolingCapacityUnit"
                                          , "W");
                        createTagWithText(SplitType, "SplitTypePower"
                                          , QString::number(powerKW));
                        createTagWithText(SplitType, "SplitTypePowerUnit", "kW");
                        createTagWithText(SplitType, "SplitTypeDescription", "?");
                        createTagWithText(SplitType, "SplitTypeCOP", QString::number(cop));
                        createTagWithText(SplitType, "SplitTypekWth", "0");
                        break;
                    }
                    case openstudio::IddObjectType::OS_Coil_Cooling_Water:{
                        model::CoilCoolingWater coolw = hvac.cast<model::CoilCoolingWater>();
                        coolcap = ((1.230*coolw.designAirFlowRate().get_value_or(0.0f))
                                         *(coolw.designInletAirTemperature().get_value_or(0.0f)-coolw.designOutletAirTemperature().get_value_or(0.0f)))
                                         +(3010*coolw.designAirFlowRate().get_value_or(0.0f)
                                         *(coolw.designInletAirHumidityRatio().get_value_or(0.0f)-coolw.designOutletAirHumidityRatio().get_value_or(0.0f)));
                    }
                    break;
                    case openstudio::IddObjectType::OS_Fan_VariableVolume:
                    {
						/*
                        model::FanVariableVolume fanv = hvac.cast<model::FanVariableVolume>();
                        QString name = fanv.name().get().c_str();
                        size_t inx = iloop+1;
                        while(inx < supplys.size()){
                            model::ModelObject & nextItem = supplys.at(inx);
                            if(isSkip(nextItem)){
                                inx++;
                            }else{
                                //if(nextItem.iddObject().type().value() == openstudio::IddObjectType::OS_Coil_Cooling_Water){
                                if(nextItem.iddObject().type().value() == openstudio::IddObjectType::OS_Coil_Cooling_Water){
                                    model::CoilCoolingWater coolw = nextItem.cast<model::CoilCoolingWater>();
                                    name = coolw.name().get().c_str();
                                    //name = QString("%1 And %2").arg(name).arg(coolw.name().get().c_str());

                                    coolcap = ((1.230*coolw.designAirFlowRate().get_value_or(0.0f))
                                                     *(coolw.designInletAirTemperature().get_value_or(0.0f)-coolw.designOutletAirTemperature().get_value_or(0.0f)))
                                                     +(3010*coolw.designAirFlowRate().get_value_or(0.0f)
                                                     *(coolw.designInletAirHumidityRatio().get_value_or(0.0f)-coolw.designOutletAirHumidityRatio().get_value_or(0.0f)));
                                    iloop = inx;
                                }
                                else{
                                    iloop = inx-1;
                                }
                                break;
                            }
                        }

                        QDomElement SplitType = createTagWithText(SplitTypeSystem, "SplitType");
                        //////////////////////////////////////////////////
                        if(fanv.fanEfficiency()==0.0f){
                            continue;
                        }

                        double maximumFlowRate = fanv.maximumFlowRate().get_value_or(1.0f);
                        double power = (maximumFlowRate*fanv.pressureRise())/(fanv.fanEfficiency()*1000);

                        createTagWithText(SplitType, "SplitTypeName", name);
                        createTagWithText(SplitType, "SplitTypeCoolingCapacity"
                                          , QString::number(coolcap));
                        createTagWithText(SplitType, "SplitTypeCoolingCapacityUnit"
                                          , "kW");
                        createTagWithText(SplitType, "SplitTypePower"
                                          , QString::number(power));
                        createTagWithText(SplitType, "SplitTypePowerUnit", "kW");
                        createTagWithText(SplitType, "SplitTypeDescription", "?");
                        createTagWithText(SplitType, "SplitTypeCOP", "0");
                        createTagWithText(SplitType, "SplitTypekWth", "0");
						*/
                        break;
                    }
                }
            }
            continue;
        }
        else if(listName.indexOf("Air Loop")>=0)
        {
            std::string parantLoopName;
            {
                std::vector<model::ModelObject> hvacs = airLoop.supplyComponents();
                for (const model::ModelObject & hvac : hvacs){
                    switch(hvac.iddObject().type().value())
                    {
                        case openstudio::IddObjectType::OS_Coil_Cooling_Water :

                        model::CoilCoolingWater coolw = hvac.cast<model::CoilCoolingWater>();
                        boost::optional<model::PlantLoop> attLoop = coolw.plantLoop();
                        boost::optional<model::AirLoopHVAC> hLoop = coolw.airLoopHVAC();
                        if(attLoop){
                            parantLoopName = attLoop.get().name().get();
                        }
                        else if(hLoop){
                            parantLoopName = hLoop.get().name().get();
                        }
                        break;
                    }
                }
            }

            QString loopName = GenParantLoopName(parantLoopName.c_str());
            qDebug() << QString("aaa Do air loop at aaa");
            doAirLoop(CentralACList, CentralACDetail, &airLoop, loopName, duplicateEq, subDuplicateEq);
            continue;
        }
    }

    for (model::PlantLoop &plantLoop : plantLoops) {
        QString listName = plantLoop.name().get().c_str();
        //processPlantLoop(CentralACList, CentralACDetail, &plantLoop);
        if(listName.indexOf("Air Loop")>=0)
        {
            qDebug() << QString("bbb Do air loop at bbb");
            doAirLoop(CentralACList, CentralACDetail, &plantLoop, QString(), duplicateEq, subDuplicateEq);
        }
    }
}

void ForwardTranslator::DoSupply(std::vector<model::ModelObject>& supplys
                                 , const std::string& listName
                                 , const std::string& loopName
                                 , QDomElement &CentralACList, QDomElement& CentralACDetail
                                 , QHash<QString, bool>& duplicate
                                 , QHash<QString, bool>& subitemDuplicate
                                 , const QString& parantLoopName)
{

    std::vector<boost::optional<model::AirLoopHVAC>> nextAirLoopHavc;
    std::vector<boost::optional<model::PlantLoop>> nextPlantLoopHavc;

    for(size_t i=0;i<supplys.size();i++){
        model::ModelObject & hvac = supplys.at(i);

        if(isSkip(hvac)){
            continue;
        }

        qDebug() << QString("[==]--------------- iddobj:'%1', name:'%2', listname:'%3', relation[%4], supply:%5")
                               .arg(hvac.iddObject().name().c_str())
                               .arg(hvac.name().get().c_str())
                               .arg(listName.c_str())
                                .arg(i)
                                .arg(supplys.size());

        //"[==]--------------- iddobj:'OS:Coil:Cooling:Water', name:'1AHU-01', listname:'Thai Chilled Water Loop', relation[3], supply:22"
        // case openstudio::IddObjectType::OS_Coil_Cooling_Water:

        if(hvac.iddObject().type().value() == openstudio::IddObjectType::OS_Coil_Cooling_Water
                && listName.find("Air Loop") == std::string::npos){
            qDebug() << "NOTE:Coil Cooling Water is on " << listName.c_str();
            continue;
        }

        QString cname = hvac.name().get().c_str();
        if(subitemDuplicate.contains(cname)){
            qDebug() << "IS DUPLATEED:" << cname;
            continue;
        }
        else{
            qDebug() << "ADD DUPLATE:" << cname;
            subitemDuplicate.insert(cname, true);
        }

        switch(hvac.iddObject().type().value())
        {
            case openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed :
            {
                model::CoilCoolingDXSingleSpeed coil = hvac.cast<model::CoilCoolingDXSingleSpeed>();

                std::string name = coil.name().get();
                double cop = coil.ratedCOP().get_value_or(1.0);
                double coolingcap = coil.ratedTotalCoolingCapacity().get_value_or(0.0);
                //boost::optional<double> power2 = coil.evaporativeCondenserPumpRatedPowerConsumption();
                if(cop == 0.0){
                    continue;
                }

                double power = coolingcap/cop;

                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACDetailName", name.c_str());
                chilledEq[listName.c_str()].append(name.c_str());
                createTagWithText(CentralACD, "CentralACDetailEQType", "Fan Coil Unit");
                createTagWithText(CentralACD, "CentralACDetailChillerType", "None");
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity", QString::number(coolingcap));
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "W");
                createTagWithText(CentralACD, "CentralACDetailPower", QString::number(power));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "W");
                createTagWithText(CentralACD, "CentralACDetailCOP", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACDetailkWth", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());

                if(coil.isRatedTotalCoolingCapacityAutosized()){
                    createTagWithText(CentralACD, "AUTOSIZE", "true");
                }else{
                    createTagWithText(CentralACD, "AUTOSIZE", "false");
                }
                break;
            }
            case openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed:
            {
                model::CoilCoolingDXTwoSpeed coil = hvac.cast<model::CoilCoolingDXTwoSpeed>();
                //////////////////////////////////////////////////
                std::string name = coil.name().get();

                double cop=0.0f, capW=0.0f, powerKW=0.0f;
                takeCoilCoolingDXTwoSpeedValues(coil, cop, capW, powerKW);

                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACDetailName", name.c_str());
                chilledEq[listName.c_str()].append(name.c_str());

                createTagWithText(CentralACD, "CentralACDetailEQType", "Fan Coil Unit");
                createTagWithText(CentralACD, "CentralACDetailChillerType", "None");
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity", QString::number(capW));
                //NOTE JUST W
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "W");
                createTagWithText(CentralACD, "CentralACDetailPower", QString::number(powerKW));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailCOP", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACDetailkWth", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());
                break;
            }
            case openstudio::IddObjectType::OS_Fan_VariableVolume:
            {
				/*
                model::FanVariableVolume fanv = hvac.cast<model::FanVariableVolume>();
                //////////////////////////////////////////////////
                std::string name = fanv.name().get();
                if(fanv.fanEfficiency()==0.0f){
                    continue;
                }
                double maximumFlowRate = fanv.maximumFlowRate().get_value_or(1.0f);
                double rfanv = fanv.fanEfficiency();

                if(rfanv==0.0)
                    continue;

                double powerKW = (maximumFlowRate*fanv.pressureRise())/(rfanv*1000);

                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACDetailName", name.c_str());
                chilledEq[listName.c_str()].append(name.c_str());

                createTagWithText(CentralACD, "CentralACDetailEQType", "Fan Coil Unit");
                createTagWithText(CentralACD, "CentralACDetailChillerType", "None");
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "W");
                createTagWithText(CentralACD, "CentralACDetailPower", QString::number(powerKW));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailCOP", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACDetailkWth", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());
				*/
                break;
            }
            case openstudio::IddObjectType::OS_Coil_Cooling_Water:
            {
				
                // TODO 004 COOLING TOWER
                model::CoilCoolingWater coolw = hvac.cast<model::CoilCoolingWater>();
                QString name = coolw.name().get().c_str();
                size_t inx = 0;
                double power=0.0;
                while(inx < supplys.size()){
                    model::ModelObject & nextItem = supplys.at(inx); 
					qDebug() << "---------------- loop on supply:" << coolw.name().get().c_str() << ":" << nextItem.name().get().c_str() << "----------------" << QString::number(inx) << "\n";
                    if(isSkip(nextItem)){
                        
                    }else{
                        if(nextItem.iddObject().type().value() == openstudio::IddObjectType::OS_Fan_VariableVolume){
							qDebug() << "!!! FOUND FOUND !!!! >> " <<  nextItem.name().get().c_str() << "\n";
                            model::FanVariableVolume fanv = nextItem.cast<model::FanVariableVolume>();
                            //name = QString("%1 And %2").arg(name).arg(fanv.name().get().c_str());
                            double maximumFlowRate = fanv.maximumFlowRate().get_value_or(1.0f);
                            double rfanv = fanv.fanEfficiency();

							if (rfanv != 0.0){
								power += (maximumFlowRate*fanv.pressureRise()) / (rfanv * 1000);
								qDebug() << "!!! POWER >>>>>  " << QString::number(power) << "\n";
							}
                        }
                    }
					inx++;
                }

                qDebug() << "\n\n\n\nname:" << coolw.name().get().c_str();
                qDebug() << QString("+++++++++++++++++++\ndesignAirFlowRate:%1\ndesignInletAirTemperature:%2\ndesignOutletAirTemperature:%3\ndesignOutletAirHumidityRatio:%4\n designInletAirHumidityRatio:%5")
                            .arg(coolw.designAirFlowRate().get_value_or(0.0))
                            .arg(coolw.designInletAirTemperature().get_value_or(0.0))
                            .arg(coolw.designOutletAirTemperature().get_value_or(0.0))
                            .arg(coolw.designOutletAirHumidityRatio().get_value_or(0.0))
                            .arg(coolw.designInletAirHumidityRatio().get_value_or(0.0));

                qDebug() << QString("XXXXXXXXXXXXXXXXXXXXX+\n1.230*designAirFlowRate:%1\ndesignInletAirTemperature-designOutletAirTemperature:%2\n 3010.0*coolw.designAirFlowRate():%3\ncoolw.designOutletAirHumidityRatio().get_value_or(0.0)-coolw.designInletAirHumidityRatio().get_value_or(0.0):%4")
                            .arg(1.230*coolw.designAirFlowRate().get_value_or(0.0))
                            .arg((coolw.designInletAirTemperature().get_value_or(0.0)-coolw.designOutletAirTemperature().get_value_or(0.0)))
                            .arg(3010.0*coolw.designAirFlowRate().get_value_or(0.0))
                            .arg(coolw.designInletAirHumidityRatio().get_value_or(0.0)-coolw.designOutletAirHumidityRatio().get_value_or(0.0));

                double coolcap = ((1.230*coolw.designAirFlowRate().get_value_or(0.0))
                                 *(coolw.designInletAirTemperature().get_value_or(0.0)-coolw.designOutletAirTemperature().get_value_or(0.0)))
                                 +(3010.0*coolw.designAirFlowRate().get_value_or(0.0)
                                 *(coolw.designInletAirHumidityRatio().get_value_or(0.0)-coolw.designOutletAirHumidityRatio().get_value_or(0.0)));

                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACDetailName", name);
                chilledEq[listName.c_str()].append(name);

                createTagWithText(CentralACD, "CentralACDetailEQType", "Air Handling Unit");
                createTagWithText(CentralACD, "CentralACDetailChillerType", "None");
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity", QString::number(coolcap));
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailPower", QString::number(power));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailCOP", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACDetailkWth", QString::number(0.0f));
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());

                boost::optional<model::PlantLoop> attLoop = coolw.plantLoop();
                if(attLoop){
					std::string strname = attLoop.get().name().get();
					qDebug() << "ccc Do air loop at ccc " << QString("%1:%2").arg(attLoop->name().get().c_str()).arg(strname.c_str()) << ":" << __LINE__;
					nextPlantLoopHavc.push_back(attLoop);
                }

                boost::optional<model::AirLoopHVAC> hLoop = coolw.airLoopHVAC();
                if(hLoop){
					std::string strname = hLoop.get().name().get();
                    qDebug() << "ddd Do air loop at ddd " << QString("%1:%2").arg(hLoop->name().get().c_str()).arg(strname.c_str()) << ":" <<__LINE__;
                    nextAirLoopHavc.push_back(hLoop);
                }

                break;
            }
            case IddObjectType::OS_Chiller_Electric_EIR:
            {
            // TODO 001 CHILLER
                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                model::ChillerElectricEIR chiller = hvac.cast<model::ChillerElectricEIR>();
                double cop = chiller.referenceCOP();
                double refcap = chiller.referenceCapacity().get_value_or(0.0f);

                if(cop==0.0)
                    break;

                QString name = hvac.name().get().c_str();
                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACDetailName", name);
                chilledEq[listName.c_str()].append(name);

                QString strType;
                if(chiller.condenserType() == "AirCooled"){
                    strType = "Air Cooled Chiller";
                }else if(chiller.condenserType() == "WaterCooled"){
                    strType = "Water Cooled Chiller";
                }else if(chiller.condenserType() == "EvaporativelyCooled"){
                    strType = "Evaporatively Cooled Chiller";
                }else{
                    strType = "None";
                }

                createTagWithText(CentralACD, "CentralACDetailEQType", strType);
                createTagWithText(CentralACD, "CentralACDetailChillerType", chiller.compressorType().c_str());
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity", QString::number(refcap/1000));
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailPower"
                                  , QString::number(refcap/(1000*cop)));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailCOP", QString::number(cop));
                createTagWithText(CentralACD, "CentralACDetailkWth", "0");
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());

                boost::optional<model::PlantLoop> attLoop = chiller.plantLoop();
                if(attLoop){
                    nextPlantLoopHavc.push_back(attLoop);
					std::string strname = attLoop.get().name().get();
                    qDebug() << "ggg Do air loop at ggg" << QString("%1:%2").arg(loopName.c_str()).arg(strname.c_str()) << ":" <<__LINE__;
                }

                boost::optional<model::PlantLoop> sattLoop = chiller.secondaryPlantLoop();
                if(sattLoop){
					std::string strname = sattLoop.get().name().get();
                    qDebug() << "fff Do air loop at fff" << QString("%1:%2").arg(loopName.c_str()).arg(strname.c_str()) << ":" <<__LINE__;
                    nextPlantLoopHavc.push_back(sattLoop);
                }
                break;
            }
            case openstudio::IddObjectType::OS_Pump_VariableSpeed:
            {
            // TODO 002 PUMP
                qDebug() << QString("xxxxxxxxxx supply:'%1', name:'%2', listname:'%3'")
                        .arg(hvac.iddObject().name().c_str())
                        .arg(hvac.name().get().c_str())
                        .arg(listName.c_str());


                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                model::PumpVariableSpeed pump = hvac.cast<model::PumpVariableSpeed>();
                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                QString name = hvac.name().get().c_str();
                createTagWithText(CentralACD, "CentralACDetailName", name);
                chilledEq[listName.c_str()].append(name);

                createTagWithText(CentralACD, "CentralACDetailEQType", "Chilled Water Pump");
                createTagWithText(CentralACD, "CentralACDetailChillerType", "None");
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity"
                                  , QString::number(0));
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailPower"
                                  , QString::number(pump.ratedPowerConsumption().get_value_or(0.0f)/1000));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailCOP", "0");
                createTagWithText(CentralACD, "CentralACDetailkWth", "0");
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());
                break;
            }
            // TODO 003 COOLING TOWER
            case IddObjectType::OS_CoolingTower_SingleSpeed:
            {
                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                model::CoolingTowerSingleSpeed tower = hvac.cast<model::CoolingTowerSingleSpeed>();

                double coolcap = tower.designWaterFlowRate().get_value_or(0.0f) * 18592.915;;
                double power = tower.fanPoweratDesignAirFlowRate().get_value_or(0.0f)/1000;

                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACDetailName", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACDetailEQType", "Cooling Tower");
                createTagWithText(CentralACD, "CentralACDetailChillerType", "All");
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity"
                                  , QString::number(coolcap));
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailPower"
                                  , QString::number(power));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailCOP", QString::number(0));
                createTagWithText(CentralACD, "CentralACDetailkWth", "0");
                createTagWithText(CentralACD, "plantLoop_name_success", hvac.name().get().c_str());
                createTagWithText(CentralACD, "iddname", hvac.iddObject().name().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());
                break;
            }

            // TODO 003 COOLING TOWER
            case IddObjectType::OS_CoolingTower_TwoSpeed:{
                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                model::CoolingTowerTwoSpeed tower = hvac.cast<model::CoolingTowerTwoSpeed>();

                double coolcap = tower.designWaterFlowRate().get_value_or(0.0f) * 18592.915;;
                double power = (tower.highFanSpeedFanPower().get_value_or(0.0f)+tower.lowFanSpeedFanPower().get_value_or(0.0f))/1000;

                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACDetailName", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACDetailEQType", "Cooling Tower");
                createTagWithText(CentralACD, "CentralACDetailChillerType", "All");
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity"
                                  , QString::number(coolcap));
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailPower"
                                  , QString::number(power));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailCOP", QString::number(0));
                createTagWithText(CentralACD, "CentralACDetailkWth", "0");
                createTagWithText(CentralACD, "plantLoop_name_success", hvac.name().get().c_str());
                createTagWithText(CentralACD, "iddname", hvac.iddObject().name().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());
                break;
            }
            case IddObjectType::OS_CoolingTower_VariableSpeed:{
            // TODO 003 COOLING TOWER
                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD");
                model::CoolingTowerVariableSpeed tower = hvac.cast<model::CoolingTowerVariableSpeed>();

                double coolcap = 1190.0 * 3.51685 * tower.designWaterFlowRate().get_value_or(0.0f) * tower.designRangeTemperature().get_value_or(0.0f);
                double power = tower.designFanPower().get_value_or(0.0f)/1000;

                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACDetailName", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACDetailEQType", "Cooling Tower");
                createTagWithText(CentralACD, "CentralACDetailChillerType", "All");
                createTagWithText(CentralACD, "CentralACDetailQuantity", "1");
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacity"
                                  , QString::number(coolcap));
                createTagWithText(CentralACD, "CentralACDetailCoolingCapacityUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailPower"
                                  , QString::number(power));
                createTagWithText(CentralACD, "CentralACDetailPowerUnit", "kW");
                createTagWithText(CentralACD, "CentralACDetailCOP", QString::number(0));
                createTagWithText(CentralACD, "CentralACDetailkWth", "0");
                createTagWithText(CentralACD, "plantLoop_name_success", hvac.name().get().c_str());
                createTagWithText(CentralACD, "iddname", hvac.iddObject().name().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());
                break;
            }
            default:
            {
                QDomElement CentralACD = createTagWithText(CentralACDetail, "CentralACD_ERROR");
                createTagWithText(CentralACD, "CentralACDetailListName", parantLoopName);
                createTagWithText(CentralACD, "CentralACDetailLoopName", listName.c_str());
                createTagWithText(CentralACD, "CentralACD_idd_name", hvac.name().get().c_str());
                createTagWithText(CentralACD, "CentralACD_idd_iddname", hvac.iddObject().name().c_str());
                break;
            }
        }

    }

    for (size_t i = 0; i < nextAirLoopHavc.size(); ++i) {
        boost::optional<model::AirLoopHVAC> airLoop = nextAirLoopHavc.at(i);
        if(airLoop){
            std::string strname = airLoop.get().name().get();
            qDebug() << "ggg Do air loop at ggg" << QString("%1:%2").arg(loopName.c_str()).arg(strname.c_str()) << ":" <<__LINE__;
            doAirLoop(CentralACList, CentralACDetail, &airLoop.get(), parantLoopName, duplicate, subitemDuplicate);
        }
    }

    for (size_t i = 0; i < nextPlantLoopHavc.size(); ++i) {
        boost::optional<model::PlantLoop> plantLoop = nextPlantLoopHavc.at(i);
        if(plantLoop){
            std::string strname = plantLoop.get().name().get();
            qDebug() << "hhh Do air loop at hhh" << QString("%1:%2").arg(loopName.c_str()).arg(strname.c_str()) << ":" <<__LINE__;
            doAirLoop(CentralACList, CentralACDetail, &plantLoop.get(), parantLoopName, duplicate, subitemDuplicate);
        }
    }
}

void ForwardTranslator::doAirLoop(QDomElement& CentralACList
                                  , QDomElement CentralACDetail
                                  , model::Loop* loop
                                  , const QString parantLoopName
                                  , QHash<QString, bool>& duplicate
                                  , QHash<QString, bool>& subitemDuplicate)
{
    QString listName = loop->name().get().c_str();
    QString baseLoop = listName;
    qDebug() << "SSSSSSSSSSSSSSSSSSS start doAirLoop:" << baseLoop;

    if(listName.indexOf("Condenser") >= 0){
        qDebug() << "Break here..." << listName << ", baseloop:" << baseLoop;
    }

    if(duplicate.contains(listName)){
        return;
    }
    else{
        duplicate.insert(listName, true);
        qDebug() << "Add duplicate list name:" << listName << ", baseloop:" << baseLoop;
        if(listName.indexOf("Chilled ")>=0){
            QDomElement CentralACL = createTagWithText(CentralACList, "CentralACL");
            createTagWithText(CentralACL, "CentralACListName", GenParantLoopName(listName));
			createTagWithText(CentralACL, "CentralACListType", loop->iddObject().name().c_str());
            int size = chilledEq[listName].size();
            createTagWithText(CentralACL, "CentralACListEQ", QString::number(size));
        }
    }

    std::vector<model::ModelObject> supplys = loop->supplyComponents();
    qDebug() << QString("==================== LOOP FOR SUPPLY:'%1' size:'%2'").arg(listName).arg(supplys.size());
	DoSupply(loop->supplyComponents(), listName.toStdString(), loop->name().get().c_str()
             , CentralACList, CentralACDetail
             , duplicate
			 , subitemDuplicate
             , parantLoopName);

    std::vector<model::ModelObject> demand = loop->demandComponents();
    qDebug() << QString(">>>>>>>>>>>>>>>>>>>> LOOP FOR DEMAND:'%1' size:'%2'").arg(listName).arg(demand.size());
    createTagWithText(CentralACDetail, "demandComponents_demandComponents");
    DoSupply(loop->demandComponents(), listName.toStdString(), loop->name().get().c_str()
             , CentralACList, CentralACDetail
             , duplicate
             , subitemDuplicate
             , parantLoopName);
}

void ForwardTranslator::doBuildingType(QDomElement &becInput, const QString& typeName)
{
    QDomElement BT = createTagWithText(becInput, "BT");
    QDomElement BuildType = createTagWithText(BT, "BuildType");
    createTagWithText(BuildType, "BuildingType", typeName);
}

void ForwardTranslator::doPV(const model::Model &model, QDomElement &becInput)
{
    QDomElement PVSystem = createTagWithText(becInput, "PVSystem");

    std::vector<model::Photovoltaic> pvs = model.getModelObjects<model::Photovoltaic>();
    for (model::Photovoltaic& pv : pvs){
        QDomElement PV = createTagWithText(PVSystem, "PV");
        createTagWithText(PV, "PVName", pv.name().get().c_str());
        createTagWithText(PV, "PVSystemEfficiency", QString::number(pv.cellEfficiency()));
        createTagWithText(PV, "PVSystemEfficiencyUnit", "%");
        createTagWithText(PV, "PVModuleArea", QString::number(pv.surfaceArea()));
        createTagWithText(PV, "PVModuleAreaUnit", "m^2");
        createTagWithText(PV, "PVAzimuth", QString::number(pv.azimuthAngle()));
        createTagWithText(PV, "PVAzimuthUnit", "degrees");
        createTagWithText(PV, "PVInclination", QString::number(pv.inclinationAngle()));
        createTagWithText(PV, "PVInclinationUnit", "degrees");
    }

    std::vector<model::PhotovoltaicThermal> pvst = model.getModelObjects<model::PhotovoltaicThermal>();
    for (model::PhotovoltaicThermal& pv : pvst){
        QDomElement PV = createTagWithText(PVSystem, "PV");
        createTagWithText(PV, "PVName", pv.name().get().c_str());
        createTagWithText(PV, "PVSystemEfficiency", QString::number(pv.cellEfficiency()));
        createTagWithText(PV, "PVSystemEfficiencyUnit", "%");
        createTagWithText(PV, "PVModuleArea", QString::number(pv.surfaceArea()));
        createTagWithText(PV, "PVModuleAreaUnit", "m^2");
        createTagWithText(PV, "PVAzimuth", QString::number(pv.azimuthAngle()));
        createTagWithText(PV, "PVAzimuthUnit", "degrees");
        createTagWithText(PV, "PVInclination", QString::number(pv.inclinationAngle()));
        createTagWithText(PV, "PVInclinationUnit", "degrees");
    }
}

void ForwardTranslator::doHotWaterSystem(const model::Model &model, QDomElement &hotWaterSystem)
{
    std::vector<openstudio::model::HVACComponent> hvacs = model.getModelObjects<model::HVACComponent>();
    for (const model::HVACComponent & hvac : hvacs) {

        boost::optional<openstudio::model::BoilerHotWater> isHW
                = hvac.optionalCast<openstudio::model::BoilerHotWater>();

        if(isHW){
            openstudio::model::BoilerHotWater boiler = isHW.get();
            QDomElement hw = createTagWithText(hotWaterSystem, "HW");
            createTagWithText(hw, "HWName"
                              , boiler.name().get().c_str());
            createTagWithText(hw, "HWBoilerType", "Gas Fired Hot Water Boiler");
            createTagWithText(hw, "HWBoilerEfficiency", QString::number(boiler.nominalThermalEfficiency()*100));
            createTagWithText(hw, "HWBoilerEfficiencyUnit", "%");
            createTagWithText(hw, "HWHeatPumpType", "None");
            //boiler.children.r.briefDescription.boilerFlowMode.ratedHeatingCOP()
            //WARNING: NOTING
            createTagWithText(hw, "HWHeatPumpCOP", "0");
            createTagWithText(hw, "HWDescription", "");
            continue;
        }
    }
}

void ForwardTranslator::DoLightBuilidingZone(const std::string & zonelistName
                                  , const std::vector<model::Lights>& lights
                                  , const std::vector<model::OtherEquipment>& others
                                  , const std::vector<model::ElectricEquipment>& elects){

    if(lights.size()>0){
        for (const model::Lights& light : lights){
            QDomElement BuildingZoneLighting = createTagWithText(BuildingZoneLightingEQ, "BuildingZoneLighting");
            createTagWithText(BuildingZoneLighting, "BuildingZoneLightingListName"
                              , zonelistName.c_str());
            createTagWithText(BuildingZoneLighting, "BuildingZoneLightingName"
                              , light.lightsDefinition().name().get().c_str());
            createTagWithText(BuildingZoneLighting, "BuildingZoneLightingQuantity"
                              , QString::number(light.quantity()));
            createTagWithText(BuildingZoneLighting, "BuildingZoneLightingDaylighted", "0");
        }
    }

    for (const model::OtherEquipment& other : others){
        QDomElement BuildingZoneOtherEQ = createTagWithText(BuildingZoneOtherEquipment, "BuildingZoneOtherEQ");
        createTagWithText(BuildingZoneOtherEQ, "BuildingZoneOtherEQListName"
                          , zonelistName.c_str());
        createTagWithText(BuildingZoneOtherEQ, "BuildingZoneOtherEQName"
                          , other.otherEquipmentDefinition().name().get().c_str());
        createTagWithText(BuildingZoneOtherEQ, "BuildingZoneOtherEQQuantity"
                          , QString::number(other.quantity()));
    }

    for (const model::ElectricEquipment& other : elects){
        QDomElement BuildingZoneOtherEQ = createTagWithText(BuildingZoneOtherEquipment, "BuildingZoneOtherEQ");
        createTagWithText(BuildingZoneOtherEQ, "BuildingZoneOtherEQListName"
                          , zonelistName.c_str());
        createTagWithText(BuildingZoneOtherEQ, "BuildingZoneOtherEQName"
                          , other.electricEquipmentDefinition().name().get().c_str());
        createTagWithText(BuildingZoneOtherEQ, "BuildingZoneOtherEQQuantity"
                          , QString::number(other.quantity()));
    }
}

void ForwardTranslator::doBuildingEnvelope(const model::Model &model, QDomElement &becInput)
{
    // Get stories
    std::vector<openstudio::model::BuildingStory> stories = model.getConcreteModelObjects<openstudio::model::BuildingStory>();
    // We will need each story to have an elevation and a floor-to-floor height.
    int floor = 1;
    for (const openstudio::model::BuildingStory& buildingStory : stories) {

        std::vector<openstudio::model::Space> spaces = buildingStory.spaces();
        for (model::Space& space : spaces){

            QDomElement buildingZoneL = _doc->createElement("BuildingZoneL");
            buildingZoneList.appendChild(buildingZoneL);

            //createTagWithText(buildingZoneL, "BuildingZoneListTYPE", surface.surfaceType().c_str());
            //createTagWithText(buildingZoneL, "BuildingZoneListIsGround", QString::number(surface.isGroundSurface()));
            std::string zonelistName = space.name().get();

            DoLightBuilidingZone(zonelistName
                                  , space.spaceType().get().lights()
                                  , space.spaceType().get().otherEquipment()
                                  , space.spaceType().get().electricEquipment());

            DoLightBuilidingZone(zonelistName
                                  , space.lights()
                                  , space.otherEquipment()
                                  , space.electricEquipment());

            if(boost::optional<model::ThermalZone> thermal = space.thermalZone()){

                QString loopName;
                QString loopType;
                boost::optional<model::AirLoopHVAC> airloop =  thermal.get().airLoopHVAC();
                if(airloop){
                    loopName = airloop.get().name().get().c_str();
                    loopType = "air";
                }

                boost::optional<model::PlantLoop> planloop = thermal.get().plantLoop();
                if(planloop){
                    loopName = planloop.get().name().get().c_str();
                    loopType = "plant";
                }

                if(loopName.indexOf("Split Type Air")>=0){
                    if(airloop){
                        std::vector<model::ModelObject> hvacs = airloop.get().supplyComponents();
                        for (const model::ModelObject & hvac : hvacs){
                            switch(hvac.iddObject().type().value())
                            {
                                case openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed :
                                case openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed:
                                QDomElement BuildingZoneDXAC
                                        = createTagWithText(BuildingZoneDXACUnit, "BuildingZoneDXAC");
                                createTagWithText(BuildingZoneDXAC, "BuildingZoneDXACListName", space.name().get().c_str());
                                createTagWithText(BuildingZoneDXAC, "BuildingZoneDXACName", hvac.name().get().c_str());
                                createTagWithText(BuildingZoneDXAC, "BuildingZoneDXACQuantity", QString::number(thermal.get().multiplier()));
                                break;
                            }
                        }
                    }
                }
                else if(loopName.indexOf("Air Loop")>=0){
					if (airloop){
                        std::vector<model::ModelObject> hvacs = airloop.get().supplyComponents();
                        for (const model::ModelObject & hvac : hvacs){
                            switch(hvac.iddObject().type().value())
                            {
                                case openstudio::IddObjectType::OS_Coil_Cooling_Water :

                                model::CoilCoolingWater coolw = hvac.cast<model::CoilCoolingWater>();
                                boost::optional<model::PlantLoop> attLoop = coolw.plantLoop();
                                boost::optional<model::AirLoopHVAC> hLoop = coolw.airLoopHVAC();
                                std::string parantLoopName;
                                if(attLoop){
                                    parantLoopName = attLoop.get().name().get();
                                }
                                else if(hLoop){
                                    parantLoopName = hLoop.get().name().get();
                                }

                                QDomElement BuildingZoneCentralAC
                                        = createTagWithText(BuildingZoneCentralACEQ, "BuildingZoneCentralAC");
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACListName", space.name().get().c_str());
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACSystemName", GenParantLoopName(parantLoopName.c_str()));
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACEQName", hvac.name().get().c_str());
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACQuantity", QString::number(1));
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACType", "LoopAir");
                                break;
                            }
                        }
                    }

					if (planloop){
                        std::vector<model::ModelObject> hvacs = planloop.get().supplyComponents();
                        for (const model::ModelObject & hvac : hvacs){
                            switch(hvac.iddObject().type().value())
                            {
                                case openstudio::IddObjectType::OS_Coil_Cooling_Water :
                                QDomElement BuildingZoneCentralAC
                                        = createTagWithText(BuildingZoneCentralACEQ, "BuildingZoneCentralAC");
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACListName", space.name().get().c_str());
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACSystemName", loopName);
								createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACEQName", hvac.name().get().c_str());
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACQuantity", QString::number(1));
                                createTagWithText(BuildingZoneCentralAC, "BuildingZoneCentralACType", "PlanLoop");
                                break;
                            }
                        }
                    }
                }
                else if(loopName.indexOf("Air Conditioner")>=0)
                {
                    QDomElement BuildingZoneDXAC
                            = createTagWithText(BuildingZoneDXACUnit, "BuildingZoneDXAC");
                    createTagWithText(BuildingZoneDXAC, "BuildingZoneDXACListName", space.name().get().c_str());
                    createTagWithText(BuildingZoneDXAC, "BuildingZoneDXACName", loopName);
                    createTagWithText(BuildingZoneDXAC, "BuildingZoneDXACQuantity", QString::number(thermal.get().multiplier()));
                }else{
                    QDomElement BuildingZoneDXAC
                            = createTagWithText(BuildingZoneDXACUnit, "BuildingZoneDXAC_UNKNOW");
                    createTagWithText(BuildingZoneDXAC, "LOOP_NAME", loopName);
                    createTagWithText(BuildingZoneDXAC, "SPACE_NAME", space.name().get().c_str());
                    createTagWithText(BuildingZoneDXAC, "loop_type", loopType);
                }
            }

            if(zonelistName.empty())
                zonelistName = std::string("ZONE_") + QString::number(floor).toStdString();

            createTagWithText(buildingZoneL, "BuildingZoneListName", zonelistName.c_str());
            createTagWithText(buildingZoneL, "BuildingZoneListFloor", buildingStory.name().get().c_str());
            createTagWithText(buildingZoneL, "BuildingZoneListDescription", "???");

            double floorArea = 0.0;
            openstudio::model::SurfaceVector surfaces = space.surfaces();
            for (openstudio::model::Surface& surface : surfaces){
                if(surface.surfaceType() == "Floor"){
                    floorArea+= surface.netArea();
                }
                else if(surface.surfaceType() == "Wall" || surface.surfaceType() == "RoofCeiling"){
                    //buildingZoneExteriorWall
                    QDomElement buildingZoneWall = createTagWithText(buildingZoneExteriorWall, "BuildingZoneWall");
                    createTagWithText(buildingZoneWall, "BuildingZoneWallListName", zonelistName.c_str());
                    //createTagWithText(buildingZoneWall, "BuildingZoneWallWallName", surface.surfaceType().c_str());
                    createTagWithText(buildingZoneWall, "BuildingZoneWallWallName", surface.name().get().c_str());
                    //TODO:LINK TO MATERIAL.
                    double area = surface.netArea();
                    createTagWithText(buildingZoneWall, "BuildingZoneWallSectionName", surface.name().get().c_str());

                    openstudio::model::SubSurfaceVector subs = surface.subSurfaces();

                    for (openstudio::model::SubSurface& sub : subs){
                        area += sub.netArea();

                        /*
                        //buildingZoneExteriorWall
                        QDomElement buildingZoneWall = createTagWithText(buildingZoneExteriorWall, "BuildingZoneWall");
                        createTagWithText(buildingZoneWall, "BuildingZoneWallListName", zonelistName.c_str());
                        //createTagWithText(buildingZoneWall, "BuildingZoneWallWallName", surface.surfaceType().c_str());
                        createTagWithText(buildingZoneWall, "BuildingZoneWallWallName", surface.name().get().c_str());
                        //TODO:LINK TO MATERIAL.
                        createTagWithText(buildingZoneWall, "BuildingZoneWallSectionName", sub.name().get().c_str());
                        createTagWithText(buildingZoneWall, "BuildingZoneWallArea", QString::number(area));
                        createTagWithText(buildingZoneWall, "BuildingZoneWallAreaUnit", "m^2");*/
                    }

                    createTagWithText(buildingZoneWall, "BuildingZoneWallArea", QString::number(area));
                    createTagWithText(buildingZoneWall, "BuildingZoneWallAreaUnit", "m^2");

                }
                else{ //RoofCeiling

                }
            }
            createTagWithText(buildingZoneL, "BuildingZoneListArea", QString::number(floorArea));
            createTagWithText(buildingZoneL, "BuildingZoneListAreaUnit", "m^2");
        }
        floor++;
    }
}

void ForwardTranslator::takeCoilCoolingDXTwoSpeedValues(const openstudio::model::CoilCoolingDXTwoSpeed &coil, double &cop, double &capW, double &power_kW)
{
    boost::optional<double> _cop = coil.ratedHighSpeedCOP();
    boost::optional<double> _coolingcap = coil.ratedHighSpeedTotalCoolingCapacity();
    boost::optional<double> _power = coil.highSpeedEvaporativeCondenserPumpRatedPowerConsumption();
    capW = cop = power_kW = 0.0;
    if(_cop){
       cop = _cop.get();
    }

    if(_coolingcap){
       capW = _coolingcap.get();
    }

    if(_power){
        power_kW = _power.get()/1000.0;
    }
}

bool ForwardTranslator::takeCoilCoolingDxSinglespeedValues(const openstudio::model::CoilCoolingDXSingleSpeed& coil
                                             , double& cop, double& capW, double& powerW)
{
    cop = coil.ratedCOP().get_value_or(1.0);
    capW = coil.ratedTotalCoolingCapacity().get_value_or(0.0);

    if(cop==0.0){
        powerW = 0.0f;
        return false;
    }

    powerW = capW/cop;
    return true;
}

boost::optional<QDomDocument> ForwardTranslator::translateModel(const openstudio::model::Model& model, const QString& type)
{
    QDomDocument doc;

    QDomProcessingInstruction instr = doc.createProcessingInstruction(
                "xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instr);

    QDomElement becInput = doc.createElement("BECInput");
    _doc = &doc;
    doc.appendChild(becInput);

    chilledEq.clear();
    //////////////////

    buildingEnvelope = _doc->createElement("BuildingEnvelope");
    becInput.appendChild(buildingEnvelope);

    buildingZoneList = _doc->createElement("BuildingZoneList");
    buildingEnvelope.appendChild(buildingZoneList);

    buildingZoneDetail = _doc->createElement("BuildingZoneDetail");
    buildingEnvelope.appendChild(buildingZoneDetail);

    buildingZoneExteriorWall = createTagWithText(buildingZoneDetail
                                                             , "BuildingZoneExteriorWall");

    BuildingZoneLightingEQ = createTagWithText(buildingZoneDetail
                                                           , "BuildingZoneLightingEQ");

    BuildingZoneDXACUnit
            = createTagWithText(buildingZoneDetail, "BuildingZoneDXACUnit");

    BuildingZoneCentralACEQ
            = createTagWithText(buildingZoneDetail, "BuildingZoneCentralACEQ");

    BuildingZoneOtherEquipment
            = createTagWithText(buildingZoneDetail, "BuildingZoneOtherEquipment");


    ////////////////////

    doBuildingType(becInput, type);
    doEnvelope(model, becInput);
    doModelLoop(model, becInput);

    //doLightingSystem(model, BECInput);
    //doACSystem(model, BECInput);
    //doPVSystem(model, BECInput);
    //doHotWaterSystem(model, BECInput);
    //doOtherEquipment(model, BECInput);
    doBuildingEnvelope(model, becInput);

    //boost::optional<QDomElement> myModel = translateMyModel(model,doc);
    //BECElement.appendChild(*myModel);

    /*
    boost::optional<model::Facility> facility = model.getOptionalUniqueModelObject<model::Facility>();
    if (facility){
      boost::optional<QDomElement> campusElement = translateFacility(*facility, doc);
      if (campusElement){
        BECElement.appendChild(*campusElement);
      }
    }
    */
    /*
    // do constructions
    for (const model::ConstructionBase& constructionBase : model.getModelObjects<model::ConstructionBase>()){
      boost::optional<QDomElement> constructionElement = translateConstructionBase(constructionBase, doc);
      if (constructionElement){
        projectElement.appendChild(*constructionElement);
      }
    }

    // do materials
    for (const model::Material& material : model.getModelObjects<model::Material>()){
      boost::optional<QDomElement> materialElement = translateMaterial(material, doc);
      if (materialElement){
        projectElement.appendChild(*materialElement);
      }
    }
*/
    /*
    // do thermal zones
    std::vector<model::ThermalZone> thermalZones = model.getConcreteModelObjects<model::ThermalZone>();
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Thermal Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)thermalZones.size());
      m_progressBar->setValue(0);
    }

    for (const model::ThermalZone& thermalZone : thermalZones){
      boost::optional<QDomElement> zoneElement = translateThermalZone(thermalZone, doc);
      if (zoneElement){
        BECElement.appendChild(*zoneElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }
    */

    return doc;
}

boost::optional<QDomElement> ForwardTranslator::translateMyModel(const openstudio::model::Model& model, QDomDocument& doc)
{
    std::string name;
    //double value;
    QDomElement result = doc.createElement("MyModel");

    // translate building
    boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();

    if (building){
        QDomElement buildingElement = doc.createElement("Building");
        name = *building.get().name();
        buildingElement.setAttribute("Name", escapeName(name));
        QDomElement buildingStoryElement = doc.createElement("buildingStory");

        // Space
        boost::optional<QDomElement> spaceElement = translateMySpace(*building,doc);
        /*
          QDomElement spaceElement = doc.createElement("Spaces");
          std::vector<model::Space> spaces = building.get().spaces();

          for (const model::Space& space : spaces){

              QDomElement m_spaceElement = doc.createElement("Space");
              name = *space.name();
              m_spaceElement.setAttribute("id", escapeName(name));

              // Space Type name
              std::string space_name = *space.spaceType().get().name();
              m_spaceElement.setAttribute("name", escapeName(space_name));
              spaceElement.appendChild(m_spaceElement);

              QDomElement m_area = doc.createElement("Area");
              double area = space.floorArea();
              m_area.appendChild(doc.createTextNode(QString::number(area)));
              m_spaceElement.appendChild(m_area);

              QDomElement m_valume = doc.createElement("Valume");
              double valume = space.volume();
              m_valume.appendChild(doc.createTextNode(QString::number(valume)));
              m_spaceElement.appendChild(m_valume);


              QDomElement loadElement = doc.createElement("Load");
              */
        /*
              std::vector<model::Lights> v_light = space.spaceType().get().lights();
              std::sort(v_light.begin(), v_light.end(), WorkspaceObjectNameLess());

              QDomElement nameElement;
              QDomElement lightElement;

              for (const model::Lights& i_light : v_light){

                  lightElement = doc.createElement("Light");

                  //!- Name
                  nameElement = doc.createElement("Name");
                  boost::optional<std::string> lightsDefinition = i_light.lightsDefinition().name().get();
                  nameElement.appendChild(doc.createTextNode(escapeName(*lightsDefinition)));
                  lightElement.appendChild(nameElement);

                  //!- Calculaion Method
                  std::string calculationMethod = i_light.lightsDefinition().designLevelCalculationMethod();
                  nameElement = doc.createElement("CalculationMethod");
                  nameElement.appendChild(doc.createTextNode(escapeName(calculationMethod)));
                  lightElement.appendChild(nameElement);

                  //!- Light Power
                  double lightPower = i_light.lightingLevel().get();
                  nameElement = doc.createElement("LightingLevel");
                  nameElement.appendChild(doc.createTextNode(QString::number(lightPower)));
                  lightElement.appendChild(nameElement);

                  //!- EnergyPerSpace
                  lightPower = i_light.powerPerFloorArea().get();
                  nameElement = doc.createElement("PowerPerArea");
                  nameElement.appendChild(doc.createTextNode(QString::number(lightPower)));
                  lightElement.appendChild(nameElement);

                  //!- EnergyPerPeople
                  lightPower = i_light.powerPerPerson().get();
                  nameElement = doc.createElement("PowerPerPerson");
                  nameElement.appendChild(doc.createTextNode(QString::number(lightPower)));
                  lightElement.appendChild(nameElement);

                  //!- Multiplier
                  double multipiler = i_light.multiplier();
                  nameElement = doc.createElement("Multiplier");
                  nameElement.appendChild(doc.createTextNode(QString::number(multipiler)));
                  lightElement.appendChild(nameElement);

                  loadElement.appendChild(lightElement);
              }

              */

        /*
              std::vector<model::OtherEquipment> v_otherEquipment = space.spaceType().get().otherEquipment();
              std::sort(v_otherEquipment.begin(), v_otherEquipment.end(), WorkspaceObjectNameLess());


              for (const model::OtherEquipment& i_otherEquipment : v_otherEquipment){
                  QDomElement otherEquipment = doc.createElement("OtherEquipment");
                  loadElement.appendChild(otherEquipment);

                  //!- Name
                  name = i_otherEquipment.otherEquipmentDefinition().name().get();
                  QDomElement nameElement = doc.createElement("Name");
                  nameElement.appendChild(doc.createTextNode(escapeName(name)));
                  otherEquipment.appendChild(nameElement);

                  //!- Calculation Method
                  std::string calculationMethod = i_otherEquipment.otherEquipmentDefinition().designLevelCalculationMethod();
                  nameElement = doc.createElement("CalculationMethod");
                  nameElement.appendChild(doc.createTextNode(escapeName(calculationMethod)));
                  otherEquipment.appendChild(nameElement);

                  //!- Design Level
                  double designLevel = i_otherEquipment.otherEquipmentDefinition().designLevel().get();
                  nameElement = doc.createElement("DesignLevel");
                  nameElement.appendChild(doc.createTextNode(QString::number(designLevel)));
                  otherEquipment.appendChild(nameElement);

                  //!- Watt Per Space
                  double wattPerSpace = i_otherEquipment.otherEquipmentDefinition().wattsperSpaceFloorArea().get();
                  nameElement = doc.createElement("WattPerSpace");
                  nameElement.appendChild(doc.createTextNode(QString::number(wattPerSpace)));
                  otherEquipment.appendChild(nameElement);

                  //!- Watt Per Space
                  double wattPerPerson = i_otherEquipment.otherEquipmentDefinition().wattsperPerson().get();
                  nameElement = doc.createElement("WattPerPerson");
                  nameElement.appendChild(doc.createTextNode(QString::number(wattPerPerson)));
                  otherEquipment.appendChild(nameElement);

                  //!- Multipiler
                  double muliplier = i_otherEquipment.multiplier();
                  nameElement = doc.createElement("Multipiler");
                  nameElement.appendChild(doc.createTextNode(QString::number(muliplier)));
                  otherEquipment.appendChild(nameElement);

              }
              */
        /*
              std::vector<model::ElectricEquipment> v_electricEquipment = space.spaceType().get().electricEquipment();
              std::sort(v_electricEquipment.begin(), v_electricEquipment.end(), WorkspaceObjectNameLess());

              for (const model::ElectricEquipment& i_electricEquipment : v_electricEquipment){

                  QDomElement electricEquipment = doc.createElement("ElectricEquipment");
                  loadElement.appendChild(electricEquipment);

                  //!- Name
                  name = i_electricEquipment.name().get();
                  QDomElement nameElement = doc.createElement("Name");
                  nameElement.appendChild(doc.createTextNode(escapeName(name)));
                  electricEquipment.appendChild(nameElement);

                  //!- Calculation Method
                  std::string calculationMethod = i_electricEquipment.electricEquipmentDefinition().designLevelCalculationMethod();
                  nameElement = doc.createElement("CalculationMethod");
                  nameElement.appendChild(doc.createTextNode(escapeName(calculationMethod)));
                  electricEquipment.appendChild(nameElement);

                  //!- Design Level
                  double designLevel = i_electricEquipment.designLevel().get();
                  nameElement = doc.createElement("DesignLevel");
                  nameElement.appendChild(doc.createTextNode(QString::number(designLevel)));
                  electricEquipment.appendChild(nameElement);

                  //!- Watt Per Space
                  double wattPerSpace = i_electricEquipment.powerPerFloorArea().get();
                  nameElement = doc.createElement("WattPerSpace");
                  nameElement.appendChild(doc.createTextNode(QString::number(wattPerSpace)));
                  electricEquipment.appendChild(nameElement);

                  //!- Watt Per Person
                  double wattPerPerson = i_electricEquipment.powerPerPerson().get();
                  nameElement = doc.createElement("WattPerPerson");
                  nameElement.appendChild(doc.createTextNode(QString::number(wattPerPerson)));
                  electricEquipment.appendChild(nameElement);

                  //!- Multipiler
                  double muliplier = i_electricEquipment.multiplier();
                  nameElement = doc.createElement("Multipiler");
                  nameElement.appendChild(doc.createTextNode(QString::number(muliplier)));
                  electricEquipment.appendChild(nameElement);
              }
              */
        /*
              m_spaceElement.appendChild(loadElement);

          }
          */

        QDomElement hvacElement = doc.createElement("HVAC");

        QDomElement materialsElement = doc.createElement("Materails");
        std::vector<model::Material> materials = model.getModelObjects<model::Material>();
        std::sort(materials.begin(), materials.end(), WorkspaceObjectNameLess());
        for (const model::Material& material : materials){

            QDomElement nameElement;
            QDomElement materialElement = doc.createElement("Materail");
            materialsElement.appendChild(materialElement);

            //!- Name
            nameElement = doc.createElement("Name");
            name = material.name().get();
            materialElement.appendChild(doc.createTextNode(escapeName(name)));
            materialElement.appendChild(nameElement);

            if (material.optionalCast<model::StandardOpaqueMaterial>()){

                //!- Conductivity{ W / m - K }
                model::StandardOpaqueMaterial m = material.optionalCast<model::StandardOpaqueMaterial>().get();

                std::double_t conductivity = m.conductivity();
                nameElement = doc.createElement("Conductivity");
                nameElement.appendChild(doc.createTextNode(QString::number(conductivity)));
                materialElement.appendChild(nameElement);

                //!- Density{ kg / m3 }
                std::double_t density = m.density();
                nameElement = doc.createElement("Density");
                nameElement.appendChild(doc.createTextNode(QString::number(density)));
                materialElement.appendChild(nameElement);

                //!- Specific Heat{ J / kg - K }
                std::double_t specificHeat = m.specificHeat();
                nameElement = doc.createElement("Specific_Heat");
                nameElement.appendChild(doc.createTextNode(QString::number(specificHeat)));
                materialElement.appendChild(nameElement);

                //!- Thermal Absorptance
                std::double_t thermalAbsorptance = m.thermalAbsorptance();
                nameElement = doc.createElement("ThermalAbsorptance");
                nameElement.appendChild(doc.createTextNode(QString::number(thermalAbsorptance)));
                materialElement.appendChild(nameElement);

                //!- Solar Absorptance
                std::double_t solarAbsorptance = m.solarAbsorptance();
                nameElement = doc.createElement("SolarAbsorptance");
                nameElement.appendChild(doc.createTextNode(QString::number(solarAbsorptance)));
                materialElement.appendChild(nameElement);

                //!- Visible Absorptance
                std::double_t visibleAbsorptance = m.visibleAbsorptance();
                nameElement = doc.createElement("VisibleAbsorptance");
                nameElement.appendChild(doc.createTextNode(QString::number(visibleAbsorptance)));
                materialElement.appendChild(nameElement);
            }
        }

        //spaceElement.appendChild(loadElement);
        //spaceElement.appendChild(hvacElement);
        buildingStoryElement.appendChild(*spaceElement);
        buildingElement.appendChild(buildingStoryElement);

        result.appendChild(materialsElement);
        result.appendChild(buildingElement);

    }

    // Load
    // Hvac

    return result;
}

boost::optional<QDomElement> ForwardTranslator::translateMySpace(const openstudio::model::Building& building, QDomDocument& doc){

    // Translate Space
    QDomElement spaceElement = doc.createElement("Spaces");
    std::vector<model::Space> spaces = building.spaces();

    for (const model::Space& space : spaces){

        QDomElement m_spaceElement = doc.createElement("Space");

        // Space Type name
        std::string space_name = *space.spaceType().get().name();
        m_spaceElement.setAttribute("SpaceType", toQString(space_name));
        spaceElement.appendChild(m_spaceElement);

        // Space name
        std::string name = *space.name();
        m_spaceElement.setAttribute("name", toQString(name));

        QDomElement m_area = doc.createElement("Area");
        double area = space.floorArea();
        m_area.appendChild(doc.createTextNode(QString::number(area)));
        m_spaceElement.appendChild(m_area);

        QDomElement m_valume = doc.createElement("Valume");
        double valume = space.volume();
        m_valume.appendChild(doc.createTextNode(QString::number(valume)));
        m_spaceElement.appendChild(m_valume);


        QDomElement loadElement = doc.createElement("Load");

        boost::optional<QDomElement> lightsElement = translateMyLights(space, doc);
        loadElement.appendChild(*lightsElement);

        boost::optional<QDomElement> OtherEquipmentElement = translateMyOtherEquipment(space, doc);
        loadElement.appendChild(*lightsElement);

        boost::optional<QDomElement> ElectronicEquipmentElement = translateMyElectronicEquipment(space, doc);
        loadElement.appendChild(*ElectronicEquipmentElement);

        m_spaceElement.appendChild(loadElement);

        // Translate Construction
        QDomElement defaultConstruction = doc.createElement("Constructions");
        boost::optional<QDomElement> constructionElement = translateMyConstruction(space, doc);
        defaultConstruction.appendChild(*constructionElement);

        m_spaceElement.appendChild(defaultConstruction);
    }

    return spaceElement;
}

boost::optional<QDomElement> ForwardTranslator::translateMyLights(const openstudio::model::Space& space, QDomDocument& doc){

    std::vector<model::Lights> v_light = space.spaceType().get().lights();
    std::sort(v_light.begin(), v_light.end(), WorkspaceObjectNameLess());

    QDomElement Lights = doc.createElement("Lights");

    QDomElement nameElement;
    QDomElement lightElement;


    for (const model::Lights& i_light : v_light){

        lightElement = doc.createElement("Light");

        //!- Name
        nameElement = doc.createElement("Name");
        boost::optional<std::string> lightsDefinition = i_light.lightsDefinition().name().get();
        nameElement.appendChild(doc.createTextNode(toQString(*lightsDefinition)));
        lightElement.appendChild(nameElement);

        //!- Calculaion Method
        std::string calculationMethod = i_light.lightsDefinition().designLevelCalculationMethod();
        nameElement = doc.createElement("CalculationMethod");
        nameElement.appendChild(doc.createTextNode(toQString(calculationMethod)));
        lightElement.appendChild(nameElement);

        //!- Light Power
        double lightPower = i_light.lightingLevel().get();
        nameElement = doc.createElement("LightingLevel");
        nameElement.appendChild(doc.createTextNode(QString::number(lightPower)));
        lightElement.appendChild(nameElement);

        //!- EnergyPerSpace
        lightPower = i_light.powerPerFloorArea().get();
        nameElement = doc.createElement("PowerPerArea");
        nameElement.appendChild(doc.createTextNode(QString::number(lightPower)));
        lightElement.appendChild(nameElement);

        //!- EnergyPerPeople
        lightPower = i_light.powerPerPerson().get();
        nameElement = doc.createElement("PowerPerPerson");
        nameElement.appendChild(doc.createTextNode(QString::number(lightPower)));
        lightElement.appendChild(nameElement);

        //!- Multiplier
        double multipiler = i_light.multiplier();
        nameElement = doc.createElement("Multiplier");
        nameElement.appendChild(doc.createTextNode(QString::number(multipiler)));
        lightElement.appendChild(nameElement);

        Lights.appendChild(lightElement);
    }
    return Lights;
}

boost::optional<QDomElement> ForwardTranslator::translateMyOtherEquipment(const openstudio::model::Space& space, QDomDocument& doc){

    QDomElement otherEquipments = doc.createElement("OtherEquipments");

    std::vector<model::OtherEquipment> v_otherEquipment = space.spaceType().get().otherEquipment();
    std::sort(v_otherEquipment.begin(), v_otherEquipment.end(), WorkspaceObjectNameLess());


    for (const model::OtherEquipment& i_otherEquipment : v_otherEquipment){

        QDomElement otherEquipment = doc.createElement("OtherEquipment");

        //!- Name
        std::string name = i_otherEquipment.otherEquipmentDefinition().name().get();
        QDomElement nameElement = doc.createElement("Name");
        nameElement.appendChild(doc.createTextNode(toQString(name)));
        otherEquipment.appendChild(nameElement);

        //!- Calculation Method
        std::string calculationMethod = i_otherEquipment.otherEquipmentDefinition().designLevelCalculationMethod();
        nameElement = doc.createElement("CalculationMethod");
        nameElement.appendChild(doc.createTextNode(toQString(calculationMethod)));
        otherEquipment.appendChild(nameElement);

        //!- Design Level
        double designLevel = i_otherEquipment.otherEquipmentDefinition().designLevel().get();
        nameElement = doc.createElement("DesignLevel");
        nameElement.appendChild(doc.createTextNode(QString::number(designLevel)));
        otherEquipment.appendChild(nameElement);

        //!- Watt Per Space
        double wattPerSpace = i_otherEquipment.otherEquipmentDefinition().wattsperSpaceFloorArea().get();
        nameElement = doc.createElement("WattPerSpace");
        nameElement.appendChild(doc.createTextNode(QString::number(wattPerSpace)));
        otherEquipment.appendChild(nameElement);

        //!- Watt Per Space
        double wattPerPerson = i_otherEquipment.otherEquipmentDefinition().wattsperPerson().get();
        nameElement = doc.createElement("WattPerPerson");
        nameElement.appendChild(doc.createTextNode(QString::number(wattPerPerson)));
        otherEquipment.appendChild(nameElement);

        //!- Multipiler
        double muliplier = i_otherEquipment.multiplier();
        nameElement = doc.createElement("Multipiler");
        nameElement.appendChild(doc.createTextNode(QString::number(muliplier)));
        otherEquipment.appendChild(nameElement);

        otherEquipments.appendChild(otherEquipment);
    }
    return otherEquipments;
}

boost::optional<QDomElement> ForwardTranslator::translateMyElectronicEquipment(const openstudio::model::Space& space, QDomDocument& doc){

    QDomElement electricEquipments = doc.createElement("ElectricEquipments");

    std::vector<model::ElectricEquipment> v_electricEquipment = space.spaceType().get().electricEquipment();
    std::sort(v_electricEquipment.begin(), v_electricEquipment.end(), WorkspaceObjectNameLess());

    for (const model::ElectricEquipment& i_electricEquipment : v_electricEquipment){

        QDomElement electricEquipment = doc.createElement("ElectricEquipment");

        //!- Name
        std::string name = i_electricEquipment.name().get();
        QDomElement nameElement = doc.createElement("Name");
        nameElement.appendChild(doc.createTextNode(toQString(name)));
        electricEquipment.appendChild(nameElement);

        //!- Calculation Method
        std::string calculationMethod = i_electricEquipment.electricEquipmentDefinition().designLevelCalculationMethod();
        nameElement = doc.createElement("CalculationMethod");
        nameElement.appendChild(doc.createTextNode(toQString(calculationMethod)));
        electricEquipment.appendChild(nameElement);

        //!- Design Level
        double designLevel = i_electricEquipment.designLevel().get();
        nameElement = doc.createElement("DesignLevel");
        nameElement.appendChild(doc.createTextNode(QString::number(designLevel)));
        electricEquipment.appendChild(nameElement);

        //!- Watt Per Space
        double wattPerSpace = i_electricEquipment.powerPerFloorArea().get();
        nameElement = doc.createElement("WattPerSpace");
        nameElement.appendChild(doc.createTextNode(QString::number(wattPerSpace)));
        electricEquipment.appendChild(nameElement);

        //!- Watt Per Person
        double wattPerPerson = i_electricEquipment.powerPerPerson().get();
        nameElement = doc.createElement("WattPerPerson");
        nameElement.appendChild(doc.createTextNode(QString::number(wattPerPerson)));
        electricEquipment.appendChild(nameElement);

        //!- Multipiler
        double muliplier = i_electricEquipment.multiplier();
        nameElement = doc.createElement("Multipiler");
        nameElement.appendChild(doc.createTextNode(QString::number(muliplier)));
        electricEquipment.appendChild(nameElement);

        electricEquipments.appendChild(electricEquipment);
    }
    return electricEquipments;
}

boost::optional<QDomElement> ForwardTranslator::translateFacility(const openstudio::model::Facility& facility, QDomDocument& doc)
{
    QDomElement result = doc.createElement("Campus");
    m_translatedObjects[facility.handle()] = result;

    boost::optional<std::string> name = facility.name();

    // id
    result.setAttribute("id", "Facility");

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    if (name){
        nameElement.appendChild(doc.createTextNode(QString::fromStdString(name.get())));
    }else{
        nameElement.appendChild(doc.createTextNode("Facility"));
    }

    model::Model model = facility.model();

    // todo: translate location

    // translate building
    boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
    if (building){
        boost::optional<QDomElement> buildingElement = translateBuilding(*building, doc);
        if (buildingElement){
            result.appendChild(*buildingElement);
        }
    }

    // translate surfaces
    std::vector<model::Surface> surfaces = model.getConcreteModelObjects<model::Surface>();
    if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Surfaces"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum((int)surfaces.size());
        m_progressBar->setValue(0);
    }

    for (const model::Surface& surface : surfaces){
        boost::optional<QDomElement> surfaceElement = translateSurface(surface, doc);
        if (surfaceElement){
            result.appendChild(*surfaceElement);
        }

        if (m_progressBar){
            m_progressBar->setValue(m_progressBar->value() + 1);
        }
    }


    return result;
}

boost::optional<QDomElement> ForwardTranslator::translateMyPeople(const openstudio::model::Space& space, QDomDocument& doc){

    QDomElement peopleElement = doc.createElement("Peoples");
    std::vector<model::People> v_people = space.people();
    for (const model::People& i_people : v_people){
        //TODO: translate people
    }

    return peopleElement;
}

boost::optional<QDomElement> ForwardTranslator::translateMyGasEquipment(const openstudio::model::Space& space, QDomDocument& doc){

    QDomElement gasEquipmentElement = doc.createElement("GasEquipments");
    std::vector<model::GasEquipment> v_gasEquipment = space.gasEquipment();
    for (const model::GasEquipment& i_gasEquipment : v_gasEquipment){
        //TODO: translate Gas Equipment
    }

    return gasEquipmentElement;
}

boost::optional<QDomElement> ForwardTranslator::translateMyWaterUse(const openstudio::model::Space space, QDomDocument& doc){

    QDomElement waterUseElement = doc.createElement("WaterUses");
    std::vector<model::WaterUseEquipment> v_waterUse = space.waterUseEquipment();
    for (const model::WaterUseEquipment& i_waterUse : v_waterUse){
        //TODO: translate Water Use
    }

    return waterUseElement;
}

boost::optional<QDomElement> ForwardTranslator::translateMyLuminaire(const openstudio::model::Space& space, QDomDocument& doc){
    QDomElement luminaireElement = doc.createElement("Luminaires");
    std::vector<model::Luminaire> v_luminaire = space.luminaires();
    for (const model::Luminaire& i_luminaire : v_luminaire){
        //TODO: translate luminaire
    }

    return luminaireElement;
}

boost::optional<QDomElement> ForwardTranslator::translateMyConstruction(const openstudio::model::Space& space, QDomDocument& doc){

    QDomElement surfacesElement = doc.createElement("Surfaces");
    std::vector<model::Surface> v_serface = space.surfaces();

    for (const model::Surface& i_surface : v_serface){
        //TODO: translate construction
        QDomElement surfaceElement = doc.createElement("Surface");

        QDomElement nameElement = doc.createElement("Name");
        nameElement.appendChild(doc.createTextNode(toQString(i_surface.name().get())));
        surfaceElement.appendChild(nameElement);


        QDomElement constructElement = doc.createElement("Construction");
        std::string mat = i_surface.construction().get().name().get();
        constructElement.appendChild(doc.createTextNode(toQString(mat)));
        surfaceElement.appendChild(constructElement);

        surfacesElement.appendChild(surfaceElement);
    }

    return surfacesElement;
}

boost::optional<QDomElement> ForwardTranslator::translateBuilding(const openstudio::model::Building& building, QDomDocument& doc)
{
    QDomElement result = doc.createElement("Building");
    m_translatedObjects[building.handle()] = result;

    // id
    std::string name = building.name().get();
    result.setAttribute("id", escapeName(name));

    // building type
    //result.setAttribute("buildingType", "Office");
    result.setAttribute("buildingType", "Unknown");

    // space type
    boost::optional<model::SpaceType> spaceType = building.spaceType();
    if (spaceType){
        //std::string spaceTypeName = spaceType->name().get();
        // todo: map to BEC types
        //result.setAttribute("buildingType", escapeName(spaceTypeName));
    }

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    // area
    QDomElement areaElement = doc.createElement("Area");
    result.appendChild(areaElement);
    areaElement.appendChild(doc.createTextNode(QString::number(building.floorArea())));

    // translate spaces
    std::vector<model::Space> spaces = building.spaces();
    if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Spaces"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum((int)spaces.size());
        m_progressBar->setValue(0);
    }

    for (const model::Space& space : spaces){
        boost::optional<QDomElement> spaceElement = translateSpace(space, doc);
        if (spaceElement){
            result.appendChild(*spaceElement);
        }

        if (m_progressBar){
            m_progressBar->setValue(m_progressBar->value() + 1);
        }
    }

    return result;
}

boost::optional<QDomElement> ForwardTranslator::translateSpace(const openstudio::model::Space& space, QDomDocument& doc)
{
    QDomElement result = doc.createElement("Space");
    m_translatedObjects[space.handle()] = result;

    // id
    std::string name = space.name().get();
    result.setAttribute("id", escapeName(name));

    // space type
    boost::optional<model::SpaceType> spaceType = space.spaceType();
    if (spaceType){
        //std::string spaceTypeName = spaceType->name().get();
        // todo: map to BEC types
        //result.setAttribute("spaceType", escapeName(spaceTypeName));
    }

    // thermal zone
    boost::optional<model::ThermalZone> thermalZone = space.thermalZone();
    if (thermalZone){
        std::string thermalZoneName = thermalZone->name().get();
        result.setAttribute("zoneIdRef", escapeName(thermalZoneName));
    }

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    // append floor area
    double area = space.floorArea();
    QDomElement areaElement = doc.createElement("Area");
    areaElement.appendChild(doc.createTextNode(QString::number(area)));
    result.appendChild(areaElement);

    // append volume
    double volume = space.volume();
    QDomElement volumeElement = doc.createElement("Volume");
    volumeElement.appendChild(doc.createTextNode(QString::number(volume)));
    result.appendChild(volumeElement);

    // Load
    QDomElement loadElement = doc.createElement("Loads");

    // lighting
    double lightPower = space.lightingPower();
    QDomElement lightElement = doc.createElement("light");
    lightElement.appendChild(doc.createTextNode(QString::number(lightPower)));
    loadElement.appendChild(lightElement);

    // Electronic
    double electPower = space.electricEquipmentPower();
    QDomElement electElement = doc.createElement("Electonic");
    electElement.appendChild(doc.createTextNode(QString::number(electPower)));
    loadElement.appendChild(electElement);

    //	std::vector<model::Lights,std::allocator<model::Lights>> v_lights = space.lights();

    result.appendChild(loadElement);

    return result;
}

boost::optional<QDomElement> ForwardTranslator::translateSurface(const openstudio::model::Surface& surface, QDomDocument& doc)
{
    // return if already translated
    if (m_translatedObjects.find(surface.handle()) != m_translatedObjects.end()){
        return boost::none;
    }

    QDomElement result = doc.createElement("Surface");
    m_translatedObjects[surface.handle()] = result;

    // id
    std::string name = surface.name().get();
    result.setAttribute("id", escapeName(name));

    // DLM: currently unhandled
    //Shade
    //FreestandingColumn
    //EmbeddedColumn

    if (surface.isAirWall()){
        result.setAttribute("surfaceType", "Air");
    }else{
        std::string surfaceType = surface.surfaceType();
        std::string outsideBoundaryCondition = surface.outsideBoundaryCondition();
        if (istringEqual("Wall", surfaceType)){
            if (istringEqual("Outdoors", outsideBoundaryCondition)){
                result.setAttribute("surfaceType", "ExteriorWall");
            }else if (istringEqual("Surface", outsideBoundaryCondition)){
                result.setAttribute("surfaceType", "InteriorWall");
            }else if (surface.isGroundSurface()){
                result.setAttribute("surfaceType", "UndergroundWall");
            }
        }else if (istringEqual("RoofCeiling", surfaceType)){
            if (istringEqual("Outdoors", outsideBoundaryCondition)){
                result.setAttribute("surfaceType", "Roof");
            }else if (istringEqual("Surface", outsideBoundaryCondition)){
                result.setAttribute("surfaceType", "Ceiling");
            }else if (surface.isGroundSurface()){
                result.setAttribute("surfaceType", "UndergroundCeiling");
            }
        }else if (istringEqual("Floor", surfaceType)){
            if (istringEqual("Outdoors", outsideBoundaryCondition)){
                result.setAttribute("surfaceType", "RaisedFloor");
            }else if (surface.isGroundSurface()){
                result.setAttribute("surfaceType", "UndergroundSlab"); // or SlabOnGrade?
            }else if (istringEqual("Surface", outsideBoundaryCondition)){
                result.setAttribute("surfaceType", "InteriorFloor");
            }
        }
    }

    // construction
    boost::optional<model::ConstructionBase> construction = surface.construction();
    if (construction){
        //std::string constructionName = construction->name().get();
        // todo:: translate construction
        //result.setAttribute("constructionIdRef", "constructionName");
    }

    // this space
    Transformation transformation;
    boost::optional<model::Space> space = surface.space();
    if (space){
        transformation = space->siteTransformation();

        std::string spaceName = space->name().get();
        QDomElement adjacentSpaceIdElement = doc.createElement("AdjacentSpaceId");
        result.appendChild(adjacentSpaceIdElement);
        adjacentSpaceIdElement.setAttribute("spaceIdRef", escapeName(spaceName));
    }

    // adjacent surface
    boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
    if (adjacentSurface){
        boost::optional<model::Space> adjacentSpace = adjacentSurface->space();
        if (adjacentSpace){
            std::string adjacentSpaceName = adjacentSpace->name().get();
            QDomElement adjacentSpaceIdElement = doc.createElement("AdjacentSpaceId");
            result.appendChild(adjacentSpaceIdElement);
            adjacentSpaceIdElement.setAttribute("spaceIdRef", escapeName(adjacentSpaceName));

            // count adjacent surface as translated
            m_translatedObjects[adjacentSurface->handle()] = result;
        }
    }

    // transform vertices to world coordinates
    Point3dVector vertices = transformation*surface.vertices();

    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = surface.grossArea();
    if (outwardNormal && area > 0){

        // get tilt, duplicate code in planar surface
        Vector3d up(0.0,0.0,1.0);
        double tiltRadians = getAngle(*outwardNormal, up);

        // get azimuth, duplicate code in planar surface
        Vector3d north(0.0,1.0,0.0);
        double azimuthRadians = getAngle(*outwardNormal, north);
        if (outwardNormal->x() < 0.0) {
            azimuthRadians = -azimuthRadians + 2.0*boost::math::constants::pi<double>();
        }

        // transform vertices to face coordinates
        Transformation faceTransformation = Transformation::alignFace(vertices);
        Point3dVector faceVertices = faceTransformation.inverse()*vertices;
        BoundingBox faceBoundingBox;
        faceBoundingBox.addPoints(faceVertices);
        double width = faceBoundingBox.maxX().get() - faceBoundingBox.minX().get();
        double height = faceBoundingBox.maxY().get() - faceBoundingBox.minY().get();
        double areaCorrection = 1.0;
        if (width > 0 && height > 0){
            areaCorrection = sqrt(area/(width*height));
        }

        // pick lower left corner vertex in face coordinates
        double minY = std::numeric_limits<double>::max();
        double minX = std::numeric_limits<double>::max();
        size_t llcIndex = 0;
        size_t N = vertices.size();
        for (size_t i = 0; i < N; ++i){
            OS_ASSERT(std::abs(faceVertices[i].z()) < 0.001);
            if ((minY > faceVertices[i].y()) || ((minY > faceVertices[i].y() - 0.00001) && (minX > faceVertices[i].x()))){
                llcIndex = i;
                minY = faceVertices[i].y();
                minX = faceVertices[i].x();
            }
        }
        Point3d vertex = vertices[llcIndex];

        // rectangular geometry
        QDomElement rectangularGeometryElement = doc.createElement("RectangularGeometry");
        result.appendChild(rectangularGeometryElement);

        QDomElement azimuthElement = doc.createElement("Azimuth");
        rectangularGeometryElement.appendChild(azimuthElement);
        azimuthElement.appendChild(doc.createTextNode(QString::number(radToDeg(azimuthRadians))));

        QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
        rectangularGeometryElement.appendChild(cartesianPointElement);

        QDomElement coordinateXElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateXElement);
        coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x())));

        QDomElement coordinateYElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateYElement);
        coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y())));

        QDomElement coordinateZElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateZElement);
        coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z())));

        QDomElement tiltElement = doc.createElement("Tilt");
        rectangularGeometryElement.appendChild(tiltElement);
        tiltElement.appendChild(doc.createTextNode(QString::number(radToDeg(tiltRadians))));

        QDomElement widthElement = doc.createElement("Width");
        rectangularGeometryElement.appendChild(widthElement);
        widthElement.appendChild(doc.createTextNode(QString::number(areaCorrection*width)));

        QDomElement heightElement = doc.createElement("Height");
        rectangularGeometryElement.appendChild(heightElement);
        heightElement.appendChild(doc.createTextNode(QString::number(areaCorrection*height)));
    }

    // planar geometry
    QDomElement planarGeometryElement = doc.createElement("PlanarGeometry");
    result.appendChild(planarGeometryElement);

    QDomElement polyLoopElement = doc.createElement("PolyLoop");
    planarGeometryElement.appendChild(polyLoopElement);
    for (const Point3d& vertex : vertices){
        QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
        polyLoopElement.appendChild(cartesianPointElement);

        QDomElement coordinateXElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateXElement);
        coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x())));

        QDomElement coordinateYElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateYElement);
        coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y())));

        QDomElement coordinateZElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateZElement);
        coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z())));
    }

    // translate sub surfaces
    for (const model::SubSurface& subSurface : surface.subSurfaces()){
        boost::optional<QDomElement> openingElement = translateSubSurface(subSurface, transformation, doc);
        if (openingElement){
            result.appendChild(*openingElement);
        }
    }

    return result;
}

boost::optional<QDomElement> ForwardTranslator::translateSubSurface(const openstudio::model::SubSurface& subSurface, const openstudio::Transformation& transformation, QDomDocument& doc)
{
    // return if already translated
    if (m_translatedObjects.find(subSurface.handle()) != m_translatedObjects.end()){
        return boost::none;
    }

    QDomElement result = doc.createElement("Opening");
    m_translatedObjects[subSurface.handle()] = result;

    // id
    std::string name = subSurface.name().get();
    result.setAttribute("id", escapeName(name));

    // construction
    boost::optional<model::ConstructionBase> construction = subSurface.construction();
    if (construction){
        //std::string constructionName = construction->name().get();
        // todo: translate construction
        // result.setAttribute("constructionIdRef", "constructionName");
    }

    // DLM: currently unhandled
    // OperableSkylight
    // SlidingDoor

    if (subSurface.isAirWall()){
        result.setAttribute("openingType", "Air");
    }else{
        std::string subSurfaceType = subSurface.subSurfaceType();
        if (istringEqual("FixedWindow", subSurfaceType)){
            result.setAttribute("openingType", "FixedWindow");
        }else if(istringEqual("OperableWindow", subSurfaceType)){
            result.setAttribute("openingType", "OperableWindow");
        }else if (istringEqual("Skylight", subSurfaceType)){
            result.setAttribute("openingType", "FixedSkylight");
        }else if (istringEqual("Door", subSurfaceType)){
            result.setAttribute("openingType", "NonSlidingDoor");
        }else if (istringEqual("OverheadDoor", subSurfaceType)){
            result.setAttribute("openingType", "NonSlidingDoor");
        }
    }

    // transform vertices to world coordinates
    Point3dVector vertices = transformation*subSurface.vertices();

    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = subSurface.grossArea();
    if (outwardNormal && area > 0){

        // get tilt, duplicate code in planar surface
        Vector3d up(0.0,0.0,1.0);
        double tiltRadians = getAngle(*outwardNormal, up);

        // get azimuth, duplicate code in planar surface
        Vector3d north(0.0,1.0,0.0);
        double azimuthRadians = getAngle(*outwardNormal, north);
        if (outwardNormal->x() < 0.0) {
            azimuthRadians = -azimuthRadians + 2.0*boost::math::constants::pi<double>();
        }

        // transform vertices to face coordinates
        Transformation faceTransformation = Transformation::alignFace(vertices);
        Point3dVector faceVertices = faceTransformation.inverse()*vertices;
        BoundingBox faceBoundingBox;
        faceBoundingBox.addPoints(faceVertices);
        double width = faceBoundingBox.maxX().get() - faceBoundingBox.minX().get();
        double height = faceBoundingBox.maxY().get() - faceBoundingBox.minY().get();
        double areaCorrection = 1.0;
        if (width > 0 && height > 0){
            areaCorrection = sqrt(area/(width*height));
        }

        // pick lower left corner vertex in face coordinates
        double minY = std::numeric_limits<double>::max();
        double minX = std::numeric_limits<double>::max();
        size_t llcIndex = 0;
        size_t N = vertices.size();
        for (size_t i = 0; i < N; ++i){
            OS_ASSERT(std::abs(faceVertices[i].z()) < 0.001);
            if ((minY > faceVertices[i].y()) || ((minY > faceVertices[i].y() - 0.00001) && (minX > faceVertices[i].x()))){
                llcIndex = i;
                minY = faceVertices[i].y();
                minX = faceVertices[i].x();
            }
        }
        Point3d vertex = vertices[llcIndex];

        // rectangular geometry
        QDomElement rectangularGeometryElement = doc.createElement("RectangularGeometry");
        result.appendChild(rectangularGeometryElement);

        QDomElement azimuthElement = doc.createElement("Azimuth");
        rectangularGeometryElement.appendChild(azimuthElement);
        azimuthElement.appendChild(doc.createTextNode(QString::number(radToDeg(azimuthRadians))));

        QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
        rectangularGeometryElement.appendChild(cartesianPointElement);

        QDomElement coordinateXElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateXElement);
        coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x())));

        QDomElement coordinateYElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateYElement);
        coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y())));

        QDomElement coordinateZElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateZElement);
        coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z())));

        QDomElement tiltElement = doc.createElement("Tilt");
        rectangularGeometryElement.appendChild(tiltElement);
        tiltElement.appendChild(doc.createTextNode(QString::number(radToDeg(tiltRadians))));

        QDomElement widthElement = doc.createElement("Width");
        rectangularGeometryElement.appendChild(widthElement);
        widthElement.appendChild(doc.createTextNode(QString::number(areaCorrection*width)));

        QDomElement heightElement = doc.createElement("Height");
        rectangularGeometryElement.appendChild(heightElement);
        heightElement.appendChild(doc.createTextNode(QString::number(areaCorrection*height)));
    }

    // planar geometry
    QDomElement planarGeometryElement = doc.createElement("PlanarGeometry");
    result.appendChild(planarGeometryElement);

    // translate vertices
    QDomElement polyLoopElement = doc.createElement("PolyLoop");
    planarGeometryElement.appendChild(polyLoopElement);
    for (const Point3d& vertex : vertices){
        QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
        polyLoopElement.appendChild(cartesianPointElement);

        QDomElement coordinateXElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateXElement);
        coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x())));

        QDomElement coordinateYElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateYElement);
        coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y())));

        QDomElement coordinateZElement = doc.createElement("Coordinate");
        cartesianPointElement.appendChild(coordinateZElement);
        coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z())));
    }

    return result;
}

boost::optional<QDomElement> ForwardTranslator::translateThermalZone(const openstudio::model::ThermalZone& thermalZone, QDomDocument& doc)
{
    QDomElement result = doc.createElement("Zone");
    m_translatedObjects[thermalZone.handle()] = result;

    // id
    std::string name = thermalZone.name().get();
    result.setAttribute("id", escapeName(name));

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    return result;
}

} // bec
} // openstudio


