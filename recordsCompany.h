#ifndef RECORDS_COMPANY_H
#define RECORDS_COMPANY_H

#include "utilesWet2.h"
#include "costumers.h"
#include "records.h"
#include "b_costumers.h"
#include <memory>



class RecordsCompany {
  private:
  static const int INITIAL_SIZE = 2;
  std::unique_ptr<Costumers> costumers;
  std::unique_ptr<Records> records;
  std::unique_ptr<B_Costumers> b_costumers;
  public:
    RecordsCompany();
    ~RecordsCompany() = default;
    StatusType newMonth(int *records_stocks, int number_of_records);
    StatusType addCostumer(int c_id, int phone);
    Output_t<int> getPhone(int c_id);
    StatusType makeMember(int c_id);
    Output_t<bool> isMember(int c_id);
    StatusType buyRecord(int c_id, int r_id);
    StatusType addPrize(int c_id1, int c_id2, double  amount);
    Output_t<double> getExpenses(int c_id);
    StatusType putOnTop(int r_id1, int r_id2);
    StatusType getPlace(int r_id, int *column, int *hight);
};

#endif