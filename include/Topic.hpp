#ifndef TOPIC_HPP
# define TOPIC_HPP

#include "ft_String.hpp"

#include <fcntl.h>
#include <unistd.h>

class Topic {
    public:
        Topic();

        ~Topic();

        void    setContent(const ft::String& content, const ft::String& nickname);

        const ft::String&  getContent() const;
        const ft::String&  getNickname() const;
        const ft::String&  getSetAt() const;

    private:
        ft::String _content;
        ft::String _nickname;
        ft::String _setAt;
};

#endif
