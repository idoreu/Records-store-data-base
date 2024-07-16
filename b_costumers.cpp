
#include "b_costumers.h"
#include <memory>
  
B_Costumers::B_Costumers(int c_id) : tree(new B_Node(c_id))
{
    tree->c_id = c_id;
}
//expence
int B_Costumers::getHeight(B_Node* node)
{
    return node->height;
}
void B_Costumers::updateHeight(B_Node* node)
{
    if(node == nullptr){
        return;
    }
    if(node->right_child == nullptr){
        if(node->left_child == nullptr){
            node->height = 0;
        } else {
            node->height = node->left_child->height +1;
        }
    } else {
        if(node->left_child == nullptr){
            node->height = node->right_child->height +1;
        } else {
            int temp = std::max(node->right_child->height, node->left_child->height);
            node->height = temp +1;
        }
    }
}

void B_Costumers::updateRank(B_Node* node)
{
    if(node == nullptr){
        return;
    }
    if(node->right_child == nullptr){
        if(node->left_child == nullptr){
            return;
        } else {
            node->rank = node->left_child->rank +1;
        }
    } else {
        if(node->left_child == nullptr){
            node->rank = node->right_child->rank +1;
        } else {
            int temp = node->right_child->rank;
            temp += node->left_child->rank;
            node->rank = temp +1;
        }
    }
}

int B_Costumers::balcanceFactor(B_Node* node)
{
    if(node == nullptr){
        return 0;
    }
    if(node->left_child == nullptr && node->right_child == nullptr){
        return 0;
    }else if(node->left_child == nullptr && node->right_child != nullptr){
        return -1*(node->right_child->height);
    }else if(node->left_child != nullptr && node->right_child == nullptr){
        return node->left_child->height;
    } else{
        return node->left_child->height - node->right_child->height;
    }
}

// Rotation
std::unique_ptr<B_Node> B_Costumers::rotateRightRight(std::unique_ptr<B_Node>& node)
{
    std::unique_ptr<B_Node> newRoot = std::move(node->right_child);
    node->right_child = std::move(newRoot->left_child);
    newRoot->left_child = std::move(node);
    
    updateHeight(node.get());
    updateRank(node.get());
    updateHeight(newRoot.get());
    updateRank(newRoot.get());
    return std::move(newRoot);
}

std::unique_ptr<B_Node> B_Costumers::rotateRightLeft(std::unique_ptr<B_Node>& node)
{
    std::unique_ptr<B_Node> temp1 = std::move(node->right_child);
    std::unique_ptr<B_Node> temp2 = std::move(temp1->left_child);
    //
    node->right_child = std::move(temp2->left_child);
    temp2->left_child = std::move(node);
    temp1->left_child = std::move(temp2->right_child);
    temp2->right_child = std::move(temp1);
    
    updateHeight(node.get());
    updateRank(node.get());
    updateHeight(temp1.get());
    updateRank(temp1.get());
    updateHeight(temp2.get());
    updateRank(temp2.get());

    return std::move(temp2);
}

std::unique_ptr<B_Node> B_Costumers::rotateLeftLeft(std::unique_ptr<B_Node>& node)
{
    std::unique_ptr<B_Node> newRoot = std::move(node->left_child);
    node->left_child = std::move(newRoot->right_child);
    newRoot->right_child = std::move(node);
    // Correcting node's members
    updateHeight(node.get());
    updateRank(node.get());
    updateHeight(newRoot.get());
    updateRank(newRoot.get());
    //
    return std::move(newRoot);
}

std::unique_ptr<B_Node> B_Costumers::rotateLeftRight(std::unique_ptr<B_Node>& node)
{
    std::unique_ptr<B_Node> temp1 = std::move(node->left_child);
    std::unique_ptr<B_Node> temp2 = std::move(temp1->right_child);
    //
    node->right_child = std::move(temp2->right_child);
    temp2->left_child = std::move(node);
    temp1->left_child = std::move(temp2->left_child);
    temp2->right_child = std::move(temp1);
    
    updateHeight(node.get());
    updateRank(node.get());
    updateHeight(temp1.get());
    updateRank(temp1.get());
    updateHeight(temp2.get());
    updateRank(temp2.get());

    return std::move(temp2);
}

