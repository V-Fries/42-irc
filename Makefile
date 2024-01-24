DEBUG					=	0

BASE_NAME				=	ircbot
BUILD_ROOT				=	.build/
NAMES =\
	$(BASE_NAME)		\
	$(BASE_NAME)_debug1	\
	$(BASE_NAME)_debug2
ARGS = 6667 password
ifeq ($(DEBUG), 0)
	NAME				=	$(BASE_NAME)
	DIR_BUILD			=	$(BUILD_ROOT)base/
endif
ifeq ($(DEBUG), 1)
	NAME				=	$(BASE_NAME)_debug1
	DIR_BUILD			=	$(BUILD_ROOT)debug1/
endif
ifeq ($(DEBUG), 2)
	NAME				=	$(BASE_NAME)_debug2
	DIR_BUILD			=	$(BUILD_ROOT)debug2/
endif


include src.mk
SRC_PATH				=	src/
OBJS					=	$(patsubst %.cpp, $(DIR_BUILD)%.o, $(SRC))

DEPS					=	$(patsubst %.cpp, $(DIR_BUILD)%.d, $(SRC))
DEPS_FLAGS				=	-MMD -MP

BASE_CPPFLAGS			=	-Wall -Wextra -Werror -std=c++98
BASE_DEBUG_CPPFLAGS		=	$(BASE_CPPFLAGS) -g3
ifeq ($(DEBUG), 0)
	CPPFLAGS			=	$(BASE_CPPFLAGS) -O2
endif
ifeq ($(DEBUG), 1)
CPPFLAGS				=	$(BASE_CPPFLAGS) -g3
endif
ifeq ($(DEBUG), 2)
	CPPFLAGS			=	$(BASE_DEBUG_CPPFLAGS) -fsanitize=address
endif


LIBFTPP_PATH			=	lib/libftpp/
LIBFTPP_INCLUDES		=	$(LIBFTPP_PATH)include/
LIBFTPP_BUILD_ROOT		=	$(LIBFTPP_PATH).build/
ifeq ($(DEBUG), 0)
	LIBFTPP_L			=	-L $(LIBFTPP_BUILD_ROOT) -l ftpp
	LIBFTPP_A			=	$(LIBFTPP_BUILD_ROOT)libftpp.a
endif
ifeq ($(DEBUG), 1)
	LIBFTPP_L			=	-L $(LIBFTPP_BUILD_ROOT) -l ftpp_debug1
	LIBFTPP_A			=	$(LIBFTPP_BUILD_ROOT)libftpp_debug1.a
endif
ifeq ($(DEBUG), 2)
	LIBFTPP_L			=	-L $(LIBFTPP_BUILD_ROOT) -l ftpp_debug2
	LIBFTPP_A			=	$(LIBFTPP_BUILD_ROOT)libftpp_debug2.a
endif
MAKE_LIBFTPP			=	$(MAKE) -C $(LIBFTPP_PATH)


DIR_INCS =\
	include/			\
	$(LIBFTPP_INCLUDES)
INCLUDES =\
	$(addprefix -I , $(DIR_INCS))

LIBS =\
	$(LIBFTPP_L)


RM						=	rm -rf
CXX						=	c++


.PHONY:	all
all:
		$(MAKE_LIBFTPP)
		$(MAKE) $(NAME)

.PHONY:	run
run:
		$(MAKE) -j
		./$(BASE_NAME) $(ARGS) || true

$(NAME):	$(OBJS)
		$(CXX) $(CPPFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $(NAME)


.PHONY:	clean
clean:
		$(MAKE_LIBFTPP) clean
		$(RM) $(BUILD_ROOT)

.PHONY:	fclean
fclean:	clean
		$(MAKE_LIBFTPP) fclean
		$(RM) $(NAMES)


.PHONY:	debug1
debug1:
		$(MAKE) DEBUG="1"

.PHONY:	runDebug1
runDebug1:
		$(MAKE) debug1 -j
		./$(BASE_NAME)_debug1 $(ARGS) || true

.PHONY:	debug2
debug2:
		$(MAKE) DEBUG="2"

.PHONY:	runDebug2
runDebug2:
		$(MAKE) debug2 -j
		./$(BASE_NAME)_debug2 $(ARGS) || true


.PHONY:	runValgrind
runValgrind:
		$(MAKE) debug1 -j
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
				 --track-fds=yes -s -q ./$(BASE_NAME)_debug1 $(ARGS)\
			|| true


.PHONY:	re
re:		fclean
		$(MAKE) all

-include $(DEPS)
$(DIR_BUILD)%.o : $(SRC_PATH)%.cpp $(LIBFTPP_A)
		@mkdir -p $(shell dirname $@)
		$(CXX) $(CPPFLAGS) $(DEPS_FLAGS) $(INCLUDES) -c $< -o $@
