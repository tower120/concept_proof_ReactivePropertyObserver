#ifndef REACTIVE_PROPERTYOBSERVER_TESTPROPERTYOBSERVER_H
#define REACTIVE_PROPERTYOBSERVER_TESTPROPERTYOBSERVER_H

#include <reactive/Property.h>
#include <reactive/Observer.h>

struct TestPropertyObserver{
    void test_1(){
        using namespace reactive;

        std::unique_ptr<Property<int>> i1 {new Property<int>()};
        Property<int> i2;



        Property<int> sum;


        observe_w_unsubscribe([&](auto unsubscribe, int i1, int i2){
            if (i2 == 20) {
                unsubscribe();
                return;
            }

            sum.set(i1+i2);
        }, *i1, i2);

        auto o = observe([](int sum){
            std::cout << "sum: " << sum << std::endl;
        }, sum);
        //o->unsubscribe();

        /*observe([](auto unsubscribe, int i2){
            std::cout << i2 << std::endl;
        }, i2);*/


        (*i1).set(1);
        //i1.reset();

        i2.set(20);
        i2.set(30);
    }
    void test_all(){
        test_1();
    }
};

#endif //REACTIVE_PROPERTYOBSERVER_TESTPROPERTYOBSERVER_H
