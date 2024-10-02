/********************************************************************************
Copyright 2024 - Daniel Walz
Foleys Finest Audio UG (haftungsbeschraenkt)
Lobith Audio UG (haftungsbeschraenkt)
********************************************************************************

This code is provided under the ISC license

This code is provided as is. The authors disclaim all warranties.
For details refer to the LICENSE.md

*******************************************************************************/


#ifndef FOLEYS_LICENSING_CLIENT_FOLEYS_OBSERVERS_H
#define FOLEYS_LICENSING_CLIENT_FOLEYS_OBSERVERS_H

#include <vector>
#include <mutex>
#include <functional>

namespace foleys
{

template<typename ObserverType>
class ObserverList
{
public:
    ObserverList() = default;

    void addObserver (ObserverType* observer)
    {
        std::scoped_lock<std::mutex> guard (observerLock);
        if (std::find (observers.begin(), observers.end(), observer) != observers.cend())
            return;

        observers.push_back (observer);
    }

    void removeObserver (ObserverType* observer)
    {
        std::scoped_lock<std::mutex> guard (observerLock);
        observers.erase (std::remove_if (observers.begin(), observers.end(), [observer] (const ObserverType* o) { return o == observer; }), observers.end());
    }

    void call (std::function<void (ObserverType&)> func)
    {
        std::scoped_lock<std::mutex> guard (observerLock);
        for (auto* o: observers)
            func (*o);
    }

    ObserverList (const ObserverList&)            = delete;
    ObserverList& operator= (const ObserverList&) = delete;

private:
    std::mutex                 observerLock;
    std::vector<ObserverType*> observers;
};

}  // namespace foleys


#endif  // FOLEYS_LICENSING_CLIENT_FOLEYS_OBSERVERS_H
