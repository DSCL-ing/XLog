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
class Weapen {
public:
    virtual std::string GetWeapen() = 0;
    virtual ~Weapen() {}
};


class GunWeapen :public Weapen {
public:
    std::string GetWeapen()override {
        return "<武器:火枪>";
    }
    ~GunWeapen() override {
        std::cout << "销毁 <武器:火枪>" << "\n";
    }
};

//机炮,加农炮
class CannonWeapen :public Weapen {
public:
    std::string GetWeapen()override {
        return "<武器:机炮>";
    }
    ~CannonWeapen() override {
        std::cout << "销毁 <武器:机炮>" << "\n";
    }
};

//导弹
class MissileWeapen :public Weapen {
public:
    std::string GetWeapen()override {
        return "<武器:导弹>";
    }
    ~MissileWeapen() override {
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
    Ship(ShipBody* shipbody, Weapen* weapen, Engine* engine)
        :_shipbody(shipbody), _weapen(weapen), _engine(engine)
    { 
        std::cout<<"造船"<<"\n";
    }

    std::string getShipInfo() {
        return "船结构 = " + _shipbody->GetShipBody() + _weapen->GetWeapen() + _engine->getEngine();
    }
    ~Ship() {
        delete _shipbody;
        delete _weapen;
        delete _engine;
    };
private:
    ShipBody* _shipbody = nullptr;
    Weapen* _weapen;
    Engine* _engine;
};


    //工厂

    int main() {
        Ship s(new WoodShipBody,new GunWeapen,new HumanEngine);
        std::cout<<s.getShipInfo()<<"\n";

    return 0;
}
