#include "card.h"
#include <stdexcept>

Card::Card(int s, int r) : suit(s), rank(r) {
    if (s < 0 || s > 3) throw invalid_argument("bad suit");
    // rank can be larger than 14 for bigger decks
    if (r < 2) throw invalid_argument("bad rank");
}

string Card::getSuitName() const {
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    return suits[suit];
}

string Card::getRankName() const {
    if (rank >= 2 && rank <= 10) return to_string(rank);
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    if (rank == 14) return "A";
    return to_string(rank); // for cards above 14
}

ostream& operator<<(ostream& os, const Card& c) {
    os << c.getRankName() << " of " << c.getSuitName();
    return os;
}