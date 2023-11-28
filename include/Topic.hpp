#ifndef TOPIC_HPP
# define TOPIC_HPP

#include <string>
#include <fcntl.h>
#include <unistd.h>

class Topic {
    public:
        Topic();

        ~Topic();

        void    setContent(const std::string& content, const std::string& nickname);

        const std::string&  getContent() const;
        const std::string&  getNickname() const;
        const std::string&  getSetAt() const;

    private:
        std::string _content;
        std::string _nickname;
        std::string _setAt;
};

#endif
