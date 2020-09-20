#include <iostream>
#include <vector>
#include <algorithm>

enum class Light : char {
    RED = 'r',
    BLUE = 'b',
    OFF = 'o'
};

class LightBulb {
public:
    LightBulb(Light l, std::string n):light(l), name(n)  {
        std::cout << name << " : Bulb created!\n";
    }
    void onCommandRecieved(bool isOn) {
        std::cout << name << " : I'm in " << static_cast<char>(isOn ? light : Light::OFF) << " state" << std::endl;
        this->isOn = isOn;
    }
    std::string getName() {
        return name;
    }
private:
    const Light light;
    bool isOn = false;
    const std::string name;
};

class Observer {
public:
    Observer() {
        std::cout << "Observer created!\n";
    }
    void subscribe(LightBulb* bulb) {
        bulbs.push_back(bulb);
    }
    void unsubscribe(LightBulb* bulb) {
        std::vector<LightBulb*>::iterator it = std::find(bulbs.begin(), bulbs.end(), bulb);
        if(it != bulbs.end()){
            bulbs.erase(it);
        }
    }
    void notify(bool isOn) {
        for (LightBulb* bulb : bulbs) {
            bulb->onCommandRecieved(isOn);
        }
    }
    bool isContained(LightBulb* bulb) {
        return std::find(bulbs.begin(), bulbs.end(), bulb) != bulbs.end();
    }
    ~Observer() {
        bulbs.clear();
    }
private:
    std::vector<LightBulb*> bulbs;
};

class Switcher {
public:
    Switcher() {
        std::cout << "Switcher created!\n";
    }
    void tap() {
        isOn = !isOn;
        observer.notify(isOn);
    }
    void connect(LightBulb* bulb) {
        std::cout << bulb->getName() << " : Try to connect this bulb\n";
        if(!observer.isContained(bulb)) {
            observer.subscribe(bulb);
            bulb->onCommandRecieved(isOn);
            std::cout << bulb->getName() << " : This bulb was connected!\n";
        }else{
            std::cout << bulb->getName() << " : This bulb is already connected!\n";
        }
    }
    void disconnect(LightBulb* bulb) {
        std::cout << bulb->getName() << " : Try to disconnect this bulb\n";
        if(observer.isContained(bulb)) {
            observer.unsubscribe(bulb);
            bulb->onCommandRecieved(false);
            std::cout << bulb->getName() << " : This bulb was disconnected!\n";
        }else{
            std::cout << bulb->getName() << " : This bulb is not connected!\n";
        }
    }
private:
    Observer observer;
    bool isOn = false;
};

int main() {
    Switcher switcher = Switcher();
    LightBulb bulb1 = LightBulb(Light::RED, "BULB1");
    LightBulb bulb2 = LightBulb(Light::BLUE, "BULB2");
    switcher.connect(&bulb1);
    switcher.tap();
    switcher.tap();
    switcher.tap();
    switcher.tap();
    switcher.connect(&bulb1);
    switcher.tap();
    switcher.tap();
    switcher.tap();
    switcher.connect(&bulb2);
    switcher.tap();
    switcher.tap();
    switcher.tap();
    return 0;
}
