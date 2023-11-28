#include "Topic.hpp"
#include "ft.hpp"

#include <ctime>
#include <sstream>

Topic::Topic() {
}

Topic::~Topic() {

}

void Topic::setContent(const std::string& content,
                       const std::string& nickname) {
    std::stringstream   time;

    _content = content;
    _nickname = nickname;
    time << std::time(NULL);
    _setAt = time.str();
}

const std::string& Topic::getContent() const {
    return (_content);
}

const std::string& Topic::getNickname() const {
    return (_nickname);
}

const std::string& Topic::getSetAt() const {
    return (_setAt);
}
