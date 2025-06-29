CC = cc
CFLAGS = -Wall -Wextra -Werror 
NAME = philo

SRCS = philo.c\
	   routine.c\
	   initialization.c\
	   utils.c\
	   checks.c\
	   forks_management.c\
	   threads.c\

SRC_DIR = ./SRC

OBJS_DIR = OBJS
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS) ./include/philo.h
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re