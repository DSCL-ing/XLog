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


class BulletFactory {
public:
    virtual std::shared_ptr<Bullet> CreateBullet() = 0;
    virtual ~BulletFactory() {};
};

class GoldBulletFactory :public BulletFactory {
public:
    GoldBulletFactory() {
        std::cout << "构造 金子弹工厂" << "\n";
    }
    ~GoldBulletFactory() {
        std::cout << "销毁 金子弹工厂" << "\n";
    }
    std::shared_ptr<Bullet> CreateBullet() {
        return std::make_shared<GoldBullet>();
    }
};
class SilverBulletFactory :public BulletFactory {
public:
    SilverBulletFactory() {
        std::cout << "构造 银子弹工厂" << "\n";
    }
    ~SilverBulletFactory() {
        std::cout << "销毁 银子弹工厂" << "\n";
    }
    std::shared_ptr<Bullet> CreateBullet() {
        return std::make_shared<SilverBullet>();
    }
};
class CopperBulletFactory :public BulletFactory {
public:
    CopperBulletFactory() {
        std::cout << "构造 铜子弹工厂" << "\n";
    }
    ~CopperBulletFactory() {
        std::cout << "销毁 铜子弹工厂" << "\n";
    }
    std::shared_ptr<Bullet> CreateBullet() {
        return std::make_shared<CopperBullet>();
    }
};

enum class BulletType :int { gold, silver, copper };
//生产工厂的工厂()
class FactoryFactory {
public:
    std::shared_ptr<BulletFactory> CreateFactory(BulletType type) {
        switch (type) {
        case BulletType::gold:
            return std::make_shared<GoldBulletFactory>();
        case BulletType::silver:
            return std::make_shared<SilverBulletFactory>();
        case BulletType::copper:
            return std::make_shared<CopperBulletFactory>();
        default:
            std::cout << "FactoryFactory::CreateFactory(BulletType type)--> BulletType member no exist " << "\n";
            //TODO
            break;
        }
        return nullptr;
    }

};

//射击
void Shoot(std::shared_ptr<Bullet> bullet) {

    std::cout << "造成伤害: " << bullet->Attack() << std::endl;
}

int main() {
    //GoldBulletFactory gbf;
    //SilverBulletFactory sbf;
    //CopperBulletFactory cbf;

    FactoryFactory ff;
    auto gbf = ff.CreateFactory(BulletType::gold);
    auto sbf = ff.CreateFactory(BulletType::silver);
    auto cbf = ff.CreateFactory(BulletType::copper);



    for (int i = 0; i < 2; i++) {
        std::async(std::launch::async, Shoot, gbf->CreateBullet());
        std::async(std::launch::async, Shoot, sbf->CreateBullet());
        std::async(std::launch::async, Shoot, cbf->CreateBullet());
    }

    return 0;
}
