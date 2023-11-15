DEBUG					=	0

BUILD_ROOT				=	.build/
NAMES =\
	$(BUILD_ROOT)libftpp.a		\
	$(BUILD_ROOT)libftpp_debug1.a	\
	$(BUILD_ROOT)libftpp_debug2.a
ifeq ($(DEBUG), 0)
	NAME				=	$(BUILD_ROOT)libftpp.a
	DIR_BUILD			=	$(BUILD_ROOT)base/
endif
ifeq ($(DEBUG), 1)
	NAME				=	$(BUILD_ROOT)libftpp_debug1.a
	DIR_BUILD			=	$(BUILD_ROOT)debug1/
endif
ifeq ($(DEBUG), 2)
	NAME				=	$(BUILD_ROOT)libftpp_debug2.a
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


DIR_INCS =\
	include/	\
	private_include/
INCLUDES =\
	$(addprefix -I , $(DIR_INCS))


RM						=	rm -rf
CXX						=	g++
AR						=	ar rcs

.PHONY:	all
all:
		$(MAKE) $(NAME)

$(NAME):	$(OBJS)
		$(AR) $(NAME) $(OBJS)


.PHONY:	clean
clean:
		$(RM) $(BUILD_ROOT)

.PHONY:	fclean
fclean:	clean
		$(RM) $(NAMES)


.PHONY:	debug1
debug1:
		$(MAKE) DEBUG="1"

.PHONY:	debug2
debug2:
		$(MAKE) DEBUG="2"


.PHONY:	re
re:		fclean
		$(MAKE) all

-include $(DEPS)
$(DIR_BUILD)%.o : $(SRC_PATH)%.cpp $(LIBFTPP_A)
		@mkdir -p $(shell dirname $@)
		$(CXX) $(CPPFLAGS) $(DEPS_FLAGS) $(INCLUDES) -c $< -o $@
