#include <cassert>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <utility>
#include <vector>
#include <unordered_map>

template<typename Product>
struct Bestsellers {
    struct Node {
        Product product;
        size_t amount;

        Node(Product p, const size_t a) : product(std::move(p)), amount(a) {
        }

        void addAmount(size_t amt) {
            this->amount += amt;
        }
    };

    void sell(const Product &p, size_t amount) {
        auto it = u_map.find(p);
        if (it == u_map.end()) {
            Node *node = new Node(p, amount);
            u_map.insert({p, node});
            auto low = std::lower_bound(vct.begin(), vct.end(), node,
                                        [](Node *n, Node *p) { return n->amount < p->amount; });
            vct.emplace(low, node);
        } else {
            size_t index = getIndex(it->second, p);
            vct[index]->addAmount(amount);
            valueUp(index);
        }
    }

    void valueUp(size_t index) {
        if (index < vct.size() - 1) {
            if (vct[index]->amount > vct[index + 1]->amount) {
                Node *tmp = vct[index];
                vct[index] = vct[index + 1];
                vct[index + 1] = tmp;
                valueUp(index + 1);
            }
        }
    }

    size_t getIndex(Node *node, const Product &p) const {
        auto low = std::lower_bound(vct.begin(), vct.end(), node,
                                    [](Node *n, Node *p) { return n->amount < p->amount; });

        size_t index = std::distance(vct.begin(), low);
        for (size_t i = index; i < vct.size() - 1; ++i) {
            if (vct[index]->product == p) {
                return index;
            }
            if (vct[i + 1]->amount == vct[index]->amount) {
                index = i + 1;
            } else {
                break;
            }
        }
        return index;
    }

    size_t rank(const Product &p) const {
        auto it = u_map.find(p);
        if (it != u_map.end()) {
            return (vct.size() -
                    getIndex(it->second, p));
        }
        throw std::out_of_range("Out of range");
    }

    const Product &product(size_t rank) const {
        if (rank < 1 || rank > u_map.size())
            throw std::out_of_range("Out of range");
        return vct[vct.size() - rank]->product;
    }

    [[nodiscard]] size_t sold(size_t rank) const {
        if (rank < 1 || rank > u_map.size())
            throw std::out_of_range("Out of range");
        return vct[vct.size() - rank]->amount;
    }

    [[nodiscard]] size_t sold(size_t from, size_t to) const {
        if (to < from || from < 1 || to > u_map.size())
            throw std::out_of_range("Out of range");

        if (vct[vct.size() - from]->amount == vct[vct.size() - to]->amount) {
            return (to - from + 1) * vct[vct.size() - from]->amount;
        }

        size_t result = 0;
        for (size_t i = from; i <= to; i++) {
            result += vct[vct.size() - i]->amount;
        }
        return result;
    }

    [[nodiscard]] size_t products() const {
        return u_map.size();
    }

    ~Bestsellers() {
        for (auto &u: u_map) {
            delete u.second;
        }
    }

    std::unordered_map<Product, Node *> u_map;
    std::vector<Node *> vct;
};

void test1() {
    Bestsellers<std::string> T;
    assert(T.products() == 0);
    T.sell("coke", 32);
    assert(T.products() == 1);
    T.sell("bread", 1);
    assert(T.products() == 2);
    T.sell("ham", 2);
    assert(T.products() == 3);
    T.sell("mushrooms", 12);
    assert(T.products() == 4);

    assert(T.rank("ham") == 3);
    assert(T.rank("mushrooms") == 2);
    assert(T.rank("coke") == 1);
    assert(T.rank("bread") == 4);
    assert(T.sold(1, 3) == 46);
    assert(T.product(2) == "mushrooms");
    assert(T.product(3) == "ham");
    assert(T.product(1) == "coke");
    assert(T.product(4) == "bread");

    T.sell("ham", 11);
    assert(T.products() == 4);
    assert(T.product(2) == "ham");
    assert(T.sold(2) == 13);
    assert(T.sold(2, 2) == 13);
    assert(T.sold(1, 2) == 45);
}

int main() {
    test1();
}
