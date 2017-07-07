#ifndef REACTIVE_PROPERTYOBSERVER_OBSERVERBASE_H
#define REACTIVE_PROPERTYOBSERVER_OBSERVERBASE_H

namespace reactive {
    class ObserverBase {
    public:
        virtual void property_died(void*) = 0;
        virtual bool run() = 0; /* unsubscribe_current? */
    };
}

#endif //REACTIVE_PROPERTYOBSERVER_OBSERVERBASE_H
