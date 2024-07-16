
#ifndef B_COSTUMERS_H_
#define B_COSTUMERS_H_

#include <memory>

struct B_Node {
    int c_id;
    int rank;
    int height;
    int expence;
    double extra;
    std::unique_ptr<B_Node> right_child;
    std::unique_ptr<B_Node> left_child;
    B_Node(int c_id = 0) : c_id(c_id), rank(1), height(0), expence(0), extra(0), right_child(nullptr), left_child(nullptr) {}
};

class B_Costumers {
    private:
    std::unique_ptr<B_Node> tree;
    public:
    B_Costumers(int c_id = 0);
    B_Costumers& operator=(const B_Costumers&) = delete;
    B_Costumers(const B_Costumers&) = delete;
    //Utilities for AVL
    int getHeight(B_Node* node);
    int getRank(B_Node* tree, int c_id);
    void updateHeight(B_Node* node);
    void updateRank(B_Node* node);
    int balcanceFactor(B_Node* node);
    // Rotation
    std::unique_ptr<B_Node> rotateRightRight(std::unique_ptr<B_Node>& node);
    std::unique_ptr<B_Node> rotateRightLeft(std::unique_ptr<B_Node>& node);
    std::unique_ptr<B_Node> rotateLeftLeft(std::unique_ptr<B_Node>& node);
    std::unique_ptr<B_Node> rotateLeftRight(std::unique_ptr<B_Node>& node);
    std::unique_ptr<B_Node> rotate(std::unique_ptr<B_Node>& node);
    std::unique_ptr<B_Node> balanceTree(std::unique_ptr<B_Node>& node);
    //
    bool exist(B_Node* tree, int c_id) const;
    void addNode(int c_id);
    double expences(B_Node* tree, int c_id) const;
    void addPrize(B_Node* tree, double amount, int rank, bool wentRight);
    void addExpences(int c_id, int amount);
    void newMonth();
    // Exception
    class B_exeption {
        private:
        int const problem;
        public:
        static const int SUCCESS          = 0;
        static const int ALLOCATION_ERROR = 1;
        static const int INVALID_INPUT    = 2;
        static const int FAILURE          = 3;
        static const int ALREADY_EXISTS   = 4;
        static const int DOESNT_EXISTS    = 5;
        B_exeption(const int p) :problem(p){}
        int what(){return problem;}
    };
    void givePrizes(int c_id1, int c_id2, double amount);
    double getExpences(int c_id) const;
};

#endif