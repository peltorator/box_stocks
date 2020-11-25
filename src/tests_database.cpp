#define CATCH_CONFIG_MAIN
#include "../catch2/catch.hpp"
#include "database.cpp"

using namespace std;

TEST_CASE("Insert-select", "[single-file]") {
    TDataBase dataBase("test_db.sqlite");

    string createQuery = "drop table if exists T; create table T (O1 integer, O2 text);";
    auto createResponse = dataBase.Query(createQuery);
    REQUIRE(createResponse.empty());

    string insertQuery = "insert into T values (1, 'a'), (2, 'b'), (4, 'd');";
    auto insertResponse = dataBase.Query(insertQuery);
    REQUIRE(insertResponse.empty());

    string selectQuery = "select * from T where O2 != 'b';";
    auto selectResponse = dataBase.Query(selectQuery);
    REQUIRE(selectResponse == vector<map<string, string>>{{{"O1", "1"}, {"O2", "a"}}, {{"O1", "4"}, {"O2", "d"}}});

    dataBase.Close();
}

TEST_CASE("Insert-select-delete-select", "[single-file]") {
    TDataBase dataBase("test_db.sqlite");

    string createQuery = "drop table if exists T; create table T (O1 integer, O2 text);";
    auto createResponse = dataBase.Query(createQuery);
    REQUIRE(createResponse.empty());

    string insertQuery = "insert into T values (1, 'a'), (2, 'b'), (4, 'd');";
    auto insertResponse = dataBase.Query(insertQuery);
    REQUIRE(insertResponse.empty());

    string selectQuery = "select * from T where O2 != 'b';";
    auto selectResponse = dataBase.Query(selectQuery);
    REQUIRE(selectResponse == vector<map<string, string>>{{{"O1", "1"}, {"O2", "a"}}, {{"O1", "4"}, {"O2", "d"}}});

    string deleteQuery = "delete from T where O1 = 1";
    auto deleteResponse = dataBase.Query(deleteQuery);
    REQUIRE(deleteResponse.empty());

    string select2Query = "select * from T where O2 != 'b';";
    auto select2Response = dataBase.Query(select2Query);
    REQUIRE(select2Response == vector<map<string, string>>{{{"O1", "4"}, {"O2", "d"}}});

    dataBase.Close();
}
