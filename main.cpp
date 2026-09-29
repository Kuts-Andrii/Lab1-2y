// Lab 1. Andrii Kuts, K-28.
// g++ 13+ / C++23.

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <stdexcept>
#include <limits>
#include <sstream>

#include "card.h"
#include "deck.h"

using namespace std;

template <typename P>
void testCompare(P& print) {
    print("\nComparison test\n");

    Card a(0, 5), b(1, 10), c(0, 5);

    print("Card a: "); print(a); print("\n");
    print("Card b: "); print(b); print("\n");
    print("Card c: "); print(c); print("\n");

    auto r = (a <=> b);
    if (r < 0)      print("a is less than b\n");
    else if (r > 0) print("a is greater than b\n");
    else            print("a is equal to b\n");

    if ((a <=> c) == 0) print("a is equal to c\n");
}

template <typename P>
void testLambda(P& print, const vector<Card>& cards) {
    print("\nLambda test\n");

    auto sorted = cards;
    sort(sorted.begin(), sorted.end(),
         [](const Card& x, const Card& y) { return x.suit < y.suit; });

    print("First 5 after sorting by suit: ");
    for (size_t i = 0; i < min<size_t>(5, sorted.size()); i++) {
        if (i > 0) print(", ");
        print(sorted[i]);
    }
    print("\n");

    int faces = count_if(cards.begin(), cards.end(),
                         [](const Card& c) { return c.rank > 10; });
    print("Face cards: " + to_string(faces) + "\n");
}

template <typename P>
void testStdFuncs(P& print, const vector<Card>& cards) {
    print("\nFunctionals test\n");

    function<int(const Card&)> rankOf = [](const Card& c) { return c.rank; };

    vector<int> ranks;
    transform(cards.begin(), cards.end(), back_inserter(ranks), rankOf);

    int total = accumulate(ranks.begin(), ranks.end(), 0, plus<int>());
    print("Sum of all ranks: " + to_string(total) + "\n");
}

vector<int> runTest(int cardsPerSuit, int total, unsigned seed) {
    DeckDealer dealer(cardsPerSuit, seed);

    vector<int> lens;
    int cur = 0;
    Card prev;
    bool first = true;

    for (int i = 0; i < total; i++) {
        Card c = dealer();

        if (first) {
            prev = c;
            cur = 1;
            first = false;
        } else if (c > prev) {
            cur++;
            prev = c;
        } else {
            lens.push_back(cur);
            cur = 1;
            prev = c;
        }
    }
    if (cur > 0) lens.push_back(cur);
    return lens;
}

template <typename P>
void printStats(P& print, const vector<int>& lens) {
    if (lens.empty()) return;

    map<int,int> freq;
    for (int l : lens) freq[l]++;

    int mode = 0, mc = 0;
    for (auto& p : freq)
        if (p.second > mc) { mc = p.second; mode = p.first; }

    double mean = accumulate(lens.begin(), lens.end(), 0.0) / lens.size();

    auto s = lens;
    sort(s.begin(), s.end());
    double med = (s.size() % 2 == 0)
        ? (s[s.size()/2 - 1] + s[s.size()/2]) / 2.0
        : s[s.size()/2];

    print("Total number of stacks: " + to_string(lens.size()) + "\n");
    print("Most frequent length: " + to_string(mode)
          + " (occurred " + to_string(mc) + " times)\n");

    ostringstream s1;
    s1 << fixed << setprecision(4) << mean;
    print("Average length: " + s1.str() + "\n");

    ostringstream s2;
    s2 << fixed << setprecision(4) << med;
    print("Median length: " + s2.str() + "\n");

    print("Distribution of stack lengths:\n");
    for (auto& p : freq) {
        double pct = 100.0 * p.second / lens.size();
        ostringstream s3;
        s3 << "  length " << p.first << ": " << p.second
           << " times, " << fixed << setprecision(2) << pct << "%\n";
        print(s3.str());
    }

    int sum = ranges::fold_left(lens, 0, plus<int>());
    int n   = ranges::fold_left(lens, 0, [](int a, int){ return a + 1; });
    print("fold_left check: sum = " + to_string(sum)
          + ", count = " + to_string(n) + "\n");
}

bool readInput(int& cps, int& deals) {
    ifstream fin("input.txt");
    if (!fin.is_open()) return false;

    if (!(fin >> cps >> deals))
        throw runtime_error("bad format in input.txt");
    if (cps < 2 || cps > 30)
        throw runtime_error("cards per suit out of [2..30]");
    if (deals < 1 || deals > 100000)
        throw runtime_error("cards to deal out of [1..100000]");
    return true;
}

int askNumber(const string& prompt, int lo, int hi) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) {
            if (v >= lo && v <= hi) return v;
            cout << "value must be in [" << lo << ".." << hi << "]\n";
        } else {
            cout << "not a number, try again\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    ofstream fout("output.txt");
    if (!fout.is_open()) {
        cerr << "cannot open output.txt\n";
        return 1;
    }

    auto print = [&](const auto& x) { cout << x; fout << x; };

    try {
        print("Andrii Kuts, group K-28\n");

        int cps = 0, deals = 0;

        if (readInput(cps, deals)) {
            print("\nInput parameters loaded from input.txt\n");
            print("Cards per suit: " + to_string(cps) + "\n");
            print("Cards to deal:  " + to_string(deals) + "\n");
        } else {
            print("\ninput.txt not found, switching to interactive input\n");
            cps   = askNumber("Enter number of cards per suit (2 to 30): ", 2, 30);
            deals = askNumber("Enter number of cards to deal (1 to 10000): ", 1, 10000);
        }

        testCompare(print);

        DeckDealer demo(13, 42);
        vector<Card> sample;
        for (int i = 0; i < 20; i++) sample.push_back(demo());

        testLambda(print, sample);
        testStdFuncs(print, sample);

        print("\nMain experiment\n");
        auto lens = runTest(cps, deals, 12345);
        printStats(print, lens);

        print("\nProgrammatic experiment\n");
        print("Dependence of average and median stack length on deck size:\n");
        for (int n : {5, 10, 13, 20}) {
            vector<int> all;
            for (int i = 0; i < 200; i++) {
                auto l = runTest(n, 1000, i * 7 + n);
                all.insert(all.end(), l.begin(), l.end());
            }

            double mean = accumulate(all.begin(), all.end(), 0.0) / all.size();

            auto s = all;
            sort(s.begin(), s.end());
            double med = (s.size() % 2 == 0)
                ? (s[s.size()/2 - 1] + s[s.size()/2]) / 2.0
                : s[s.size()/2];

            ostringstream s4;
            s4 << fixed << setprecision(4);
            s4 << "  cards per suit " << n
               << ", average stack length " << mean
               << ", median stack length " << med << "\n";
            print(s4.str());
        }

        print("\nDone.\n");
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        fout << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}