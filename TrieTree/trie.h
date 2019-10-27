#include <string>
#include <unordered_map>
#include <iostream>

class TrieNode {
    char v;
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isTerminated;
public:
    TrieNode(): isTerminated(false) {}
    TrieNode(const char &c): v(c), isTerminated(false) {}
    void addString(const std::string &s) {
        if (!children.count(s[0])) children[s[0]] = std::make_shared<TrieNode>(s[0]);

        if (s.size() > 1) children[s[0]]->addString(s.substr(1));
        else children[s[0]]->setIsTeminated();
    }
    void setIsTeminated() {
        isTerminated = true;
    }
    bool getIsTerminated() const {
        return isTerminated;
    }
    std::shared_ptr<TrieNode> getNext(const char &c) const {
        if (children.count(c) == 0) return nullptr;
        return children.at(c);
    }
};

class Trie {
    std::shared_ptr<TrieNode> root;
public:
    Trie(): root(std::make_shared<TrieNode>()) {}
    void addString(const std::string &s) {
        root->addString(s);
    }
    bool find(const std::string &s) const {
        std::shared_ptr<TrieNode> node = root;
        for (const char &c: s) {
            node = node->getNext(c);
            if (node == nullptr) return false;
        }
        return node->getIsTerminated();
    }
};
