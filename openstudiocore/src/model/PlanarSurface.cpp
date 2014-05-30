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

#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Model.hpp"

#include "PlanarSurfaceGroup.hpp"
#include "Space.hpp"
#include "ModelExtensibleGroup.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "LayeredConstruction.hpp"
#include "LayeredConstruction_Impl.hpp"
#include "Material.hpp"
#include "AirWallMaterial.hpp"
#include "AirWallMaterial_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"

#include "../utilities/sql/SqlFile.hpp"

#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Transformation.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/math/constants/constants.hpp>
#include <boost/lexical_cast.hpp>

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;

using boost::to_upper_copy;

namespace openstudio {
namespace model {

  namespace detail {

    // constructor
    PlanarSurface_Impl::PlanarSurface_Impl(IddObjectType type,
                                           Model_Impl* model)
      : ParentObject_Impl(type, model)
    {
      // connect signals
      bool connected = connect(this, SIGNAL(onChange()), this, SLOT(clearCachedVariables()));
      OS_ASSERT(connected);
    }

    // constructor
    PlanarSurface_Impl::PlanarSurface_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle)
    {
      // connect signals
      bool connected = connect(this, SIGNAL(onChange()), this, SLOT(clearCachedVariables()));
      OS_ASSERT(connected);
    }

    PlanarSurface_Impl::PlanarSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
      : ParentObject_Impl(other,model,keepHandle)
    {
      // connect signals
      bool connected = connect(this, SIGNAL(onChange()), this, SLOT(clearCachedVariables()));
      OS_ASSERT(connected);
    }

    PlanarSurface_Impl::PlanarSurface_Impl(const PlanarSurface_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
      : ParentObject_Impl(other,model,keepHandle)
    {
      // connect signals
      bool connected = connect(this, SIGNAL(onChange()), this, SLOT(clearCachedVariables()));
      OS_ASSERT(connected);
    }

    boost::optional<ConstructionBase> PlanarSurface_Impl::construction() const
    {
      boost::optional<std::pair<ConstructionBase, int> > result = this->constructionWithSearchDistance();
      if (result){
        return result->first;
      }
      return boost::none;
    }

    /// get the vertices
    Point3dVector PlanarSurface_Impl::vertices() const
    {
      if (!m_cachedVertices){
        Point3dVector result;

        for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups()))
        {
          OptionalDouble x = group.getDouble(0);
          OptionalDouble y = group.getDouble(1);
          OptionalDouble z = group.getDouble(2);

          if (x && y && z){
            result.push_back(Point3d(*x, *y, *z));
          }else{
            LOG(Error, "Could not read vertex " << group.groupIndex() << " in " << briefDescription() << "." );
          }
        }

        m_cachedVertices = result;
      }

      return m_cachedVertices.get();
    }

    /// set the vertices
    bool PlanarSurface_Impl::setVertices(const std::vector<Point3d>& vertices)
    {
      unsigned n = vertices.size();

      if (n < 3) {
        LOG(Error, "Cannot set vertices because size of vertices is " << vertices.size() << ", which is less than 3.");
        return false;
      }

      // compute plane
      // DLM: still ok to make sure we can compute a plane at least
      boost::optional<Plane> plane;
      try {
        plane = Plane(vertices);
      }catch (const std::exception&){
        LOG(Error, "Could not compute plane for vertices for '" << this->name().get() << "'.");
        return false;
      }

      bool result = true;

      LOG(Debug, "Before setVertices have " << numFields() << " fields.");

      clearExtensibleGroups(false);

      LOG(Debug, "After clearExtensibleGroups in setVertices have " << numFields() << " fields.");

      // set the vertices
      // bool nonPlanarPointFound = false;
      for (unsigned vertexIndex = 0; vertexIndex < n; ++vertexIndex) {

        // DLM: fitting points to plane here as well as in SketchUp was resulting in unacceptable rounding errors
        // just use point directly
        Point3d vertex = vertices[vertexIndex];

        // project point to plane
        //Point3d vertex = plane->project(vertices[vertexIndex]);
        //LOG(Debug, "Original vertex = " << vertices[vertexIndex] << ", new vertex = " << vertex);

        // see how far off we were
        //if (!nonPlanarPointFound){
        //  Vector3d diff = vertex - vertices[vertexIndex];
        //  // see if more than 1 inch off of plane
        //  if (diff.length() > 0.0254){
        //    LOG(Warn, "PlanarSurface '" << this->name().get() << "' given non-planar vertices, these have been corrected.");
        //    nonPlanarPointFound = true;
        //  }
        //}

        std::vector<std::string> values;
        values.push_back(toString(vertex.x()));
        values.push_back(toString(vertex.y()));
        values.push_back(toString(vertex.z()));

        ModelExtensibleGroup group = pushExtensibleGroup(values, false).cast<ModelExtensibleGroup>();
        OS_ASSERT(!group.empty());
      }

      LOG(Debug, "After setVertices have " << numFields() << " fields.  Size of vertices is "
          << vertices.size() << ".");

      this->emitChangeSignals();

      return result;
    }

