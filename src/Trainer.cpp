#include <iostream>
#include "../include/Trainer.h"
#include <vector>


using namespace std;

Trainer::Trainer(int t_capacity):salary(0),capacity(t_capacity),open(false),customersList(std::vector<Customer*>()),orderList(std::vector<OrderPair>()){}
int Trainer::getCapacity() const{return capacity;}
void Trainer::addCustomer(Customer* customer){
    if (int(customersList.size()) == capacity) {
        std:: cout<<"This trainer is full"<<std :: endl;
    }
    else{
        if (customer->toString() == customer->getName()+" ,chp") {
            Customer *c = new CheapCustomer(customer->getName(), customer->getId());
            this->customersList.push_back(c);
        }
        else if (customer->toString() == customer->getName()+" ,swt") {
            Customer *c = new SweatyCustomer(customer->getName(), customer->getId());
            this->customersList.push_back(c);
        }
        else if (customer->toString() == customer->getName()+" ,fbd") {
            Customer *c = new FullBodyCustomer(customer->getName(), customer->getId());
            this->customersList.push_back(c);
        }
        else if (customer->toString() == customer->getName()+" ,mcl") {
            Customer *c = new HeavyMuscleCustomer(customer->getName(), customer->getId());
            this->customersList.push_back(c);
        }
    }
}
void Trainer::removeCustomer(int id) {
    for (int i = 0; i < int(customersList.size()); i++) {
        if (customersList[i]->getId() == id) {
            for (OrderPair &x: orderList) {
                if (x.first == id) {
                    x.first = -1;
                    salary = salary - x.second.getPrice();
                }
            }
            delete customersList[i];
            customersList.erase(customersList.begin() + i);
            break;
        }
    }
    if (customersList.empty()){
        open = false;
    }
}
Customer* Trainer::getCustomer(int id){
    for(auto i : customersList){
        if(i->getId()==id) {
            return i;
        }
    }
    std :: cout<<"There is no customer with this id ";
    return nullptr;
}
std::vector<Customer*>& Trainer::getCustomers(){return customersList;}
std::vector<OrderPair>& Trainer:: getOrders(){return orderList;}
void Trainer::order(const int customer_id, const std::vector<int>& workout_ids, const std::vector<Workout>& workout_options){
    for (int workout_id : workout_ids){
        OrderPair p= std::make_pair(customer_id,workout_options[workout_id]);
        orderList.push_back(p);
        salary=salary+p.second.getPrice();

    }
}
void Trainer::openTrainer(){ this->open=true;}
void Trainer::closeTrainer(){
    open=false;
    for (auto * i: customersList) {
        delete i;
    }
    customersList.clear();
    orderList.clear();
}

void Trainer::setSalary(int t_salary){salary = t_salary;}
int Trainer::getSalary() const{return salary;}
bool Trainer::isOpen(){return open;}

Trainer::Trainer(const Trainer &other) { // copy constructor
    this->capacity = other.capacity;
    this->salary = other.salary;
    this->open = other.open;
    for(auto order : other.orderList) {
        std::pair<int, Workout> t = make_pair(order.first, order.second);
        this->orderList.push_back(t);
    }
    for (auto *customer : other.customersList) {
        if (customer->toString() == customer->getName()+" ,chp") {
            Customer *c = new CheapCustomer(customer->getName(), customer->getId());
            this->customersList.push_back(c);
        }
        else if (customer->toString() == customer->getName()+" ,swt") {
            Customer *c = new SweatyCustomer(customer->getName(), customer->getId());
            this->customersList.push_back(c);
        }
        else if (customer->toString() == customer->getName()+" ,fbd") {
            Customer *c = new FullBodyCustomer(customer->getName(), customer->getId());
            this->customersList.push_back(c);
        }
        else if (customer->toString() == customer->getName()+" ,mcl") {
            Customer *c = new HeavyMuscleCustomer(customer->getName(), customer->getId());
            this->customersList.push_back(c);
        }
    }
}
void Trainer::clear() {
    for(auto *c: customersList){
        delete c;
    }
    customersList.clear();
    orderList.clear();
}
Trainer::~Trainer() {//destructor
    this->clear();
}

Trainer &Trainer::operator=(const Trainer &other) {//copy assignment
    if(this != &other){
        delete this;
        this->capacity = other.capacity;
        this->salary = other.salary;
        this->open = other.open;
        for(const auto & order : other.orderList)
            this->orderList.push_back(order);
        for (auto customer : other.customersList) {
            this->customersList.push_back(customer);
        }
    }
    return *this;
}

Trainer::Trainer(Trainer &&other) {//move constructor
    this->capacity = other.capacity;
    this->salary = other.salary;
    this->open = other.open;
    this->orderList = move(other.orderList);
    this->customersList = move(other.customersList);
}

Trainer& Trainer::operator=(Trainer &&other) {//move assignment
    if(this != &other) {
        delete this;

        this->capacity = other.capacity;
        this->salary = other.salary;
        this->open = other.open;
        this->orderList = move(other.orderList);
        this->customersList = move(other.customersList);
    }
    return *this;
}


