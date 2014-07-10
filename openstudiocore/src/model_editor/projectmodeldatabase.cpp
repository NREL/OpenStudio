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

#include "projectmodeldatabase.hpp"
namespace ProjectModel {
using namespace litesql;
AnalysisProjectRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : project(AnalysisProjectRelation::Project), analysis(AnalysisProjectRelation::Analysis) {
    switch(rec.size()) {
    case 2:
        project = rec[1];
    case 1:
        analysis = rec[0];
    }
}
const std::string AnalysisProjectRelation::table__("Analysis_Project_");
const litesql::FieldType AnalysisProjectRelation::Analysis("Analysis1","INTEGER",table__);
const litesql::FieldType AnalysisProjectRelation::Project("Project2","INTEGER",table__);
void AnalysisProjectRelation::link(const litesql::Database& db, const ProjectModel::Analysis& o0, const ProjectModel::Project& o1) {
    Record values;
    Split fields;
    fields.push_back(Analysis.name());
    values.push_back(o0.id);
    fields.push_back(Project.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void AnalysisProjectRelation::unlink(const litesql::Database& db, const ProjectModel::Analysis& o0, const ProjectModel::Project& o1) {
    db.delete_(table__, (Analysis == o0.id && Project == o1.id));
}
void AnalysisProjectRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<AnalysisProjectRelation::Row> AnalysisProjectRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Analysis.fullName());
    sel.result(Project.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<AnalysisProjectRelation::Row>(db, sel);
}
template <> litesql::DataSource<ProjectModel::Analysis> AnalysisProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Analysis.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::Analysis>(db, ProjectModel::Analysis::Id.in(sel) && expr);
}
template <> litesql::DataSource<ProjectModel::Project> AnalysisProjectRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Project.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::Project>(db, ProjectModel::Project::Id.in(sel) && expr);
}
AnalysisModelRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : model(AnalysisModelRelation::Model), analysis(AnalysisModelRelation::Analysis) {
    switch(rec.size()) {
    case 2:
        model = rec[1];
    case 1:
        analysis = rec[0];
    }
}
const std::string AnalysisModelRelation::table__("Analysis_Model_");
const litesql::FieldType AnalysisModelRelation::Analysis("Analysis1","INTEGER",table__);
const litesql::FieldType AnalysisModelRelation::Model("Model2","INTEGER",table__);
void AnalysisModelRelation::link(const litesql::Database& db, const ProjectModel::Analysis& o0, const ProjectModel::Model& o1) {
    Record values;
    Split fields;
    fields.push_back(Analysis.name());
    values.push_back(o0.id);
    fields.push_back(Model.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void AnalysisModelRelation::unlink(const litesql::Database& db, const ProjectModel::Analysis& o0, const ProjectModel::Model& o1) {
    db.delete_(table__, (Analysis == o0.id && Model == o1.id));
}
void AnalysisModelRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<AnalysisModelRelation::Row> AnalysisModelRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Analysis.fullName());
    sel.result(Model.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<AnalysisModelRelation::Row>(db, sel);
}
template <> litesql::DataSource<ProjectModel::Analysis> AnalysisModelRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Analysis.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::Analysis>(db, ProjectModel::Analysis::Id.in(sel) && expr);
}
template <> litesql::DataSource<ProjectModel::Model> AnalysisModelRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Model.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::Model>(db, ProjectModel::Model::Id.in(sel) && expr);
}
ModelResultRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : result(ModelResultRelation::Result), model(ModelResultRelation::Model) {
    switch(rec.size()) {
    case 2:
        result = rec[1];
    case 1:
        model = rec[0];
    }
}
const std::string ModelResultRelation::table__("Model_Result_");
const litesql::FieldType ModelResultRelation::Model("Model1","INTEGER",table__);
const litesql::FieldType ModelResultRelation::Result("Result2","INTEGER",table__);
void ModelResultRelation::link(const litesql::Database& db, const ProjectModel::Model& o0, const ProjectModel::Result& o1) {
    Record values;
    Split fields;
    fields.push_back(Model.name());
    values.push_back(o0.id);
    fields.push_back(Result.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void ModelResultRelation::unlink(const litesql::Database& db, const ProjectModel::Model& o0, const ProjectModel::Result& o1) {
    db.delete_(table__, (Model == o0.id && Result == o1.id));
}
void ModelResultRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<ModelResultRelation::Row> ModelResultRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Model.fullName());
    sel.result(Result.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<ModelResultRelation::Row>(db, sel);
}
template <> litesql::DataSource<ProjectModel::Model> ModelResultRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Model.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::Model>(db, ProjectModel::Model::Id.in(sel) && expr);
}
template <> litesql::DataSource<ProjectModel::Result> ModelResultRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Result.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::Result>(db, ProjectModel::Result::Id.in(sel) && expr);
}
ResultResultQueryRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : resultQuery(ResultResultQueryRelation::ResultQuery), result(ResultResultQueryRelation::Result) {
    switch(rec.size()) {
    case 2:
        resultQuery = rec[1];
    case 1:
        result = rec[0];
    }
}
const std::string ResultResultQueryRelation::table__("Result_ResultQuery_");
const litesql::FieldType ResultResultQueryRelation::Result("Result1","INTEGER",table__);
const litesql::FieldType ResultResultQueryRelation::ResultQuery("ResultQuery2","INTEGER",table__);
void ResultResultQueryRelation::link(const litesql::Database& db, const ProjectModel::Result& o0, const ProjectModel::ResultQuery& o1) {
    Record values;
    Split fields;
    fields.push_back(Result.name());
    values.push_back(o0.id);
    fields.push_back(ResultQuery.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void ResultResultQueryRelation::unlink(const litesql::Database& db, const ProjectModel::Result& o0, const ProjectModel::ResultQuery& o1) {
    db.delete_(table__, (Result == o0.id && ResultQuery == o1.id));
}
void ResultResultQueryRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<ResultResultQueryRelation::Row> ResultResultQueryRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Result.fullName());
    sel.result(ResultQuery.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<ResultResultQueryRelation::Row>(db, sel);
}
template <> litesql::DataSource<ProjectModel::Result> ResultResultQueryRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Result.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::Result>(db, ProjectModel::Result::Id.in(sel) && expr);
}
template <> litesql::DataSource<ProjectModel::ResultQuery> ResultResultQueryRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(ResultQuery.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::ResultQuery>(db, ProjectModel::ResultQuery::Id.in(sel) && expr);
}
RequestedResultResultQueryRelation::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : resultQuery(RequestedResultResultQueryRelation::ResultQuery), requestedResult(RequestedResultResultQueryRelation::RequestedResult) {
    switch(rec.size()) {
    case 2:
        resultQuery = rec[1];
    case 1:
        requestedResult = rec[0];
    }
}
const std::string RequestedResultResultQueryRelation::table__("RequestedResult_ResultQuery_");
const litesql::FieldType RequestedResultResultQueryRelation::RequestedResult("RequestedResult1","INTEGER",table__);
const litesql::FieldType RequestedResultResultQueryRelation::ResultQuery("ResultQuery2","INTEGER",table__);
void RequestedResultResultQueryRelation::link(const litesql::Database& db, const ProjectModel::RequestedResult& o0, const ProjectModel::ResultQuery& o1) {
    Record values;
    Split fields;
    fields.push_back(RequestedResult.name());
    values.push_back(o0.id);
    fields.push_back(ResultQuery.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void RequestedResultResultQueryRelation::unlink(const litesql::Database& db, const ProjectModel::RequestedResult& o0, const ProjectModel::ResultQuery& o1) {
    db.delete_(table__, (RequestedResult == o0.id && ResultQuery == o1.id));
}
void RequestedResultResultQueryRelation::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<RequestedResultResultQueryRelation::Row> RequestedResultResultQueryRelation::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(RequestedResult.fullName());
    sel.result(ResultQuery.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<RequestedResultResultQueryRelation::Row>(db, sel);
}
template <> litesql::DataSource<ProjectModel::RequestedResult> RequestedResultResultQueryRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(RequestedResult.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::RequestedResult>(db, ProjectModel::RequestedResult::Id.in(sel) && expr);
}
template <> litesql::DataSource<ProjectModel::ResultQuery> RequestedResultResultQueryRelation::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(ResultQuery.fullName());
    sel.where(srcExpr);
    return DataSource<ProjectModel::ResultQuery>(db, ProjectModel::ResultQuery::Id.in(sel) && expr);
}
const litesql::FieldType Project::Own::Id("id_","INTEGER","Project_");
Project::AnalysesHandle::AnalysesHandle(const Project& owner)
         : litesql::RelationHandle<Project>(owner) {
}
void Project::AnalysesHandle::link(const Analysis& o0) {
    AnalysisProjectRelation::link(owner->getDatabase(), o0, *owner);
}
void Project::AnalysesHandle::unlink(const Analysis& o0) {
    AnalysisProjectRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Project::AnalysesHandle::del(const litesql::Expr& expr) {
    AnalysisProjectRelation::del(owner->getDatabase(), expr && AnalysisProjectRelation::Project == owner->id);
}
litesql::DataSource<Analysis> Project::AnalysesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return AnalysisProjectRelation::get<Analysis>(owner->getDatabase(), expr, (AnalysisProjectRelation::Project == owner->id) && srcExpr);
}
litesql::DataSource<AnalysisProjectRelation::Row> Project::AnalysesHandle::getRows(const litesql::Expr& expr) {
    return AnalysisProjectRelation::getRows(owner->getDatabase(), expr && (AnalysisProjectRelation::Project == owner->id));
}
const std::string Project::type__("Project");
const std::string Project::table__("Project_");
const std::string Project::sequence__("Project_seq");
const litesql::FieldType Project::Id("id_","INTEGER",table__);
const litesql::FieldType Project::Type("type_","TEXT",table__);
const litesql::FieldType Project::Name("name_","TEXT",table__);
const litesql::FieldType Project::Projecttype("projecttype_","TEXT",table__);
void Project::defaults() {
    id = 0;
}
Project::Project(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), projecttype(Projecttype) {
    defaults();
}
Project::Project(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), projecttype(Projecttype) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: projecttype = convert<const std::string&, std::string>(rec[3]);
        projecttype.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Project::Project(const Project& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), projecttype(obj.projecttype) {
}
const Project& Project::operator=(const Project& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        projecttype = obj.projecttype;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Project::AnalysesHandle Project::analyses() {
    return Project::AnalysesHandle(*this);
}
std::string Project::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(projecttype.name());
    values.push_back(projecttype);
    projecttype.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Project::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Project::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, projecttype);
}
void Project::addIDUpdates(Updates& updates) {
}
void Project::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Projecttype);
}
void Project::delRecord() {
    deleteFromTable(table__, id);
}
void Project::delRelations() {
    AnalysisProjectRelation::del(*db, (AnalysisProjectRelation::Project == id));
}
void Project::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Project::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Project> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Project::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Project> Project::upcast() {
    return auto_ptr<Project>(new Project(*this));
}
std::auto_ptr<Project> Project::upcastCopy() {
    Project* np = new Project(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->projecttype = projecttype;
    np->inDatabase = inDatabase;
    return auto_ptr<Project>(np);
}
std::ostream & operator<<(std::ostream& os, Project o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.projecttype.name() << " = " << o.projecttype << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Analysis::Own::Id("id_","INTEGER","Analysis_");
Analysis::ProjectHandle::ProjectHandle(const Analysis& owner)
         : litesql::RelationHandle<Analysis>(owner) {
}
void Analysis::ProjectHandle::link(const Project& o0) {
    AnalysisProjectRelation::link(owner->getDatabase(), *owner, o0);
}
void Analysis::ProjectHandle::unlink(const Project& o0) {
    AnalysisProjectRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Analysis::ProjectHandle::del(const litesql::Expr& expr) {
    AnalysisProjectRelation::del(owner->getDatabase(), expr && AnalysisProjectRelation::Analysis == owner->id);
}
litesql::DataSource<Project> Analysis::ProjectHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return AnalysisProjectRelation::get<Project>(owner->getDatabase(), expr, (AnalysisProjectRelation::Analysis == owner->id) && srcExpr);
}
litesql::DataSource<AnalysisProjectRelation::Row> Analysis::ProjectHandle::getRows(const litesql::Expr& expr) {
    return AnalysisProjectRelation::getRows(owner->getDatabase(), expr && (AnalysisProjectRelation::Analysis == owner->id));
}
Analysis::ModelsHandle::ModelsHandle(const Analysis& owner)
         : litesql::RelationHandle<Analysis>(owner) {
}
void Analysis::ModelsHandle::link(const Model& o0) {
    AnalysisModelRelation::link(owner->getDatabase(), *owner, o0);
}
void Analysis::ModelsHandle::unlink(const Model& o0) {
    AnalysisModelRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Analysis::ModelsHandle::del(const litesql::Expr& expr) {
    AnalysisModelRelation::del(owner->getDatabase(), expr && AnalysisModelRelation::Analysis == owner->id);
}
litesql::DataSource<Model> Analysis::ModelsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return AnalysisModelRelation::get<Model>(owner->getDatabase(), expr, (AnalysisModelRelation::Analysis == owner->id) && srcExpr);
}
litesql::DataSource<AnalysisModelRelation::Row> Analysis::ModelsHandle::getRows(const litesql::Expr& expr) {
    return AnalysisModelRelation::getRows(owner->getDatabase(), expr && (AnalysisModelRelation::Analysis == owner->id));
}
const std::string Analysis::type__("Analysis");
const std::string Analysis::table__("Analysis_");
const std::string Analysis::sequence__("Analysis_seq");
const litesql::FieldType Analysis::Id("id_","INTEGER",table__);
const litesql::FieldType Analysis::Type("type_","TEXT",table__);
const litesql::FieldType Analysis::Name("name_","TEXT",table__);
const litesql::FieldType Analysis::Analysistype("analysistype_","TEXT",table__);
void Analysis::defaults() {
    id = 0;
}
Analysis::Analysis(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), analysistype(Analysistype) {
    defaults();
}
Analysis::Analysis(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), analysistype(Analysistype) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: analysistype = convert<const std::string&, std::string>(rec[3]);
        analysistype.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Analysis::Analysis(const Analysis& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), analysistype(obj.analysistype) {
}
const Analysis& Analysis::operator=(const Analysis& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        analysistype = obj.analysistype;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Analysis::ProjectHandle Analysis::project() {
    return Analysis::ProjectHandle(*this);
}
Analysis::ModelsHandle Analysis::models() {
    return Analysis::ModelsHandle(*this);
}
std::string Analysis::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(analysistype.name());
    values.push_back(analysistype);
    analysistype.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Analysis::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Analysis::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, analysistype);
}
void Analysis::addIDUpdates(Updates& updates) {
}
void Analysis::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Analysistype);
}
void Analysis::delRecord() {
    deleteFromTable(table__, id);
}
void Analysis::delRelations() {
    AnalysisProjectRelation::del(*db, (AnalysisProjectRelation::Analysis == id));
    AnalysisModelRelation::del(*db, (AnalysisModelRelation::Analysis == id));
}
void Analysis::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Analysis::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Analysis> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Analysis::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Analysis> Analysis::upcast() {
    return auto_ptr<Analysis>(new Analysis(*this));
}
std::auto_ptr<Analysis> Analysis::upcastCopy() {
    Analysis* np = new Analysis(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->analysistype = analysistype;
    np->inDatabase = inDatabase;
    return auto_ptr<Analysis>(np);
}
std::ostream & operator<<(std::ostream& os, Analysis o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.analysistype.name() << " = " << o.analysistype << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Model::Own::Id("id_","INTEGER","Model_");
Model::AnalysesHandle::AnalysesHandle(const Model& owner)
         : litesql::RelationHandle<Model>(owner) {
}
void Model::AnalysesHandle::link(const Analysis& o0) {
    AnalysisModelRelation::link(owner->getDatabase(), o0, *owner);
}
void Model::AnalysesHandle::unlink(const Analysis& o0) {
    AnalysisModelRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Model::AnalysesHandle::del(const litesql::Expr& expr) {
    AnalysisModelRelation::del(owner->getDatabase(), expr && AnalysisModelRelation::Model == owner->id);
}
litesql::DataSource<Analysis> Model::AnalysesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return AnalysisModelRelation::get<Analysis>(owner->getDatabase(), expr, (AnalysisModelRelation::Model == owner->id) && srcExpr);
}
litesql::DataSource<AnalysisModelRelation::Row> Model::AnalysesHandle::getRows(const litesql::Expr& expr) {
    return AnalysisModelRelation::getRows(owner->getDatabase(), expr && (AnalysisModelRelation::Model == owner->id));
}
Model::ResultsHandle::ResultsHandle(const Model& owner)
         : litesql::RelationHandle<Model>(owner) {
}
void Model::ResultsHandle::link(const Result& o0) {
    ModelResultRelation::link(owner->getDatabase(), *owner, o0);
}
void Model::ResultsHandle::unlink(const Result& o0) {
    ModelResultRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Model::ResultsHandle::del(const litesql::Expr& expr) {
    ModelResultRelation::del(owner->getDatabase(), expr && ModelResultRelation::Model == owner->id);
}
litesql::DataSource<Result> Model::ResultsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ModelResultRelation::get<Result>(owner->getDatabase(), expr, (ModelResultRelation::Model == owner->id) && srcExpr);
}
litesql::DataSource<ModelResultRelation::Row> Model::ResultsHandle::getRows(const litesql::Expr& expr) {
    return ModelResultRelation::getRows(owner->getDatabase(), expr && (ModelResultRelation::Model == owner->id));
}
const std::string Model::type__("Model");
const std::string Model::table__("Model_");
const std::string Model::sequence__("Model_seq");
const litesql::FieldType Model::Id("id_","INTEGER",table__);
const litesql::FieldType Model::Type("type_","TEXT",table__);
const litesql::FieldType Model::Name("name_","TEXT",table__);
const litesql::FieldType Model::Path("path_","TEXT",table__);
const litesql::FieldType Model::Guid("guid_","TEXT",table__);
void Model::defaults() {
    id = 0;
}
Model::Model(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), path(Path), guid(Guid) {
    defaults();
}
Model::Model(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), path(Path), guid(Guid) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: guid = convert<const std::string&, std::string>(rec[4]);
        guid.setModified(false);
    case 4: path = convert<const std::string&, std::string>(rec[3]);
        path.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Model::Model(const Model& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), path(obj.path), guid(obj.guid) {
}
const Model& Model::operator=(const Model& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        path = obj.path;
        guid = obj.guid;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Model::AnalysesHandle Model::analyses() {
    return Model::AnalysesHandle(*this);
}
Model::ResultsHandle Model::results() {
    return Model::ResultsHandle(*this);
}
std::string Model::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(path.name());
    values.push_back(path);
    path.setModified(false);
    fields.push_back(guid.name());
    values.push_back(guid);
    guid.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Model::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Model::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, path);
    updateField(updates, table__, guid);
}
void Model::addIDUpdates(Updates& updates) {
}
void Model::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Path);
    ftypes.push_back(Guid);
}
void Model::delRecord() {
    deleteFromTable(table__, id);
}
void Model::delRelations() {
    AnalysisModelRelation::del(*db, (AnalysisModelRelation::Model == id));
    ModelResultRelation::del(*db, (ModelResultRelation::Model == id));
}
void Model::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Model::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Model> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Model::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Model> Model::upcast() {
    return auto_ptr<Model>(new Model(*this));
}
std::auto_ptr<Model> Model::upcastCopy() {
    Model* np = new Model(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->path = path;
    np->guid = guid;
    np->inDatabase = inDatabase;
    return auto_ptr<Model>(np);
}
std::ostream & operator<<(std::ostream& os, Model o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.path.name() << " = " << o.path << std::endl;
    os << o.guid.name() << " = " << o.guid << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Result::Own::Id("id_","INTEGER","Result_");
Result::ModelHandle::ModelHandle(const Result& owner)
         : litesql::RelationHandle<Result>(owner) {
}
void Result::ModelHandle::link(const Model& o0) {
    ModelResultRelation::link(owner->getDatabase(), o0, *owner);
}
void Result::ModelHandle::unlink(const Model& o0) {
    ModelResultRelation::unlink(owner->getDatabase(), o0, *owner);
}
void Result::ModelHandle::del(const litesql::Expr& expr) {
    ModelResultRelation::del(owner->getDatabase(), expr && ModelResultRelation::Result == owner->id);
}
litesql::DataSource<Model> Result::ModelHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ModelResultRelation::get<Model>(owner->getDatabase(), expr, (ModelResultRelation::Result == owner->id) && srcExpr);
}
litesql::DataSource<ModelResultRelation::Row> Result::ModelHandle::getRows(const litesql::Expr& expr) {
    return ModelResultRelation::getRows(owner->getDatabase(), expr && (ModelResultRelation::Result == owner->id));
}
Result::ResultqueriesHandle::ResultqueriesHandle(const Result& owner)
         : litesql::RelationHandle<Result>(owner) {
}
void Result::ResultqueriesHandle::link(const ResultQuery& o0) {
    ResultResultQueryRelation::link(owner->getDatabase(), *owner, o0);
}
void Result::ResultqueriesHandle::unlink(const ResultQuery& o0) {
    ResultResultQueryRelation::unlink(owner->getDatabase(), *owner, o0);
}
void Result::ResultqueriesHandle::del(const litesql::Expr& expr) {
    ResultResultQueryRelation::del(owner->getDatabase(), expr && ResultResultQueryRelation::Result == owner->id);
}
litesql::DataSource<ResultQuery> Result::ResultqueriesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ResultResultQueryRelation::get<ResultQuery>(owner->getDatabase(), expr, (ResultResultQueryRelation::Result == owner->id) && srcExpr);
}
litesql::DataSource<ResultResultQueryRelation::Row> Result::ResultqueriesHandle::getRows(const litesql::Expr& expr) {
    return ResultResultQueryRelation::getRows(owner->getDatabase(), expr && (ResultResultQueryRelation::Result == owner->id));
}
const std::string Result::type__("Result");
const std::string Result::table__("Result_");
const std::string Result::sequence__("Result_seq");
const litesql::FieldType Result::Id("id_","INTEGER",table__);
const litesql::FieldType Result::Type("type_","TEXT",table__);
const litesql::FieldType Result::Value("value_","INTEGER",table__);
void Result::defaults() {
    id = 0;
    value = 0;
}
Result::Result(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), value(Value) {
    defaults();
}
Result::Result(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), value(Value) {
    defaults();
    size_t size = (rec.size() > 3) ? 3 : rec.size();
    switch(size) {
    case 3: value = convert<const std::string&, int>(rec[2]);
        value.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Result::Result(const Result& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), value(obj.value) {
}
const Result& Result::operator=(const Result& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        value = obj.value;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Result::ModelHandle Result::model() {
    return Result::ModelHandle(*this);
}
Result::ResultqueriesHandle Result::resultqueries() {
    return Result::ResultqueriesHandle(*this);
}
std::string Result::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(value.name());
    values.push_back(value);
    value.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Result::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Result::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, value);
}
void Result::addIDUpdates(Updates& updates) {
}
void Result::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Value);
}
void Result::delRecord() {
    deleteFromTable(table__, id);
}
void Result::delRelations() {
    ModelResultRelation::del(*db, (ModelResultRelation::Result == id));
    ResultResultQueryRelation::del(*db, (ResultResultQueryRelation::Result == id));
}
void Result::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Result::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<Result> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Result::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<Result> Result::upcast() {
    return auto_ptr<Result>(new Result(*this));
}
std::auto_ptr<Result> Result::upcastCopy() {
    Result* np = new Result(*this);
    np->id = id;
    np->type = type;
    np->value = value;
    np->inDatabase = inDatabase;
    return auto_ptr<Result>(np);
}
std::ostream & operator<<(std::ostream& os, Result o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.value.name() << " = " << o.value << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ResultQuery::Own::Id("id_","INTEGER","ResultQuery_");
ResultQuery::ResultHandle::ResultHandle(const ResultQuery& owner)
         : litesql::RelationHandle<ResultQuery>(owner) {
}
void ResultQuery::ResultHandle::link(const Result& o0) {
    ResultResultQueryRelation::link(owner->getDatabase(), o0, *owner);
}
void ResultQuery::ResultHandle::unlink(const Result& o0) {
    ResultResultQueryRelation::unlink(owner->getDatabase(), o0, *owner);
}
void ResultQuery::ResultHandle::del(const litesql::Expr& expr) {
    ResultResultQueryRelation::del(owner->getDatabase(), expr && ResultResultQueryRelation::ResultQuery == owner->id);
}
litesql::DataSource<Result> ResultQuery::ResultHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return ResultResultQueryRelation::get<Result>(owner->getDatabase(), expr, (ResultResultQueryRelation::ResultQuery == owner->id) && srcExpr);
}
litesql::DataSource<ResultResultQueryRelation::Row> ResultQuery::ResultHandle::getRows(const litesql::Expr& expr) {
    return ResultResultQueryRelation::getRows(owner->getDatabase(), expr && (ResultResultQueryRelation::ResultQuery == owner->id));
}
ResultQuery::RequestedresultsHandle::RequestedresultsHandle(const ResultQuery& owner)
         : litesql::RelationHandle<ResultQuery>(owner) {
}
void ResultQuery::RequestedresultsHandle::link(const RequestedResult& o0) {
    RequestedResultResultQueryRelation::link(owner->getDatabase(), o0, *owner);
}
void ResultQuery::RequestedresultsHandle::unlink(const RequestedResult& o0) {
    RequestedResultResultQueryRelation::unlink(owner->getDatabase(), o0, *owner);
}
void ResultQuery::RequestedresultsHandle::del(const litesql::Expr& expr) {
    RequestedResultResultQueryRelation::del(owner->getDatabase(), expr && RequestedResultResultQueryRelation::ResultQuery == owner->id);
}
litesql::DataSource<RequestedResult> ResultQuery::RequestedresultsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return RequestedResultResultQueryRelation::get<RequestedResult>(owner->getDatabase(), expr, (RequestedResultResultQueryRelation::ResultQuery == owner->id) && srcExpr);
}
litesql::DataSource<RequestedResultResultQueryRelation::Row> ResultQuery::RequestedresultsHandle::getRows(const litesql::Expr& expr) {
    return RequestedResultResultQueryRelation::getRows(owner->getDatabase(), expr && (RequestedResultResultQueryRelation::ResultQuery == owner->id));
}
const std::string ResultQuery::type__("ResultQuery");
const std::string ResultQuery::table__("ResultQuery_");
const std::string ResultQuery::sequence__("ResultQuery_seq");
const litesql::FieldType ResultQuery::Id("id_","INTEGER",table__);
const litesql::FieldType ResultQuery::Type("type_","TEXT",table__);
const litesql::FieldType ResultQuery::Query("query_","TEXT",table__);
void ResultQuery::defaults() {
    id = 0;
}
ResultQuery::ResultQuery(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), query(Query) {
    defaults();
}
ResultQuery::ResultQuery(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), query(Query) {
    defaults();
    size_t size = (rec.size() > 3) ? 3 : rec.size();
    switch(size) {
    case 3: query = convert<const std::string&, std::string>(rec[2]);
        query.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
ResultQuery::ResultQuery(const ResultQuery& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), query(obj.query) {
}
const ResultQuery& ResultQuery::operator=(const ResultQuery& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        query = obj.query;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
ResultQuery::ResultHandle ResultQuery::result() {
    return ResultQuery::ResultHandle(*this);
}
ResultQuery::RequestedresultsHandle ResultQuery::requestedresults() {
    return ResultQuery::RequestedresultsHandle(*this);
}
std::string ResultQuery::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(query.name());
    values.push_back(query);
    query.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void ResultQuery::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void ResultQuery::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, query);
}
void ResultQuery::addIDUpdates(Updates& updates) {
}
void ResultQuery::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Query);
}
void ResultQuery::delRecord() {
    deleteFromTable(table__, id);
}
void ResultQuery::delRelations() {
    ResultResultQueryRelation::del(*db, (ResultResultQueryRelation::ResultQuery == id));
    RequestedResultResultQueryRelation::del(*db, (RequestedResultResultQueryRelation::ResultQuery == id));
}
void ResultQuery::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void ResultQuery::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<ResultQuery> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ResultQuery::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<ResultQuery> ResultQuery::upcast() {
    return auto_ptr<ResultQuery>(new ResultQuery(*this));
}
std::auto_ptr<ResultQuery> ResultQuery::upcastCopy() {
    ResultQuery* np = new ResultQuery(*this);
    np->id = id;
    np->type = type;
    np->query = query;
    np->inDatabase = inDatabase;
    return auto_ptr<ResultQuery>(np);
}
std::ostream & operator<<(std::ostream& os, ResultQuery o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.query.name() << " = " << o.query << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType RequestedResult::Own::Id("id_","INTEGER","RequestedResult_");
RequestedResult::ResultqueryHandle::ResultqueryHandle(const RequestedResult& owner)
         : litesql::RelationHandle<RequestedResult>(owner) {
}
void RequestedResult::ResultqueryHandle::link(const ResultQuery& o0) {
    RequestedResultResultQueryRelation::link(owner->getDatabase(), *owner, o0);
}
void RequestedResult::ResultqueryHandle::unlink(const ResultQuery& o0) {
    RequestedResultResultQueryRelation::unlink(owner->getDatabase(), *owner, o0);
}
void RequestedResult::ResultqueryHandle::del(const litesql::Expr& expr) {
    RequestedResultResultQueryRelation::del(owner->getDatabase(), expr && RequestedResultResultQueryRelation::RequestedResult == owner->id);
}
litesql::DataSource<ResultQuery> RequestedResult::ResultqueryHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return RequestedResultResultQueryRelation::get<ResultQuery>(owner->getDatabase(), expr, (RequestedResultResultQueryRelation::RequestedResult == owner->id) && srcExpr);
}
litesql::DataSource<RequestedResultResultQueryRelation::Row> RequestedResult::ResultqueryHandle::getRows(const litesql::Expr& expr) {
    return RequestedResultResultQueryRelation::getRows(owner->getDatabase(), expr && (RequestedResultResultQueryRelation::RequestedResult == owner->id));
}
const std::string RequestedResult::type__("RequestedResult");
const std::string RequestedResult::table__("RequestedResult_");
const std::string RequestedResult::sequence__("RequestedResult_seq");
const litesql::FieldType RequestedResult::Id("id_","INTEGER",table__);
const litesql::FieldType RequestedResult::Type("type_","TEXT",table__);
const litesql::FieldType RequestedResult::Result("result_","TEXT",table__);
void RequestedResult::defaults() {
    id = 0;
}
RequestedResult::RequestedResult(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), result(Result) {
    defaults();
}
RequestedResult::RequestedResult(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), result(Result) {
    defaults();
    size_t size = (rec.size() > 3) ? 3 : rec.size();
    switch(size) {
    case 3: result = convert<const std::string&, std::string>(rec[2]);
        result.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
RequestedResult::RequestedResult(const RequestedResult& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), result(obj.result) {
}
const RequestedResult& RequestedResult::operator=(const RequestedResult& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        result = obj.result;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
RequestedResult::ResultqueryHandle RequestedResult::resultquery() {
    return RequestedResult::ResultqueryHandle(*this);
}
std::string RequestedResult::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(result.name());
    values.push_back(result);
    result.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void RequestedResult::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void RequestedResult::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, result);
}
void RequestedResult::addIDUpdates(Updates& updates) {
}
void RequestedResult::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Result);
}
void RequestedResult::delRecord() {
    deleteFromTable(table__, id);
}
void RequestedResult::delRelations() {
    RequestedResultResultQueryRelation::del(*db, (RequestedResultResultQueryRelation::RequestedResult == id));
}
void RequestedResult::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void RequestedResult::del() {
    if (typeIsCorrect() == false) {
        std::auto_ptr<RequestedResult> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool RequestedResult::typeIsCorrect() {
    return type == type__;
}
std::auto_ptr<RequestedResult> RequestedResult::upcast() {
    return auto_ptr<RequestedResult>(new RequestedResult(*this));
}
std::auto_ptr<RequestedResult> RequestedResult::upcastCopy() {
    RequestedResult* np = new RequestedResult(*this);
    np->id = id;
    np->type = type;
    np->result = result;
    np->inDatabase = inDatabase;
    return auto_ptr<RequestedResult>(np);
}
std::ostream & operator<<(std::ostream& os, RequestedResult o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.result.name() << " = " << o.result << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
ProjectModelDatabase::ProjectModelDatabase(std::string backendType, std::string connInfo)
     : litesql::Database(backendType, connInfo) {
    initialize();
}
std::vector<litesql::Database::SchemaItem> ProjectModelDatabase::getSchema() const {
    vector<Database::SchemaItem> res;
    res.push_back(Database::SchemaItem("schema_","table","CREATE TABLE schema_ (name_ TEXT, type_ TEXT, sql_ TEXT);"));
    if (backend->supportsSequences()) {
        res.push_back(Database::SchemaItem("Project_seq","sequence","CREATE SEQUENCE Project_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Analysis_seq","sequence","CREATE SEQUENCE Analysis_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Model_seq","sequence","CREATE SEQUENCE Model_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("Result_seq","sequence","CREATE SEQUENCE Result_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("ResultQuery_seq","sequence","CREATE SEQUENCE ResultQuery_seq START 1 INCREMENT 1"));
        res.push_back(Database::SchemaItem("RequestedResult_seq","sequence","CREATE SEQUENCE RequestedResult_seq START 1 INCREMENT 1"));
    }
    res.push_back(Database::SchemaItem("Project_","table","CREATE TABLE Project_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,projecttype_ TEXT)"));
    res.push_back(Database::SchemaItem("Analysis_","table","CREATE TABLE Analysis_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,analysistype_ TEXT)"));
    res.push_back(Database::SchemaItem("Model_","table","CREATE TABLE Model_ (id_ " + backend->getRowIDType() + ",type_ TEXT,name_ TEXT,path_ TEXT,guid_ TEXT)"));
    res.push_back(Database::SchemaItem("Result_","table","CREATE TABLE Result_ (id_ " + backend->getRowIDType() + ",type_ TEXT,value_ INTEGER)"));
    res.push_back(Database::SchemaItem("ResultQuery_","table","CREATE TABLE ResultQuery_ (id_ " + backend->getRowIDType() + ",type_ TEXT,query_ TEXT)"));
    res.push_back(Database::SchemaItem("RequestedResult_","table","CREATE TABLE RequestedResult_ (id_ " + backend->getRowIDType() + ",type_ TEXT,result_ TEXT)"));
    res.push_back(Database::SchemaItem("Analysis_Project_","table","CREATE TABLE Analysis_Project_ (Analysis1 INTEGER UNIQUE,Project2 INTEGER)"));
    res.push_back(Database::SchemaItem("Analysis_Model_","table","CREATE TABLE Analysis_Model_ (Analysis1 INTEGER,Model2 INTEGER)"));
    res.push_back(Database::SchemaItem("Model_Result_","table","CREATE TABLE Model_Result_ (Model1 INTEGER,Result2 INTEGER UNIQUE)"));
    res.push_back(Database::SchemaItem("Result_ResultQuery_","table","CREATE TABLE Result_ResultQuery_ (Result1 INTEGER,ResultQuery2 INTEGER UNIQUE)"));
    res.push_back(Database::SchemaItem("RequestedResult_ResultQuery_","table","CREATE TABLE RequestedResult_ResultQuery_ (RequestedResult1 INTEGER UNIQUE,ResultQuery2 INTEGER)"));
    res.push_back(Database::SchemaItem("Analysis_Project_Analysis1idx","index","CREATE INDEX Analysis_Project_Analysis1idx ON Analysis_Project_ (Analysis1)"));
    res.push_back(Database::SchemaItem("Analysis_Project_Project2idx","index","CREATE INDEX Analysis_Project_Project2idx ON Analysis_Project_ (Project2)"));
    res.push_back(Database::SchemaItem("Analysis_Project__all_idx","index","CREATE INDEX Analysis_Project__all_idx ON Analysis_Project_ (Analysis1,Project2)"));
    res.push_back(Database::SchemaItem("Analysis_Model_Analysis1idx","index","CREATE INDEX Analysis_Model_Analysis1idx ON Analysis_Model_ (Analysis1)"));
    res.push_back(Database::SchemaItem("Analysis_Model_Model2idx","index","CREATE INDEX Analysis_Model_Model2idx ON Analysis_Model_ (Model2)"));
    res.push_back(Database::SchemaItem("Analysis_Model__all_idx","index","CREATE INDEX Analysis_Model__all_idx ON Analysis_Model_ (Analysis1,Model2)"));
    res.push_back(Database::SchemaItem("Model_Result_Model1idx","index","CREATE INDEX Model_Result_Model1idx ON Model_Result_ (Model1)"));
    res.push_back(Database::SchemaItem("Model_Result_Result2idx","index","CREATE INDEX Model_Result_Result2idx ON Model_Result_ (Result2)"));
    res.push_back(Database::SchemaItem("Model_Result__all_idx","index","CREATE INDEX Model_Result__all_idx ON Model_Result_ (Model1,Result2)"));
    res.push_back(Database::SchemaItem("Result_ResultQuery_Result1idx","index","CREATE INDEX Result_ResultQuery_Result1idx ON Result_ResultQuery_ (Result1)"));
    res.push_back(Database::SchemaItem("_adae9bdcddb7a007f787a1e9d30abe24","index","CREATE INDEX _adae9bdcddb7a007f787a1e9d30abe24 ON Result_ResultQuery_ (ResultQuery2)"));
    res.push_back(Database::SchemaItem("Result_ResultQuery__all_idx","index","CREATE INDEX Result_ResultQuery__all_idx ON Result_ResultQuery_ (Result1,ResultQuery2)"));
    res.push_back(Database::SchemaItem("_b062b423e41fc7d8c60cc214049c7f25","index","CREATE INDEX _b062b423e41fc7d8c60cc214049c7f25 ON RequestedResult_ResultQuery_ (RequestedResult1)"));
    res.push_back(Database::SchemaItem("_3157d5ff2ade9c5a719de38f794d3e23","index","CREATE INDEX _3157d5ff2ade9c5a719de38f794d3e23 ON RequestedResult_ResultQuery_ (ResultQuery2)"));
    res.push_back(Database::SchemaItem("_cf9773c6dfdff623b053cea7675093e6","index","CREATE INDEX _cf9773c6dfdff623b053cea7675093e6 ON RequestedResult_ResultQuery_ (RequestedResult1,ResultQuery2)"));
    return res;
}
void ProjectModelDatabase::initialize() {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
}
}
