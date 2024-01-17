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
VALGRIND_NAME			=	$(BUILD_ROOT)libftpp_debug1.a


include src.mk
SRC_PATH				=	src/
OBJS					=	$(patsubst %.cpp, $(DIR_BUILD)%.o, $(SRC))
DEPS					=	$(patsubst %.cpp, $(DIR_BUILD)%.d, $(SRC))

include test_src.mk
TEST_NAME				=	libftpp_test
TEST_SRC_PATH			=	test/

DIR_BUILD_TEST			=	$(BUILD_ROOT)test/
TEST_OBJS				=	$(patsubst %.cpp, $(DIR_BUILD_TEST)%.o, $(TEST_SRC))
DEPS_TEST				=	$(patsubst %.cpp, $(DIR_BUILD_TEST)%.d, $(TEST_SRC))

DEPS_FLAGS				=	-MMD -MP

BASE_CPPFLAGS			=	-Wall -Wextra -Werror -std=c++98
BASE_DEBUG_CPPFLAGS		=	$(BASE_CPPFLAGS) -g3
VALGRIND_CPP_FLAGS		=	$(BASE_CPPFLAGS) -g3
ifeq ($(DEBUG), 0)
	CPPFLAGS			=	$(BASE_CPPFLAGS) -O2
endif
ifeq ($(DEBUG), 1)
	CPPFLAGS			=	$(VALGRIND_CPP_FLAGS)
endif
ifeq ($(DEBUG), 2)
	CPPFLAGS			=	$(BASE_DEBUG_CPPFLAGS) -fsanitize=address
endif

VALGRIND_DEBUG_LEVEL	=	1


DIR_INCS =\
	include/	\
	private_include/
INCLUDES =\
	$(addprefix -I , $(DIR_INCS))

TEST_DIR_INCS =\
	test/include/
TEST_INCLUDES =\
	$(addprefix -I , $(TEST_DIR_INCS))


RM						=	rm -rf
CXX						=	c++
AR						=	ar rcs

.PHONY:	all
all:
		$(MAKE) $(NAME)

$(NAME):	$(OBJS)
		$(RM) $(NAME)
		$(AR) $(NAME) $(OBJS)


.PHONY:	clean
clean:
		$(RM) $(BUILD_ROOT)

.PHONY:	fclean
fclean:	clean
		$(RM) $(NAMES)
		$(RM) $(TEST_NAME)


.PHONY:	debug1
debug1:
		$(MAKE) DEBUG="1"

.PHONY:	debug2
debug2:
		$(MAKE) DEBUG="2"


.PHONY:	re
re:		fclean
		$(MAKE) all

$(TEST_NAME):	$(TEST_OBJS)
		$(CXX) $(VALGRIND_CPP_FLAGS) $(INCLUDES) $(TEST_INCLUDES) $(OBJS) $(TEST_OBJS) $(VALGRIND_NAME) -o $(TEST_NAME)

.PHONY: runTest
runTest:
		@if [ $(DEBUG) -eq $(VALGRIND_DEBUG_LEVEL) ]; then \
			$(MAKE) -j; \
			$(MAKE) -j $(TEST_NAME); \
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
							 --track-fds=yes -s -q ./$(TEST_NAME) \
						|| true; \
		else \
			$(MAKE) -j runTest DEBUG="$(VALGRIND_DEBUG_LEVEL)"; \
		fi

.PHONY: reRunTest
reRunTest: fclean
		$(MAKE) runTest

-include $(DEPS)
$(DIR_BUILD)%.o : $(SRC_PATH)%.cpp $(LIBFTPP_A)
		@mkdir -p $(shell dirname $@)
		$(CXX) $(CPPFLAGS) $(DEPS_FLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS_TEST)
$(DIR_BUILD_TEST)%.o : $(TEST_SRC_PATH)%.cpp $(VALGRIND_NAME)
		@mkdir -p $(shell dirname $@)
		$(CXX) $(VALGRIND_CPP_FLAGS) $(DEPS_FLAGS) $(INCLUDES) $(TEST_INCLUDES) -c $< -o $@
