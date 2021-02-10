INITMK = initmk

SRC_PREFIX = src/
OBJ_PREFIX = obj/

CXX = g++
CXXFLAGS += -Wall -Werror -Wextra -std=c++2a

SRC = main.cpp options.cpp parser.cpp initmk.cpp
OBJ = $(addprefix $(OBJ_PREFIX), $(SRC:.cpp=.o))

all: $(CREATE_OBJ) $(INITMK)

$(OBJ_PREFIX)/%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $^

$(INITMK): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ): $(OBJ_PREFIX)%.o: $(SRC_PREFIX)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

create_obj:
	@mkdir -p $(OBJ_PREFIX)

clean:
	@rm -f $(OBJ_PREFIX)/*.o
	@rm -f $(INITMK)
