#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <queue>

using Place = size_t;

struct Map {
    size_t places;
    Place start, end;
    std::vector<std::pair<Place, Place> > connections;
    std::vector<std::vector<Place> > items;
};

template<typename F, typename S>
struct std::hash<std::pair<F, S> > {
    std::size_t operator()(const std::pair<F, S> &p) const noexcept {
        // something like boost::combine would be much better
        return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
    }
};

struct Node {
    Place m_place;
    unsigned int m_bits;
    unsigned int m_distance;
    std::list<Place> m_path;

    Node(const Place place, const unsigned int bites, const unsigned int distance, const std::list<Place> &path) {
        this->m_place = place;
        this->m_bits = bites;
        this->m_distance = distance;
        m_path = path;
        m_path.push_back(m_place);
    }
};

bool nodeInitial(std::vector<std::vector<size_t> > &m_distance, std::queue<Node> &m_queue, const Map &map,
                 const unsigned int m_types, const std::vector<unsigned int> &m_itm) {
    if (map.start == map.end) {
        if (m_itm[map.start] == m_types - 1) {
            return false;
        }
    }
    m_distance[map.start][m_itm[map.start]] = 0;
    m_queue.push(Node(map.start, m_itm[map.start], 0, {}));
    return true;
}

std::list<Place> bfs(const std::vector<std::vector<Place> > &m_graph, const Map &map,
                     const std::vector<unsigned int> &m_itm) {
    std::queue<Node> m_queue;
    const unsigned int m_types = 1 << map.items.size();
    std::vector<std::vector<size_t> > m_distance(m_graph.size());
    for (auto &d: m_distance) {
        d.resize(m_types, INT32_MAX);
    }

    if (!nodeInitial(m_distance, m_queue, map, m_types, m_itm)) {
        return {map.start};
    }

    while (!m_queue.empty()) {
        Node node = m_queue.front();
        m_queue.pop();

        for (auto &i: m_graph[node.m_place]) {
            unsigned int bits = node.m_bits;
            bits = bits | m_itm[i];
            if (const size_t newDistance = node.m_distance + 1; newDistance < m_distance[i][bits]) {
                m_distance[i][bits] = newDistance;
                auto newNode = Node(i, bits, newDistance, node.m_path);
                m_queue.push(newNode);
                if (i == map.end && bits == m_types - 1) {
                    return newNode.m_path;
                }
            }
        }
    }
    return {};
}

void addEdges(std::vector<std::vector<Place> > &m_graph, const Map &map, std::vector<unsigned int> &m_itm) {
    for (const auto &[fst, snd]: map.connections) {
        m_graph[fst].push_back(snd);
        m_graph[snd].push_back(fst);
    }
    for (size_t i = 0; i < map.items.size(); i++) {
        for (auto &j: map.items[i]) {
            m_itm[j] = (m_itm[j] | (1 << i));
        }
    }
}

std::list<Place> find_path(const Map &map) {
    std::vector<std::vector<Place> > m_graph(map.places);
    std::vector<unsigned int> m_itm(map.places);
    addEdges(m_graph, map, m_itm);
    if (m_graph.empty())
        return {};
    return bfs(m_graph, map, m_itm);
}

using TestCase = std::pair<size_t, Map>;

const std::array examples = {

    TestCase{
        1, Map{
            2, 0, 0,
            {{0, 1}},
            {{0}}
        }
    },
    TestCase{
        3, Map{
            2, 0, 0,
            {{0, 1}},
            {{1}}
        }
    },
    TestCase{
        3, Map{
            4, 0, 1,
            {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
            {}
        }
    },
    TestCase{
        4, Map{
            4, 0, 1,
            {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
            {{2}}
        }
    },
    TestCase{
        0, Map{
            4, 0, 1,
            {{0, 2}, {2, 3}, {0, 3}, {3, 1}},
            {{2}, {}}
        }
    },
    TestCase{
        0, Map{
            3, 0, 2,
            {{1, 0}, {0, 1}},
            {}
        }
    },
    TestCase{
        0, Map{
            3, 0, 2,
            {{1, 0}, {0, 1}},
            {{2}}
        }
    },
    TestCase{
        0, Map{
            3, 0, 2,
            {{1, 0}, {0, 1}},
            {{1}}
        }
    },
    TestCase{
        0, Map{
            3, 0, 2,
            {{1, 0}, {0, 1}},
            {{1}, {0, 1, 2}}
        }
    },
    TestCase{
        2, Map{
            4, 2, 3,
            {{1, 0}, {2, 3}},
            {{3}}
        }
    },
    TestCase{
        2, Map{
            4, 2, 3,
            {{1, 0}, {2, 3}},
            {{3, 2}}
        }
    },
    TestCase{
        2, Map{
            4, 2, 3,
            {{1, 0}, {2, 3}},
            {{3, 3}}
        }
    },
    TestCase{
        2, Map{
            4, 2, 3,
            {{1, 0}, {2, 3}},
            {{3, 3}, {3}}
        }
    },
    TestCase{
        1, Map{
            1, 0, 0,
            {},
            {{0}, {0}, {0}, {0}, {0}, {0}}
        }
    },
    TestCase{
        1, Map{
            1, 0, 0,
            {},
            {
                {0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0}, {0}, {0},
                {0, 0, 0, 0, 0}
            }
        }
    },
    TestCase{
        0, Map{
            8, 3, 7,
            {{0, 1}, {1, 2}, {0, 2}, {0, 5}, {2, 3}, {5, 6}},
            {{0}, {1}, {2}, {3}, {5}, {0}}
        }
    },
    TestCase{
        6, Map{
            8, 3, 0,
            {{0, 1}, {1, 2}, {0, 2}, {0, 5}, {2, 3}, {5, 6}},
            {{0}, {1}, {2}, {3}, {5}, {0}}
        }
    },
    TestCase{
        3, Map{
            8, 3, 0,
            {{0, 1}, {1, 2}, {0, 2}, {0, 5}, {2, 3}, {5, 6}},
            {{0}}
        }
    },
    TestCase{
        9, Map{
            5, 0, 0,
            {{0, 1}, {0, 2}, {0, 3}, {0, 4}},
            {{0}, {1}, {2}, {3}, {4}}
        }
    },
    TestCase{
        6, Map{
            10, 0, 9,
            {
                {0, 1}, {0, 3}, {0, 4}, {0, 5}, {4, 1},
                {1, 2}, {2, 7}, {8, 7}, {8, 9}, {5, 6}, {6, 1}
            },
            {}
        }
    },

    TestCase{
        3, Map{
            3, 0, 2,
            {{0, 1}, {1, 2}},
            {{1}, {1}, {2}, {1}, {1}}
        }
    },
};

int main() {
    int fail = 0;
    for (size_t i = 0; i < examples.size(); i++) {
        std::cout << i << " --- ";
        if (auto sol = find_path(examples[i].second); sol.size() != examples[i].first) {
            std::cout << "Wrong answer for map " << i << std::endl;
            fail++;
        }
    }

    if (fail) std::cout << "Failed " << fail << " tests" << std::endl;
    else std::cout << "All tests completed" << std::endl;

    return 0;
}
