/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
    std::unique_ptr<Project> upcast();
    std::unique_ptr<Project> upcastCopy();
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
    std::unique_ptr<Analysis> upcast();
    std::unique_ptr<Analysis> upcastCopy();
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
    std::unique_ptr<Model> upcast();
    std::unique_ptr<Model> upcastCopy();
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
    std::unique_ptr<Result> upcast();
    std::unique_ptr<Result> upcastCopy();
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
    std::unique_ptr<ResultQuery> upcast();
    std::unique_ptr<ResultQuery> upcastCopy();
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
    std::unique_ptr<RequestedResult> upcast();
    std::unique_ptr<RequestedResult> upcastCopy();
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
