#pragma once

#include "card.h"
#include <vector>
#include <random>

using namespace std;

// functional object: returns next card, reshuffles when exhausted
class DeckDealer {
public:
    DeckDealer(int cardsPerSuit, unsigned seed = random_device{}());

    Card operator()();

private:
    vector<Card> deck;
    size_t pos;
    mt19937 rng;

    void build(int cardsPerSuit);
    void reshuffle();
};