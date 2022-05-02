#include "Observer.hpp"

auto Subject::attach(std::weak_ptr<Observer> o) -> bool {
    follower.push_back(o);
    return true;
}
auto Subject::detach(std::weak_ptr<Observer> o) -> bool {
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

void Subject::notify() {
    // std::cout << "notify here " << size() << this << "\n";
    for (auto &f : follower) {
        auto spf = f.lock();
        if (spf) {
            auto var = get_private_data();
            spf->update(std::move(var));
        }
    }
}
