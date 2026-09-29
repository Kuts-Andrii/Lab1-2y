#include "deck.h"
#include <algorithm>
#include <stdexcept>

DeckDealer::DeckDealer(int cardsPerSuit, unsigned seed) : pos(0), rng(seed) {
    build(cardsPerSuit);
}

void DeckDealer::build(int cardsPerSuit) {
    if (cardsPerSuit <= 0)
        throw invalid_argument("cards per suit must be > 0");

    deck.clear();
    for (int s = 0; s < 4; s++)
        for (int r = 2; r < 2 + cardsPerSuit; r++)
            deck.emplace_back(s, r);

    reshuffle();
}

void DeckDealer::reshuffle() {
    shuffle(deck.begin(), deck.end(), rng);
    pos = 0;
}

Card DeckDealer::operator()() {
    if (pos >= deck.size()) reshuffle(); // deck exhausted, shuffle a new one
    return deck[pos++];
}