#ifndef COSTUMERS_H_
#define COSTUMERS_H_

#include <memory>

struct ArrayCell {
    int c_id;
    int phone;
    bool member;
    std::unique_ptr<ArrayCell> next;
    ArrayCell(int c_id = -1, int phone = -1) : c_id(c_id), phone(phone), member(false), next(nullptr) {}
    ArrayCell(ArrayCell&);
    ArrayCell& operator=(ArrayCell&);

};

class Costumers {
    private:
    int arrSize;
    int c_num;
    std::unique_ptr<ArrayCell>* array;
    //std::unique_ptr<ArrayCell[]> array;
    static const int INITIAL_SIZE = 4357;
    static const int SMALL_PRIME = 59;
    static const int MEDIUM_PRIME = 47;
    static const int PRIME = 111111;
    public:

    Costumers(int size = INITIAL_SIZE, int c_num = 1);
    Costumers(const Costumers&) = delete;
    Costumers& operator=(const Costumers&) = delete;
    //
    int getPhone(int c_id) const;
    bool isMember(int c_id) const;
    //
    bool exist(int c_id);
    void makeMember(int c_id);
    void insert(int c_id, int phone);
    //
    class Iterator {};
    //
    int hashing(int c_id) const;
    void doubleSize();


    class CostumersException {
        private:
        int problem;
        public:
        static const int SUCCESS          = 0;
        static const int ALLOCATION_ERROR = 1;
        static const int INVALID_INPUT    = 2;
        static const int FAILURE          = 3;
        static const int ALREADY_EXISTS   = 4;
        static const int DOESNT_EXISTS    = 5;
        CostumersException(int p = 3) : problem(p){}
        int what(){return problem;}
    };
};



#endif