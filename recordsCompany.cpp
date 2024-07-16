
#include "recordsCompany.h"
#include "utilesWet2.h"
#include "costumers.h"
#include "records.h"
#include "b_costumers.h"
#include <memory>

RecordsCompany::RecordsCompany() : costumers(new Costumers()), records(new Records()), b_costumers(new B_Costumers()) {}

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records)
{
    if(number_of_records < 0){
        return StatusType::INVALID_INPUT;
    }
    try{
        std::unique_ptr<Records> newRecords(new Records(number_of_records));
        newRecords->stockRecords(records_stocks, number_of_records);
        b_costumers->newMonth();
        if(newRecords != nullptr){
            records = std::move(newRecords);
        } else{
            return StatusType::ALLOCATION_ERROR;
        }
    }
    catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addCostumer(int c_id, int phone)
{
    if(c_id < 0 || phone < 0){
        return StatusType::INVALID_INPUT;
    }else if(costumers->exist(c_id)){
        return StatusType::ALREADY_EXISTS;
    }
    try{
        costumers->insert(c_id, phone);
    }
    catch(std::bad_alloc){
        return StatusType::ALLOCATION_ERROR;
    }
    catch(...){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

Output_t<int> RecordsCompany::getPhone(int c_id)
{
    if(c_id < 0){
        return Output_t<int>(StatusType::INVALID_INPUT);
    } else if(!costumers->exist(c_id)){
        return StatusType::DOESNT_EXISTS;
    }
    int phone;
    try{
        phone = costumers->getPhone(c_id);
    }
    catch(...){
        return StatusType::FAILURE;
    }
    return Output_t<int>(phone);
}

StatusType RecordsCompany::makeMember(int c_id)
{
    if(c_id < 0){
        return StatusType::INVALID_INPUT;
    }else if(!costumers->exist(c_id)){
        return StatusType::DOESNT_EXISTS;
    }
    if(costumers->isMember(c_id)){
        return StatusType::ALREADY_EXISTS;
    }
    try{
        costumers->makeMember(c_id);
        b_costumers->addNode(c_id);
    }
    catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

Output_t<bool> RecordsCompany::isMember(int c_id)
{
    if(c_id < 0){
        return Output_t<bool>(StatusType::INVALID_INPUT);
    } else if(!costumers->exist(c_id)){
        return Output_t<bool>(StatusType::DOESNT_EXISTS);
    }
    const bool ans = costumers->isMember(c_id);
    return Output_t<bool>(ans);
}

StatusType RecordsCompany::buyRecord(int c_id, int r_id)
{
    if(c_id < 0 || r_id < 0){
        return StatusType::INVALID_INPUT;
    }else if(!costumers->exist(c_id)){
        return StatusType::DOESNT_EXISTS;
    } else if(records->getRecordNum() <= r_id){
        return StatusType::DOESNT_EXISTS;
    }
    try{
        int t =  records->addPurcahse(r_id);
        b_costumers->addExpences(c_id, 100 + t);
    }
    catch(Records::recordsException& e){
        return StatusType::DOESNT_EXISTS;
    }
    catch(...){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount)
{
    if(c_id1 < 0 || c_id2 < 0 || amount <= 0){
        return StatusType::INVALID_INPUT;
    }
    b_costumers->givePrizes(c_id1, c_id2, amount);
    return StatusType::SUCCESS;
}

Output_t<double> RecordsCompany::getExpenses(int c_id)
{
    if(c_id < 0){
        return StatusType::INVALID_INPUT;
    }else if(!costumers->exist(c_id)){
        return StatusType::DOESNT_EXISTS;
    } else if(!costumers->isMember(c_id)){
        return StatusType::DOESNT_EXISTS;
    }
    const double res = b_costumers->getExpences(c_id);
    return Output_t<double>(res);
}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2)
{
    if(r_id1 < 0 || r_id2 < 0){
        return StatusType::INVALID_INPUT;
    }else if(records->getRecordNum() <= r_id1 || records->getRecordNum() <= r_id2){
        return StatusType::DOESNT_EXISTS;
    }
    try{
        if(records->findColumn(r_id1) == records->findColumn(r_id2)){
            return StatusType::FAILURE;
        }
        records->unionColumn(records->findColumn(r_id1), records->findColumn(r_id2));
    }
    catch(Records::recordsException& e){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight)
{
    if(r_id < 0 || column == NULL || hight == NULL){
        return StatusType::INVALID_INPUT;
    }else if(records->getRecordNum() <= r_id){
        return StatusType::DOESNT_EXISTS;
    }
    try {
        int hightResult = records->getHeight(r_id);
        int columnResault = records->findColumn(r_id);
        *hight = hightResult;
        *column = columnResault;
    }
    catch(...){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}