    bool PlanarSurface_Impl::setUFactor(boost::optional<double> value) {
      // DLM: change interface to take a double?
      OS_ASSERT(value);
      return setUFactor(*value);
    }

    bool PlanarSurface_Impl::setThermalConductance(boost::optional<double> value) {
      // DLM: change interface to take a double?
      OS_ASSERT(value);
      return setThermalConductance(*value);
    }

    /// is this surface an air wall
    bool PlanarSurface_Impl::isAirWall() const
    {
      bool result = false;

      OptionalConstructionBase oConstruction = this->construction();
      if (oConstruction && oConstruction->isModelPartition()) {
        LayeredConstruction construction = oConstruction->cast<LayeredConstruction>();
        if (construction.numLayers() == 1) {
          MaterialVector layers = construction.layers();
          OS_ASSERT(layers.size() == 1u);
          result = layers[0].optionalCast<AirWallMaterial>();
        }else if (construction.numLayers() == 0) {
          LOG(Error, "Air wall detected with zero layers, classifying as air wall");
          result = true;
        }else {
          LOG(Error, "Air wall detected with more than one layer, classifying as non-air wall");
          result = false;
        }
      }
      return result;
    }

    // compute gross area (m^2)
    double PlanarSurface_Impl::grossArea() const
    {
      double result = 0.0;
      OptionalDouble area = getArea(vertices());
      if (area){
        result = *area;
      }
      return result;
    }

    // compute net area (m^2)
    double PlanarSurface_Impl::netArea() const
    {
      double result = this->grossArea();

      // subtract net area of child planar surfaces
      for (const ModelObject& child : this->children()){
        OptionalPlanarSurface surface = child.optionalCast<PlanarSurface>();
        if (surface){
          if (surface->subtractFromGrossArea()){
            double multiplier = 1.0;
            OptionalSubSurface subSurface = child.optionalCast<SubSurface>();
            if (subSurface){
              multiplier = subSurface->multiplier();
            }
            result -= multiplier * surface->grossArea();
          }
        }
      }

      return result;
    }

    /// get the outward normal
    Vector3d PlanarSurface_Impl::outwardNormal() const
    {
      if (!m_cachedOutwardNormal){
        Point3dVector vertices = this->vertices();
        m_cachedOutwardNormal = getOutwardNormal(vertices);
        if(!m_cachedOutwardNormal){
          std::string surfaceNameMsg;
          boost::optional<std::string> name = this->name();
          if (name){
            surfaceNameMsg = ", surface name = '" + *name + "'";
          }
          LOG_AND_THROW("Cannot compute outward normal for vertices " << vertices << surfaceNameMsg);
        }
      }
      return m_cachedOutwardNormal.get();
    }

    double PlanarSurface_Impl::tilt() const {
      OptionalVector3d on = outwardNormal();
      if (!on) {
        LOG_AND_THROW("Cannot calculate PlanarSurface tilt because there is no outwardNormal.");
      }
      Vector3d n = *on;
      Vector3d up(0.0,0.0,1.0);
      return getAngle(n,up);
    }