//assuming node is the lowest node in the tree that is not balanced
std::unique_ptr<B_Node> B_Costumers::rotate(std::unique_ptr<B_Node>& node)
{
    if(-2 < balcanceFactor(node.get()) && balcanceFactor(node.get()) < 2){
        return std::move(node);
    }
    if(balcanceFactor(node->right_child.get()) > 1){
        if(balcanceFactor(node->left_child.get()) >= 0){
            return std::move(rotateLeftLeft(node));
        } else {
            return std::move(rotateLeftRight(node));
        }
    } else {
        if(balcanceFactor(node->right_child.get()) <=0){
            return std::move(rotateRightRight(node));
        } else {
            return std::move(rotateRightLeft(node));
        }
    }
}

std::unique_ptr<B_Node> B_Costumers::balanceTree(std::unique_ptr<B_Node>& tree)
{
    if(tree == nullptr){
        return std::move(tree);
    }
    if(balcanceFactor(tree.get()) > 1){
        int check = balcanceFactor(tree->left_child.get());
        if(check > 1 || check < -1){
            tree->left_child = std::move(balanceTree(tree->left_child));
            return std::move(balanceTree(tree));// this is for balancing after a deletion of a node
        } else {
            return std::move(rotate(tree));
        }
    } else if(balcanceFactor(tree.get()) < -1){
        int check = balcanceFactor(tree->right_child.get());
        if(check > 1 || check < -1){
            tree->right_child = std::move(balanceTree(tree->right_child));
            return std::move(balanceTree(tree));
        } else {
            return std::move(rotate(tree));
        }
    }
    return std::move(tree);
}
// Data insertion

bool B_Costumers::exist(B_Node* tree, int c_id) const
{
    if(tree == nullptr){
        return false;
    }
    if(tree->c_id < c_id){
        if(tree->right_child == nullptr){
            return false;
        }
        return exist(tree->right_child.get(), c_id);
    } else if( tree->c_id > c_id){
        if(tree->left_child == nullptr){
            return false;
        }
        return exist(tree->left_child.get(), c_id);
    }
    return true;
}

B_Node* seekFather(B_Node* tree, int c_id)
{
    if(tree == nullptr){
        return nullptr;
    }
    if(tree->c_id < c_id){
        if(tree->right_child == nullptr){
            return tree;
        } else {
            if(tree->right_child->c_id == c_id){
                return tree;
            }
            return seekFather(tree->right_child.get(), c_id);
        }
    } else if(tree->c_id > c_id){
        if(tree->left_child->c_id == c_id){
            return tree;
        }
        return seekFather(tree->left_child.get(), c_id);
    }
    throw(B_Costumers::B_exeption(B_Costumers::B_exeption::FAILURE));
}

void B_Costumers::addNode(int c_id)
{
    if(tree != nullptr && tree->c_id == 0){
        tree->c_id = c_id;
        return;
    }
    std::unique_ptr<B_Node> newNode(new B_Node(c_id));
    newNode->c_id = c_id;
    B_Node* current = tree.get();
    while (current != nullptr)
    {
        if(current->c_id < c_id){
            if(!current->right_child){
                current->right_child = std::move(newNode);
                current->height +=1;
                tree = std::move(balanceTree(tree));
                return;
            } else {
                current = current->right_child.get();
            }
        } else if(current->c_id > c_id){
            if(!current->left_child){
                current->left_child = std::move(newNode);
                current->height +=1;
                tree = std::move(balanceTree(tree));
                return;
            } else {
                current = current->left_child.get();
            }
        } else {
            break;
        }
    }
    tree = std::move(balanceTree(tree));
    return;
}

