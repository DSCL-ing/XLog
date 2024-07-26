#include<iostream>
#include<memory>
#include<string>

class Computer {

public:
    void setBoard(const std::string& board) {
        _board = board;
    }
    void setDisplay(const std::string& display) {
        _display = display;
    }
    virtual void setOs() = 0;

    std::string toString() {
        std::string computer = "Computer:{\n";
        computer += "\tboard=" + _board + ",\n";
        computer += "\tdisplay=" + _display + ",\n";
        computer += "\tOs=" + _os + ",\n";
        computer += "}\n";
        return computer;
    }

protected:
    std::string _board;
    std::string _display;
    std::string _os;
};

/*具体产品类*/
class MacBook : public Computer {
public:
    using ptr = std::shared_ptr<MacBook>;
    MacBook() {}
    virtual void setOs() {
        _os = "Max OS";
    }
};

/*抽象建造者类：包含创建⼀个产品对象的各个部件的抽象接⼝*/
class Builder {
public:
    virtual void buildBoard(const std::string& board) = 0;
    virtual void buildDisplay(const std::string& display) = 0;
    virtual void buildOs() = 0;
    virtual std::shared_ptr<Computer> build() = 0;
};

/*具体产品的具体建造者类：实现抽象接⼝，构建和组装各个部件*/
class MacBookBuilder : public Builder {
public:
    MacBookBuilder() : _computer(new MacBook()) {}
    virtual void buildBoard(const std::string& board) {
        _computer->setBoard(board);
    }
    virtual void buildDisplay(const std::string& display) {
        _computer->setDisplay(display);
    }
    virtual void buildOs() {
        _computer->setOs();
    }
    std::shared_ptr<Computer> build()override {
        return _computer;
    }

    private:
    std::shared_ptr<Computer> _computer;
};

/*指挥者类，提供给调⽤者使⽤，通过指挥者来构造复杂产品*/
class Director {
public:
    Director(Builder* builder) :_builder(builder) {}
    void construct(const std::string& board, const std::string& display) {
        _builder->buildBoard(board);
        _builder->buildDisplay(display);
        _builder->buildOs();
    }
private:
    std::shared_ptr<Builder> _builder;
};

int main()
{
    Builder* buidler = new MacBookBuilder;
    std::unique_ptr<Director> pd(new Director(buidler));
    pd->construct("英特尔", "AOC显示器");
    auto computer = buidler->build();
    std::cout << computer->toString();
    return 0;
}
