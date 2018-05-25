#include <iostream>
#include <string>
/**
 * Builder 4 要素  
 *    - Abstract Builder: 抽象建造者  
 *    - Concrete Builder: 具体建造者  
 *    - Director: 指挥者  
 *    - Product: 产品角色 
 * 
 *  以用户点餐(Pizza)为例:
 *    dough: 面团, sauce: 酱, topping: 佐料
*/
using namespace std;

/**
 * Product
*/
class Pizza {
 public:
    void setDough(string d) { dough_ = d;}
    void setSauce(string s) { sauce_ = s;}
    void setTopping(string t) { topping_ = t;}
    string getTaste() 
    { 
        return "Taste: " + dough_ + 
                ", " + sauce_ + ", " + topping_;
    }

 private:
    string dough_;
    string sauce_;
    string topping_;
};

/**
 * Abstract Builder
*/
class PizzaBuilder {
public:
    Pizza* getPizza() { return pizza_; };
    void createNewPizza() { pizza_ = new Pizza(); }

    virtual void buildDough() = 0;
    virtual void buildSauce() = 0;
    virtual void buildTopping() = 0;

protected:
    Pizza* pizza_;
};
 
/**
 * Concrete Builder: Hawaiian Pizza 夏威夷披萨
 * 
*/
class HawaiianPizzaBuilder : public PizzaBuilder {
public: 
    void buildDough() { pizza_->setDough("cross"); }
    void buildSauce() { pizza_->setSauce("mild"); }
    void buildTopping() { pizza_->setTopping("pineapple"); }
};

/**
 * Concrete Builder: Spicy Pizza 麻辣披萨
 * 
*/
class SpicyPizzaBuilder : public PizzaBuilder {
public: 
    void buildDough() { pizza_->setDough("pan_baked"); }
    void buildSauce() { pizza_->setSauce("hot"); }
    void buildTopping() { pizza_->setTopping("pepperoni+salami"); }
};

/**
 * Director
*/
class Waiter {
public:
    void setPizzaBuilder (PizzaBuilder *pb) { pzaBdr_ = pb; }
    Pizza* getPizza() {return pzaBdr_->getPizza(); }
    void construct() 
    {
        pzaBdr_->createNewPizza();
        pzaBdr_->buildDough();
        pzaBdr_->buildSauce();
        pzaBdr_->buildTopping();
    }

private:
    PizzaBuilder *pzaBdr_;
};

int main()
{
    PizzaBuilder* pb = new HawaiianPizzaBuilder();
    PizzaBuilder* pb_ = new SpicyPizzaBuilder();

    Waiter* wt = new Waiter();
    wt->setPizzaBuilder(pb_);
    wt-> construct();
    cout << wt->getPizza()->getTaste() << endl;
    
    return 0;
}