// the identifier c_id doesn't need to be in the tree
int B_Costumers::getRank(B_Node* tree, int c_id)
{
    if(tree == nullptr){
        return 0;
    }
    if(tree->c_id == c_id){
        if(tree->right_child == nullptr){
            return tree->rank;
        }
        return tree->rank - tree->right_child->rank;
    }
    if(tree->c_id < c_id){
        int sum = 1;
        if(tree->left_child != nullptr){
            sum = tree->left_child->rank +1;
        }
        if(tree->right_child != nullptr){
            B_Node* right = tree->right_child.get();
            return sum += getRank(right, c_id);
        }
        return sum;
    }
    if(tree->left_child != nullptr){
        B_Node* left = tree->left_child.get();
        return getRank(left, c_id);
    }
    return 0;
}

void B_Costumers::addPrize(B_Node* tree, double amount, int rank, bool rightChild)
{
    if(tree == nullptr){
        return;
    }
    if(tree->rank < rank){ // turn right
        int temp = rank - getRank(tree, tree->c_id);
        if(!rightChild){
            tree->extra += amount;
        }
        if(tree->right_child != nullptr){
            addPrize(tree->right_child.get(), amount, temp, true);
        }
        return;
    } else {
        if(tree->rank > rank){
            if(rightChild){
                tree->extra -= amount;
            }
            if(tree->left_child != nullptr){
                addPrize(tree->left_child.get(), amount, rank, false);
            }
            return;
        }
    }
    if(!rightChild){
        tree->extra += amount;
    }
    if(tree->right_child != nullptr){
        tree->right_child->extra -= amount;
    }
    return;
}

void B_Costumers::addExpences(int c_id, int amount)
{
    if(tree == nullptr){
        throw(B_Costumers::B_exeption(B_exeption::FAILURE));
    } 
    if(!exist(tree.get(), c_id)){
        throw(B_Costumers::B_exeption(B_exeption::FAILURE));
    }
    try{ 
        B_Node* father = seekFather(tree.get(), c_id);
        if(father->c_id < c_id){
            father->right_child->expence += amount;
        } else {
            father->left_child->expence += amount;
        }
    }
    catch(B_exeption& e){
        tree->expence += amount;
        tree->expence +=0;
    }
}

void B_Costumers::givePrizes(int c_id1, int c_id2, double amount)
{
    if(c_id1 == c_id2 || tree == nullptr){
        return;
    }
    int rank2 = getRank(tree.get(), c_id2);
    int rank1 = getRank(tree.get(), c_id1);
    if(rank2 > 0){
        if(!exist(tree.get(), c_id2)){
            addPrize(tree.get(), -amount, rank2, false);
        } else {
            addPrize(tree.get(), -amount, rank2-1, false);
        }
    }
    if(rank1 >= 1){
        if(!exist(tree.get(), c_id1)){
            addPrize(tree.get(), amount, rank1, false);
        } else {
            addPrize(tree.get(), amount, rank1-1, false);
        }
    }
}

double B_Costumers::expences(B_Node* tree, int c_id) const
{
    if(tree == nullptr){
        return 0.0;
    }
    double sum = tree->extra;
    if(tree->c_id < c_id){
        if(tree->right_child == nullptr){
            return 0.0;
        }
        return sum + expences(tree->right_child.get(), c_id);
    } else if( tree->c_id > c_id){
        if(tree->left_child == nullptr){
            return 0.0;
        }
        return sum += expences(tree->left_child.get(), c_id);
    } else {
        return sum+ tree->expence;
    }
}

double B_Costumers::getExpences(int c_id) const
{
    if(tree == nullptr){
        return 0.0;
    }
    double res = expences(tree.get(), c_id);
    if(res <=0.0){
        return 0.0;
    }
    return res;
}

void eraseExpences(B_Node* tree)
{
    if(tree == nullptr){
        return;
    }
    if(tree->left_child != nullptr){
        eraseExpences(tree->left_child.get());
    }
    tree->extra = 0;
    tree->expence = 0;
    if(tree->right_child != nullptr){
        eraseExpences(tree->right_child.get());
    }
}

void B_Costumers::newMonth()
{
    if(tree== nullptr){
        return;
    }
    eraseExpences(tree.get());
}