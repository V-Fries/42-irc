#include "Topic.hpp"
#include "ft.hpp"

#include <ctime>
#include <sstream>

Topic::Topic(): _setAt("") {
}

Topic::~Topic() {

}

void Topic::setContent(const ft::String& content,
                       const ft::String& nickname) {
    std::stringstream   time;

    _content = content;
    _nickname = nickname;
    time << std::time(NULL);
    _setAt = time.str();
}

const ft::String& Topic::getContent() const {
    return (_content);
}

const ft::String& Topic::getNickname() const {
    return (_nickname);
}

const ft::String& Topic::getSetAt() const {
    return (_setAt);
}
