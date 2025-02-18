# COLORS

ORANGE			=	\x1b[0m\x1b[38;2;255;127;0m
LIGHT_ORANGE	=	\x1b[1m\x1b[38;2;255;165;77m
DARK_ORANGE		=	\x1b[0m\x1b[38;2;179;89;0m
WHITE			=	\x1b[0m\x1b[38;2;255;255;255m

# TARGET

NAME			=	cub3d

LIBFT			=	libft.a
LIBFT_DIR		=	libft
LIBFT_PATH		=	$(LIBFT_DIR)/$(LIBFT)

MLX				=	MLX42
LIBMLX 			=	$(MLX)/build/libmlx42.a

# FLAGS

MAKEFLAGS		+=	-s
CFLAGS			=	-Wall -Werror -Wextra -g -pthread							\
					-I inc -I libft -I libft/gnl -I libft/printf/ft_printf_fd	\
					-I MLX42/include											\

LFLAGS			=	-ldl -lglfw -lm												\

# FILES

FILES			=	cub3d														\
\
					parsing/args												\
					parsing/check												\
					parsing/floodfill											\
					parsing/is													\
					parsing/map													\
					parsing/parsing												\
					parsing/path												\
					parsing/player												\
\
					game/raycasting/raycasting game/raycasting/raycasting_calc	\
					game/raycasting/raycasting_background						\
					game/raycasting/door										\
					game/movement game/hooks									\
					game/visuals												\
					game/minimap												\
\
					structs/init structs/init2 structs/free						\
\
					utils/utils utils/exit										\
\

SRC				=	$(addprefix src/, $(addsuffix .c, $(FILES)))
OBJ				=	$(addprefix src/, $(addsuffix .o, $(FILES)))

# RULES

all				:	fullscreen

fullscreen		:	CFLAGS += -D SCREENWIDTH=$$(xrandr --current | grep '*' | head -n1 | awk '{print $$1}' | cut -d 'x' -f1)
fullscreen		:	CFLAGS += -D SCREENHEIGHT=$$(xrandr --current | grep '*' | head -n1 | awk '{print $$1}' | cut -d 'x' -f2)
fullscreen		:	$(NAME)

midres			:	CFLAGS += -D SCREENWIDTH=1280
midres			:	CFLAGS += -D SCREENHEIGHT=720
midres			:	$(NAME)

lowres			:	CFLAGS += -D SCREENWIDTH=720
lowres			:	CFLAGS += -D SCREENHEIGHT=480
lowres			:	$(NAME)

$(LIBFT_PATH)	:
					$(MAKE) -C $(LIBFT_DIR) $(LIBFT) -j $$(nproc)

$(LIBMLX)		:
					if [ ! -d "$(MLX)" ]; then	\
						echo "\nClone of MLX42 library...";	\
						git clone https://github.com/codam-coding-college/MLX42.git $(MLX);	\
					fi
					echo "\nCompilation of MLX42 library..."
					cmake -S $(MLX) -B $(MLX)/build
					cmake --build $(MLX)/build

$(NAME)			:	$(LIBMLX) $(LIBFT_PATH) $(OBJ)
					$(CC) $(LFLAGS) $(OBJ) $(LIBFT_PATH) $(LIBMLX) -o $@
					echo -e '$(LIGHT_ORANGE) \tCompiled$(DARK_ORANGE) $@$(WHITE)'

clean			:
					$(RM) $(OBJ)
					$(MAKE) -C $(LIBFT_DIR) f$@
					echo -e '$(LIGHT_ORANGE) \tCleaned$(ORANGE) $(OBJ)$(WHITE)'

fclean			:	clean
					$(RM) $(NAME)
					$(RM) -r $(MLX)/build
					echo -e '$(LIGHT_ORANGE) \tCleaned$(DARK_ORANGE) $(NAME)$(WHITE)'

re				:	fclean all

.PHONY			=	all clean fclean re
