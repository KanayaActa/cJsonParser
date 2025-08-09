/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_rt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmotono <kmotono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:59:54 by kmotono           #+#    #+#             */
/*   Updated: 2025/03/02 15:39:07 by kmotono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_RT_H
# define MINI_RT_H

# include "../lib/libft.h"
# include "../minilibx-linux/mlx.h"
# include "../minilibx-linux/mlx_int.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h>
# include <float.h>
# include <limits.h>
# include <math.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// Window Name
# define WIN_NAME "miniRT"

// Window Settings
# define WIN_HEIGHT 960
# define WIN_WIDTH 1280

// Key Definitions
# define KEY_ESC 65307
# define KEY_SPACE 32

# define KEY_PLUS 61
# define KEY_MINUS 45

# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_RIGHT 65363
# define KEY_LEFT 65361

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_E 101
# define KEY_Q 113

# define KEY_R 114
# define KEY_L 108

// Error Codes
# define FAILED_OPEN_FILE -1

// Mathematical Constants
# define PI 3.14159265358979323846
# define C_EPSILON 0.0078125

// Rendering Modes
# define TEXTURE_MODE 1
# define BUMP_MODE 1
# define CHECKER_MODE 0
# define TOON_MODE 0
# define TOON_STEP 5

# define CUB3D_MODE 0
# define DENY_PENETRATION 0

// Iteration Settings
# ifndef DEFAULT_ITER
#  define DEFAULT_ITER 100000
# endif
# ifndef LOW_ITER
#  define LOW_ITER 1200
# endif
# define DEFAULT_MOSAIK 32

// Ray Tracing Settings
# define MAX_DEPTH 20

typedef enum e_dir
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	FORWARD,
	BACKWARD
}					t_dir;

enum
{
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
};

typedef enum e_obj_type
{
	NONE,
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER,
	CIRCLE,
	TRIANGLE,
	SQUARE,
	QUAD,
	AABB,
	CUBE,
	REMOVED
}					t_obj_type;

typedef enum e_quad_type
{
	QUAD_ELLIPSOID,
	QUAD_HYPERBOLOID_ONE_SHEET,
	QUAD_HYPERBOLOID_TWO_SHEETS,
	QUAD_CONE,
	QUAD_PARABOLOID,
	QUAD_HYPERBOLIC_PARABOLOID
}					t_quad_type;

typedef struct s_list
{
	int				content_type;
	void			*content;
	struct s_list	*next;
}					t_list;

typedef union u_vec3
{
	double			_[3];
	struct
	{
		double		x;
		double		y;
		double		z;
	};
}					t_vec3;

typedef union u_mat3x3
{
	double			_[3][3];
	struct
	{
		double		_00;
		double		_01;
		double		_02;
		double		_10;
		double		_11;
		double		_12;
		double		_20;
		double		_21;
		double		_22;
	};
}					t_mat3x3;

typedef struct s_conv_coord
{
	t_mat3x3		mat;
	t_mat3x3		mat_inv;
	t_vec3			translation;
	t_vec3			translation_inv;
}					t_conv_coord;

typedef struct s_color
{
	double			k_r;
	double			k_g;
	double			k_b;
}					t_color;

typedef struct s_material
{
	t_color			k_ambient;
	t_color			k_diffuse;
	t_color			k_specular;
	double			reflectivity;
	double			ior;
	double			shininess;
}					t_material;

typedef struct s_texture
{
	unsigned int	*data;
	int				*grey_height;
	int				*bump_x;
	int				*bump_y;
	unsigned int	width;
	unsigned int	height;
}					t_texture;

typedef struct s_img_data
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				width;
	int				height;
}					t_img_data;

typedef struct s_camera
{
	t_vec3			pos;
	t_vec3			dir;
	double			fov;
	double			dist_to_screen;
	t_vec3			right;
	t_vec3			up;
}					t_camera;

typedef struct s_ambient
{
	double			brightness;
	t_color			color;
}					t_ambient;

typedef struct s_light
{
	t_vec3			pos;
	double			brightness;
	t_color			color;
}					t_light;

typedef struct s_sphere
{
	t_vec3			center;
	double			r;
}					t_sphere;

typedef struct s_plane
{
	t_vec3			normal;
	t_vec3			pos;
}					t_plane;

typedef struct s_circle
{
	t_vec3			normal;
	t_vec3			center;
	double			r;
}					t_circle;

typedef struct s_cylinder
{
	t_vec3			pos;
	t_vec3			dir;
	double			r;
	double			h;
}					t_cylinder;

typedef struct s_triangle
{
	t_vec3			normal;
	t_vec3			p1;
	t_vec3			p2;
	t_vec3			p3;
}					t_triangle;

typedef struct s_square
{
	t_vec3			normal;
	t_vec3			p1;
	t_vec3			p2;
	t_vec3			q;
}					t_square;

typedef struct s_cube
{
	t_vec3			min;
	t_vec3			max;
}					t_cube;

typedef struct s_quad
{
	t_vec3			scale;
	t_vec3			axis;
	int				angle;
	t_vec3			center;
	t_quad_type		type;
}					t_quad;

typedef struct s_aabb
{
	t_vec3			min;
	t_vec3			max;
}					t_aabb;

typedef struct s_obj
{
	int				type;
	void			*content;
	t_aabb			aabb;
	t_vec3			centroid;
	t_conv_coord	conv_coord;
	t_color			color;
	t_material		material;
	t_texture		texture;
	t_img_data		*img_xpm;
	char			*xpm_path;
	int				checker_u;
	int				checker_v;
}					t_obj;

typedef struct s_bvh
{
	t_aabb			*aabb;
	struct s_bvh	*left;
	struct s_bvh	*right;
	t_obj			**objs;
	int				obj_count;
}					t_bvh;

typedef struct s_ray
{
	t_vec3			origin;
	t_vec3			dir;
	t_vec3			inv_dir;
}					t_ray;

typedef struct s_ray_cast_vars
{
	t_ray			ray;
	t_obj			*obj;
	double			t;
	t_vec3			pos;
	t_vec3			normal;
	double			depth;
	double			rate;
	double			distance;
	int				checker_reverse;
}					t_ray_cast_vars;

typedef struct s_rt
{
	t_camera		*camera;
	t_camera		*init_camera;
	int				camera_step;
	t_ambient		*ambient;
	t_list			*lights;
	int				lights_count;
	t_list			*objs;
	int				objs_count;
	t_list			*objs_addition;
	t_bvh			*bvh;
	t_obj			**objs_array;
}					t_rt;

typedef struct s_pixel_data
{
	t_obj			*obj;
	t_vec3			pos;
	t_color			color;
	int				calc_done;
}					t_pixel_data;

typedef struct s_vars_ex
{
	int				need_reinit;
	int				iter;
	int				current_index;
	int				current_mosaic;
	int				prev_x;
	int				prev_y;
	int				is_dragging;
}					t_vars_ex;

typedef struct s_action
{
	int				up;
	int				down;
	int				right;
	int				left;
	int				forward;
	int				backward;
	int				rotate_up;
	int				rotate_down;
	int				rotate_right;
	int				rotate_left;
	int				reset;
}					t_action;

typedef struct s_vars
{
	void			*mlx;
	void			*mlx_win;
	int				win_width;
	int				win_height;
	t_img_data		*img_data;
	t_pixel_data	*pixel_data;
	t_rt			*rt;
	t_action		action;
	t_vars_ex		extra;
	int				is_cub;
}					t_vars;

typedef struct s_inter_aabb_vars
{
	double			tmin;
	double			tmax;
	double			inv_dir[3];
	double			t1;
	double			t2;
	int				i;
}					t_inter_aabb_vars;

typedef struct s_construct_bvh_vars
{
	double			best_cost;
	int				best_axis;
	int				best_split;
	double			cost;
	double			sa_root;
	int				tmp_l;
	int				tmp_r;
	int				tmp_m1;
	int				tmp_m2;
	double			tmp_c1;
	double			tmp_c2;
}					t_construct_bvh_vars;

// srcs/initialize/
void				validate_input(int argc, char **argv);
void				initialize(t_vars *vars, char *filename);
void				initialize_rt(t_vars *vars);
void				build_rt(t_vars *vars);
void				initialize_window_and_image(t_vars *vars);
int					allocate_pixel_data(t_vars *vars);
int					initialize_objs_xpm_and_texture(t_vars *vars);
int					set_obj_bump(t_vars *vars, t_obj *obj);
int					pixel_bump(t_obj *obj, unsigned int i, unsigned int j);
int					initialize_bvh(t_vars *vars);
int					calc_camera_step(t_vars *vars);
int					calc_objs_centroid(t_vars *vars);
int					initialize_all_pixels(t_vars *vars);
int					initialize_pixel_data(t_vars *vars,
						t_pixel_data *pixel_data);

int					validate_range(t_vars *vars);
int					validate_range_obj_color(t_vars *vars, t_obj *obj);
int					validate_range_light_color(t_vars *vars);
int					validate_range_sphere(t_sphere *sphere);
int					validate_range_cylinder(t_cylinder *cylinder);
int					validate_range_circle(t_circle *circle);
int					validate_dir_vec(t_vars *vars);

void				construct_bvh(t_obj **objects, int count, t_bvh *root);
void				construct_bvh_internal(t_obj **objects, int count,
						t_bvh *node);
void				update_best_split(t_construct_bvh_vars *cb_vars,
						double cost, int axis, int i);
double				surface_area(const t_aabb *bbox);
void				create_aabb_from_objects(t_obj **objects, int count,
						t_aabb *bbox);
void				empty_aabb(t_aabb *bbox);
void				merge_aabb(const t_aabb *bbox1, const t_aabb *bbox2,
						t_aabb *result);

// srcs/initialize/objects_coord/
int					init_obj_conv_coord(t_vars *vars, t_obj *obj);
int					initialize_sphere_conv_coord(t_obj *obj);
int					initialize_plane_conv_coord(t_obj *obj);
int					initialize_cylinder_conv_coord(t_obj *obj);
int					initialize_circle_conv_coord(t_obj *obj);
int					initialize_triangle_conv_coord(t_obj *obj);
int					initialize_square_conv_coord(t_obj *obj);
int					initialize_quad_conv_coord(t_obj *obj);

// srcs/render/
int					draw_img(t_vars *vars);
t_ray_cast_vars		create_ray_cast_vars(t_vars *vars, int pixel_index);
int					reset_pixel_data(t_pixel_data *pixel_data);
int					next_index_with_mosaic(int idx, int mosaic);
int					fill_neighbors(t_vars *vars, int si, int sj, int mosaic);
t_ray				ray_to_screen(t_vars *vars, int idx);
int					update_pixel_data(t_vars *vars, t_pixel_data *pixel_data);
void				process_pixel(t_vars *vars, t_pixel_data *pixel_data,
						t_vars_ex *extra);
int					update_mosaic_state(t_vars_ex *extra);
int					get_nearest_pos_data(t_vars *vars,
						t_ray_cast_vars *ray_cast_vars);
void				handle_direct_lighting(t_vars *vars,
						t_pixel_data *pixel_data,
						t_ray_cast_vars *ray_cast_vars);
void				handle_direct_lighting(t_vars *vars,
						t_pixel_data *pixel_data,
						t_ray_cast_vars *ray_cast_vars);
void				handle_reflection(t_vars *vars, t_pixel_data *pixel_data,
						t_ray_cast_vars *ray_cast_vars);
int					get_nearest_obj_and_t(t_vars *vars, t_ray *ray, t_obj **obj,
						double *t);
int					get_nearest_obj_and_t_addition(t_vars *vars, t_ray *ray,
						t_obj **obj, double *t);
int					in_shadow(t_vars *vars, t_vec3 *pos, t_light *light);

t_color				calc_obj_color(t_vars *vars,
						t_ray_cast_vars *ray_cast_vars);
t_color				calculate_ambient_lighting(t_vars *vars,
						t_ray_cast_vars *ray_cast_vars);
t_color				calculate_diffuse_lighting(t_vars *vars,
						t_ray_cast_vars *ray_cast_vars, t_light *light);
t_color				calculate_specular_lighting(t_vars *vars,
						t_ray_cast_vars *ray_cast_vars, t_light *light);
t_pixel_data		*get_data_with_ray_cast(t_vars *vars,
						t_pixel_data *pixel_data,
						t_ray_cast_vars *ray_cast_vars);
double				toonize(double value, t_vec3 *normal, t_vec3 *view_dir);
double				compute_attenuation(double distance);
t_color				final_color_correction(t_color color);
t_color				convert_checker(t_color color, int checkered);

int					get_normal_of_ray_intersection(t_ray_cast_vars \
						*ray_cast_vars, t_obj *obj);
void				get_normal_of_quad(t_quad *quad,
						t_ray_cast_vars *ray_cast_vars);
void				get_ellipsoid_normal(t_ray_cast_vars *ray_cast_vars);
void				get_hyperboloid_one_normal(t_ray_cast_vars *ray_cast_vars);
void				get_hyperboloid_two_normal(t_ray_cast_vars *ray_cast_vars);
void				get_cone_normal(t_ray_cast_vars *ray_cast_vars);
void				get_paraboloid_normal(t_ray_cast_vars *ray_cast_vars);
void				get_hyperbolic_paraboloid_normal(t_ray_cast_vars \
						*ray_cast_vars);

void				draw_mini_map(t_vars *vars);

// srcs/uv_utils/
void				convert_to_sph_uv(t_vec3 pos, double *u, double *v);
void				convert_to_cyl_uv(t_vec3 pos, double *u, double *v);
void				convert_to_pla_uv(t_vec3 pos, double *u, double *v,
						double side);

// srcs/utils/
int					iterate_objects(t_vars *vars, int (*func)(t_vars *,
							t_obj *));
int					free_rt(t_vars *vars);
void				del_obj(void *content);
int					close_window(t_vars *vars);
void				my_mlx_pixel_put(t_vars *vars, int i, int j,
						unsigned int color);
ssize_t				my_getline(int fd, char **line, size_t *len);
void				handle_error(t_vars *vars, const char *message);

int					set_camera_basis(t_vars *vars);
int					camera_move(t_vars *vars);
int					set_init_camera(t_vars *vars);
int					validate_camera(t_vars *vars);

t_camera			*new_camera(void);
t_ambient			*new_ambient(void);
t_light				*new_light(void);
t_obj				*new_obj(void *content, t_obj_type type);
t_bvh				*new_bvh(void);

t_sphere			*new_sphere(void);
t_plane				*new_plane(void);
t_circle			*new_circle(void);
t_cylinder			*new_cylinder(void);
t_triangle			*new_triangle(void);
t_square			*new_square(void);
t_cube				*new_cube(void);
t_quad				*new_quad(void);
t_aabb				*new_aabb(void);

void				print_camera(t_camera *camera, int step);
void				print_light(t_vars *vars);
void				print_bvh(t_bvh *bvh, int parent_num);
void				print_vec3(t_vec3 vec);
void				print_mat3x3(t_mat3x3 mat);

int					print_obj(t_vars *vars, t_obj *obj);
int					print_sphere(t_sphere *sphere);
int					print_plane(t_plane *plane);
int					print_circle(t_circle *circle);
int					print_cylinder(t_cylinder *cylinder);
int					print_triangle(t_triangle *triangle);
int					print_square(t_square *square);
int					print_cube(t_cube *cube);
int					print_quad(t_quad *quad);
int					print_aabb(t_aabb *aabb);

// srcs/mapping/
int					get_pla_mapping(t_ray_cast_vars *ray_cast_vars,
						t_texture *texture, unsigned int *x, unsigned int *y);
int					get_cyl_mapping(t_ray_cast_vars *ray_cast_vars,
						t_texture *texture, unsigned int *x, unsigned int *y);
int					get_sph_mapping(t_ray_cast_vars *ray_cast_vars,
						t_texture *texture, unsigned int *x, unsigned int *y);

// srcs/bump/
int					apply_bump_pla(t_ray_cast_vars *ray_cast_vars,
						t_texture *texture, unsigned int *x, unsigned int *y);
int					apply_bump_sph(t_ray_cast_vars *ray_cast_vars,
						t_texture *texture, unsigned int *x, unsigned int *y);
int					apply_bump_cyl(t_ray_cast_vars *ray_cast_vars,
						t_texture *texture, unsigned int *x, unsigned int *y);

// srcs/input/
int					key_press(int keycode, t_vars *vars);
int					key_release(int keycode, t_vars *vars);

int					key_press_init_camera(t_vars *vars);

int					key_press_translate(t_vars *vars);
int					penetrate(t_vars *vars, t_dir dir);
int					translate_up(t_vars *vars);
int					translate_down(t_vars *vars);
int					translate_right(t_vars *vars);
int					translate_left(t_vars *vars);
int					translate_forward(t_vars *vars);
int					translate_backward(t_vars *vars);

int					key_press_rotate(t_vars *vars);
int					rotate_up(t_vars *vars);
int					rotate_down(t_vars *vars);
int					rotate_right(t_vars *vars);
int					rotate_left(t_vars *vars);

int					key_press_put_light(int keycode, t_vars *vars);
int					key_press_put_cube(int keycode, t_vars *vars);

int					mouse_press(int button, int j, int i, t_vars *vars);
int					mouse_release(int button, int j, int i, t_vars *vars);
int					mouse_move(int j, int i, t_vars *vars);
int					mouse_press_zoom(int button, int x, int y, t_vars *vars);
int					mouse_press_remove_obj(int button, int x, int y,
						t_vars *vars);

// srcs/parse_file/
int					open_file(const char *filename);
int					parse_file(t_vars *vars, const char *filename);
int					parse_line(t_vars *vars, char *line);

// srcs/my_math/
void				solve_quadratic(t_vec3 abc, double *root1, double *root2);
int					is_checker_reverse(t_ray_cast_vars *ray_cast_vars);
void				convert_to_local_space(t_obj *obj, t_ray *ray,
						t_vec3 *origin, t_vec3 *dir);

// srcs/my_math/vec3/
t_vec3				vec3_create(double x, double y, double z);
t_vec3				vec3_zero(void);
t_vec3				vec3_scale(t_vec3 a, double t);
t_vec3				vec3_add(t_vec3 a, t_vec3 b);
t_vec3				vec3_sub(t_vec3 a, t_vec3 b);
double				vec3_dot(t_vec3 a, t_vec3 b);
t_vec3				vec3_cross(t_vec3 a, t_vec3 b);
double				vec3_len(t_vec3 a);
t_vec3				vec3_unit(t_vec3 a);
t_vec3				vec3_neg(t_vec3 a);
t_vec3				vec3_min(t_vec3 a, t_vec3 b);
t_vec3				vec3_max(t_vec3 a, t_vec3 b);
t_vec3				vec3_reflect(t_vec3 v, t_vec3 n);
t_vec3				vec3_normal_of_vec3(t_vec3 normal);
int					vec3_is_zero(t_vec3 vec);

// srcs/my_math/mat3x3/
t_mat3x3			mat3x3_e(void);
t_mat3x3			mat3x3_scale(t_mat3x3 mat, double scale);
t_mat3x3			mat3x3_inv(t_mat3x3 mat);
t_vec3				mat3x3_mul_vec3(t_mat3x3 mat, t_vec3 vec);
t_vec3				mat3x3_tr_mul_vec3(t_mat3x3 mat, t_vec3 vec);
t_mat3x3			mat3x3_mul_mat3x3(t_mat3x3 a, t_mat3x3 b);
t_mat3x3			mat3x3_rodrigues(t_vec3 axis, double theta);
t_vec3				vec3_rotate(t_vec3 vec, t_vec3 axis, double theta);
t_mat3x3			mat3x3_v2v(t_vec3 before, t_vec3 after);

// srcs/my_math/intersection/
double				intersection_obj(t_obj *obj, t_ray *ray);
double				intersection_plane(t_plane *plane, t_ray *ray);
double				intersection_aabb(t_aabb *aabb, t_ray *ray);
t_obj				**objs_to_array(t_list *lst, int count);

double				inter_pla_coord(t_obj *obj, t_ray *ray);
double				inter_tri_coord(t_obj *obj, t_ray *ray);
double				inter_squ_coord(t_obj *obj, t_ray *ray);
double				inter_cyl_coord(t_obj *obj, t_ray *ray);
double				inter_sph_coord(t_obj *obj, t_ray *ray);
double				inter_cir_coord(t_obj *obj, t_ray *ray);
double				inter_quad_coord(t_obj *obj, t_ray *ray);

// srcs/my_math/aabb/
t_aabb				*aabb_new(t_vec3 min, t_vec3 max);
int					calc_aabb_diameter(t_aabb *aabb);
double				calc_aabb_surface_area(t_aabb *aabb);
t_aabb				*aabb_merge(t_aabb *box0, t_aabb *box1);

int					set_aabb_of_obj(t_vars *vars, t_obj *obj);
int					set_aabb_of_sphere(t_obj *obj);
int					set_aabb_of_plane(t_obj *obj);
int					set_aabb_of_cylinder(t_obj *obj);
int					set_aabb_of_circle(t_obj *obj);
int					set_aabb_of_triangle(t_obj *obj);
int					set_aabb_of_square(t_obj *obj);
int					set_aabb_of_aabb(t_obj *obj);
// srcs/color/
t_color				color_create(double r, double g, double b);
t_color				color_add(t_color c1, t_color c2);
t_color				color_clamp(t_color c);
t_color				color_add_clamped(t_color c1, t_color c2);
unsigned int		color_add_clamped_uint(unsigned int color1,
						unsigned int color2);
t_color				color_lerp(t_color c1, t_color c2, double t);
t_color				color_mul(t_color c1, t_color c2);
t_color				color_scale_clamped(t_color c, double t);
unsigned int		color_to_uint(t_color c);
t_color				uint_to_color(unsigned int color);

// srcs/list/
t_list				*list_new(void *content);
void				list_add_front(t_list **lst, t_list *new);
void				list_clear(t_list **lst, void (*del)(void *));
size_t				list_size(t_list *lst);

void				print_error(const char *msg);

#endif