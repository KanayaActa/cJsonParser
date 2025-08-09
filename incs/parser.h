/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmotono <kmotono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:08:13 by miwasa            #+#    #+#             */
/*   Updated: 2025/03/02 15:40:14 by kmotono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "mini_rt.h"
# include <ctype.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>

# define SUCCESS 0
# define INCORRECT_FORMAT 1
# define OUT_OF_RANGE 1
# define MALLOC_ERROR 1

# define NEXT_ENTRY 2

# define NO_GROUP 0
# define COLOR_GROUP 1
# define MATERIAL_GROUP 2
# define OPTIONAL_GROUP 3
# define MAX_GROUP 4
# define BUFFER_SIZE 4096

# define WHITESPACES " \t\n\r\v\f"

typedef int				(*t_parse_object_func)(t_vars *, char *);

typedef struct s_parse_table
{
	const char			*prefix;
	t_parse_object_func	func;
}						t_parse_table;

typedef struct s_parse_entry
{
	int					group;
	const char			*name;
	int					(*parse_func)(char **, void *);
	void				*value;
}						t_parse_entry;

typedef struct s_square_params
{
	t_vec3				p1;
	t_vec3				p2;
	t_vec3				p3;
	char				*xpm_path;
}						t_square_params;

typedef struct s_cub_config
{
	char				*north_xpm;
	char				*south_xpm;
	char				*west_xpm;
	char				*east_xpm;
	int					floor_color;
	int					ceiling_color;
	char				**map;
	int					map_height;
	bool				is_no;
	bool				is_so;
	bool				is_we;
	bool				is_ea;
	bool				is_f;
	bool				is_c;

	bool				found_player;
	int					player_x;
	int					player_y;
	char				player_dir;
}						t_cub_config;

typedef struct s_parse_table_cub
{
	const char			*prefix;
	int					(*func)(t_cub_config *conf, char *line);
}						t_parse_table_cub;

typedef struct s_map_data
{
	char				**map;
	int					w;
	int					h;
}						t_map_data;

typedef struct s_strtod_ctx
{
	const char			*str;
	const char			*start;
	char				**endptr;
	double				val;
	int					sign;
	int					exponent;
	bool				has_digits;
}						t_strtod_ctx;

// utils
void					*xrealloc(void *ptr, size_t size);
void					skip_spaces(t_strtod_ctx *c);
void					parse_sign(t_strtod_ctx *c);
bool					parse_infinity(t_strtod_ctx *c);
void					parse_number_part(t_strtod_ctx *c);
void					parse_exponent_part(t_strtod_ctx *c);
double					ft_strtod(const char *nptr, char **endptr);
void					print_error(const char *msg);

// parse_rt
int						parse_file_rt(t_vars *vars, const char *filename);
int						parse_line_rt(t_vars *vars, char *line);
int						parse_object_rt(t_vars *vars, char *line);

// objs
int						parse_ambient(t_vars *vars, char *line);
int						parse_camera(t_vars *vars, char *line);
int						parse_light(t_vars *vars, char *line);
int						parse_lights(t_vars *vars, char *line);
int						parse_sphere(t_vars *vars, char *line);
int						parse_plane(t_vars *vars, char *line);
int						parse_cylinder(t_vars *vars, char *line);
int						parse_triangle(t_vars *vars, char *line);
int						parse_square(t_vars *vars, char *line);
int						parse_aabb(t_vars *vars, char *line);
int						parse_quad(t_vars *vars, char *line);
int						parse_circle(t_vars *vars, char *line);
int						parse_unknown(t_vars *vars, char *line);
// entries
int						parse_entries(char *line, const t_parse_entry *entries,
							size_t size);
int						parse_entry(char **line, const t_parse_entry *entry);
// utils
int						parse_text(char **line, const char *name);
int						parse_color(char **line, void *value);
int						parse_vec3(char **line, void *value);
int						parse_double(char **line, void *value);
int						parse_int(char **line, void *value);
int						parse_xpm(char **line, void *value);
int						parse_checker(char **line, void *value);
int						parse_comment(char **line, void *value);

// cub
int						parse_file_cub(t_vars *vars, const char *filename);

int						_parse_file_cub(t_cub_config *conf,
							const char *filename);

int						parse_line_cub(t_cub_config *conf, char *line);

int						parse_map(int fd, t_cub_config *conf,
							char *first_line_if_any);

int						parse_north(t_cub_config *conf, char *line);
int						parse_south(t_cub_config *conf, char *line);
int						parse_west(t_cub_config *conf, char *line);
int						parse_east(t_cub_config *conf, char *line);
int						parse_floor(t_cub_config *conf, char *line);
int						parse_ceiling(t_cub_config *conf, char *line);
int						parse_unknown_cub(t_cub_config *conf, char *line);

int						is_valid_character(t_cub_config *conf);
int						is_valid_map_structure(t_cub_config *conf);

bool					is_start_map_point(const char *line);

int						create_cub_obj(t_vars *vars, t_cub_config *conf);
int						add_floor_obj(t_vars *vars, t_cub_config *conf);
int						add_ceiling_obj(t_vars *vars, t_cub_config *conf);
int						add_camera_obj(t_vars *vars, t_vec3 pos, char dir_char,
							double fov_deg);
int						add_default_ambient(t_vars *vars);

int						add_wall_south(t_vars *vars, double x, double z,
							char *xpm_path);
int						add_wall_north(t_vars *vars, double x, double z,
							char *xpm_path);
int						add_wall_east(t_vars *vars, double x, double z,
							char *xpm_path);
int						add_wall_west(t_vars *vars, double x, double z,
							char *xpm_path);
int						parse_blank_cub(char **line);
bool					is_blank_line(const char *line);
void					skip_leading_spaces(char **line);
int						parse_cub_param_line(t_cub_config *conf, char *line);
int						load_map_lines(t_cub_config *conf, int fd,
							char *first_line_if_any);

int						validate_map_walls(t_cub_config *conf);
int						validate_map_chars_and_player(t_cub_config *conf);
int						validate_map(t_cub_config *conf);
bool					flood_fill(t_map_data d, int x, int y, bool **visited);
int						set_player_position_if_not_found(t_cub_config *conf,
							char c, int x, int y);

// parse_json
int						parse_file_json(t_vars *vars, const char *filename);

#endif
