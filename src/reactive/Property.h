#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "ObserverBase.h"

namespace reactive{
    template<class T>
    class Property {
    public:
        struct Data{
            std::vector<std::shared_ptr<ObserverBase>> observers;
            T value;

            void die(){
                // schedule death
                for(auto& observer : observers){
                    observer->property_died(this);
                    observer.reset();
                }
                observers.clear();
                observers.shrink_to_fit();
            }

            void set(const T& new_value){
                value = new_value;
                pulse();
            }

            void pulse(){
                // just for loop observers->run()
                // with self deletion

                int count = observers.size();
                int i = 0;
                while(i < count){
                    auto& observer = observers[i];
                    const bool do_unsubscribe_self = observer->run(/* observer */);

                    if (!do_unsubscribe_self){
                        i++;
                    } else {
                        // unordered delete O(1)
                        std::iter_swap(observers.begin()+i, observers.end() - 1);
                        observers.pop_back();

                        count--;
                    }
                }
            }

            void subscribe(const std::shared_ptr<ObserverBase>& observer) {
                observers.emplace_back(observer);
            }
            void unsubscribe(ObserverBase* observer) {
                do_unsubscribe(observer);
            }
            void do_unsubscribe(ObserverBase* observer){
                auto it = std::find_if(observers.begin(), observers.end(), [&](const auto& element) {
                    return (element.get() == observer);
                });

                // unordered delete O(1)
                if (it != observers.end()) {
                    std::iter_swap(it, observers.end() - 1);
                    observers.pop_back();
                }
            }

            const auto& get() const{
                return value;
            }
        };
        std::shared_ptr<Data> data;
    public:
        // for observer
        const std::shared_ptr<Data>& getData() const {
            return data;
        }

        Property()
            :data(std::make_shared<Data>())
        {}

        void set(const T& new_value){
            data->set(new_value);
        }

        ~Property() {
            if (data) data->die();
        };
    };
}