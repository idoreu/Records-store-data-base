
#include "records.h"
#include <memory>

Records::Records(int size): r_num(size)
{
    records = new std::shared_ptr<Record_Node>[size];
}

Record_Node::Record_Node(Record_Node const& node): copies(node.copies), column(node.column), height(node.height), purchased(node.purchased),
            relativeHeight(node.relativeHeight)
{
    if(node.father == nullptr){
        father = nullptr;
    } else {
        father = std::move(node.father);
    }
}

Records::Records(const Records& other) : r_num(other.r_num)
{
    records = new std::shared_ptr<Record_Node>[other.r_num];
    for(int i = 0; i < r_num; ++i){
        records[i] = std::move(other.records[i]);
    }
}

void Records::stockRecords(int *recordStocks, int r_num)
{
    for(int i = 0; i < r_num; ++i){
        std::shared_ptr<Record_Node> record(new Record_Node(recordStocks[i], i));
        records[i] = record;
    }
}

int Records::getPurchase(int r_id)
{
    return records[r_id]->purchased;
}

void Records::unionColumn(int column1, int column2)
{
    if(column1 == column2){
        return;
    }
    int find1 = findColumn(column1);
    int find2 = findColumn(column2);
    if(records[find1] == nullptr || records[find2] == nullptr){
        throw(recordsException(recordsException::FAILURE));
    }
    Record_Node* current = records[find1].get();
    current->height += records[find2]->towerHight;
    current->relativeHeight = current->height - records[find2]->height;
    records[find2]->towerHight += current->towerHight;
    current->father = records[find2];
}

int Records::findColumn(int r_id)
{
    if(records[r_id] == nullptr){
        throw(recordsException(recordsException::FAILURE));
    }
    if(records[r_id]->father == nullptr){
        return records[r_id]->column;
    }
    //
    Record_Node* current = records[r_id].get();
    int relative = 0;
    while (current->father != nullptr)
    {
        relative += current->relativeHeight;
        if(current->father->father == nullptr){
            break;
        }
        current = current->father.get();
    }
    relative +=current->father->relativeHeight;
    records[r_id]->relativeHeight = relative;
    records[r_id]->father = current->father;
    //
    Record_Node* iterator = records[r_id].get();
    findColumn(iterator->father->column);
    return current->father->column;
}

int Records::getHeight(int r_id)
{
    if(records[r_id] == nullptr){
        throw(recordsException(recordsException::FAILURE));
    }
    Record_Node* current = records[r_id].get();
    int relative = 0;
    while(current->father != nullptr){
        relative += current->relativeHeight;
        current = current->father.get();
    }
    return current->height + relative;
}

int Records::getRecordNum()
{
    return r_num;
}

int Records::addPurcahse(int r_id)
{
    if(records[r_id]->copies <=1){
        records[r_id]->copies = 0;
    }
    if(records[r_id]->height >= 1){
        records[r_id]->height -=1;
        records[r_id]->relativeHeight -=1;
    }
    int purchased = records[r_id]->purchased;
    records[r_id]->purchased += 1;
    return (purchased);
}
