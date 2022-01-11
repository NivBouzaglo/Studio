#include <algorithm>
#include <utility>
#include "../include/Customer.h"


Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){}
std::string Customer::getName() const{ return this->name;}
int Customer::getId() const{return this->id;}

Customer::~Customer() {}

//Cheap costumer
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){}
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> output;
    output.assign(1,workout_options.front().getId());
    int min = workout_options.front().getPrice();
    for(const auto & workout_option : workout_options) {
        if (workout_option.getPrice() < min) {
            min = workout_option.getPrice();
            output.assign(1, workout_option.getId());
        }
    }
    return output;
}


std::string CheapCustomer::toString() const {
    return this->getName()+" ,chp" ;
}

//SweatyCostumer
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer( name,id){}

std::vector<int> SweatyCustomer:: order(const std::vector<Workout> &workout_options){
    std::vector<int> output;
    for (const auto & workout_option : workout_options){
        if (workout_option.getType() == CARDIO){
            output.push_back(workout_option.getId());
        }
    }
    return output;
}
std::string SweatyCustomer::toString() const {
    return  this->getName()+" ,swt";
}


//HeavyMuscelesCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name, id){}
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> output;
    std::vector<int> prices;
    prices.push_back(-1);
    output.push_back(-1);
    for (auto &workout_option: workout_options) {
        if (workout_option.getType() == ANAEROBIC) {
            int i = 0;
            while (prices[i] > workout_option.getPrice()) {
                i++;
            }
            prices.insert(prices.begin() + i, workout_option.getPrice());
            output.insert(output.begin() + i, workout_option.getId());
        }
    }
    output.pop_back();
    return output;
}
std::string HeavyMuscleCustomer::toString() const {
    return  this->getName()+" ,mcl";
}

//FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name, id){}
std::vector<int> FullBodyCustomer ::order(const std::vector<Workout> &workout_options){
    int c_price = -1 , m_price = -1 , a_price  =-1  ,c_id , m_id , a_id;
    for (const auto & workout_option : workout_options){
        if (workout_option.getType() == CARDIO ){
            if(c_price == -1) {
                c_price = workout_option.getPrice();
                c_id = workout_option.getId();
            }
            else if (workout_option.getPrice()<c_price){
                c_price = workout_option.getPrice();
                c_id = workout_option.getId();
            }
        }
        if (workout_option.getType() == MIXED){
            if(m_price == -1) {
                m_price = workout_option.getPrice();
                m_id = workout_option.getId();
            }
            else if (workout_option.getPrice()>m_price){
                m_price = workout_option.getPrice();
                m_id = workout_option.getId();
            }
        }
        if(workout_option.getType() == ANAEROBIC){
            if(a_price == -1) {
                a_price = workout_option.getPrice();
                a_id = workout_option.getId();
            }
            else if (workout_option.getPrice()<a_price){
                a_price = workout_option.getPrice();
                a_id = workout_option.getId();
            }

        }
    }
    std::vector<int> output;
    output.push_back(c_id);
    output.push_back(m_id);
    output.push_back(a_id);
    return output;
}

std::string FullBodyCustomer::toString() const {
    return  this->getName()+" ,fbd";
}
