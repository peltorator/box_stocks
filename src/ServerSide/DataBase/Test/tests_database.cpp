#define CATCH_CONFIG_MAIN
#include "../../../../libs/catch2/catch.hpp"
#include "../database.cpp"

using namespace std;

TEST_CASE("Insert-select", "[single-file]") {
    NDataBase::Open("test_db.sqlite");

    string createQuery = "drop table if exists T; create table T (O1 integer, O2 text);";
    auto createResponse = NDataBase::Query(createQuery);
    REQUIRE(createResponse.empty());

    string insertQuery = "insert into T values (1, 'a'), (2, 'b'), (4, 'd');";
    auto insertResponse = NDataBase::Query(insertQuery);
    REQUIRE(insertResponse.empty());

    string selectQuery = "select * from T where O2 != 'b';";
    auto selectResponse = NDataBase::Query(selectQuery);
    REQUIRE(selectResponse == vector<map<string, string>>{{{"O1", "1"}, {"O2", "a"}}, {{"O1", "4"}, {"O2", "d"}}});

    NDataBase::Close();
}

TEST_CASE("Insert-select-delete-select", "[single-file]") {
    NDataBase::Open("test_db.sqlite");

    string createQuery = "drop table if exists T; create table T (O1 integer, O2 text);";
    auto createResponse = NDataBase::Query(createQuery);
    REQUIRE(createResponse.empty());

    string insertQuery = "insert into T values (1, 'a'), (2, 'b'), (4, 'd');";
    auto insertResponse = NDataBase::Query(insertQuery);
    REQUIRE(insertResponse.empty());

    string selectQuery = "select * from T where O2 != 'b';";
    auto selectResponse = NDataBase::Query(selectQuery);
    REQUIRE(selectResponse == vector<map<string, string>>{{{"O1", "1"}, {"O2", "a"}}, {{"O1", "4"}, {"O2", "d"}}});

    string deleteQuery = "delete from T where O1 = 1";
    auto deleteResponse = NDataBase::Query(deleteQuery);
    REQUIRE(deleteResponse.empty());

    string select2Query = "select * from T where O2 != 'b';";
    auto select2Response = NDataBase::Query(select2Query);
    REQUIRE(select2Response == vector<map<string, string>>{{{"O1", "4"}, {"O2", "d"}}});

    NDataBase::Close();
}

TEST_CASE("Shop-test", "[single-file]") {
    NDataBase::Open("test_db.sqlite");

    string insertItemsQuery = "insert into Item(itemName, weight, volume, cost, amount, image) values ('Sushi', 100, 200, 300, 10, ''), ('Pizza', 150, 250, 350, 4, '');";
    auto insertItemsResponse = NDataBase::Query(insertItemsQuery);
    REQUIRE(insertItemsResponse.empty());

    string insertBoxesQuery = "insert into Box(boxName, maxWeight, maxVolume, cost, available, image) values ('small', 100, 200, 300, 1, ''), ('big', 150, 250, 350, 0, '');";
    auto insertBoxesResponse = NDataBase::Query(insertBoxesQuery);
    REQUIRE(insertBoxesResponse.empty());

    string selectItemsQuery = "select itemName from Item;";
    auto selectItemsResponse = NDataBase::Query(selectItemsQuery);
    REQUIRE(selectItemsResponse == vector<map<string, string>>{{{"itemName", "Sushi"}}, {{"itemName", "Pizza"}}});

    string selectBoxesQuery = "select boxName from Box where available = 1;";
    auto selectBoxesResponse = NDataBase::Query(selectBoxesQuery);
    REQUIRE(selectBoxesResponse == vector<map<string, string>>{{{"boxName", "small"}}});

    NDataBase::Close();
}
