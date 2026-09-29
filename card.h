#pragma once

#include <compare>
#include <iostream>
#include <string>

using namespace std;

class Card {
public:
    int suit; // 0..3
    int rank; // >= 2

    Card(int s = 0, int r = 2);

    // compare by rank only
    strong_ordering operator<=>(const Card& other) const {
        return rank <=> other.rank;
    }

    friend ostream& operator<<(ostream& os, const Card& c);

    string getSuitName() const;
    string getRankName() const;
};