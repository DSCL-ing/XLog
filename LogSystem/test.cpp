#include<iostream>
#include<memory>
#include<thread>
#include<future>


class Bullet {
public:
    //伤害
    virtual int Attack() = 0;
    virtual ~Bullet() {};
};

//金子弹
class GoldBullet :public Bullet {
public:
    GoldBullet() {
        std::cout << "金子弹" << std::endl;
    }
    virtual int Attack() override {
        return 3;
    }
    virtual ~GoldBullet()override {
        std::cout << "销毁 金子弹" << std::endl;
    }
};

//银子弹
class SilverBullet : public Bullet {
public:
    SilverBullet() {
        std::cout << "银子弹" << std::endl;
    }
    virtual int Attack() override {
        return 2;
    }
    virtual ~SilverBullet()override {
        std::cout << "销毁 银子弹" << std::endl;
    }
};

//铜子弹
class CopperBullet : public Bullet {
public:
    CopperBullet() {
        std::cout << "铜子弹" << std::endl;
    }
    virtual int Attack() override {
        return 1;
    }
    virtual ~CopperBullet()override {
        std::cout << "销毁 铜子弹" << std::endl;
    }

};

enum class BulletType :int { gold, silver, copper };
class BulletFactory {
public:
    std::shared_ptr<Bullet> CreateBullet(BulletType type) {
        switch (type) {
        case BulletType::gold:
            //return new GoldBullet;                //错误
            return std::make_shared<GoldBullet>();
        case BulletType::silver:
            return std::make_shared<SilverBullet>();
        case BulletType::copper:
            return std::make_shared<CopperBullet>();
        default:
            std::cout << "CreateBullet type error" << std::endl;
            //TODO
            return nullptr;
        }
    }
};

//射击
void Shoot(std::shared_ptr<Bullet> bullet){
     
    std::cout<<"造成伤害: "<<bullet->Attack()<<std::endl;
}

int main() {
    BulletFactory factory;

    for (int i = 0; i < 1; i++) {
        std::async(std::launch::async|std::launch::deferred,Shoot,factory.CreateBullet(BulletType::gold));
        std::async(Shoot,factory.CreateBullet(BulletType::silver));
        std::async(Shoot,factory.CreateBullet(BulletType::copper));
    }
        
    return 0;
}
