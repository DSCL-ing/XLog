#include<iostream>
#include<memory>
#include<thread>
#include<future>
#include<string>


//船体
class ShipBody {
public:
    virtual std::string GetShipBody() = 0;
    virtual ~ShipBody() {}
};

//木材船体
class WoodShipBody :public ShipBody {
public:
    std::string GetShipBody() override {
        return "<船体:木材> ";
    }
    ~WoodShipBody()override {
        std::cout << "销毁 <船体:木材>" << "\n";
    }
};

//钢铁船体
class IronShipBody :public ShipBody {
public:
    std::string GetShipBody() override {
        return "<船体:钢铁> ";
    }
    ~IronShipBody()override {
        std::cout << "销毁 <船体:钢铁>" << "\n";
    }
};

//合金
class MetalShipBody :public ShipBody {
public:
    std::string GetShipBody() override {
        return "<船体:合金> ";
    }
    ~MetalShipBody()override {
        std::cout << "销毁 <船体:合金>" << "\n";
    }
};

//武器
class Weapon {
public:
    virtual std::string GetWeapon() = 0;
    virtual ~Weapon() {}
};


class GunWeapon :public Weapon {
public:
    std::string GetWeapon()override {
        return "<武器:火枪>";
    }
    ~GunWeapon() override {
        std::cout << "销毁 <武器:火枪>" << "\n";
    }
};

//机炮,加农炮
class CannonWeapon :public Weapon {
public:
    std::string GetWeapon()override {
        return "<武器:机炮>";
    }
    ~CannonWeapon() override {
        std::cout << "销毁 <武器:机炮>" << "\n";
    }
};

//导弹
class MissileWeapon :public Weapon {
public:
    std::string GetWeapon()override {
        return "<武器:导弹>";
    }
    ~MissileWeapon() override {
        std::cout << "销毁 <武器:导弹>" << "\n";
    }
};

//引擎
class Engine {
public:
    virtual std::string getEngine() = 0;
    virtual ~Engine() {};
};

//人力发动机
class HumanEngine :public Engine {
public:
    std::string getEngine()override {
        return "<引擎:人力>";
    }
    ~HumanEngine() {
        std::cout << "销毁 <引擎:人力>" << "\n";
    }
};

//柴油
class DieselEngine :public Engine {
public:
    std::string getEngine() {
        return "<引擎:柴油>";
    }
    ~DieselEngine() {
        std::cout << "销毁 <引擎:柴油>" << "\n";
    }
};


//电能发动机
class ElectricEngine :public Engine {
public:
    std::string getEngine()override {
        return "<引擎:电能>";
    }
    ~ElectricEngine() {
        std::cout << "销毁 <引擎:电能>" << "\n";
    }
};

//核能发动机
class NuclearEngine :public Engine {
public:
    std::string getEngine()override {
        return "<引擎:核能>";
    }
    ~NuclearEngine() {
        std::cout << "销毁 <引擎:核能>" << "\n";
    }
};



//船
class Ship {
public:
    Ship(std::shared_ptr<ShipBody> shipbody, std::shared_ptr<Weapon> weapon, std::shared_ptr<Engine> engine)
        :_shipbody(shipbody), _weapon(weapon), _engine(engine)
    {
        std::cout << "造船" << "\n";
    }

    std::string getShipInfo() {
        return "船结构 = " + _shipbody->GetShipBody() + _weapon->GetWeapon() + _engine->getEngine();
    }
    ~Ship() {};
private:
    std::shared_ptr<ShipBody> _shipbody;
    std::shared_ptr<Weapon> _weapon;
    std::shared_ptr<Engine> _engine;
};


//工厂
class ShipFactory {
public:
    virtual std::shared_ptr<Ship> CreateShip() = 0;
    virtual ~ShipFactory() {};
};

//基础版
class BasicShipFactory :public ShipFactory {
public:
    std::shared_ptr<Ship> CreateShip()override {
        return  std::make_shared<Ship>(std::make_shared<WoodShipBody>(), std::make_shared<GunWeapon>(), std::make_shared<HumanEngine>());
    }
};

//标准版
class StandardShipFactory :public ShipFactory {
public:
    std::shared_ptr<Ship> CreateShip()override {
        return  std::make_shared<Ship>(std::make_shared<IronShipBody>(), std::make_shared<CannonWeapon>(), std::make_shared<DieselEngine>());
    }
};

//旗舰版 
class UltimateShipFactory :public ShipFactory {
public:
    std::shared_ptr<Ship> CreateShip()override {
        return  std::make_shared<Ship>(std::make_shared<MetalShipBody>(), std::make_shared<MissileWeapon>(), std::make_shared<NuclearEngine>());
    }
};


int main() {
    BasicShipFactory bsf; 
    StandardShipFactory ssf;
    UltimateShipFactory usf;

    std::shared_ptr<Ship> foo1 = bsf.CreateShip();
    auto foo2 = ssf.CreateShip();
    auto foo3 = usf.CreateShip();

    return 0;
}
