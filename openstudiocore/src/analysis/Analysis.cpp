/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <analysis/Analysis.hpp>
#include <analysis/Analysis_Impl.hpp>

#include <analysis/Algorithm_Impl.hpp>
#include <analysis/DakotaAlgorithm.hpp>
#include <analysis/DakotaAlgorithm_Impl.hpp>
#include <analysis/DataPoint_Impl.hpp>
#include <analysis/Measure.hpp>
#include <analysis/InputVariable.hpp>
#include <analysis/OpenStudioAlgorithm.hpp>
#include <analysis/OpenStudioAlgorithm_Impl.hpp>
#include <analysis/Problem_Impl.hpp>

#include <runmanager/lib/Job.hpp>

#include <utilities/document/Table.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Json.hpp>
#include <utilities/core/PathHelpers.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  Analysis_Impl::Analysis_Impl(const std::string& name,
                               const Problem& problem,
                               const FileReferenceType& seedType)
    : AnalysisObject_Impl(name),
      m_problem(problem),
      m_seed(FileReference(toPath("*." + seedType.valueDescription()))),
      m_resultsAreInvalid(false),
      m_dataPointsAreInvalid(false)
  {
    m_seed.makePathRelative();
    if (problem.inputFileType() && (seedType != problem.inputFileType())) {
      LOG_AND_THROW("Unable to construct Analysis '" << name << "', because the seed file is of "
        << "type '*." << seedType.valueDescription() << "', but Problem '"
        << problem.name() << " takes an input file of type '*."
        << problem.inputFileType()->valueDescription() << "'.");
    }
    connectChild(m_problem,false);
  }

  Analysis_Impl::Analysis_Impl(const std::string& name,
                               const Problem& problem,
                               const FileReference& seed)
    : AnalysisObject_Impl(name),
      m_problem(problem),
      m_seed(seed),
      m_resultsAreInvalid(false),
      m_dataPointsAreInvalid(false)
  {
    if (problem.inputFileType() && (seed.fileType() != problem.inputFileType())) {
      LOG_AND_THROW("Unable to construct Analysis '" << name << "', because the seed file is of "
        << "type '*." << seed.fileType().valueDescription() << "', but Problem '"
        << problem.name() << " takes an input file of type '*."
        << problem.inputFileType()->valueDescription() << "'.");
    }
    connectChild(m_problem,false);
  }

  Analysis_Impl::Analysis_Impl(const std::string& name,
                               const Problem& problem,
                               const FileReference& seed,
                               const FileReference& weatherFile)
    : AnalysisObject_Impl(name),
      m_problem(problem),
      m_seed(seed),
      m_weatherFile(weatherFile),
      m_resultsAreInvalid(false),
      m_dataPointsAreInvalid(false)
  {
    if (problem.inputFileType() && (seed.fileType() != problem.inputFileType())) {
      LOG_AND_THROW("Unable to construct Analysis '" << name << "', because the seed file is of "
        << "type '*." << seed.fileType().valueDescription() << "', but Problem '"
        << problem.name() << " takes an input file of type '*."
        << problem.inputFileType()->valueDescription() << "'.");
    }
    if (weatherFile.fileType() != FileReferenceType::EPW) {
      LOG_AND_THROW("Weather file '" << toString(weatherFile.path()) << "' is not an epw file.");
    }
    connectChild(m_problem,false);
  }

  Analysis_Impl::Analysis_Impl(const std::string& name,
                               const Problem& problem,
                               const Algorithm& algorithm,
                               const FileReference& seed)
    : AnalysisObject_Impl(name),
      m_problem(problem),
      m_algorithm(algorithm),
      m_seed(seed),
      m_resultsAreInvalid(false),
      m_dataPointsAreInvalid(false)
  {
    if (!m_algorithm->isCompatibleProblemType(m_problem)) {
      LOG_AND_THROW("Unable to construct Analysis '" << name << "', because Problem '"
          << problem.name() << "' is incompatible with Algorithm '" << algorithm.name() << "'.");
    }
    if (problem.inputFileType() && (seed.fileType() != problem.inputFileType())) {
      LOG_AND_THROW("Unable to construct Analysis '" << name << "', because the seed file is of "
        << "type '*." << seed.fileType().valueDescription() << "', but Problem '"
        << problem.name() << " takes an input file of type '*."
        << problem.inputFileType()->valueDescription() << "'.");
    }
    connectChild(m_problem,false);
    connectChild(*m_algorithm,false);
  }

  Analysis_Impl::Analysis_Impl(const std::string& name,
                               const Problem& problem,
                               const Algorithm& algorithm,
                               const FileReference& seed,
                               const FileReference& weatherFile)
    : AnalysisObject_Impl(name),
      m_problem(problem),
      m_algorithm(algorithm),
      m_seed(seed),
      m_weatherFile(weatherFile),
      m_resultsAreInvalid(false),
      m_dataPointsAreInvalid(false)
  {
    if (!m_algorithm->isCompatibleProblemType(m_problem)) {
      LOG_AND_THROW("Unable to construct Analysis '" << name << "', because Problem '"
          << problem.name() << "' is incompatible with Algorithm '" << algorithm.name() << "'.");
    }
    if (problem.inputFileType() && (seed.fileType() != problem.inputFileType())) {
      LOG_AND_THROW("Unable to construct Analysis '" << name << "', because the seed file is of "
        << "type '*." << seed.fileType().valueDescription() << "', but Problem '"
        << problem.name() << " takes an input file of type '*."
        << problem.inputFileType()->valueDescription() << "'.");
    }
    if (weatherFile.fileType() != FileReferenceType::EPW) {
      LOG_AND_THROW("Weather file '" << toString(weatherFile.path()) << "' is not an epw file.");
    }
    connectChild(m_problem,false);
    connectChild(*m_algorithm,false);
  }

  Analysis_Impl::Analysis_Impl(const UUID& uuid,
                               const UUID& versionUUID,
                               const std::string& name,
                               const std::string& displayName,
                               const std::string& description,
                               const Problem& problem,
                               const boost::optional<Algorithm>& algorithm,
                               const FileReference& seed,
                               const boost::optional<FileReference>& weatherFile,
                               const std::vector<DataPoint>& dataPoints,
                               bool resultsAreInvalid,
                               bool dataPointsAreInvalid)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_problem(problem),
      m_algorithm(algorithm),
      m_seed(seed),
      m_weatherFile(weatherFile),
      m_dataPoints(dataPoints),
      m_resultsAreInvalid(resultsAreInvalid),
      m_dataPointsAreInvalid(dataPointsAreInvalid)
  {
    // override default of objects not being dirty when they are de-serialized
    if (resultsAreInvalid || dataPointsAreInvalid) {
      setDirtyFlag();
    }
    connectChild(m_problem,false);
    if (m_algorithm) {
      connectChild(*m_algorithm,false);
    }
    BOOST_FOREACH(DataPoint& dataPoint,m_dataPoints) {
      connectChild(dataPoint,false);
    }
  }

  Analysis_Impl::Analysis_Impl(const Analysis_Impl &other)
    : AnalysisObject_Impl(other),
      m_problem(other.problem().clone().cast<Problem>()),
      m_seed(other.seed().clone()),
      m_resultsAreInvalid(other.resultsAreInvalid()),
      m_dataPointsAreInvalid(other.dataPointsAreInvalid())
  {
    connectChild(m_problem,false);
    if (other.algorithm()) {
      m_algorithm = other.algorithm().get().clone().cast<Algorithm>();
      connectChild(*m_algorithm,false);
    }
    if (other.weatherFile()) {
      m_weatherFile = other.weatherFile().get().clone();
    }
    BOOST_FOREACH(const DataPoint& dataPoint,other.dataPoints()) {
      m_dataPoints.push_back(dataPoint.clone().cast<DataPoint>());
      m_dataPoints.back().setProblem(m_problem);
      connectChild(m_dataPoints.back(),false);
    }
  }

  AnalysisObject Analysis_Impl::clone() const {
    boost::shared_ptr<Analysis_Impl> impl(new Analysis_Impl(*this));
    Analysis result(impl);
    Problem problem = result.problem();
    problem.setParent(result);
    OptionalAlgorithm algorithm = result.algorithm();
    if (algorithm) {
      algorithm->setParent(result);
    }
    DataPointVector dataPoints = result.dataPoints();
    BOOST_FOREACH(DataPoint& dataPoint,dataPoints) {
      dataPoint.setParent(result);
    }
    return result;
  }

  Problem Analysis_Impl::problem() const {
    return m_problem;
  }

  boost::optional<Algorithm> Analysis_Impl::algorithm() const {
    return m_algorithm;
  }

  FileReference Analysis_Impl::seed() const {
    return m_seed;
  }

  boost::optional<FileReference> Analysis_Impl::weatherFile() const {
    return m_weatherFile;
  }

  std::vector<DataPoint> Analysis_Impl::dataPoints() const {
    return m_dataPoints;
  }

  std::vector<DataPoint> Analysis_Impl::dataPointsToQueue() const {
    DataPointVector result;
    BOOST_FOREACH(const DataPoint& dataPoint,m_dataPoints) {
      if (dataPoint.selected() && (!dataPoint.complete())) {
        result.push_back(dataPoint);
      }
    }
    return result;
  }

  std::vector<DataPoint> Analysis_Impl::completeDataPoints() const {
    DataPointVector result;
    BOOST_FOREACH(const DataPoint& dataPoint,m_dataPoints) {
      if (dataPoint.isComplete()) {
        result.push_back(dataPoint);
      }
    }
    return result;
  }

  std::vector<DataPoint> Analysis_Impl::successfulDataPoints() const {
    DataPointVector result;
    BOOST_FOREACH(const DataPoint& dataPoint,m_dataPoints) {
      if (dataPoint.isComplete() && !dataPoint.failed()) {
        result.push_back(dataPoint);
      }
    }
    return result;
  }

  std::vector<DataPoint> Analysis_Impl::failedDataPoints() const {
    DataPointVector result;
    BOOST_FOREACH(const DataPoint& dataPoint,m_dataPoints) {
      if (dataPoint.isComplete() && dataPoint.failed()) {
        result.push_back(dataPoint);
      }
    }
    return result;
  }

  std::vector<DataPoint> Analysis_Impl::dataPointsNeedingDetails() const {
    DataPointVector result;
    BOOST_FOREACH(const DataPoint& dataPoint, m_dataPoints) {
      if (dataPoint.isComplete() && 
          (dataPoint.runType() == DataPointRunType::CloudDetailed) &&
          dataPoint.directory().empty())
      {
        result.push_back(dataPoint);
      }
    }
    return result;
  }

  std::vector<DataPoint> Analysis_Impl::getDataPoints(
      const std::vector<QVariant>& variableValues) const
  {
    DataPointVector result;
    BOOST_FOREACH(const DataPoint& dataPoint, m_dataPoints) {
      if (dataPoint.matches(variableValues)) {
        result.push_back(dataPoint);
      }
    }
    return result;
  }

  std::vector<DataPoint> Analysis_Impl::getDataPoints(
      const std::vector< boost::optional<Measure> >& measures) const
  {
    DataPointVector result;
    std::vector<QVariant> variableValues = problem().getVariableValues(measures);
    if (variableValues.size() == measures.size()) {
      // problem was able to match all measures
      result = getDataPoints(variableValues);
    }
    return result;
  }

  std::vector<DataPoint> Analysis_Impl::getDataPoints(const std::string& tag) const {
    DataPointVector result;
    BOOST_FOREACH(const DataPoint& dataPoint,m_dataPoints) {
      if (dataPoint.isTag(tag)) {
        result.push_back(dataPoint);
      }
    }
    return result;
  }

  boost::optional<DataPoint> Analysis_Impl::getDataPoint(
      const std::vector<Measure>& measures) const
  {
    OptionalDataPoint result;
    std::vector<QVariant> variableValues = problem().getVariableValues(measures);
    if (variableValues.size() == measures.size()) {
      // problem was able to match all measures
      DataPointVector intermediate = getDataPoints(variableValues);
      OS_ASSERT(intermediate.size() < 2u);
      if (intermediate.size() == 1u) {
        result = intermediate[0];
      }
    }
    return result;
  }

  boost::optional<DataPoint> Analysis_Impl::getDataPointByUUID(const UUID& uuid) const {
    OptionalDataPoint result;
    BOOST_FOREACH(const DataPoint& myDataPoint,dataPoints()) {
      if (myDataPoint.uuid() == uuid) {
        result = myDataPoint;
        break;
      }
    }
    return result;
  }

  boost::optional<DataPoint> Analysis_Impl::getDataPointByUUID(const DataPoint& dataPoint) const {
    OptionalDataPoint result;
    BOOST_FOREACH(const DataPoint& myDataPoint,dataPoints()) {
      if (dataPoint.uuidEqual(myDataPoint)) {
        result = myDataPoint;
        break;
      }
    }
    return result;
  }

  bool Analysis_Impl::resultsAreInvalid() const {
    return m_resultsAreInvalid;
  }

  bool Analysis_Impl::dataPointsAreInvalid() const {
    return m_dataPointsAreInvalid;
  }

  bool Analysis_Impl::setProblem(Problem& problem) {
    // problem is in charge of making sure it is internally consistent

    // need problem to be consistent with seed
    if (problem.inputFileType() && (seed().fileType() != problem.inputFileType().get()))
    {
      LOG(Error,"Cannot set problem to '" << problem.name() << "', because its input file type is "
          << problem.inputFileType().get().valueName() << " and the seed model type is "
          << seed().fileType().valueName() << ".");
      return false;
    }

    // need problem to be consistent with algorithm
    if (m_algorithm && !(m_algorithm->isCompatibleProblemType(problem))) {
      LOG(Error,"Cannot set problem to '" << problem.name() << "', because it is not compatible "
          << "with Algorithm '" << m_algorithm->name() << "'.");
      return false;
    }

    disconnectChild(m_problem);
    m_problem = problem;
    connectChild(m_problem,true);

    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool Analysis_Impl::setAlgorithm(Algorithm& algorithm) {
    if (!algorithm.isCompatibleProblemType(m_problem)) {
      LOG(Error,"Cannot set algorithm to '" << algorithm.name() << "', because it is incompatible "
          << "with this analysis's current problem '" << m_problem.name() << "'.");
      return false;
    }

    if (m_algorithm) {
      disconnectChild(*m_algorithm);
    }
    m_algorithm = algorithm;
    connectChild(*m_algorithm,true);
    onChange(AnalysisObject_Impl::Benign);
    return true;
  }

  void Analysis_Impl::clearAlgorithm() {
    if (m_algorithm) {
      disconnectChild(*m_algorithm);
      m_algorithm.reset();
      onChange(AnalysisObject_Impl::Benign);
    }
  }

  bool Analysis_Impl::setSeed(const FileReference& newSeed) {
    if (problem().inputFileType() &&
        (newSeed.fileType() != problem().inputFileType().get()))
    {
      return false;
    }

    m_seed = newSeed;
    emit seedChanged();
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  bool Analysis_Impl::clearSeed(const FileReferenceType& seedType) {
    if (problem().inputFileType() &&
        (seedType != problem().inputFileType().get()))
    {
      return false;
    }

    m_seed = FileReference(toPath("*." + seedType.valueDescription()));
    m_seed.makePathRelative();
    emit seedChanged();
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  bool Analysis_Impl::setWeatherFile(const FileReference& newWeatherFile) {
    if (newWeatherFile.fileType() != FileReferenceType::EPW) {
      return false;
    }
    m_weatherFile = newWeatherFile;
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  void Analysis_Impl::clearWeatherFile() {
    m_weatherFile.reset();
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool Analysis_Impl::addDataPoint(DataPoint& dataPoint) {
    if (m_dataPointsAreInvalid) {
      LOG(Info,"Current data points are invalid. Call removeAllDataPoints before adding new ones.");
      return false;
    }
    if (!(dataPoint.problemUUID() == problem().uuid())) {
      LOG(Error,"Cannot add given DataPoint to Analysis '" << name() <<
          "', because it is not associated with Problem '" << problem().name() << "'.");
      return false;
    }
    DataPointVector existingDataPoints = getDataPoints(dataPoint.variableValues());
    if (existingDataPoints.size() > 0) {
      OS_ASSERT(existingDataPoints.size() == 1); // dataPoint must be fully specified to be valid
      LOG(Info,"DataPoint not added to Analysis '" << name() << "', because it already exists.");
      return false;
    }
    if (!dataPoint.hasProblem()) {
      dataPoint.setProblem(problem());
    }
    m_dataPoints.push_back(dataPoint);
    connectChild(m_dataPoints.back(),true);
    onChange(AnalysisObject_Impl::Benign);
    return true;
  }

  bool Analysis_Impl::addDataPoint(const std::vector<Measure>& measures) {
    OptionalDataPoint dataPoint = problem().createDataPoint(measures);
    if (dataPoint) {
      return addDataPoint(*dataPoint);
    }
    LOG(Error,"Cannot add DataPoint to Analysis '" << name() << "', because the provided "
        "measures were invalid for Problem '" << problem().name() << "'.");
    return false;
  }

  bool Analysis_Impl::setDataPointRunInformation(DataPoint& dataPoint, const runmanager::Job& topLevelJob, const std::vector<openstudio::path>& dakotaParametersFiles)
  {
    OptionalDataPoint exactDataPoint = getDataPointByUUID(dataPoint);
    if (exactDataPoint) {
      exactDataPoint->getImpl<analysis::detail::DataPoint_Impl>()->setTopLevelJob(topLevelJob);
      exactDataPoint->getImpl<analysis::detail::DataPoint_Impl>()->setDakotaParametersFiles(dakotaParametersFiles);
      return true;
    }
    return false;
  }

  bool Analysis_Impl::removeDataPoint(const DataPoint &dataPoint) {
    OptionalDataPoint exactDataPoint = getDataPointByUUID(dataPoint);
    if (exactDataPoint) {
      DataPointVector::iterator it = std::find(m_dataPoints.begin(),m_dataPoints.end(),*exactDataPoint);
      OS_ASSERT(it != m_dataPoints.end());
      disconnectChild(*it);
      m_dataPoints.erase(it);
      // TODO: It may be that the algorithm should be reset, or at least marked not-complete.
      if (m_dataPoints.empty()) {
        m_resultsAreInvalid = false;
        m_dataPointsAreInvalid = false;
      }
      onChange(AnalysisObject_Impl::Benign);
      return true;
    }
    return false;
  }

  void Analysis_Impl::removeAllDataPoints() {
    BOOST_FOREACH(DataPoint& dataPoint, m_dataPoints) {
      disconnectChild(dataPoint);
    }
    m_dataPoints.clear();
    if (m_algorithm) {
      m_algorithm->reset();
    }
    m_resultsAreInvalid = false;
    m_dataPointsAreInvalid = false;
    onChange(AnalysisObject_Impl::Benign);
  }

  bool Analysis_Impl::clearResults(DataPoint& dataPoint) {
    OptionalDataPoint exactDataPoint = getDataPointByUUID(dataPoint);
    if (exactDataPoint) {
      exactDataPoint->clearResults();
      // TODO: It may be that the algorithm should be reset, or at least marked not-complete.
      if (m_resultsAreInvalid && completeDataPoints().empty()) {
        m_resultsAreInvalid = false;
      }
      onChange(AnalysisObject_Impl::Benign);
      return true;
    }
    return false;
  }

  void Analysis_Impl::clearAllResults() {
    BOOST_FOREACH(DataPoint& dataPoint, m_dataPoints) {
      dataPoint.clearResults();
    }
    if (m_algorithm) {
      // no results, so algorithm needs to be re-run
      m_algorithm->reset();
    }
    m_resultsAreInvalid = false;
    onChange(AnalysisObject_Impl::Benign);
  }

  bool Analysis_Impl::clearDirtyFlag() {
    if (m_resultsAreInvalid || m_dataPointsAreInvalid) {
      if (m_dataPointsAreInvalid) {
        LOG(Info,"Cannot clear this analysis's dirty flag because it contains invalid data "
            << "points. Call removeAllDataPoints first.");
      }
      else {
        LOG(Info,"Cannot clear this analysis's dirty flag because it contains invalid results. "
            << "Call clearAllResults first.");
      }
      return false;
    }
    AnalysisObject_Impl::clearDirtyFlag();
    return true;
  }

  void Analysis_Impl::updateDataPoint(DataPoint& dataPoint,
                                      const runmanager::Job& completedJob)
  {
    boost::optional<DataPoint> myDataPoint = getDataPointByUUID(dataPoint);
    if (!myDataPoint) {
      LOG_AND_THROW("Cannot update a DataPoint not already in Analysis '" << name() << "'.");
    }
    problem().updateDataPoint(*myDataPoint,completedJob);
    onChange(AnalysisObject_Impl::Benign);
  }

  void Analysis_Impl::initializeDakotaAlgorithm(DakotaAlgorithm& dakotaAlgorithm,
                                                const runmanager::Job& dakotaJob)
  {
    OptionalAlgorithm algorithm = this->algorithm();
    if (!algorithm || !(algorithm.get() == dakotaAlgorithm)) {
      LOG_AND_THROW("Improper use of Analysis::initializeDakotaAlgorithm.");
    }
    dakotaAlgorithm.resetComplete();
    dakotaAlgorithm.resetFailed();
    openstudio::path jobDir = dakotaJob.outdir();
    FileReference restartFile(jobDir / toPath("dakota.rst"));
    FileReference outFile(jobDir / toPath("dakota.out"));
    dakotaAlgorithm.setRestartFileReference(restartFile);
    dakotaAlgorithm.setOutFileReference(outFile);
    dakotaAlgorithm.getImpl<detail::DakotaAlgorithm_Impl>()->setJob(dakotaJob);
  }

  void Analysis_Impl::updateDakotaAlgorithm(const runmanager::Job& completedDakotaJob) {
    if (!algorithm() || !algorithm()->optionalCast<DakotaAlgorithm>()) {
      LOG_AND_THROW("Improper use of Analysis::updateDakotaAlgorithm.");
    }

    DakotaAlgorithm dakAlg = algorithm()->cast<DakotaAlgorithm>();

    if (!dakAlg.job() || dakAlg.job()->uuid() != completedDakotaJob.uuid()) {
      LOG_AND_THROW("Improper use of Analysis::updateDakotaAlgorithm.");
    }

    if (completedDakotaJob.jobType() != runmanager::JobType::Dakota) {
      LOG_AND_THROW("Improper use of Analysis::updateDakotaAlgorithm.");
    }

    runmanager::TreeStatusEnum status = completedDakotaJob.treeStatus();

    if (!((status == runmanager::TreeStatusEnum::Finished) ||
          (status == runmanager::TreeStatusEnum::Failed)))
    {
      LOG_AND_THROW("Improper call to Analysis::updateDakotaAlgorithm. Dakota job not actually "
                    << "complete.");
    }

    LOG(Info,"DakotaJob for Analysis '" << name() << "' is complete.");

    // output errors and warnings
    runmanager::JobErrors jobErrors = completedDakotaJob.errors();

    if (!jobErrors.allErrors.empty())
    {
      LOG(Warn, "The DakotaJob associated with Analysis '" << name() << "' generated "
          << "the following errors / warnings: ");

      typedef std::pair<runmanager::ErrorType, std::string> Err;

      BOOST_FOREACH(const Err& error, jobErrors.allErrors) {
        if (error.first == runmanager::ErrorType::Error)
        {
          LOG(Error, error.first.valueName() << ": " << error.second);
        } else if (error.first == runmanager::ErrorType::Warning) {
          LOG(Warn, error.first.valueName() << ": " << error.second);
        } else {
          LOG(Info, error.first.valueName() << ": " << error.second);
        }
      }
    }

    dakAlg.markComplete();
    if ((status == runmanager::TreeStatusEnum::Failed) || (!jobErrors.errors().empty())) {
      dakAlg.markFailed();
    }
    onChange(AnalysisObject_Impl::Benign);
  }

  Table Analysis_Impl::summaryTable() const {
    Table table;

    TableRow row;
    BOOST_FOREACH(const InputVariable& variable, problem().variables()) {
      row.push_back(variable.displayName());
    }
    table.appendRow(row);
    row.clear();

    BOOST_FOREACH(const DataPoint& dataPoint,dataPoints()) {
      BOOST_FOREACH(const QVariant& value,dataPoint.variableValues()) {
        row.push_back(TableElement(value));
      }
      table.appendRow(row);
      row.clear();
    }

    return table;
  }

  void Analysis_Impl::updateInputPathData(const openstudio::path& originalBase,
                                          const openstudio::path& newBase)
  {
    LOG(Debug,"Updating paths that were relative to '" << toString(originalBase) << 
        "' to be relative to '" << toString(newBase) << "' now.");

    // seed
    openstudio::path temp = relocatePath(seed().path(),originalBase,newBase);
    LOG(Debug,"Seed was at '" << toString(seed().path()) << "', relocatePath determined that it "
      << "should now be at '" << toString(temp) << "'.");
    if (!temp.empty()) {
      m_seed.setPath(temp);
    }

    // weather file
    if (weatherFile()) {
      temp = relocatePath(weatherFile()->path(),originalBase,newBase);
      if (!temp.empty()) {
        m_weatherFile->setPath(temp);
      }
    }

    // problem
    m_problem.getImpl<detail::Problem_Impl>()->updateInputPathData(originalBase,newBase);

    // algorithm
    //
    // Doing nothing because paths are outputs from running an analysis.
    //
    if (algorithm()) {
      m_algorithm->getImpl<detail::Algorithm_Impl>()->updateInputPathData(originalBase,newBase);
    }

    // data points
    //
    // Doing nothing because paths are outputs from running an analysis.
    //
    DataPointVector dataPoints = this->dataPoints();
    BOOST_FOREACH(DataPoint& dataPoint,dataPoints) {
      dataPoint.getImpl<detail::DataPoint_Impl>()->updateInputPathData(originalBase,newBase);
    }
  }

  bool Analysis_Impl::saveJSON(const openstudio::path& p,
                               const AnalysisSerializationOptions& options,                               bool overwrite) const
  {
    QVariant json = toVariant(options);
    return openstudio::saveJSON(json,p,overwrite);
  }

  std::ostream& Analysis_Impl::toJSON(std::ostream& os,
                                      const AnalysisSerializationOptions& options) const
  {
    os << toJSON(options);
    return os;
  }

  std::string Analysis_Impl::toJSON(const AnalysisSerializationOptions& options) const {
    QVariant json = this->toVariant(options);
    return openstudio::toJSON(json);
  }

  QVariant Analysis_Impl::toVariant() const {
    QVariantMap analysisData = AnalysisObject_Impl::toVariant().toMap();

    analysisData["problem"] = problem().toVariant();
    if (algorithm()) {
      analysisData["algorithm"] = algorithm()->toVariant();
    }
    analysisData["seed"] = openstudio::detail::toVariant(seed());
    if (weatherFile()) {
      analysisData["weather_file"] = openstudio::detail::toVariant(weatherFile().get());
    }
    analysisData["results_are_invalid"] = QVariant(resultsAreInvalid());
    analysisData["data_points_are_invalid"] = QVariant(dataPointsAreInvalid());

    return QVariant(analysisData);
  }

  QVariant Analysis_Impl::toVariant(const AnalysisSerializationOptions& options) const {
    QVariantMap analysisData = toVariant().toMap();

    if (options.scope == AnalysisSerializationScope::Full) {
      // add data point information
      QVariantList dataPointList;
      Q_FOREACH(const DataPoint& dataPoint, dataPoints()) {
        dataPointList.push_back(dataPoint.toVariant());
      }

      analysisData["data_points"] = QVariant(dataPointList);
    }

    QVariantMap metadata = jsonMetadata().toMap();

    if (!options.projectDir.empty()) {
      metadata["project_dir"] = toQString(options.projectDir);
    }

    if (options.osServerView) {

      // this data is not read upon deserialization
      QVariantMap serverView = problem().toServerFormulationVariant().toMap();

      if (options.scope == AnalysisSerializationScope::Full) {
        QVariantList dataPointList;
        Q_FOREACH(const DataPoint& dataPoint, dataPoints()) {
          if (dataPoint.hasProblem()) {
            dataPointList.push_back(dataPoint.toServerDataPointsVariant());
          }
        }
        serverView["data_points"] = QVariant(dataPointList);
      }

      metadata.unite(serverView);
    }

    // create top-level of final file
    QVariantMap result;
    result["metadata"] = metadata;
    result["analysis"] = QVariant(analysisData);

    return result;
  }

  Analysis Analysis_Impl::fromVariant(const QVariant& variant,const VersionString& version) {
    QVariantMap map = variant.toMap();
    Problem problem = Problem_Impl::factoryFromVariant(map["problem"],version);
    OptionalAlgorithm algorithm;
    if (map.contains("algorithm")) {
      algorithm =  Algorithm_Impl::factoryFromVariant(map["algorithm"],version);
    }
    DataPointVector dataPoints;
    if (map.contains("data_points")) {
      dataPoints = deserializeUnorderedVector<DataPoint>(
            map["data_points"].toList(),
            boost::function<DataPoint (const QVariant&)>(boost::bind(openstudio::analysis::detail::DataPoint_Impl::factoryFromVariant,_1,version,problem)));
    }
    return Analysis(toUUID(map["uuid"].toString().toStdString()),
                    toUUID(map["version_uuid"].toString().toStdString()),
                    map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                    map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                    map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                    problem,
                    algorithm,
                    openstudio::detail::toFileReference(map["seed"],version),
                    map.contains("weather_file") ? openstudio::detail::toFileReference(map["weather_file"],version) : OptionalFileReference(),
                    dataPoints,
                    map["results_are_invalid"].toBool(),
                    map["data_points_are_invalid"].toBool());
  }

  void Analysis_Impl::onChange(ChangeType changeType) {
    AnalysisObject_Impl::onChange(changeType);
    if ((changeType == AnalysisObject_Impl::InvalidatesResults) &&
        (!completeDataPoints().empty()))
    {
      m_resultsAreInvalid = true;
    }
    if ((changeType == AnalysisObject_Impl::InvalidatesDataPoints) &&
        (!m_dataPoints.empty()))
    {
      m_resultsAreInvalid = true;
      m_dataPointsAreInvalid = true;
    }
  }

} // detail

AnalysisSerializationOptions::AnalysisSerializationOptions(
    const openstudio::path& t_projectDir,
    const AnalysisSerializationScope& t_scope,
    bool t_osServerView)
  : projectDir(t_projectDir),
    scope(t_scope),
    osServerView(t_osServerView)
{}

Analysis::Analysis(const std::string& name,
                   const Problem& problem,
                   const FileReferenceType& seedType)
  : AnalysisObject(boost::shared_ptr<detail::Analysis_Impl>(
                     new detail::Analysis_Impl(name,problem,seedType)))
{
  Analysis copyOfThis(getImpl<detail::Analysis_Impl>());
  problem.setParent(copyOfThis);
}

Analysis::Analysis(const std::string& name,
                   const Problem& problem,
                   const FileReference& seed)
  : AnalysisObject(boost::shared_ptr<detail::Analysis_Impl>(
        new detail::Analysis_Impl(name,problem,seed)))
{
  Analysis copyOfThis(getImpl<detail::Analysis_Impl>());
  problem.setParent(copyOfThis);
}

Analysis::Analysis(const std::string& name,
                   const Problem& problem,
                   const FileReference& seed,
                   const FileReference& weatherFile)
  : AnalysisObject(boost::shared_ptr<detail::Analysis_Impl>(
        new detail::Analysis_Impl(name,problem,seed,weatherFile)))
{
  Analysis copyOfThis(getImpl<detail::Analysis_Impl>());
  problem.setParent(copyOfThis);
}

Analysis::Analysis(const std::string& name,
                   const Problem& problem,
                   const Algorithm& algorithm,
                   const FileReference& seed)
  : AnalysisObject(boost::shared_ptr<detail::Analysis_Impl>(
        new detail::Analysis_Impl(name,problem,algorithm,seed)))
{
  Analysis copyOfThis(getImpl<detail::Analysis_Impl>());
  problem.setParent(copyOfThis);
  algorithm.setParent(copyOfThis);
}

Analysis::Analysis(const std::string& name,
                   const Problem& problem,
                   const Algorithm& algorithm,
                   const FileReference& seed,
                   const FileReference& weatherFile)
  : AnalysisObject(boost::shared_ptr<detail::Analysis_Impl>(
        new detail::Analysis_Impl(name,problem,algorithm,seed,weatherFile)))
{
  Analysis copyOfThis(getImpl<detail::Analysis_Impl>());
  problem.setParent(copyOfThis);
  algorithm.setParent(copyOfThis);
}

Analysis::Analysis(const UUID& uuid,
                   const UUID& versionUUID,
                   const std::string& name,
                   const std::string& displayName,
                   const std::string& description,
                   const Problem& problem,
                   const boost::optional<Algorithm>& algorithm,
                   const FileReference& seed,
                   const boost::optional<FileReference>& weatherFile,
                   const std::vector<DataPoint>& dataPoints,
                   bool resultsAreInvalid,
                   bool dataPointsAreInvalid)
  : AnalysisObject(boost::shared_ptr<detail::Analysis_Impl>(
        new detail::Analysis_Impl(uuid,versionUUID,name,displayName,description,
                                  problem,algorithm,seed,weatherFile,dataPoints,
                                  resultsAreInvalid,dataPointsAreInvalid)))
{
  Analysis copyOfThis(getImpl<detail::Analysis_Impl>());
  problem.setParent(copyOfThis);
  if (algorithm) {
    algorithm->setParent(copyOfThis);
  }
  BOOST_FOREACH(const DataPoint& dataPoint,dataPoints) {
    dataPoint.setParent(copyOfThis);
  }
}

Problem Analysis::problem() const {
  return getImpl<detail::Analysis_Impl>()->problem();
}

boost::optional<Algorithm> Analysis::algorithm() const {
  return getImpl<detail::Analysis_Impl>()->algorithm();
}

FileReference Analysis::seed() const {
  return getImpl<detail::Analysis_Impl>()->seed();
}

boost::optional<FileReference> Analysis::weatherFile() const {
  return getImpl<detail::Analysis_Impl>()->weatherFile();
}

std::vector<DataPoint> Analysis::dataPoints() const {
  return getImpl<detail::Analysis_Impl>()->dataPoints();
}

std::vector<DataPoint> Analysis::dataPointsToQueue() const {
  return getImpl<detail::Analysis_Impl>()->dataPointsToQueue();
}

std::vector<DataPoint> Analysis::completeDataPoints() const {
  return getImpl<detail::Analysis_Impl>()->completeDataPoints();
}

std::vector<DataPoint> Analysis::successfulDataPoints() const {
  return getImpl<detail::Analysis_Impl>()->successfulDataPoints();
}

std::vector<DataPoint> Analysis::failedDataPoints() const {
  return getImpl<detail::Analysis_Impl>()->failedDataPoints();
}

std::vector<DataPoint> Analysis::dataPointsNeedingDetails() const {
  return getImpl<detail::Analysis_Impl>()->dataPointsNeedingDetails();
}

std::vector<DataPoint> Analysis::getDataPoints(const std::vector<QVariant>& variableValues) const
{
  return getImpl<detail::Analysis_Impl>()->getDataPoints(variableValues);
}

std::vector<DataPoint> Analysis::getDataPoints(
    const std::vector< boost::optional<Measure> >& measures) const
{
  return getImpl<detail::Analysis_Impl>()->getDataPoints(measures);
}

std::vector<DataPoint> Analysis::getDataPoints(const std::string& tag) const {
  return getImpl<detail::Analysis_Impl>()->getDataPoints(tag);
}

boost::optional<DataPoint> Analysis::getDataPoint(
    const std::vector<Measure>& measures) const
{
  return getImpl<detail::Analysis_Impl>()->getDataPoint(measures);
}

boost::optional<DataPoint> Analysis::getDataPointByUUID(const UUID& uuid) const {
  return getImpl<detail::Analysis_Impl>()->getDataPointByUUID(uuid);
}

boost::optional<DataPoint> Analysis::getDataPointByUUID(const DataPoint& dataPoint) const {
  return getImpl<detail::Analysis_Impl>()->getDataPointByUUID(dataPoint);
}

bool Analysis::resultsAreInvalid() const {
  return getImpl<detail::Analysis_Impl>()->resultsAreInvalid();
}

bool Analysis::dataPointsAreInvalid() const {
  return getImpl<detail::Analysis_Impl>()->dataPointsAreInvalid();
}

bool Analysis::setProblem(Problem& problem) {
  return getImpl<detail::Analysis_Impl>()->setProblem(problem);
}

bool Analysis::setAlgorithm(Algorithm& algorithm) {
  return getImpl<detail::Analysis_Impl>()->setAlgorithm(algorithm);
}

void Analysis::clearAlgorithm() {
  getImpl<detail::Analysis_Impl>()->clearAlgorithm();
}

bool Analysis::setSeed(const FileReference& newSeed) {
  return getImpl<detail::Analysis_Impl>()->setSeed(newSeed);
}

bool Analysis::clearSeed(const FileReferenceType& seedType) {
  return getImpl<detail::Analysis_Impl>()->clearSeed(seedType);
}

bool Analysis::setWeatherFile(const FileReference& newWeatherFile) {
  return getImpl<detail::Analysis_Impl>()->setWeatherFile(newWeatherFile);
}

void Analysis::clearWeatherFile() {
  getImpl<detail::Analysis_Impl>()->clearWeatherFile();
}

bool Analysis::addDataPoint(DataPoint& dataPoint) {
  return getImpl<detail::Analysis_Impl>()->addDataPoint(dataPoint);
}

bool Analysis::addDataPoint(const std::vector<Measure>& measures) {
  return getImpl<detail::Analysis_Impl>()->addDataPoint(measures);
}

bool Analysis::setDataPointRunInformation(DataPoint& dataPoint, const runmanager::Job& topLevelJob, const std::vector<openstudio::path>& dakotaParametersFiles)
{
  return getImpl<detail::Analysis_Impl>()->setDataPointRunInformation(dataPoint, topLevelJob, dakotaParametersFiles);
}

bool Analysis::removeDataPoint(const DataPoint& dataPoint) {
  return getImpl<detail::Analysis_Impl>()->removeDataPoint(dataPoint);
}

void Analysis::removeAllDataPoints() {
  getImpl<detail::Analysis_Impl>()->removeAllDataPoints();
}

bool Analysis::clearResults(DataPoint& dataPoint) {
  return getImpl<detail::Analysis_Impl>()->clearResults(dataPoint);
}

void Analysis::clearAllResults() {
  getImpl<detail::Analysis_Impl>()->clearAllResults();
}

void Analysis::updateDataPoint(DataPoint& dataPoint,
                     const runmanager::Job& completedJob)
{
  getImpl<detail::Analysis_Impl>()->updateDataPoint(dataPoint,completedJob);
}

void Analysis::initializeDakotaAlgorithm(DakotaAlgorithm& dakotaAlgorithm,
                                         const runmanager::Job& dakotaJob)
{
  getImpl<detail::Analysis_Impl>()->initializeDakotaAlgorithm(dakotaAlgorithm,dakotaJob);
}

void Analysis::updateDakotaAlgorithm(const runmanager::Job& completedDakotaJob) {
  getImpl<detail::Analysis_Impl>()->updateDakotaAlgorithm(completedDakotaJob);
}

Table Analysis::summaryTable() const {
  return getImpl<detail::Analysis_Impl>()->summaryTable();
}

void Analysis::updateInputPathData(const openstudio::path& originalBase,
                                   const openstudio::path& newBase)
{
  return getImpl<detail::Analysis_Impl>()->updateInputPathData(originalBase,newBase);
}

bool Analysis::saveJSON(const openstudio::path& p,
                        const AnalysisSerializationOptions& options,
                        bool overwrite) const
{
  return getImpl<detail::Analysis_Impl>()->saveJSON(p,options,overwrite);
}

std::ostream& Analysis::toJSON(std::ostream& os,
                               const AnalysisSerializationOptions& options) const
{
  return getImpl<detail::Analysis_Impl>()->toJSON(os,options);
}

std::string Analysis::toJSON(const AnalysisSerializationOptions& options) const {
  return getImpl<detail::Analysis_Impl>()->toJSON(options);
}

/// @cond
Analysis::Analysis(boost::shared_ptr<detail::Analysis_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

boost::optional<OpenStudioAlgorithm> getOpenStudioAlgorithm(const Analysis& analysis) {
  OptionalOpenStudioAlgorithm result;
  if (analysis.algorithm()) {
    result = analysis.algorithm()->optionalCast<OpenStudioAlgorithm>();
  }
  return result;
}

boost::optional<DakotaAlgorithm> getDakotaAlgorithm(const Analysis& analysis) {
  OptionalDakotaAlgorithm result;
  if (analysis.algorithm()) {
    result = analysis.algorithm()->optionalCast<DakotaAlgorithm>();
  }
  return result;
}

} // analysis
} // openstudio

