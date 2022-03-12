#include "Observer.hpp"

auto Subject::attach(Observer *o) -> bool {
    follower.push_back(o);
    return true;
}
auto Subject::detach(Observer *o) -> bool {
    auto it = std::find_if(
        follower.begin(), follower.end(),
        [o](Observer *v) -> bool { return v->getID() == o->getID(); });
    if (it != follower.end()) {
        follower.erase(it);
    }

    return true;
}

void Subject::notify() {
    // std::cout << "notify here " << size() << this << "\n";
    for (auto &f : follower) {
        f->update(get_private_data());
    }
}
