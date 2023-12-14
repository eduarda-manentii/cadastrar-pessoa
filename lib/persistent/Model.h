#ifndef _PERSISTENT_MODEL_H_
#define _PERSISTENT_MODEL_H_

#include "Condition.h"
#include "Exception.h"
#include "Result.h"
#include "Sql.h"
#include "Table.h"
#include "Transaction.h"
#include "boolean.h"
#include "datetime.h"
#include "driver/Connection.h"

//#include <iostream>
// using namespace std;

#include <functional>
#include <memory>

namespace persistent {

template <typename T>
class DatabaseTable : public Table<T> {
   public:
    DatabaseTable(std::shared_ptr<Connection> con) {
        con->update_schema(this->definition());
    }
};

template <class T>
class Model {
   public:
    std::string to_string() const {
        std::string s = table_name() + "{ ";
        for (AbstractColumn<T>* col : _table.columns()) {
            s += col->name + ": " + col->to_string(self());
            s += '\t';
        }
        s += "} ";
        return s;
    }

    bool new_record() {
        return _new_record;
    }

    static T find(int id) {
        T obj;
        (*_table.pks().begin())->set(obj, id);
        if (!obj.select()) {
            throw Exception("Could not find " + table_name() +
                            " with id=" + boost::lexical_cast<std::string>(id));
        }
        return obj;
    }

    bool find() {
        return select();
    }

    static Result<T> select(Conditions&& conditions) {
        return {std::forward<Conditions>(conditions)};
    }

    static Result<T> first(Conditions&& conditions = Conditions()) {
        return {std::forward<Conditions>(conditions)};
    }

    template <typename... Args>
    static Result<T> where(const std::string& sql, Args... args) {
        return select(persistent::where(sql, args...));
    }

    static Result<T> all() {
        return {persistent::all()};
    }

    template <typename... Args>
    void update_all(const std::vector<std::string>& column_names,
                    const std::string& sql, Args... args) {
        update_all(column_names, persistent::where(sql, args...));
    }

    void update_all(const std::vector<std::string>& column_names,
                    Conditions&& conditions) {
        update_all(get_columns(column_names, _table.non_pks()),
                   std::forward<Conditions>(conditions));
    }

    template <typename Callback>
    void find_alike(Conditions&& conditions, Callback callback) {
        add_like_conditions(conditions, modified());
        select_all(std::forward<Conditions>(conditions), callback);
    }

    template <typename Callback>
    static void find(Conditions&& cond, Callback callback) {
        return select_all(std::forward<Conditions>(cond), callback);
    }

    void save() {
        self().before_save();
        if (_new_record) {
            insert();
        } else {
            update();
        }
        self().after_save();
    }

    void insert_or_update() {
        self().before_save();
        if (modified(_table.pks()).size() == 0) {
            insert();
        } else if (T::count(make_conditions(_table.pks())) > 0) {
            _new_record = false;
            update();
        } else {
            insert();
        }
        self().after_save();
    }

    void delete_() {
        std::string sql =
            Sql::delete_(table_name(), get_conditions(_table.pks()));
        log_sql(sql);
        auto connection = get_connection();
        auto st = prepare(connection, sql);
        bind(*st, modified(_table.pks()));
        st->execute();
        _new_record = true;
    }

    static int count(Conditions&& conditions = Conditions()) {
        std::string sql =
            Sql::select(from_clause(conditions), "count(*)", conditions.get());
        log_sql(sql);
        auto connection = get_connection();
        auto st = prepare(connection, sql);
        conditions.bind(*st);
        auto ex = st->execute_query();
        if (ex->next_row()) {
            int val;
            ex->extract_int(val);
            return val;
        }
        throw Exception("error counting.");
    }

    int count_alike(Conditions&& conditions = Conditions()) {
        add_like_conditions(conditions, modified());
        return count(std::forward<Conditions>(conditions));
    }

    static double calculate(const std::string& calc_sql,
                            Conditions&& conditions = Conditions()) {
        std::string sql =
            Sql::select(from_clause(conditions), calc_sql, conditions.get());
        log_sql(sql);
        auto connection = get_connection();
        auto st = prepare(connection, sql);
        conditions.bind(*st);
        auto ex = st->execute_query();
        if (ex->next_row()) {
            double val;
            ex->extract(val);
            return val;
        }
        return 0;
    }

    double calculate_alike(const std::string& calc_sql,
                           Conditions&& conditions = Conditions()) {
        add_like_conditions(conditions, modified());
        return calculate(calc_sql, std::forward<Conditions>(conditions));
    }

    static void transaction(std::function<void()> callback) {
        auto con = get_connection();
        Transaction t(con);
        con->transaction([&callback] {
            callback();
        });
    }

    inline static const std::string& table_name() {
        return _table.name();
    }

   protected:
    Model() : _new_record(true) {
        for (AbstractColumn<T>* col : _table.columns()) {
            col->construct_null(self());
        }
    }

    inline static std::string from_clause(Conditions& conditions) {
        return table_name() + conditions.joins();
    }

    void before_save() {
    }

    void after_save() {
    }

    void after_select() {
    }

    template <typename C>
    static void require(const C& val,
                        const std::string& message = "requirement not met") {
        if (val == null::value<C>()) {
            throw Exception(message);
        }
    }
    void validates_presence_of(const std::string& column_name) {
        if (_table.get_column(column_name)->is_null(self())) {
            throw Exception(_table.name() + " must have a " + column_name +
                            ".");
        }
    }

