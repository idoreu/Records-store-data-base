
#include <cstdio>
#include "costumers.h"
#include <memory>

Costumers::Costumers(int size, int c_num): arrSize(size), c_num(c_num), array(new std::unique_ptr<ArrayCell>[size])
{
    for(int i = 0; i< arrSize; ++i){
        array[i].reset(new ArrayCell());
    }
}

bool Costumers::exist(int c_id)
{
    if(c_id < 0) {
        return false;
    }
    int key = hashing(c_id);
    if(array[key] == nullptr){
        return false;
    }else if( array[key]->c_id < 0 || array[key]->phone < 0){
        return false;
    }
    if(array[key]->c_id == c_id){
        return true;
    }
    ArrayCell* current = array[key].get();
    while(current->next != nullptr){
        current = current->next.get();
        if(current->c_id == c_id){
            return true;
        }
    }
    return false;
}
// assume c_id is in the hash-table
int Costumers::getPhone(int c_id) const
{
    int key = hashing(c_id);
    if(array[key] == nullptr){
        throw(Costumers::CostumersException(CostumersException::FAILURE));
    }else if( array[key]->c_id < 0 || array[key]->phone < 0){
        throw(Costumers::CostumersException(CostumersException::FAILURE));
    }
    if(array[key]->c_id == c_id){
        return array[key]->phone;
    }
    ArrayCell* current = array[key].get();
    while(current->next != nullptr){
        current = current->next.get();
        if(current->c_id == c_id){
            return current->phone;
        }
    }
    throw(Costumers::CostumersException(CostumersException::FAILURE));
}
// assume c_id is in the hash-table
bool Costumers::isMember(int c_id) const
{
    if(c_id < 0){
        return false;
    }
    int key = hashing(c_id);
    if(array[key]->c_id == c_id){
        return array[key]->member;
    }
    ArrayCell* current = array[key].get();
    while(current->next != nullptr){
        current = current->next.get();
        if(current->c_id == c_id){
            return current->member;
        }
    }
    return false;
}


void Costumers::insert(int c_id, int phone)
{
    if(c_id < 0 || phone < 0){
        return;
    }
    if(c_num + 1 >= arrSize/2){
        doubleSize();
    }
    int key = hashing(c_id);
    std::unique_ptr<ArrayCell> newNode(new ArrayCell(c_id, phone));
    if(array[key] == nullptr){
        array[key] = std::move(newNode);
    }else if(array[key]->c_id<0 || array[key]->phone< 0){
        array[key] = std::move(newNode);
    } else if(array[key]->next == nullptr){
        array[key]->next = std::move(newNode);
    } else {
        ArrayCell* current = array[key].get();
        while (current->next != nullptr)
        {
            current = current->next.get();
        }
        current->next = std::move(newNode);
    }
    c_num+=1;
}

void Costumers::makeMember(int c_id)
{
    if(c_id < 0) {
        return;
    }
    int key = hashing(c_id);
    if(array[key] == nullptr){
        throw(Costumers::CostumersException(CostumersException::FAILURE));
    }
    if(array[key]->c_id == c_id){
        array[key]->member = true;
        return;
    }
    ArrayCell* current = array[key].get();
    while (current->next != nullptr)
    {
        current = current->next.get();
        if(current->c_id == c_id){
            current->member = true;
            return;
        }
    }
    throw(Costumers::CostumersException(CostumersException::FAILURE));
}

void Costumers::doubleSize()
{
    int newCapacity = arrSize * SMALL_PRIME;
    std::unique_ptr<ArrayCell>* tempArray = new std::unique_ptr<ArrayCell>[newCapacity];
    for(int i = 0; i < newCapacity; ++i){
        tempArray[i].reset(new ArrayCell());
    }
    int len = arrSize;
    arrSize = newCapacity;
    for(int i = 0; i < len; ++i){
        if(array[i] == nullptr){
            continue;
        } else if(array[i]){
            try{
                if(array[i]->c_id < 0 || array[i]->phone < 0){
                    (void)0;
                }
            }
            catch(...){
                continue;
            }
        } else if(array[i]->c_id >=0  && array[i]->phone >= 0){
        //if(array[i]->c_id >= 0 && array[i]->phone >= 0){
            int newKey = hashing(array[i]->c_id);
            if(tempArray[newKey] == nullptr){
                tempArray[newKey] = std::move(array[i]);
            } else if(tempArray[newKey]->c_id < 0 || tempArray[newKey]->phone < 0){
                tempArray[newKey]->c_id = array[i]->c_id;
                tempArray[newKey]->phone = array[i]->phone;
                tempArray[newKey]->member = array[i]->member;
            } else {
                ArrayCell* tempCurrent = tempArray[newKey].get();
                while (tempCurrent->next != nullptr)
                {
                    tempCurrent = tempCurrent->next.get();
                }
                tempCurrent->c_id = array[i]->c_id;
                tempCurrent->phone = array[i]->phone;
                tempCurrent->member = array[i]->member;
            }
            //
            while(array[i]->next != nullptr){
                std::unique_ptr<ArrayCell> temp = std::move(array[i]->next);
                if(temp->c_id < 0 || temp->phone < 0 || temp == nullptr){
                    continue;
                }
                int tempKey = hashing(temp->c_id);
                if(tempArray[tempKey] == nullptr){
                    tempArray[tempKey] = std::move(temp);
                } else if(tempArray[tempKey]->c_id < 0 || tempArray[tempKey]->phone < 0){
                    tempArray[tempKey]->c_id = temp->c_id;
                    tempArray[tempKey]->phone = temp->phone;
                    tempArray[tempKey]->member = temp->member;
                } else {
                    ArrayCell* tempCurrent = tempArray[tempKey].get();
                    while (tempCurrent->next != nullptr)
                    {
                        tempCurrent = tempCurrent->next.get();
                    }
                    tempCurrent->c_id = temp->c_id;
                    tempCurrent->phone = temp->phone;
                    tempCurrent->member = temp->member;
                }
            }
        }
    }
    delete []array;
    array = tempArray;
}
int Costumers::hashing(int c_id) const
{
    c_id *= MEDIUM_PRIME;
    return (c_id % arrSize);
}