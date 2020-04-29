#include <iostream>
using namespace std;

#include "Database.hpp"

namespace LogSupervisor::Database{

Database::Database() :
    db(":memory:")
{
    // SQLite has no DATETIME format. Let's store everything as string
    db.exec("CREATE TABLE 'auths'('id' INTEGER PRIMARY KEY AUTOINCREMENT,          "
            "                     'user' TEXT, 'origin' TEXT, 'timepoint' TEXT,    "
            "                     'validity' INTEGER );");
}

void Database::insert(const Authentication& auth){
    shared_ptr<SQLite::Statement> ps = db.prepare("INSERT INTO auths (user, origin, timepoint, validity)" \
                                                  "VALUES (?, ?, ?, ?); ");
    ps->bindText(1, auth.user);
    ps->bindText(2, *(auth.origin.to_string()));
    ps->bindText(3, auth.timestamp.to_string());
    ps->bindInt(4, auth.validity);
    int rc = ps->step();
}


Request Database::all(){
    shared_ptr<SQLite::Statement> statement = db.prepare("SELECT * from auths;");
    return Request(statement);
}

void Database::updateValidity(int id, Authentication::Validity v){
    shared_ptr<SQLite::Statement> statement = db.prepare("UPDATE 'auths' SET validity = ? WHERE id = ?");
    statement->bindInt(1, v);
    statement->bindInt(2, id);
    statement->step();
}

std::shared_ptr<Authentication> Database::fetch(int id){
    shared_ptr<SQLite::Statement> statement = db.prepare("SELECT * from auths WHERE id = ?;");
    statement->bindInt(1, id);
    return Request(statement).firstRow();
}


} // namespace