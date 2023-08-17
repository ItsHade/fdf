#include "../include/fdf.h"

//0XAARRGGBB
//AA = alpha, RR = red, GG = green, BB = blue
//00 = 0 | FF = 255
#define RED 0X00FF0000
#define BLUE 0X000000FF
#define GREEN 0X0000FF00
#define BLACK 0X00000000
#define WHITE 0X00FFFFFF

#define WIDTH 1280
#define HEIGHT 720

typedef struct
{
	void *img;
	char *addr;
	int bits_per_pixels;
	int line_length;
	int endian;
}	t_data;

//y = ordonne / x = abscisse
typedef struct
{
    void *mlx;
    void *win;
    t_data img;
    char **map;
    int map_y;
    int map_x;
    char *file;
    
} parameters;

void ft_mlx_pixel_put(t_data *img, int x, int y, int color)
{
	char *dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixels / 8));
	*(unsigned int*)dst = color;
}

int ft_get_map_x(parameters *par)
{
    char buffer[1];
    int nbBytesRead;
    int inNum;
    int fd;

    buffer[0] = 0;
    par->map_x = 0;
    nbBytesRead = 1;
    inNum = 0;
    fd = open(par->file, O_RDONLY);
    if (fd < 0)
        return (-1);
    while (nbBytesRead != 0)
    {
        nbBytesRead = read(fd, buffer, 1);
        if (buffer[0] == '\n' || nbBytesRead == 0)
            break;
        if (buffer[0] != ' ' && inNum == 0)
        {
            inNum = 1;
            par->map_x++;
        }
        else if (buffer[0] == ' ')
            inNum = 0;
    }
    ft_putnbr(par->map_x);
    ft_putchar('\n');
    close(fd);
    return (0);
}

//a verifier si pas de ligne vide a la fin du fichier
int ft_get_map_y(parameters *par)
{
    char buffer[1];
    int fd;

    fd = open(par->file, O_RDONLY);
    if (fd < 0)
        return (-1);
    par->map_y = 0;
    while (read(fd, buffer, 1) != 0)
    {
        if ((buffer[0] == '\n'))
            par->map_y++;
    }
    close(fd);
    return (0);
}

void ft_create_square(t_data *img, int pos_x, int pos_y, int size, int color)
{
	int x;
	int y;

	x = 0;
	while (x < size)
	{
		y = 0;
		while (y < size)
		{
			ft_mlx_pixel_put(img, pos_x + x, pos_y + y, color);
			y++;
		}
		x++;
	}
}


void ft_put_matrix_to_img(parameters *par, int pos_x, int pos_y)
{
    int y;
    int x;

    y = 0;
    while (y < par->map_y)
    {
        x = 0;
        while (x < par->map_x)
        {
            if (par->map[y][x] == '1')
                ft_mlx_pixel_put(&par->img, pos_x + x, pos_y + y, RED);
            else
                ft_mlx_pixel_put(&par->img, pos_x + x, pos_y + y, BLUE);
            x++;
        }
        y++;
    }
}

void ft_display_matrix(parameters *par)
{
    int y;
    int x;

    y = 0;
    while (y < par->map_y)
    {
        x = 0;
        while (x < par->map_x)
        {
            ft_putchar('[');
            ft_putchar(par->map[y][x]);
            ft_putchar(']');
            x++;
        }
        ft_putchar('\n');
        y++;
    }
}

//doesn't work for maps with color
void ft_fill_map(parameters *par)
{
    int y;
    int x;
    int fd;
    char buffer[1];
    int nbBytesRead;

    y = 0;
    nbBytesRead = 1;
    fd = open(par->file, O_RDONLY);
    if (fd < 0)
        return (-1);
    while (y < par->map_y)
    {
        x = 0;
        while(x < par->map_x)
        {
            nbBytesRead = (fd, buffer, 1);
            if(nbBytesRead == '\n')
                y++;
            if(buffer[0] == ' ')
                x++;
            
        }
    }
}

void ft_get_map(parameters *par)
{
    int y;
    int x;
    
    y = 0;
    par->map = malloc(sizeof(char*) * par->map_x);
    while (y < par->map_y)
    {
        par->map[y] = malloc(sizeof(char) * par->map_y);
        x = 0;
        while (x < par->map_x)
        {
            par->map[y][x] = '0';
            x++;
        }
        y++;
    }
}

void ft_free_matrix(parameters *par)
{
    int y;
    
    y = 0;
    while (y < par->map_y)
    {
        free(par->map[y]);
        y++;
    }
    free(par->map);
}

int ft_exit(parameters *par)
{
    ft_free_matrix(par);
    mlx_destroy_image(par->mlx, par->img.img);
    mlx_destroy_window(par->mlx, par->win);
    mlx_destroy_display(par->mlx);
    free(par->mlx);
    exit(0);
    return (0);
}

int ft_keyboard_hook(int keycode, parameters *par)
{
	if (keycode == 65307)
		ft_exit(par);
}

int main(int argc, char **argv)
{
    parameters par;

    if (argc != 2)
        return (0);
    par.file = argv[1];
    ft_putstr(par.file);
    ft_get_map_y(&par);
    ft_get_map_x(&par);
    ft_putstr("y: ");
    ft_putnbr(par.map_y);
    ft_putchar('\n');
    ft_putstr("x: ");
    ft_putnbr(par.map_x);
    ft_putchar('\n');
    ft_get_map(&par);
    ft_display_matrix(&par);
    par.mlx = mlx_init();
    par.win = mlx_new_window(par.mlx, WIDTH, HEIGHT, "fdf");
    par.img.img = mlx_new_image(par.mlx, WIDTH, HEIGHT);
    par.img.addr = mlx_get_data_addr(par.img.img, &par.img.bits_per_pixels, &par.img.line_length, &par.img.endian);
    ft_create_square(&par.img, 20, 20, 20, WHITE);
    ft_put_matrix_to_img(&par, 20, 20);
    mlx_put_image_to_window(par.mlx, par.win, par.img.img, 0, 0);
    mlx_loop_hook(par.mlx, NULL, NULL);
	mlx_hook(par.win, 17, 0, ft_exit, &par); // button fermer la fenetre
	mlx_hook(par.win, 2, 1L<<0, ft_keyboard_hook, &par);
    mlx_loop(par.mlx);
}