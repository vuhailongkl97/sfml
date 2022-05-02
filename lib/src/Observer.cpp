#include "Observer.hpp"
#include <algorithm>
#include <vector>

struct Subject::Pimpl {
    std::vector<std::weak_ptr<Observer>> follower;

    void push_back(std::weak_ptr<Observer> o) { follower.push_back(o); }

    void notify(std::shared_ptr<void> var) {
        for (auto &f : follower) {
            auto spf = f.lock();
            if (spf) {
                spf->update(var);
            }
        }
    }

    auto detach(std::weak_ptr<Observer> o) -> bool {
        auto it = std::find_if(follower.begin(), follower.end(),
                               [&o](std::weak_ptr<Observer> &v) -> bool {
                                   auto spf = o.lock();
                                   if (!spf) {
                                       return true;
                                   }
                                   return spf->getID() == v.lock()->getID();
                               });
        if (it != follower.end()) {
            follower.erase(it);
        }
        return true;
    }

    auto size() -> size_t { return follower.size(); }
};

auto Subject::attach(std::weak_ptr<Observer> o) -> bool {
    follower->push_back(o);
    return true;
}
auto Subject::detach(std::weak_ptr<Observer> o) -> bool {
    follower->detach(o);
    return true;
}

void Subject::notify() {
    auto &&var = get_private_data();
    follower->notify(std::move(var));
}

auto Subject::size() -> size_t { return follower->size(); }

Observer::~Observer() = default;
Observer::Observer() = default;
Subject::Subject() : follower(new Subject::Pimpl()) {}
Subject::~Subject() = default;
