#include <iostream>
#include <string>


using namespace std;

/**
 * Product  
 *   组成部分:  头, 左手, 右手, 左脚, 右脚
*/
class Role { 
public:
    string getRoleAttr() 
    { 
        return "head: " + h_ + 
               "\nleftArm: " + la_ +
               "\nrightArm: " + ra_ +
               "\nleftLeg: " + ll_ +
               "\nrightLeg: " + rl_;   // 让用户去控制是否换行
    }
    void setHead(string h) { h_ = h;}
    void setLeftArm(string la) { la_ = la;}
    void setRightArm(string ra) { ra_ = ra;}
    void setLeftLeg(string ll) { ll_ = ll;}
    void setRightLeg(string rl) { rl_ = rl;}
private:
    string h_;
    string la_;
    string ra_;
    string ll_;
    string rl_;
};

class Builder {
public:
    // Builder() { role_ = new Role(); }
    
    Role* getRole() { return role_;}
    string getRoleAttr() { return role_->getRoleAttr();}

    virtual void buildHead() = 0;
    virtual void buildLeftArm() = 0;
    virtual void buildRightArm() = 0;
    virtual void buildLeftLeg() = 0;
    virtual void buildRightLeg() = 0;

protected:
    Role* role_;
};

/**
 * 内部建造
*/
class ThinBoyBuilder : public Builder{
public:
    ThinBoyBuilder() { role_ = new Role(); }
    
    void buildHead() 
    {
        role_->setHead("head_ThinBoy");
    }
    void buildLeftArm() 
    {
        role_->setLeftArm("leftArm_ThinBoy");
    }
    void buildRightArm() 
    {
        role_->setRightArm("rightArm_ThinBoy");
    }
    void buildLeftLeg()
    {
        role_->setLeftLeg("leftLeg_ThinBoy");
    } 
    void buildRightLeg()
    {
        role_->setRightLeg("rightLeg_ThinBoy");
    } 
};

class TallGirlBuilder : public Builder{
public:
    TallGirlBuilder() { role_ = new Role(); }
    
    void buildHead() 
    {
        role_->setHead("head_TallGirl");
    }
    void buildLeftArm() 
    {
        role_->setLeftArm("leftArm_TallGirl");
    }
    void buildRightArm() 
    {
        role_->setRightArm("rightArm_TallGirl");
    }
    void buildLeftLeg()
    {
        role_->setLeftLeg("leftLeg_TallGirl");
    } 
    void buildRightLeg()
    {
        role_->setRightLeg("rightLeg_TallGirl");
    } 
};

/**
 * Director, 定义建造流程.   
*/
class Director {
public:
    Director() {}

    void construct(Builder * bldr)
    {
        bldr->buildHead();
        bldr->buildLeftArm();
        bldr->buildRightArm();
        bldr->buildLeftLeg();
        bldr->buildRightLeg();
    }
};


int main()
{

    Builder* bldr = new ThinBoyBuilder();
    Director* d = new Director();
    d->construct(bldr);

    cout << bldr->getRoleAttr() << endl;

    return 0;
}