    double PlanarSurface_Impl::azimuth() const {
      OptionalVector3d on = outwardNormal();
      if (!on) {
        LOG_AND_THROW("Cannot calculate PlanarSurface azimuth because there is no outwardNormal.");
      }
      Vector3d n = *on;
      Vector3d north(0.0,1.0,0.0);
      double rawAngle = getAngle(n,north);
      if (on->x() < 0.0) { return -rawAngle + 2.0*boost::math::constants::pi<double>(); }
      return rawAngle;
    }

    boost::optional<double> PlanarSurface_Impl::uFactor() const {
      return boost::none;
    }

    boost::optional<double> PlanarSurface_Impl::thermalConductance() const {
      return boost::none;
    }

    bool PlanarSurface_Impl::setUFactor(double value) {
      return false;
    }

    bool PlanarSurface_Impl::setThermalConductance(double value) {
      return false;
    }

    boost::optional<double> PlanarSurface_Impl::heatCapacity() const {
      OptionalDouble result;
      OptionalConstructionBase oConstruction = this->construction();

      if (oConstruction) { result = oConstruction->heatCapacity(); }

      return result;
    }

    boost::optional<double> PlanarSurface_Impl::interiorVisibleAbsorptance() const
    {
      OptionalDouble result;
      OptionalSqlFile sqlFile = model().sqlFile();
      OptionalConstructionBase oConstruction = this->construction();

      if (oConstruction) {
        // from input
        OptionalDouble inputResult = oConstruction->interiorVisibleAbsorptance();

        // from output
        OptionalDouble outputResult;
        if (sqlFile) {
          OptionalString constructionName = oConstruction->name();
          if (constructionName){
            std::string query = "SELECT InsideAbsorpVis FROM constructions WHERE Name='" + to_upper_copy(*constructionName) + "'";
            outputResult = sqlFile->execAndReturnFirstDouble(query);
          }
        }

        if (inputResult) {
          result = inputResult;
          if (outputResult) {
            compareInputAndOutput(*oConstruction,"interior visible absorptance",*inputResult,*outputResult,1.0E-5);
          }
        }
        else { result = outputResult; }

      }

      return result;
    }

    /// get exterior visible absorptance (unitless)
    boost::optional<double> PlanarSurface_Impl::exteriorVisibleAbsorptance() const
    {
      OptionalDouble result;
      OptionalSqlFile sqlFile = model().sqlFile();
      OptionalConstructionBase oConstruction = this->construction();

      if (oConstruction) {
        // from input
        OptionalDouble inputResult = oConstruction->exteriorVisibleAbsorptance();

        // from output
        OptionalDouble outputResult;
        if (sqlFile) {
          OptionalString constructionName = oConstruction->name();
          if (constructionName){
            std::string query = "SELECT OutsideAbsorpVis FROM constructions WHERE Name='" + to_upper_copy(*constructionName) + "'";
            outputResult = sqlFile->execAndReturnFirstDouble(query);
          }
        }

        if (inputResult) {
          result = inputResult;
          if (outputResult) {
            compareInputAndOutput(*oConstruction,"exterior visible absorptance",*inputResult,*outputResult,1.0E-5);
          }
        }
        else { result = outputResult; }
      }

      return result;
    }

    /// get visible transmittance (unitless)
    /// requires 'EnvelopeSummary' summary table
    boost::optional<double> PlanarSurface_Impl::visibleTransmittance() const
    {
      OptionalDouble result;
      OptionalSqlFile sqlFile = model().sqlFile();
      OptionalConstructionBase oConstruction = this->construction();

      if (oConstruction) {
        // from input
        OptionalDouble inputResult = oConstruction->visibleTransmittance();

        // from output
        OptionalDouble outputResult;
        if (sqlFile) {
          OptionalString constructionName = oConstruction->name();
          if (constructionName){
            std::string query = "SELECT RowId FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Exterior Fenestration' AND ColumnName='Construction' AND Value='" + to_upper_copy(*constructionName) + "'";
            OptionalInt rowId = sqlFile->execAndReturnFirstInt(query);
            if (rowId) {
              std::stringstream ss;
              ss << "SELECT Value FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Exterior Fenestration' AND RowId='";
              ss << *rowId << "' AND ColumnName='Glass Visible Transmittance'";
              query = ss.str();
              outputResult = sqlFile->execAndReturnFirstDouble(query);
            }else{
              query = "SELECT RowId FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Interior Fenestration' AND ColumnName='Construction' AND Value='" + to_upper_copy(*constructionName) + "'";
              rowId = sqlFile->execAndReturnFirstInt(query);
              if (rowId) {
                std::stringstream ss;
                ss << "SELECT Value FROM tabulardatawithstrings WHERE ReportName='EnvelopeSummary' AND ReportForString='Entire Facility' AND TableName='Interior Fenestration' AND RowId='";
                ss << *rowId << "' AND ColumnName='Glass Visible Transmittance'";
                query = ss.str();
                outputResult = sqlFile->execAndReturnFirstDouble(query);
              }
            }

          }
        }

        if (inputResult) {
          result = inputResult;
          if (outputResult) {
            compareInputAndOutput(*oConstruction,"visible transmittance",*inputResult,*outputResult,1.0E-5);
          }
        }
        else { result = outputResult; }

      }

      return result;
    }

