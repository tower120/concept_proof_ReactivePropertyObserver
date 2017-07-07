#ifndef REACTIVE_PROPERTYOBSERVER_OBSERVER_H
#define REACTIVE_PROPERTYOBSERVER_OBSERVER_H

#include <vector>
#include <functional>
#include <atomic>

#include "helpers.h"
#include "ObserverBase.h"

namespace reactive{

    // lives in shared_ptr
    // observer dies when property dies, or unsubscribe
    template<bool w_unsubscribe, class Closure, class ...Properties>
    class Observer : public ObserverBase{
        Closure closure;
        std::tuple<std::shared_ptr<typename Properties::Data>...> properties;
    public:
        template<class ClosureT>
        Observer(ClosureT&& closure, Properties&... properties)
            : closure(std::forward<ClosureT>(closure))
            , properties(properties.getData()...)
        {}

        virtual void property_died(void* dead_property) override {
            unsubscribe(dead_property);
        }

        void unsubscribe(void* except_property = nullptr){
            utils::foreach_tuple([&](auto i, auto& property){
                if(property.get() == except_property) return;

                property->unsubscribe(this);

                property.reset();
            }, properties);
        }

    private:
        bool do_run(std::true_type with_unsubscribe){
            bool do_unsubscribe_self = false;
            utils::apply([&](auto&... properties){
                closure( [&](){ /*this->unsubscribe(nullptr);*/ do_unsubscribe_self = true; },  properties->get()...);
            }, properties);
            return do_unsubscribe_self;
        }
        bool do_run(std::false_type with_unsubscribe){
            utils::apply([&](auto&... properties){
                closure( properties->get()...);
            }, properties);
            return false;
        }
    public:
        virtual bool run() override {
            return do_run(std::integral_constant<bool, w_unsubscribe>{});
        }

        ~Observer(){
            unsubscribe(nullptr);
        }
    };

    template<bool w_unsubscribe = false, class Closure, class ...Properties>
    auto observe(Closure&& closure, Properties&... properties){
        using O = Observer<w_unsubscribe, std::decay_t<Closure>, Properties...>;
        auto observer = std::make_shared<O>(std::forward<Closure>(closure), properties...);

        utils::foreach([&](auto i, auto& property){
            property.getData()->subscribe(observer);
        }, properties...);

        return observer;
    }

    template<class Closure, class ...Properties>
    auto observe_w_unsubscribe(Closure&& closure, Properties&... properties){
        return observe<true>(std::forward<Closure>(closure), properties...);
    }
}

#endif //REACTIVE_PROPERTYOBSERVER_OBSERVER_H
