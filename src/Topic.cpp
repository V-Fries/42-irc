#include "Topic.hpp"
#include "ft.hpp"

#include <cstring>

Topic::Topic() {
}

Topic::~Topic() {

}

void Topic::setContent(const std::string& content,
                       const std::string& nickname) {
    char    buffer[1024];

    _content = content;
    _nickname = nickname;

    const int sinceEpochFd = open("/sys/class/rtc/rtc0/since_epoch", O_RDONLY);
    if (sinceEpochFd == -1) {
        ft::Log::error << "open /sys/class/rtc/rtc0/since_epoch file error: " << strerror(errno);
        return;
    }
    ssize_t len = read(sinceEpochFd, buffer, 1024);
    if (len == -1) {
        close(sinceEpochFd);
        ft::Log::error << "read /sys/class/rtc/rtc0/since_epoch file error: " << strerror(errno);
        return;
    }
    if (len > 0 && buffer[len - 1] == '\n')
        len--;
    _setAt = std::string(buffer, len);
    close(sinceEpochFd);
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