    bool PlanarSurface_Impl::equalVertices(const PlanarSurface& other) const
    {
      std::vector<Point3d> vertices1 = this->vertices();
      std::vector<Point3d> vertices2 = other.vertices();

      boost::optional<PlanarSurfaceGroup> group;

      Transformation t1;
      group = this->planarSurfaceGroup();
      if (group){
        t1 = group->buildingTransformation();
      }

      Transformation t2;
      group = other.planarSurfaceGroup();
      if (group){
        t2 = group->buildingTransformation();
      }

      return circularEqual(t1*vertices1, t2*vertices2);
    }

    bool PlanarSurface_Impl::reverseEqualVertices(const PlanarSurface& other) const
    {
      std::vector<Point3d> vertices1 = this->vertices();
      std::vector<Point3d> vertices2 = other.vertices();
      std::reverse(vertices2.begin(), vertices2.end());

      boost::optional<PlanarSurfaceGroup> group;

      Transformation t1;
      group = this->planarSurfaceGroup();
      if (group){
        t1 = group->buildingTransformation();
      }

      Transformation t2;
      group = other.planarSurfaceGroup();
      if (group){
        t2 = group->buildingTransformation();
      }

      return circularEqual(t1*vertices1, t2*vertices2);
    }

    Plane PlanarSurface_Impl::plane() const
    {
      if (!m_cachedPlane){
        m_cachedPlane = Plane(this->vertices());
      }
      return m_cachedPlane.get();
    }

    std::vector<std::vector<Point3d> > PlanarSurface_Impl::triangulation() const
    {
      if (m_cachedTriangulation.empty()){
        Transformation faceTransformation = Transformation::alignFace(this->vertices());
        Transformation faceTransformationInverse = faceTransformation.inverse();

        std::vector<Point3d> faceVertices = faceTransformationInverse*this->vertices();

        std::vector<std::vector<Point3d> > faceHoles;
        for (const ModelObject& child : this->children()){
          OptionalPlanarSurface surface = child.optionalCast<PlanarSurface>();
          if (surface){
            if (surface->subtractFromGrossArea()){
              faceHoles.push_back(faceTransformationInverse*surface->vertices());
            }
          }
        }

        std::vector<std::vector<Point3d> > faceTriangulation = computeTriangulation(faceVertices, faceHoles);

        for (const std::vector<Point3d>& faceTriangle : faceTriangulation){
          m_cachedTriangulation.push_back(faceTransformation*faceTriangle);
        }
      }
      return m_cachedTriangulation;
    }

    Point3d PlanarSurface_Impl::centroid() const
    {
      boost::optional<Point3d> result = getCentroid(this->vertices());
      OS_ASSERT(result);
      return *result;
    }

    boost::optional<ModelObject> PlanarSurface_Impl::constructionAsModelObject() const
    {
      return static_cast<boost::optional<ModelObject> >(this->construction());
    }

