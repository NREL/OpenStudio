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

#ifndef MODELEDITOR_PROJECTMODELDATABASE_HPP
#define MODELEDITOR_PROJECTMODELDATABASE_HPP

#include "litesql.hpp"

namespace ProjectModel {
class Project;
class Analysis;
class Model;
class Result;
class ResultQuery;
class RequestedResult;
class AnalysisProjectRelation {
public:
    class Row {
    public:
        litesql::Field<int> project;
        litesql::Field<int> analysis;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Analysis;
    static const litesql::FieldType Project;
    static void link(const litesql::Database& db, const ProjectModel::Analysis& o0, const ProjectModel::Project& o1);
    static void unlink(const litesql::Database& db, const ProjectModel::Analysis& o0, const ProjectModel::Project& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<AnalysisProjectRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class AnalysisModelRelation {
public:
    class Row {
    public:
        litesql::Field<int> model;
        litesql::Field<int> analysis;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Analysis;
    static const litesql::FieldType Model;
    static void link(const litesql::Database& db, const ProjectModel::Analysis& o0, const ProjectModel::Model& o1);
    static void unlink(const litesql::Database& db, const ProjectModel::Analysis& o0, const ProjectModel::Model& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<AnalysisModelRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class ModelResultRelation {
public:
    class Row {
    public:
        litesql::Field<int> result;
        litesql::Field<int> model;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Model;
    static const litesql::FieldType Result;
    static void link(const litesql::Database& db, const ProjectModel::Model& o0, const ProjectModel::Result& o1);
    static void unlink(const litesql::Database& db, const ProjectModel::Model& o0, const ProjectModel::Result& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<ModelResultRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class ResultResultQueryRelation {
public:
    class Row {
    public:
        litesql::Field<int> resultQuery;
        litesql::Field<int> result;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Result;
    static const litesql::FieldType ResultQuery;
    static void link(const litesql::Database& db, const ProjectModel::Result& o0, const ProjectModel::ResultQuery& o1);
    static void unlink(const litesql::Database& db, const ProjectModel::Result& o0, const ProjectModel::ResultQuery& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<ResultResultQueryRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class RequestedResultResultQueryRelation {
public:
    class Row {
    public:
        litesql::Field<int> resultQuery;
        litesql::Field<int> requestedResult;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType RequestedResult;
    static const litesql::FieldType ResultQuery;
    static void link(const litesql::Database& db, const ProjectModel::RequestedResult& o0, const ProjectModel::ResultQuery& o1);
    static void unlink(const litesql::Database& db, const ProjectModel::RequestedResult& o0, const ProjectModel::ResultQuery& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<RequestedResultResultQueryRelation::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class Project : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class AnalysesHandle : public litesql::RelationHandle<Project> {
    public:
        AnalysesHandle(const Project& owner);
        void link(const Analysis& o0);
        void unlink(const Analysis& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Analysis> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<AnalysisProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Name;
    litesql::Field<std::string> name;
    static const litesql::FieldType Projecttype;
    litesql::Field<std::string> projecttype;
protected:
    void defaults();
public:
    Project(const litesql::Database& db);
    Project(const litesql::Database& db, const litesql::Record& rec);
    Project(const Project& obj);
    const Project& operator=(const Project& obj);
    Project::AnalysesHandle analyses();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Project> upcast();
    std::auto_ptr<Project> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Project o);
class Analysis : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ProjectHandle : public litesql::RelationHandle<Analysis> {
    public:
        ProjectHandle(const Analysis& owner);
        void link(const Project& o0);
        void unlink(const Project& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Project> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<AnalysisProjectRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ModelsHandle : public litesql::RelationHandle<Analysis> {
    public:
        ModelsHandle(const Analysis& owner);
        void link(const Model& o0);
        void unlink(const Model& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Model> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<AnalysisModelRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Name;
    litesql::Field<std::string> name;
    static const litesql::FieldType Analysistype;
    litesql::Field<std::string> analysistype;
protected:
    void defaults();
public:
    Analysis(const litesql::Database& db);
    Analysis(const litesql::Database& db, const litesql::Record& rec);
    Analysis(const Analysis& obj);
    const Analysis& operator=(const Analysis& obj);
    Analysis::ProjectHandle project();
    Analysis::ModelsHandle models();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Analysis> upcast();
    std::auto_ptr<Analysis> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Analysis o);
class Model : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class AnalysesHandle : public litesql::RelationHandle<Model> {
    public:
        AnalysesHandle(const Model& owner);
        void link(const Analysis& o0);
        void unlink(const Analysis& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Analysis> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<AnalysisModelRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ResultsHandle : public litesql::RelationHandle<Model> {
    public:
        ResultsHandle(const Model& owner);
        void link(const Result& o0);
        void unlink(const Result& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Result> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ModelResultRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Name;
    litesql::Field<std::string> name;
    static const litesql::FieldType Path;
    litesql::Field<std::string> path;
    static const litesql::FieldType Guid;
    litesql::Field<std::string> guid;
protected:
    void defaults();
public:
    Model(const litesql::Database& db);
    Model(const litesql::Database& db, const litesql::Record& rec);
    Model(const Model& obj);
    const Model& operator=(const Model& obj);
    Model::AnalysesHandle analyses();
    Model::ResultsHandle results();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Model> upcast();
    std::auto_ptr<Model> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Model o);
class Result : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ModelHandle : public litesql::RelationHandle<Result> {
    public:
        ModelHandle(const Result& owner);
        void link(const Model& o0);
        void unlink(const Model& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Model> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ModelResultRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class ResultqueriesHandle : public litesql::RelationHandle<Result> {
    public:
        ResultqueriesHandle(const Result& owner);
        void link(const ResultQuery& o0);
        void unlink(const ResultQuery& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<ResultQuery> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ResultResultQueryRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Value;
    litesql::Field<int> value;
protected:
    void defaults();
public:
    Result(const litesql::Database& db);
    Result(const litesql::Database& db, const litesql::Record& rec);
    Result(const Result& obj);
    const Result& operator=(const Result& obj);
    Result::ModelHandle model();
    Result::ResultqueriesHandle resultqueries();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<Result> upcast();
    std::auto_ptr<Result> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, Result o);
class ResultQuery : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ResultHandle : public litesql::RelationHandle<ResultQuery> {
    public:
        ResultHandle(const ResultQuery& owner);
        void link(const Result& o0);
        void unlink(const Result& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Result> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<ResultResultQueryRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class RequestedresultsHandle : public litesql::RelationHandle<ResultQuery> {
    public:
        RequestedresultsHandle(const ResultQuery& owner);
        void link(const RequestedResult& o0);
        void unlink(const RequestedResult& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<RequestedResult> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<RequestedResultResultQueryRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Query;
    litesql::Field<std::string> query;
protected:
    void defaults();
public:
    ResultQuery(const litesql::Database& db);
    ResultQuery(const litesql::Database& db, const litesql::Record& rec);
    ResultQuery(const ResultQuery& obj);
    const ResultQuery& operator=(const ResultQuery& obj);
    ResultQuery::ResultHandle result();
    ResultQuery::RequestedresultsHandle requestedresults();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<ResultQuery> upcast();
    std::auto_ptr<ResultQuery> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, ResultQuery o);
class RequestedResult : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class ResultqueryHandle : public litesql::RelationHandle<RequestedResult> {
    public:
        ResultqueryHandle(const RequestedResult& owner);
        void link(const ResultQuery& o0);
        void unlink(const ResultQuery& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<ResultQuery> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<RequestedResultResultQueryRelation::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Result;
    litesql::Field<std::string> result;
protected:
    void defaults();
public:
    RequestedResult(const litesql::Database& db);
    RequestedResult(const litesql::Database& db, const litesql::Record& rec);
    RequestedResult(const RequestedResult& obj);
    const RequestedResult& operator=(const RequestedResult& obj);
    RequestedResult::ResultqueryHandle resultquery();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect();
    std::auto_ptr<RequestedResult> upcast();
    std::auto_ptr<RequestedResult> upcastCopy();
};
std::ostream & operator<<(std::ostream& os, RequestedResult o);
class ProjectModelDatabase : public litesql::Database {
public:
    ProjectModelDatabase(std::string backendType, std::string connInfo);
protected:
    virtual std::vector<litesql::Database::SchemaItem> getSchema() const;
    static void initialize();
};
}
#endif // MODELEDITOR_PROJECTMODELDATABASE_HPP
