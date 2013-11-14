#include <sdd/ReverseTranslator.hpp>
#include <sdd/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/MasslessOpaqueMaterial.hpp>
#include <model/MasslessOpaqueMaterial_Impl.hpp>
#include <model/AirGap.hpp>
#include <model/AirGap_Impl.hpp>
#include <model/StandardOpaqueMaterial.hpp>
#include <model/StandardOpaqueMaterial_Impl.hpp>
#include <model/FFactorGroundFloorConstruction.hpp>
#include <model/FFactorGroundFloorConstruction_Impl.hpp>
#include <model/CFactorUndergroundWallConstruction.hpp>
#include <model/CFactorUndergroundWallConstruction_Impl.hpp>
#include <model/FenestrationMaterial.hpp>
#include <model/FenestrationMaterial_Impl.hpp>
#include <model/ModelPartitionMaterial.hpp>
#include <model/ModelPartitionMaterial_Impl.hpp>
#include <model/SimpleGlazing.hpp>
#include <model/SimpleGlazing_Impl.hpp>

#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/IPUnit.hpp>
#include <utilities/units/SIUnit.hpp>
#include <utilities/units/BTUUnit.hpp>
#include <utilities/units/WhUnit.hpp>
#include <utilities/core/Assert.hpp>

#include <QFile>
#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace sdd {

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateConstructAssembly(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    boost::optional<openstudio::model::ModelObject> result;

    QDomElement nameElement = element.firstChildElement("Name");
    OS_ASSERT(!nameElement.isNull()); // what type of error handling do we want?

    QDomElement specificationElement = element.firstChildElement("SpecMthd");
    OS_ASSERT(!specificationElement.isNull()); // what type of error handling do we want?

    if (specificationElement.text() == "Layers" ||
        specificationElement.text() == "UFactor" ){ // temp code

      openstudio::model::Construction construction(model);
      construction.setName(escapeName(nameElement.text()));

      std::vector<model::Material> materials;
      QDomNodeList materialElements = element.elementsByTagName("MatRef");
      for (int i = 0; i < materialElements.count(); i++){
        QDomElement materialElement = materialElements.at(i).toElement();
        std::string materialName = escapeName(materialElement.text());
        boost::optional<model::Material> material = model.getModelObjectByName<model::Material>(materialName);
        if( ! material )
        {
          LOG(Error,"Construction: " << construction.name().get() << " references material: " << materialName << " that is not defined.");
        }
        OS_ASSERT(material); // what type of error handling do we want?
        materials.push_back(*material);
      }

      unsigned n = materials.size();
      if (n > 0){

        // DLM: are these fields only on layered constructions? 
        QDomElement extRoughnessElement = element.firstChildElement("ExtRoughness"); // enumerated value
        QDomElement extSolAbsElement = element.firstChildElement("ExtSolAbs"); 
        QDomElement extThrmlAbsElement = element.firstChildElement("ExtThrmlAbs"); 
        QDomElement extVisAbsElement = element.firstChildElement("ExtVisAbs"); 
        QDomElement intSolAbsElement = element.firstChildElement("IntSolAbs"); 
        QDomElement intThrmlAbsElement = element.firstChildElement("IntThrmlAbs"); 
        QDomElement intVisAbsElement = element.firstChildElement("IntVisAbs"); 

        if (materials[0].optionalCast<model::StandardOpaqueMaterial>()){
          model::StandardOpaqueMaterial extMaterial = materials[0].cast<model::StandardOpaqueMaterial>();
          
          std::string roughness = "MediumRough";
          if (!extRoughnessElement.isNull()){
            roughness = extRoughnessElement.text().toStdString();
          }
          extMaterial.setRoughness(roughness);
      
          if (!extThrmlAbsElement.isNull()){
            extMaterial.setThermalAbsorptance(extThrmlAbsElement.text().toDouble());
          }

          if (!extSolAbsElement.isNull()){
            extMaterial.setSolarAbsorptance(extSolAbsElement.text().toDouble());
          }
            
          if (!extVisAbsElement.isNull()){
            extMaterial.setVisibleAbsorptance(extVisAbsElement.text().toDouble());
          }
        }else if (materials[0].optionalCast<model::MasslessOpaqueMaterial>()){
          model::MasslessOpaqueMaterial extMaterial = materials[0].cast<model::MasslessOpaqueMaterial>();
          
          std::string roughness = "MediumRough";
          if (!extRoughnessElement.isNull()){
            roughness = extRoughnessElement.text().toStdString();
          }
          extMaterial.setRoughness(roughness);
      
          if (!extThrmlAbsElement.isNull()){
            extMaterial.setThermalAbsorptance(extThrmlAbsElement.text().toDouble());
          }

          if (!extSolAbsElement.isNull()){
            extMaterial.setSolarAbsorptance(extSolAbsElement.text().toDouble());
          }
            
          if (!extVisAbsElement.isNull()){
            extMaterial.setVisibleAbsorptance(extVisAbsElement.text().toDouble());
          }
        }

        if (materials[n-1].optionalCast<model::StandardOpaqueMaterial>()){
          model::StandardOpaqueMaterial intMaterial = materials[n-1].cast<model::StandardOpaqueMaterial>();
      
          if (!intThrmlAbsElement.isNull()){
            intMaterial.setThermalAbsorptance(intThrmlAbsElement.text().toDouble());
          }

          if (!intSolAbsElement.isNull()){
            intMaterial.setSolarAbsorptance(intSolAbsElement.text().toDouble());
          }
            
          if (!intVisAbsElement.isNull()){
            intMaterial.setVisibleAbsorptance(intVisAbsElement.text().toDouble());
          }
        }else if (materials[n-1].optionalCast<model::MasslessOpaqueMaterial>()){
          model::MasslessOpaqueMaterial intMaterial = materials[n-1].cast<model::MasslessOpaqueMaterial>();
          
          if (!intThrmlAbsElement.isNull()){
            intMaterial.setThermalAbsorptance(intThrmlAbsElement.text().toDouble());
          }

          if (!intSolAbsElement.isNull()){
            intMaterial.setSolarAbsorptance(intSolAbsElement.text().toDouble());
          }
            
          if (!intVisAbsElement.isNull()){
            intMaterial.setVisibleAbsorptance(intVisAbsElement.text().toDouble());
          }
        }

      }

      bool test = construction.setLayers(materials);
      OS_ASSERT(test); // what type of error handling do we want?

      result = construction;

    }else if (specificationElement.text() == "CFactor"){
  
      // DLM: per input from David Reddy this construction will be cloned 
      // for each surface that uses it and height set per surface
      // 
      openstudio::model::CFactorUndergroundWallConstruction construction(model);
      construction.setName(escapeName(nameElement.text()));

      QDomElement cFactorElement = element.firstChildElement("CFactor"); 
      OS_ASSERT(!cFactorElement.isNull());

      // sdd units = Btu/(hr*ft^2*F), os units = W/(m^2*K) 
      Quantity cFactorIP(cFactorElement.text().toDouble(), BTUUnit(BTUExpnt(1,-2,-1,-1)));
      OptionalQuantity cFactorSI = QuantityConverter::instance().convert(cFactorIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(cFactorSI);
      OS_ASSERT(cFactorSI->units() == WhUnit(WhExpnt(1,0,-2,-1)));
      construction.setCFactor(cFactorSI->value());

      result = construction;

    }else if (specificationElement.text() == "FFactor"){

      // DLM: per input from David Reddy this construction will be cloned 
      // for each surface that uses it and area and perimeter set per surface
      // 
      openstudio::model::FFactorGroundFloorConstruction construction(model);
      construction.setName(escapeName(nameElement.text()));

      QDomElement fFactorElement = element.firstChildElement("FFactor"); 
      OS_ASSERT(!fFactorElement.isNull());
            
      // sdd units = Btu/(hr*ft*F), os units = W/(m*K)
      Quantity fFactorIP(fFactorElement.text().toDouble(), BTUUnit(BTUExpnt(1,-1,-1,-1)));
      OptionalQuantity fFactorSI = QuantityConverter::instance().convert(fFactorIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(fFactorSI);
      OS_ASSERT(fFactorSI->units() == WhUnit(WhExpnt(1,0,-1,-1)));
      construction.setFFactor(fFactorSI->value());

      result = construction;

    }else if (specificationElement.text() == "UFactor"){

      // DLM: todo create object, do we need this now?

    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateDoorConstruction(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement uFactorElement = element.firstChildElement("UFactor"); // Btu/h-ft2-F

    if (!nameElement.isNull() && !uFactorElement.isNull()){
      
      // sdd units = Btu/(hr*ft^2*F), os units = W/m^2-K
      Quantity uFactorIP(uFactorElement.text().toDouble(), BTUUnit(BTUExpnt(1,-2,-1,-1)));
      OptionalQuantity uFactorWh = QuantityConverter::instance().convert(uFactorIP, whSys);
      OS_ASSERT(uFactorWh);
      OS_ASSERT(uFactorWh->units() == WhUnit(WhExpnt(1,0,-2,-1)));

      // m^2-K/W
      double rValueSI = 1.0 / uFactorWh->value();

      std::string name = escapeName(nameElement.text());

      model::MasslessOpaqueMaterial material(model);
      material.setName(name + " Material");
      material.setRoughness("MediumSmooth");
      material.setThermalResistance(rValueSI);

      openstudio::model::MaterialVector layers;
      layers.push_back(material);

      openstudio::model::Construction construction(model);
      construction.setName(name);
      construction.setLayers(layers);

      return construction;
    }
    return boost::none;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateFenestrationConstruction(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement shgcElement = element.firstChildElement("SHGC"); // unitless
    QDomElement uFactorElement = element.firstChildElement("UFactor"); // Btu/h-ft2-F
    QDomElement vtElement = element.firstChildElement("VT"); // unitless

    if (!nameElement.isNull() && !shgcElement.isNull() && !uFactorElement.isNull() && !vtElement.isNull()){
      
      std::string name = escapeName(nameElement.text());

      openstudio::model::SimpleGlazing material(model);
      material.setName(name + " Material");
      material.setSolarHeatGainCoefficient(shgcElement.text().toDouble());
      material.setVisibleTransmittance(vtElement.text().toDouble());

      // sdd units = Btu/(hr*ft^2*F), os units = W/m^2-K
      Quantity uFactorIP(uFactorElement.text().toDouble(), BTUUnit(BTUExpnt(1,-2,-1,-1)));
      OptionalQuantity uFactorWh = QuantityConverter::instance().convert(uFactorIP, whSys);
      OS_ASSERT(uFactorWh);
      OS_ASSERT(uFactorWh->units() == WhUnit(WhExpnt(1,0,-2,-1)));
      material.setUFactor(uFactorWh->value());

      openstudio::model::MaterialVector layers;
      layers.push_back(material);

      openstudio::model::Construction construction(model);
      construction.setName(name);
      construction.setLayers(layers);

      return construction;
    }
    return boost::none;
  }
  
  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateMaterial(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
   
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    QDomElement nameElement = element.firstChildElement("Name");
    //QDomElement roughnessElement = element.firstChildElement("Roughness"); // {VeryRough, Rough, MediumRough, MediumSmooth, Smooth, VerySmooth}
    QDomElement thicknessElement = element.firstChildElement("Thkns"); // in
    QDomElement thermalConductivityElement = element.firstChildElement("ThrmlCndct"); // Btu/(hr*ft*F)
    QDomElement densityElement = element.firstChildElement("Dens"); // lb/ft^3 // DLM: is this right? schema says could also be lb/ft^2
    QDomElement specificHeatElement = element.firstChildElement("SpecHt"); // Btu/(lb*F)
    //QDomElement thermalAbsorptanceElement = element.firstChildElement("ThrmlAbs"); // 0-1
    //QDomElement solarAbsorbanceElement = element.firstChildElement("SolAbs"); // 0-1 
    //QDomElement visibleAbsorptanceElement = element.firstChildElement("VisAbs"); // 0-1
    QDomElement rValueElement = element.firstChildElement("RVal"); // hr*ft2*degF/Btu

    /*
    OS:Material
      Name - required
      Roughness - default to MediumRough
      Thickness - required, m
      Conductivity - required, W/m-K
      Density - required, kg/m3
      Specific Heat - required, J/kg-K
      Thermal Absorptance, IDD default of 0.9
      Solar Absorptance, IDD default of 0.7
      Visible Absorptance, IDD default of 0.7
    */
    if (!nameElement.isNull() && !thicknessElement.isNull() && !thermalConductivityElement.isNull() && !densityElement.isNull() && !specificHeatElement.isNull()){
      
      openstudio::model::StandardOpaqueMaterial material(model);

      material.setName(escapeName(nameElement.text()));

      // DLM: set on construction
      //std::string roughness = "MediumRough";
      //if (!roughnessElement.isNull()){
      //  roughness = roughnessElement.text().toStdString();
      //}
      //material.setRoughness(roughness);

      // sdd units = in, os units = m
      double thicknessInches = thicknessElement.text().toDouble();
      double thicknessFeet = thicknessInches/12.0;
      Quantity thicknessIP(thicknessFeet, IPUnit(IPExpnt(0,1,0)));
      OptionalQuantity thicknessSI = QuantityConverter::instance().convert(thicknessIP, siSys);
      OS_ASSERT(thicknessSI);
      OS_ASSERT(thicknessSI->units() == SIUnit(SIExpnt(0,1,0)));
      material.setThickness(thicknessSI->value());

      // sdd units = Btu/(hr*ft*F), os units = W/m-K
      Quantity thermalConductivityIP(thermalConductivityElement.text().toDouble(), BTUUnit(BTUExpnt(1,-1,-1,-1)));
      OptionalQuantity thermalConductivityWh = QuantityConverter::instance().convert(thermalConductivityIP, whSys);
      OS_ASSERT(thermalConductivityWh);
      OS_ASSERT(thermalConductivityWh->units() == WhUnit(WhExpnt(1,0,-1,-1)));
      material.setThermalConductivity(thermalConductivityWh->value());

      // sdd units = lb/ft^3, os units = kg/m3
      Quantity densityIP(densityElement.text().toDouble(), IPUnit(IPExpnt(1,-3,0)));
      OptionalQuantity densitySI = QuantityConverter::instance().convert(densityIP, siSys);
      OS_ASSERT(densitySI);
      OS_ASSERT(densitySI->units() == SIUnit(SIExpnt(1,-3,0)));
      material.setDensity(densitySI->value());

      // sdd units = Btu/(lb*F), os units = J/kg-K = m^2/(s^2*K)
      Quantity specificHeatIP(specificHeatElement.text().toDouble(), BTUUnit(BTUExpnt(1,0,0,-1))*IPUnit(IPExpnt(-1)));
      OptionalQuantity specificHeatSI = QuantityConverter::instance().convert(specificHeatIP, siSys);
      OS_ASSERT(specificHeatSI);
      OS_ASSERT(specificHeatSI->units() == SIUnit(SIExpnt(0,2,-2,-1)));
      material.setSpecificHeat(specificHeatSI->value());

      // DLM: set on construction
      //if (!thermalAbsorptanceElement.isNull()){
      //  material.setThermalAbsorptance(thermalAbsorptanceElement.text().toDouble());
      //}

      // DLM: set on construction
      //if (!solarAbsorbanceElement.isNull()){
      //  material.setSolarAbsorptance(solarAbsorbanceElement.text().toDouble());
      //}
        
      // DLM: set on construction
      //if (!visibleAbsorptanceElement.isNull()){
      //  material.setVisibleAbsorptance(visibleAbsorptanceElement.text().toDouble());
      //}

      return material;
    }

    /*
    OS:Material:NoMass,
      Name - required
      Roughness - default to MediumRough
      Thermal Resistance - required, m2-K/W
      Thermal Absorptance, IDD default of 0.9
      Solar Absorptance, IDD default of 0.7
      Visible Absorptance, IDD default of 0.7
    */
    if (!nameElement.isNull() && !rValueElement.isNull()){
      
      openstudio::model::MasslessOpaqueMaterial material(model);

      material.setName(escapeName(nameElement.text()));

      // DLM: set on construction
      //std::string roughness = "MediumRough";
      //if (!roughnessElement.isNull()){
      //  roughness = roughnessElement.text().toStdString();
      //}
      //material.setRoughness(roughness);

      // sdd units = hr*ft2*degF/Btu, os units = m2-K/W
      Quantity rValueIP(rValueElement.text().toDouble(), BTUUnit(BTUExpnt(-1,2,1,1)));
      OptionalQuantity rValueWh = QuantityConverter::instance().convert(rValueIP, whSys);
      OS_ASSERT(rValueWh);
      OS_ASSERT(rValueWh->units() == WhUnit(WhExpnt(-1,0,2,1)));
      material.setThermalResistance(rValueWh->value());

      // DLM: set on construction
      //if (!thermalAbsorptanceElement.isNull()){
      //  material.setThermalAbsorptance(thermalAbsorptanceElement.text().toDouble());
      //}

      // DLM: set on construction
      //if (!solarAbsorbanceElement.isNull()){
      //  material.setSolarAbsorptance(solarAbsorbanceElement.text().toDouble());
      //}
        
      // DLM: set on construction
      //if (!visibleAbsorptanceElement.isNull()){
      //  material.setVisibleAbsorptance(visibleAbsorptanceElement.text().toDouble());
      //}

      return material;
    }

    return boost::none;
  }

  boost::optional<QDomElement> ForwardTranslator::translateConstructionBase(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc)
  {
    QDomElement result;
    
    if (constructionBase.optionalCast<model::Construction>()){
      model::Construction construction = constructionBase.cast<model::Construction>();

      // check if any layer has not been translated, constructions using simple glazing material will
      // be skipped here because the simple glazing material is not recorded as mapped
      std::vector<model::Material> layers = construction.layers();
      BOOST_FOREACH(const model::Material& material, layers){
        if (m_translatedObjects.find(material.handle()) ==  m_translatedObjects.end()){
          return boost::none;
        }
      }
      
      result = doc.createElement("ConsAssm");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result.appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // specification method
      QDomElement specMthdElement = doc.createElement("SpecMthd");
      result.appendChild(specMthdElement);
      specMthdElement.appendChild(doc.createTextNode("Layers"));

      unsigned n = layers.size();
      if (n > 0){

        std::string extRoughness;
        boost::optional<double> extSolarAbsorptance;
        boost::optional<double> extThermalAbsorptance;
        boost::optional<double> extVisibleAbsorptance;
        boost::optional<double> intSolarAbsorptance;
        boost::optional<double> intThermalAbsorptance;
        boost::optional<double> intVisibleAbsorptance;

        if (layers[0].optionalCast<model::StandardOpaqueMaterial>()){
          model::StandardOpaqueMaterial extMaterial = layers[0].cast<model::StandardOpaqueMaterial>();
          extRoughness = extMaterial.roughness();
          extSolarAbsorptance = extMaterial.solarAbsorptance();
          extThermalAbsorptance = extMaterial.thermalAbsorptance();
          extVisibleAbsorptance = extMaterial.visibleAbsorptance();
        }else if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()){
          model::MasslessOpaqueMaterial extMaterial = layers[0].cast<model::MasslessOpaqueMaterial>();
          extRoughness = extMaterial.roughness();
          extSolarAbsorptance = extMaterial.solarAbsorptance();
          extThermalAbsorptance = extMaterial.thermalAbsorptance();
          extVisibleAbsorptance = extMaterial.visibleAbsorptance();
        }

        if (layers[n-1].optionalCast<model::StandardOpaqueMaterial>()){
          model::StandardOpaqueMaterial intMaterial = layers[n-1].cast<model::StandardOpaqueMaterial>();
          intSolarAbsorptance = intMaterial.solarAbsorptance();
          intThermalAbsorptance = intMaterial.thermalAbsorptance();
          intVisibleAbsorptance = intMaterial.visibleAbsorptance();
        }else if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()){
          model::MasslessOpaqueMaterial intMaterial = layers[n-1].cast<model::MasslessOpaqueMaterial>();
          intSolarAbsorptance = intMaterial.solarAbsorptance();
          intThermalAbsorptance = intMaterial.thermalAbsorptance();
          intVisibleAbsorptance = intMaterial.visibleAbsorptance();
        }

        // ext roughness
        if (!extRoughness.empty()){
          QDomElement roughnessElement = doc.createElement("ExtRoughness");
          result.appendChild(roughnessElement);
          roughnessElement.appendChild(doc.createTextNode(toQString(extRoughness)));
        }

        // ext solarAbsorptance
        if (extSolarAbsorptance){
          QDomElement solarAbsorptanceElement = doc.createElement("ExtSolAbs"); 
          result.appendChild(solarAbsorptanceElement);
          solarAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*extSolarAbsorptance)));
        }

        // ext thermalAbsorptance
        if (extThermalAbsorptance){
          QDomElement thermalAbsorptanceElement = doc.createElement("ExtThrmlAbs");
          result.appendChild(thermalAbsorptanceElement);
          thermalAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*extThermalAbsorptance)));
        }

        // ext visibleAbsorptance
        if (extVisibleAbsorptance){
          QDomElement visibleAbsorptanceElement = doc.createElement("ExtVisAbs");
          result.appendChild(visibleAbsorptanceElement);
          visibleAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*extVisibleAbsorptance)));
        }

        // int solarAbsorptance
        if (intSolarAbsorptance){
          QDomElement solarAbsorptanceElement = doc.createElement("IntSolAbs"); 
          result.appendChild(solarAbsorptanceElement);
          solarAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*intSolarAbsorptance)));
        }

        // int thermalAbsorptance
        if (intThermalAbsorptance){
          QDomElement thermalAbsorptanceElement = doc.createElement("IntThrmlAbs");
          result.appendChild(thermalAbsorptanceElement);
          thermalAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*intThermalAbsorptance)));
        }

        // int visibleAbsorptance
        if (intVisibleAbsorptance){
          QDomElement visibleAbsorptanceElement = doc.createElement("IntVisAbs");
          result.appendChild(visibleAbsorptanceElement);
          visibleAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*intVisibleAbsorptance)));
        }
      }

      BOOST_FOREACH(const model::Material& material, layers){

        // materialReference
        std::string materialName = material.name().get();
        QDomElement materialReferenceElement = doc.createElement("MatRef");
        result.appendChild(materialReferenceElement);
        materialReferenceElement.appendChild(doc.createTextNode(escapeName(materialName)));
      }

      m_translatedObjects[construction.handle()] = result;

    }else if (constructionBase.optionalCast<model::FFactorGroundFloorConstruction>()){
      model::FFactorGroundFloorConstruction construction =  constructionBase.cast<model::FFactorGroundFloorConstruction>();

      result = doc.createElement("ConsAssm");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result.appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // specification method
      QDomElement specMthdElement = doc.createElement("SpecMthd");
      result.appendChild(specMthdElement);
      specMthdElement.appendChild(doc.createTextNode("FFactor"));

      // specification method
      QDomElement fFactorElement = doc.createElement("FFactor");
      result.appendChild(fFactorElement);

      // sdd units = Btu/(hr*ft*F), os units = W/(m*K)
      Quantity fFactorSI(construction.fFactor(), WhUnit(WhExpnt(1,0,-1,-1)));
      OptionalQuantity fFactorIP = QuantityConverter::instance().convert(fFactorSI, UnitSystem(UnitSystem::BTU));
      OS_ASSERT(fFactorIP);
      OS_ASSERT(fFactorIP->units() == BTUUnit(BTUExpnt(1,-1,-1,-1)));
      fFactorElement.appendChild(doc.createTextNode(QString::number(fFactorIP->value())));

      // DLM: cannot write out
      //<IntSolAbs>0.7</IntSolAbs>
      //<IntThrmlAbs>0.9</IntThrmlAbs>
      //<IntVisAbs>0.5</IntVisAbs>
      //<MatRef index="0">NACM_Concrete 4in</MatRef>
      //<MatRef index="1">NACM_Carpet Pad</MatRef>

      m_translatedObjects[construction.handle()] = result;

    }else if (constructionBase.optionalCast<model::CFactorUndergroundWallConstruction>()){
      model::CFactorUndergroundWallConstruction construction =  constructionBase.cast<model::CFactorUndergroundWallConstruction>();

      result = doc.createElement("ConsAssm");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result.appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // specification method
      QDomElement specMthdElement = doc.createElement("SpecMthd");
      result.appendChild(specMthdElement);
      specMthdElement.appendChild(doc.createTextNode("CFactor"));

      // specification method
      QDomElement cFactorElement = doc.createElement("CFactor");
      result.appendChild(cFactorElement);

      // sdd units = Btu/(hr*ft^2*F), os units = W/(m^2*K)
      Quantity cFactorSI(construction.cFactor(), WhUnit(WhExpnt(1,0,-2,-1)));
      OptionalQuantity cFactorIP = QuantityConverter::instance().convert(cFactorSI, UnitSystem(UnitSystem::BTU));
      OS_ASSERT(cFactorIP);
      OS_ASSERT(cFactorIP->units() == BTUUnit(BTUExpnt(1,-2,-1,-1)));
      cFactorElement.appendChild(doc.createTextNode(QString::number(cFactorIP->value())));

      // DLM: cannot write out
      //<IntSolAbs>0.7</IntSolAbs>
      //<IntThrmlAbs>0.9</IntThrmlAbs>
      //<IntVisAbs>0.5</IntVisAbs>
      //<MatRef index="0">NACM_Concrete 4in</MatRef>
      //<MatRef index="1">NACM_Carpet Pad</MatRef>

      m_translatedObjects[construction.handle()] = result;

    }
    
    // todo: handle u-factor constructions

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateDoorConstruction(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc)
  {
    boost::optional<QDomElement> result;

    UnitSystem ipSys(UnitSystem::IP);
    UnitSystem btuSys(UnitSystem::BTU);

    if (constructionBase.optionalCast<model::Construction>()){
      model::Construction construction = constructionBase.cast<model::Construction>();

      result = doc.createElement("DrCons");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result->appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // can only get UFactor for special cases
      bool foundUFactor = false;

      std::vector<model::Material> layers = construction.layers();
      if (layers.size() == 1){
        if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()){

          model::MasslessOpaqueMaterial material = layers[0].cast<model::MasslessOpaqueMaterial>();

          // UFactor
          // os units = W/m2-K, sdd units = Btu/(hr*f2t*F)
          double uFactor = material.thermalConductance();
          Quantity uFactorSI(uFactor, WhUnit(WhExpnt(1,0,-2,-1)));
          OptionalQuantity uFactorIP = QuantityConverter::instance().convert(uFactorSI, btuSys);
          OS_ASSERT(uFactorIP);
          OS_ASSERT(uFactorIP->units() == BTUUnit(BTUExpnt(1,-2,-1,-1)));
          QDomElement uFactorElement = doc.createElement("UFactor");
          result->appendChild(uFactorElement);
          uFactorElement.appendChild(doc.createTextNode(QString::number(uFactorIP->value())));

          foundUFactor = true;
        }
      }

      if (!foundUFactor){
        LOG(Error, "Could not calculate UFactor for DrCons '" << name << "'");
      }

      // mark the construction as translated, not the material
      m_translatedObjects[construction.handle()] = result.get();
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateFenestrationConstruction(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc)
  {
    boost::optional<QDomElement> result;

    UnitSystem ipSys(UnitSystem::IP);
    UnitSystem btuSys(UnitSystem::BTU);

    if (constructionBase.optionalCast<model::Construction>()){
      model::Construction construction = constructionBase.cast<model::Construction>();

      result = doc.createElement("FenCons");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result->appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // can only get SHGC, TVis, UFactor for special cases
      bool foundUFactor = false;
      bool foundVT = false;

      std::vector<model::Material> layers = construction.layers();
      if (layers.size() == 1){
        if (layers[0].optionalCast<model::SimpleGlazing>()){

          model::SimpleGlazing simpleGlazing = layers[0].cast<model::SimpleGlazing>();
          
          // SHGC
          double shgc = simpleGlazing.solarHeatGainCoefficient();
          QDomElement shgcElement = doc.createElement("SHGC"); 
          result->appendChild(shgcElement);
          shgcElement.appendChild(doc.createTextNode(QString::number(shgc)));

          // UFactor
          // os units = W/m2-K, sdd units = Btu/(hr*f2t*F)
          double uFactor = simpleGlazing.uFactor();
          Quantity uFactorSI(uFactor, WhUnit(WhExpnt(1,0,-2,-1)));
          OptionalQuantity uFactorIP = QuantityConverter::instance().convert(uFactorSI, btuSys);
          OS_ASSERT(uFactorIP);
          OS_ASSERT(uFactorIP->units() == BTUUnit(BTUExpnt(1,-2,-1,-1)));
          QDomElement uFactorElement = doc.createElement("UFactor");
          result->appendChild(uFactorElement);
          uFactorElement.appendChild(doc.createTextNode(QString::number(uFactorIP->value())));

          foundUFactor = true;

          // VT
          boost::optional<double> vt = simpleGlazing.visibleTransmittance();
          if (vt){
            QDomElement vtElement = doc.createElement("VT"); 
            result->appendChild(vtElement);
            vtElement.appendChild(doc.createTextNode(QString::number(*vt)));

            foundVT = true;
          }
        }
      }

      if (!foundUFactor){
        LOG(Error, "Could not calculate SHGC, UFactor, or VT for FenCons '" << name << "'");
      }else if (!foundVT){
        LOG(Error, "Could not calculate VT for FenCons '" << name << "'");
      }

      // mark the construction as translated, not the material
      m_translatedObjects[construction.handle()] = result.get();
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateMaterial(const openstudio::model::Material& material, QDomDocument& doc)
  {
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem ipSys(UnitSystem::IP);
    UnitSystem btuSys(UnitSystem::BTU);

    boost::optional<QDomElement> result;

    if (material.optionalCast<model::StandardOpaqueMaterial>()){
      model::StandardOpaqueMaterial standardOpaqueMaterial = material.cast<model::StandardOpaqueMaterial>();
      result = doc.createElement("Mat");
      m_translatedObjects[standardOpaqueMaterial.handle()] = *result;

      // name
      std::string name = standardOpaqueMaterial.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result->appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // DLM: set in construction
      // roughness
      //std::string roughness = standardOpaqueMaterial.roughness();
      //QDomElement roughnessElement = doc.createElement("Roughness");
      //result->appendChild(roughnessElement);
      //roughnessElement.appendChild(doc.createTextNode(toQString(roughness)));

      // DLM: set in construction
      // thermalAbsorptance
      //boost::optional<double> thermalAbsorptance = standardOpaqueMaterial.thermalAbsorptance();
      //if (thermalAbsorptance){
      //  QDomElement thermalAbsorptanceElement = doc.createElement("ThrmlAbs");
      //  result->appendChild(thermalAbsorptanceElement);
      //  thermalAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*thermalAbsorptance)));
      //}

      // DLM: set in construction
      // solarAbsorptance
      //boost::optional<double> solarAbsorptance = standardOpaqueMaterial.solarAbsorptance();
      //if (solarAbsorptance){
      //  QDomElement solarAbsorptanceElement = doc.createElement("SolAbs"); 
      //  result->appendChild(solarAbsorptanceElement);
      //  solarAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*solarAbsorptance)));
      //}

      // DLM: set in construction
      // visibleAbsorptance
      //boost::optional<double> visibleAbsorptance = standardOpaqueMaterial.visibleAbsorptance();
      //if (visibleAbsorptance){
      //  QDomElement visibleAbsorptanceElement = doc.createElement("VisAbs");
      //  result->appendChild(visibleAbsorptanceElement);
      //  visibleAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*visibleAbsorptance)));
      //}

      // thickness
      // os units = m, sdd units = in
      double thickness = standardOpaqueMaterial.thickness();
      Quantity thicknessSI(thickness, SIUnit(SIExpnt(0,1,0)));
      OptionalQuantity thicknessIP = QuantityConverter::instance().convert(thicknessSI, ipSys);
      OS_ASSERT(thicknessIP);
      OS_ASSERT(thicknessIP->units() == IPUnit(IPExpnt(0,1,0)));
      double thicknessInches = thicknessIP->value() * 12.0;
      QDomElement thicknessElement = doc.createElement("Thkns");
      result->appendChild(thicknessElement);
      thicknessElement.appendChild(doc.createTextNode(QString::number(thicknessInches)));

      // conductivity
      // os units = W/m-K, sdd units = Btu/(hr*ft*F)
      double conductivity = standardOpaqueMaterial.thermalConductivity();
      Quantity conductivitySI(conductivity, WhUnit(WhExpnt(1,0,-1,-1)));
      OptionalQuantity conductivityIP = QuantityConverter::instance().convert(conductivitySI, btuSys);
      OS_ASSERT(conductivityIP);
      OS_ASSERT(conductivityIP->units() == BTUUnit(BTUExpnt(1,-1,-1,-1)));
      QDomElement conductivityElement = doc.createElement("ThrmlCndct");
      result->appendChild(conductivityElement);
      conductivityElement.appendChild(doc.createTextNode(QString::number(conductivityIP->value())));

      // density
      // os units = kg/m3, sdd units = lb/ft^3
      double density = standardOpaqueMaterial.density();
      Quantity densitySI(density, SIUnit(SIExpnt(1,-3,0)));
      OptionalQuantity densityIP = QuantityConverter::instance().convert(densitySI, ipSys);
      OS_ASSERT(densityIP);
      OS_ASSERT(densityIP->units() == IPUnit(IPExpnt(1,-3,0)));
      QDomElement densityElement = doc.createElement("Dens");
      result->appendChild(densityElement);
      densityElement.appendChild(doc.createTextNode(QString::number(densityIP->value())));

      // specificHeat
      // os units = J/kg-K, sdd units = Btu/(lb*F)
      double specificHeat = standardOpaqueMaterial.specificHeat();
      Quantity specificHeatSI(specificHeat, SIUnit(SIExpnt(0,2,-2,-1)));
      OptionalQuantity specificHeatIP = QuantityConverter::instance().convert(specificHeatSI, BTUUnit(BTUExpnt(1,0,0,-1))*IPUnit(IPExpnt(-1)));
      OS_ASSERT(specificHeatIP);
      OS_ASSERT(specificHeatIP->units() == BTUUnit(BTUExpnt(1,0,0,-1))*IPUnit(IPExpnt(-1)));
      QDomElement specificHeatElement = doc.createElement("SpecHt");
      result->appendChild(specificHeatElement);
      specificHeatElement.appendChild(doc.createTextNode(QString::number(specificHeatIP->value())));

    }else if (material.optionalCast<model::MasslessOpaqueMaterial>()){
      model::MasslessOpaqueMaterial masslessOpaqueMaterial = material.cast<model::MasslessOpaqueMaterial>();
      result = doc.createElement("Mat");
      m_translatedObjects[masslessOpaqueMaterial.handle()] = *result;

      // name
      std::string name = masslessOpaqueMaterial.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result->appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // DLM: set in construction
      // roughness
      //std::string roughness = masslessOpaqueMaterial.roughness();
      //QDomElement roughnessElement = doc.createElement("Roughness");
      //result->appendChild(roughnessElement);
      //roughnessElement.appendChild(doc.createTextNode(toQString(roughness)));

      // DLM: set in construction
      // thermalAbsorptance
      //boost::optional<double> thermalAbsorptance = masslessOpaqueMaterial.thermalAbsorptance();
      //if (thermalAbsorptance){
      //  QDomElement thermalAbsorptanceElement = doc.createElement("ThrmlAbs");
      //  result->appendChild(thermalAbsorptanceElement);
      //  thermalAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*thermalAbsorptance)));
      //}

      // DLM: set in construction
      // solarAbsorptance
      //boost::optional<double> solarAbsorptance = masslessOpaqueMaterial.solarAbsorptance();
      //if (solarAbsorptance){
      //  QDomElement solarAbsorptanceElement = doc.createElement("SolAbs"); 
      //  result->appendChild(solarAbsorptanceElement);
      //  solarAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*solarAbsorptance)));
      //}

      // DLM: set in construction
      // visibleAbsorptance
      //boost::optional<double> visibleAbsorptance = masslessOpaqueMaterial.visibleAbsorptance();
      //if (visibleAbsorptance){
      //  QDomElement visibleAbsorptanceElement = doc.createElement("VisAbs");
      //  result->appendChild(visibleAbsorptanceElement);
      //  visibleAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*visibleAbsorptance)));
      //}

      // thermalResistance
      // os units = m2-K/W, sdd units = hr*ft2*degF/Btu
      double thermalResistance = masslessOpaqueMaterial.thermalResistance();
      Quantity rValueWh(thermalResistance, WhUnit(WhExpnt(-1,0,2,1)));
      OptionalQuantity rValueIP = QuantityConverter::instance().convert(rValueWh, btuSys);
      OS_ASSERT(rValueIP);
      OS_ASSERT(rValueIP->units() ==  BTUUnit(BTUExpnt(-1,2,1,1)));
      QDomElement rValueElement = doc.createElement("RVal");
      result->appendChild(rValueElement);
      rValueElement.appendChild(doc.createTextNode(QString::number(rValueIP->value())));
    }else if (material.optionalCast<model::AirGap>()){
      model::AirGap airGap = material.cast<model::AirGap>();
      result = doc.createElement("Mat");
      m_translatedObjects[airGap.handle()] = *result;

      // name
      std::string name = airGap.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result->appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // thermalResistance
      // os units = m2-K/W, sdd units = hr*ft2*degF/Btu
      double thermalResistance = airGap.thermalResistance();
      Quantity rValueWh(thermalResistance, WhUnit(WhExpnt(-1,0,2,1)));
      OptionalQuantity rValueIP = QuantityConverter::instance().convert(rValueWh, btuSys);
      OS_ASSERT(rValueIP);
      OS_ASSERT(rValueIP->units() ==  BTUUnit(BTUExpnt(-1,2,1,1)));
      QDomElement rValueElement = doc.createElement("RVal");
      result->appendChild(rValueElement);
    }

    return result;
  }

    
} // sdd
} // openstudio