    boost::optional<ModelObject> PlanarSurface_Impl::planarSurfaceGroupAsModelObject() const {
      OptionalModelObject result;
      OptionalPlanarSurfaceGroup intermediate = planarSurfaceGroup();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> PlanarSurface_Impl::spaceAsModelObject() const {
      OptionalModelObject result;
      OptionalSpace intermediate = space();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    void PlanarSurface_Impl::clearCachedVariables()
    {
      m_cachedVertices.reset();
      m_cachedPlane.reset();
      m_cachedOutwardNormal.reset();
      m_cachedTriangulation.clear();
    }

    bool PlanarSurface_Impl::setConstructionAsModelObject(boost::optional<ModelObject> modelObject)
    {
      bool result = false;
      if (!modelObject){
        // reset construction not currently implemented
        result = false;
      }else{
        boost::optional<ConstructionBase> construction = modelObject->optionalCast<ConstructionBase>();
        if (construction){
          result = this->setConstruction(*construction);
        }
      }
      return result;
    }

  }// detail

PlanarSurface::PlanarSurface(IddObjectType type, const std::vector<Point3d>& vertices,
                             const Model& model)
  : ParentObject(type,model)
{
  OS_ASSERT(getImpl<detail::PlanarSurface_Impl>());
  bool ok = this->setVertices(vertices);
  if (!ok){
    this->remove();
    LOG_AND_THROW("Cannot create a surface with vertices " << vertices);
  }
}

PlanarSurface::PlanarSurface(std::shared_ptr<detail::PlanarSurface_Impl> p)
  : ParentObject(p)
{}

boost::optional<ConstructionBase> PlanarSurface::construction() const
{
  return getImpl<detail::PlanarSurface_Impl>()->construction();
}

boost::optional<std::pair<ConstructionBase, int> > PlanarSurface::constructionWithSearchDistance() const
{
  return getImpl<detail::PlanarSurface_Impl>()->constructionWithSearchDistance();
}

bool PlanarSurface::isConstructionDefaulted() const
{
  return getImpl<detail::PlanarSurface_Impl>()->isConstructionDefaulted();
}

/// get the vertices
Point3dVector PlanarSurface::vertices() const
{
  return getImpl<detail::PlanarSurface_Impl>()->vertices();
}

/// set the vertices
bool PlanarSurface::setVertices(const std::vector<Point3d>& vertices)
{
  return getImpl<detail::PlanarSurface_Impl>()->setVertices(vertices);
}

/// set the construction object
bool PlanarSurface::setConstruction(const ConstructionBase& construction)
{
  return getImpl<detail::PlanarSurface_Impl>()->setConstruction(construction);
}

void PlanarSurface::resetConstruction()
{
  getImpl<detail::PlanarSurface_Impl>()->resetConstruction();
}

bool PlanarSurface::setUFactor(double value) {
  return getImpl<detail::PlanarSurface_Impl>()->setUFactor(value);
}

bool PlanarSurface::setThermalConductance(double value) {
  return getImpl<detail::PlanarSurface_Impl>()->setThermalConductance(value);
}

/// is this surface an air wall
bool PlanarSurface::isAirWall() const
{
  return getImpl<detail::PlanarSurface_Impl>()->isAirWall();
}

// compute gross area (m^2)
double PlanarSurface::grossArea() const
{
  return getImpl<detail::PlanarSurface_Impl>()->grossArea();
}

/// should subtract this surface from parent's gross area for net area
bool PlanarSurface::subtractFromGrossArea() const
{
  return getImpl<detail::PlanarSurface_Impl>()->subtractFromGrossArea();
}

// compute net area (m^2)
double PlanarSurface::netArea() const
{
  return getImpl<detail::PlanarSurface_Impl>()->netArea();
}

/// get the outward normal
Vector3d PlanarSurface::outwardNormal() const
{
  return getImpl<detail::PlanarSurface_Impl>()->outwardNormal();
}

double PlanarSurface::tilt() const {
  return getImpl<detail::PlanarSurface_Impl>()->tilt();
}

double PlanarSurface::azimuth() const {
  return getImpl<detail::PlanarSurface_Impl>()->azimuth();
}

boost::optional<double> PlanarSurface::uFactor() const {
  return getImpl<detail::PlanarSurface_Impl>()->uFactor();
}

boost::optional<double> PlanarSurface::thermalConductance() const {
  return getImpl<detail::PlanarSurface_Impl>()->thermalConductance();
}

boost::optional<double> PlanarSurface::heatCapacity() const {
  return getImpl<detail::PlanarSurface_Impl>()->heatCapacity();
}

/// get interior visible absorptance (unitless)
OptionalDouble PlanarSurface::interiorVisibleAbsorptance() const
{
  return getImpl<detail::PlanarSurface_Impl>()->interiorVisibleAbsorptance();
}

OptionalDouble PlanarSurface::interiorVisibleAbsorbtance() const
{
  LOG(Warn,"interiorVisibleAbsorbtance() is deprecated and will be removed after 1.4.0");
  return interiorVisibleAbsorptance();
}

/// get exterior visible absorptance (unitless)
OptionalDouble PlanarSurface::exteriorVisibleAbsorptance() const
{
  return getImpl<detail::PlanarSurface_Impl>()->exteriorVisibleAbsorptance();
}

OptionalDouble PlanarSurface::exteriorVisibleAbsorbtance() const
{
  LOG(Warn,"exteriorVisibleAbsorbtance() is deprecated and will be removed after 1.4.0");
  return exteriorVisibleAbsorptance();
}

boost::optional<PlanarSurfaceGroup> PlanarSurface::planarSurfaceGroup() const
{
  return getImpl<detail::PlanarSurface_Impl>()->planarSurfaceGroup();
}

boost::optional<Space> PlanarSurface::space() const
{
  return getImpl<detail::PlanarSurface_Impl>()->space();
}

/// get visible transmittance (unitless)
/// requires 'EnvelopeSummary' summary table
boost::optional<double> PlanarSurface::visibleTransmittance() const
{
  return getImpl<detail::PlanarSurface_Impl>()->visibleTransmittance();
}

bool PlanarSurface::equalVertices(const PlanarSurface& other) const
{
  return getImpl<detail::PlanarSurface_Impl>()->equalVertices(other);
}

bool PlanarSurface::reverseEqualVertices(const PlanarSurface& other) const
{
  return getImpl<detail::PlanarSurface_Impl>()->reverseEqualVertices(other);
}

Plane PlanarSurface::plane() const
{
  return getImpl<detail::PlanarSurface_Impl>()->plane();
}

std::vector<std::vector<Point3d> > PlanarSurface::triangulation() const
{
  return getImpl<detail::PlanarSurface_Impl>()->triangulation();
}

Point3d PlanarSurface::centroid() const
{
  return getImpl<detail::PlanarSurface_Impl>()->centroid();
}

std::vector<PlanarSurface> PlanarSurface::findPlanarSurfaces(const std::vector<PlanarSurface>& planarSurfaces,
                                                             boost::optional<double> minDegreesFromNorth,
                                                             boost::optional<double> maxDegreesFromNorth,
                                                             boost::optional<double> minDegreesTilt,
                                                             boost::optional<double> maxDegreesTilt,
                                                             double tol)
{
  std::vector<PlanarSurface> result;

  // check inputs
  bool error = false;

  if (minDegreesFromNorth && ((*minDegreesFromNorth < 0) || (*minDegreesFromNorth > 360))){
    error = true;
    LOG(Error, "minDegreesFromNorth out of range [0-360], " << *minDegreesFromNorth);
  }

  if (maxDegreesFromNorth && ((*maxDegreesFromNorth < 0) || (*maxDegreesFromNorth > 360))){
    error = true;
    LOG(Error, "maxDegreesFromNorth out of range [0-360], " << *maxDegreesFromNorth);
  }

  if (minDegreesTilt && ((*minDegreesTilt < 0) || (*minDegreesTilt > 180))){
    error = true;
    LOG(Error, "minDegreesTilt out of range [0-180], " << *minDegreesTilt);
  }

  if (maxDegreesTilt && ((*maxDegreesTilt < 0) || (*maxDegreesTilt > 180))){
    error = true;
    LOG(Error, "maxDegreesTilt out of range [0-180], " << *maxDegreesTilt);
  }

  if (minDegreesTilt && maxDegreesTilt && (*minDegreesTilt > *maxDegreesTilt)){
    error = true;
    LOG(Error, "minDegreesTilt (" << *minDegreesTilt << ") > maxDegreesTilt (" << *maxDegreesTilt << ")");
  }

  if (error){
    return result;
  }

  std::map<PlanarSurfaceGroup, Transformation> siteTransformationMap;
  Vector3d up(0.0,0.0,1.0);
  Vector3d north(0.0,1.0,0.0);

  // inputs ok, loop over surfaces
  for (const PlanarSurface& planarSurface : planarSurfaces){

    // find the transformation to site coordinates
    Transformation siteTransformation;

    OptionalPlanarSurfaceGroup group = planarSurface.planarSurfaceGroup();
    if (group){
      std::map<PlanarSurfaceGroup, Transformation>::const_iterator it = siteTransformationMap.find(*group);

      if (it != siteTransformationMap.end()){
        siteTransformation = it->second;
      }else{
        siteTransformation = group->buildingTransformation();
        siteTransformationMap.insert(std::make_pair(*group, siteTransformation));
      }
    }

    // vertices and outward normal in site coordinates
    Point3dVector siteVertices = siteTransformation * planarSurface.vertices();
    OptionalVector3d siteOutwardNormal = getOutwardNormal(siteVertices);

    if (!siteOutwardNormal){
      LOG(Error, "Could not compute outward normal for planarSurface " << planarSurface);
      continue;
    }

    double degreesTilt = radToDeg( getAngle(*siteOutwardNormal, up) );

    if (minDegreesTilt && (*minDegreesTilt - tol > degreesTilt)){
      continue;
    }

    if (maxDegreesTilt && (*maxDegreesTilt + tol < degreesTilt)){
      continue;
    }

    double degreesFromNorth;
    if (siteOutwardNormal->x() < 0.0){
      degreesFromNorth = 360 - radToDeg( getAngle(*siteOutwardNormal, north) );
    }else{
      degreesFromNorth = radToDeg( getAngle(*siteOutwardNormal, north) );
    }

    if (minDegreesFromNorth && maxDegreesFromNorth){
      if (*maxDegreesFromNorth >= *minDegreesFromNorth){
        if ((*minDegreesFromNorth - tol > degreesFromNorth) || (*maxDegreesFromNorth + tol < degreesFromNorth)){
          continue;
        }
      }else{
        if ((*minDegreesFromNorth - tol > degreesFromNorth) && (*maxDegreesFromNorth + tol < degreesFromNorth)){
          continue;
        }
      }
    }else if (minDegreesFromNorth && (*minDegreesFromNorth - tol > degreesFromNorth)){
      continue;
    }else if (maxDegreesFromNorth && (*maxDegreesFromNorth + tol < degreesFromNorth)){
      continue;
    }

    result.push_back(planarSurface);

  }

  return result;
}

double PlanarSurface::filmResistance(const FilmResistanceType& type) {
  // assumes suface emmittance of 0.90
  switch (type.value()) {
    case FilmResistanceType::StillAir_HorizontalSurface_HeatFlowsUpward :
      return 0.107427212046;
    case FilmResistanceType::StillAir_45DegreeSurface_HeatFlowsUpward :
      return 0.109188313883;
    case FilmResistanceType::StillAir_VerticalSurface :
      return 0.119754924904;
    case FilmResistanceType::StillAir_45DegreeSurface_HeatFlowsDownward :
      return 0.133843739599;
    case FilmResistanceType::StillAir_HorizontalSurface_HeatFlowsDownward :
      return 0.162021368988;
    case FilmResistanceType::MovingAir_15mph :
      return 0.029938731226;
    case FilmResistanceType::MovingAir_7p5mph :
      return 0.044027545921;
    default:
      LOG_AND_THROW("Unknown FilmResistanceType.");
  };
  OS_ASSERT(false);
  return 0.0;
}

double PlanarSurface::stillAirFilmResistance(double tilt) {
  // filmResistance = a + b*tilt^2
  // approximately 1% relative accuracy on five interpolated points, which is about all you can
  // expect given that the original data (IP units) was in just two significant figures.
  return 0.106042621636483 + 0.005513085609325 * std::pow(tilt,2);
}


} // model
} // openstudio
