#ifndef REACTIVE_PROPERTYOBSERVER_BENCHMARK_H_H
#define REACTIVE_PROPERTYOBSERVER_BENCHMARK_H_H

#include <vector>
#include <chrono>


#include <reactive/Property.h>
#include <reactive/Observer.h>

struct Benchmark{
    const int count = 100'000;

    template<class T, class R>
    struct Data {
        T x1, x2, x3, x4;
        R sum;

        Data(){
            // test small update on 4 observables
            reactive::observe([&](int x1, int x2, int x3, int x4){
                sum.set( x1+x2+x3+x4 );
            }, x1, x2, x3, x4);
        }

        template<class I1, class I2, class I3, class I4>
        void update(I1&& x1, I2&& x2, I3&& x3, I4&& x4) {
            this->x1.set(x1);
            this->x2.set(x2);
            this->x3.set(x3);
            this->x4.set(x4);
        }
    };

    template<class Container>
    void benchmark_fill(Container& container) {
        using namespace std::chrono;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        container.reserve(count);
        for (int i = 0; i < count; i++) {
            container.emplace_back();
        }
        /*for (auto&& p : container) {
            p.update(100,20,40,600);
        }*/


        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2 - t1).count();
        std::cout << "filled in : " << duration << std::endl;
    }

    template<class Container>
    void benchmark_update(Container& container) {
        using namespace std::chrono;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        long long sum = 0;
        for (int i = 0; i < count; i++) {
            container[i].update(i, i+1, i+2, i+3);
            //sum += container[i].sum.getCopy();
        }

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(t2 - t1).count();
        std::cout << "updated in : " << duration
                  << " (" << sum << ")"
                  << std::endl;
    }


    void benchmark_all(){
        using T = long long;
        {
            using Element = Data<reactive::Property<T>, reactive::Property<T> >;
            std::vector<Element> list;
            benchmark_fill(list);
            benchmark_update(list);
            std::cout << "---"  << std::endl;
        }
    }
};

#endif //REACTIVE_PROPERTYOBSERVER_BENCHMARK_H_H
