#include <doctest/doctest.h>

#include "data/field.hpp"

TEST_CASE("resize") {
    wg::Field field;
    const int x = 12;
    const int y = 24;

    field.resize(x, y);

    CHECK(field.size_x == x);
    CHECK(field.size_y == y);
    CHECK(field.elements.size() == x * y);
}

TEST_CASE("get_id_coordinates") {
    wg::Field field;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    // center elements
    CHECK(field.get_id_coordinates(0).first == 0);
    CHECK(field.get_id_coordinates(0).second == 0);
    CHECK(field.get_id_coordinates(1).first == 1);
    CHECK(field.get_id_coordinates(1).second == 0);
    CHECK(field.get_id_coordinates(2).first == 2);
    CHECK(field.get_id_coordinates(2).second == 0);

    CHECK(field.get_id_coordinates(3).first == 0);
    CHECK(field.get_id_coordinates(3).second == 1);
    CHECK(field.get_id_coordinates(4).first == 1);
    CHECK(field.get_id_coordinates(4).second == 1);
    CHECK(field.get_id_coordinates(5).first == 2);
    CHECK(field.get_id_coordinates(5).second == 1);

    CHECK(field.get_id_coordinates(6).first == 0);
    CHECK(field.get_id_coordinates(6).second == 2);
    CHECK(field.get_id_coordinates(7).first == 1);
    CHECK(field.get_id_coordinates(7).second == 2);
    CHECK(field.get_id_coordinates(8).first == 2);
    CHECK(field.get_id_coordinates(8).second == 2);

    CHECK(field.get_id_coordinates(9).first == 0);
    CHECK(field.get_id_coordinates(9).second == 3);
    CHECK(field.get_id_coordinates(10).first == 1);
    CHECK(field.get_id_coordinates(10).second == 3);
    CHECK(field.get_id_coordinates(11).first == 2);
    CHECK(field.get_id_coordinates(11).second == 3);
}

TEST_CASE("get_element_id") {
    wg::Field field;
    const int x = 3;
    const int y = 4;

    field.resize(x, y);

    // center elements
    CHECK(field.get_element_id(0, 0) == 0);
    CHECK(field.get_element_id(1, 0) == 1);
    CHECK(field.get_element_id(2, 0) == 2);

    CHECK(field.get_element_id(0, 1) == 3);
    CHECK(field.get_element_id(1, 1) == 4);
    CHECK(field.get_element_id(2, 1) == 5);

    CHECK(field.get_element_id(0, 2) == 6);
    CHECK(field.get_element_id(1, 2) == 7);
    CHECK(field.get_element_id(2, 2) == 8);

    CHECK(field.get_element_id(0, 3) == 9);
    CHECK(field.get_element_id(1, 3) == 10);
    CHECK(field.get_element_id(2, 3) == 11);

    // outer elements
    CHECK(field.get_element_id(-1, 0) == -1);
    CHECK(field.get_element_id(0, -1) == -1);
    CHECK(field.get_element_id(-1, -1) == -1);

    CHECK(field.get_element_id(3, 0) == -1);
    CHECK(field.get_element_id(0, 4) == -1);
    CHECK(field.get_element_id(3, 4) == -1);
}

TEST_CASE("get_element") {
    wg::Field field;
    const int x = 3;
    const int y = 7;
    int counter = 0;

    field.resize(x, y);

    for (int ix = 0; ix < x; ix++) {
        for (int iy = 0; iy < y; iy++) {
            field.get_element(1, 3).elevation = counter++;
        }
    }

    CHECK(field.get_element(1, 3).elevation == field.elements.at(x * 3 + 1).elevation);
    CHECK(field.get_element(2, 6).elevation == field.elements.at(x * 6 + 2).elevation);
}

TEST_CASE("get_neighbourhood_moore") {
    wg::Field field;
    const int x = 5;
    const int y = 5;

    field.resize(x, y);

    const int id1 = field.get_element_id(2, 2);
    const int id2 = field.get_element_id(1, 0);
    const int neighbourhood_size1 = 1;
    const int neighbourhood_size2 = 2;
    const std::vector<int> neighbourhood_ids1 =
        field.get_neighbourhood(id1, neighbourhood_size1, wg::Field::Neighbourhood::MOORE);
    const std::vector<int> neighbourhood_ids2 =
        field.get_neighbourhood(id2, neighbourhood_size2, wg::Field::Neighbourhood::MOORE);

    // check size
    CHECK(neighbourhood_ids1.size() == 9);
    CHECK(neighbourhood_ids2.size() == 25);

    // check ids of set 1
    CHECK(neighbourhood_ids1.at(0) == 6);
    CHECK(neighbourhood_ids1.at(1) == 7);
    CHECK(neighbourhood_ids1.at(2) == 8);

    CHECK(neighbourhood_ids1.at(3) == 11);
    CHECK(neighbourhood_ids1.at(4) == 12);
    CHECK(neighbourhood_ids1.at(5) == 13);

    CHECK(neighbourhood_ids1.at(6) == 16);
    CHECK(neighbourhood_ids1.at(7) == 17);
    CHECK(neighbourhood_ids1.at(8) == 18);

    // check ids of set 2
    CHECK(neighbourhood_ids2.at(0) == -1);
    CHECK(neighbourhood_ids2.at(1) == -1);
    CHECK(neighbourhood_ids2.at(2) == -1);
    CHECK(neighbourhood_ids2.at(3) == -1);
    CHECK(neighbourhood_ids2.at(4) == -1);

    CHECK(neighbourhood_ids2.at(5) == -1);
    CHECK(neighbourhood_ids2.at(6) == -1);
    CHECK(neighbourhood_ids2.at(7) == -1);
    CHECK(neighbourhood_ids2.at(8) == -1);
    CHECK(neighbourhood_ids2.at(9) == -1);

    CHECK(neighbourhood_ids2.at(10) == -1);
    CHECK(neighbourhood_ids2.at(11) == 0);
    CHECK(neighbourhood_ids2.at(12) == 1);
    CHECK(neighbourhood_ids2.at(13) == 2);
    CHECK(neighbourhood_ids2.at(14) == 3);

    CHECK(neighbourhood_ids2.at(15) == -1);
    CHECK(neighbourhood_ids2.at(16) == 5);
    CHECK(neighbourhood_ids2.at(17) == 6);
    CHECK(neighbourhood_ids2.at(18) == 7);
    CHECK(neighbourhood_ids2.at(19) == 8);

    CHECK(neighbourhood_ids2.at(20) == -1);
    CHECK(neighbourhood_ids2.at(21) == 10);
    CHECK(neighbourhood_ids2.at(22) == 11);
    CHECK(neighbourhood_ids2.at(23) == 12);
    CHECK(neighbourhood_ids2.at(24) == 13);
}