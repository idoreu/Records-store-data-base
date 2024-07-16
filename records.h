#ifndef RECORDS_H_
#define RECORDS_H_

#include <memory>

struct Record_Node {
    int copies;
    int column;
    int height;
    int purchased;
    int relativeHeight;
    int towerHight;
    std::shared_ptr<Record_Node> father;

    Record_Node(int copies = 0, int column = 0) : copies(copies), column(column), height(0), purchased(0),
             relativeHeight(0), towerHight(copies), father(nullptr) {}
    Record_Node(Record_Node const& node);
    Record_Node& operator=(const Record_Node&) = delete;
};

class Records {
    private:
    int r_num;
    std::shared_ptr<Record_Node>* records;
    public:
    Records(int size= 1);
    Records(const Records&);
    Records& operator=(const Records&) = delete;

    class recordsException {
        private:
        int problem;
        public:
        static const int SUCCESS          = 0;
        static const int ALLOCATION_ERROR = 1;
        static const int INVALID_INPUT    = 2;
        static const int FAILURE          = 3;
        static const int ALREADY_EXISTS   = 4;
        static const int DOESNT_EXISTS    = 5;
        recordsException(int p = 3) : problem(p){}
        int what(){return problem;}
    };
    int getRecordNum();
    void stockRecords(int *recordStocks, int r_num);
    int getPurchase(int r_id);
    int findColumn(int r_id);
    int getHeight(int r_id);
    void unionColumn(int column1, int column2);
    int addPurcahse(int r_id);
};


#endif