   private:
    inline T& self() {
        return static_cast<T&>(*this);
    }

    inline const T& self() const {
        return static_cast<const T&>(*this);
    }

    vector<AbstractColumn<T>*> modified(
        const vector<AbstractColumn<T>*>& columns = _table.columns()) {
        vector<AbstractColumn<T>*> v;
        for (AbstractColumn<T>* col : columns) {
            if (!col->is_null(self())) {
                v.push_back(col);
            }
        }
        return v;
    }

    void bind(Statement& st, const vector<AbstractColumn<T>*>& columns) const {
        for (AbstractColumn<T>* col : columns) {
            col->bind(st, self());
        }
    }

    void read(Extractor& ex, const vector<AbstractColumn<T>*>& columns) {
        for (AbstractColumn<T>* col : columns) {
            col->read(ex, self());
        }
    }

    void add_like_conditions(Conditions& conditions,
                             const vector<AbstractColumn<T>*>& columns) {
        for (AbstractColumn<T>* col : columns) {
            col->add_like_condition(self(), conditions);
        }
    }

    Conditions make_conditions(const vector<AbstractColumn<T>*>& columns) {
        Conditions conditions;
        for (auto col : columns) {
            col->add_condition(self(), conditions);
        }
        return conditions;
    }

    void insert() {
        vector<AbstractColumn<T>*> values = modified();
        std::string sql = Sql::insert(table_name(), get_simple_names(values));
        log_sql(sql);
        auto connection = get_connection();
        auto st = prepare(connection, sql);
        bind(*st, values);
        if (_table.pks().size() == 1 && (*_table.pks().begin())->is_integer()) {
            uint64_t rowid = st->execute_insert((*_table.pks().begin())->name);
            if (!_table.pks().empty()) {
                (*_table.pks().begin())->set(self(), rowid);
            }
        } else {
            st->execute();
        }
        _new_record = false;
    }

    void update() {
        std::string sql =
            Sql::update(table_name(), get_simple_names(_table.non_pks()),
                        get_conditions(_table.pks()));
        log_sql(sql);
        auto connection = get_connection();
        auto st = prepare(connection, sql);
        bind(*st, _table.non_pks());
        bind(*st, _table.pks());
        st->execute();
    }

    bool select() {
        std::string sql = Sql::select(table_name(), get_names(_table.non_pks()),
                                      get_conditions(_table.pks()));
        log_sql(sql);
        auto connection = get_connection();
        auto st = prepare(connection, sql);
        bind(*st, modified(_table.pks()));
        auto ex = st->execute_query();
        if (ex->next_row()) {
            read(*ex, _table.non_pks());
            _new_record = false;
            return true;
        }
        return false;
    }

    template <typename Callback>
    static void select_all(Conditions&& conditions, Callback callback) {
        std::string sql =
            Sql::select(from_clause(conditions), get_names(_table.columns()),
                        conditions.get()) +
            conditions.order() + conditions.limit();
        log_sql(sql);
        auto connection = get_connection();
        auto st = prepare(connection, sql);
        conditions.bind(*st);
        auto ex = st->execute_query();
        T obj;
        obj._new_record = false;
        while (ex->next_row()) {
            obj.read(*ex, _table.columns());
            obj.after_select();
            callback(obj);
        }
    }

    void update_all(const std::vector<AbstractColumn<T>*>& columns,
                    Conditions&& conditions) {
        std::string sql = Sql::update(table_name(), get_simple_names(columns),
                                      conditions.get());
        log_sql(sql);
        auto connection = get_connection();
        auto st = prepare(connection, sql);
        bind(*st, columns);
        conditions.bind(*st);
        st->execute();
    }

    vector<AbstractColumn<T>*> get_columns(
        const vector<std::string>& column_names,
        const vector<AbstractColumn<T>*>& columns) {
        vector<AbstractColumn<T>*> v;
        for (const auto& name : column_names) {
            for (auto col : columns) {
                if (col->name == name) {
                    v.push_back(col);
                    break;
                }
            }
        }
        return v;
    }

    static vector<std::string> get_simple_names(
        const vector<AbstractColumn<T>*>& columns) {
        vector<std::string> v;
        for (AbstractColumn<T>* col : columns) {
            v.push_back(col->name);
        }
        return v;
    }

    static vector<std::string> get_names(
        const vector<AbstractColumn<T>*>& columns) {
        vector<std::string> v;
        for (AbstractColumn<T>* col : columns) {
            v.push_back(table_name() + "." + col->name);
        }
        return v;
    }

    vector<std::string> get_conditions(
        const vector<AbstractColumn<T>*>& columns) {
        vector<std::string> v;
        for (AbstractColumn<T>* col : columns) {
            v.push_back(col->condition(self()));
        }
        return v;
    }

    static std::unique_ptr<Statement> prepare(
        std::shared_ptr<Connection> connection, const std::string& sql) {
        return connection->prepare(sql);
    }

    static std::shared_ptr<Connection> get_connection() {
        return Transaction::get_current_connection();
    }

    static inline void log_sql(const std::string& sql) {
        std::cout << "SQL: " << sql << std::endl;
    }

    static const DatabaseTable<T> _table;
    bool _new_record;
};

template <class T>
const DatabaseTable<T> Model<T>::_table(get_connection());

}  // namespace persistent

#endif  //_PERSISTENT_MODEL_H_
