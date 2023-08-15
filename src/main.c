#include "../include/fdf.h"

int ft_get_line_length(int fd)
{
    char buffer[1];
    int nbBytesRead;
    int lineLength;

    buffer[0] = 0;
    lineLength = 0;
    nbBytesRead = 1;
    while (nbBytesRead != 0)
    {
        nbBytesRead = read(fd, buffer, 1);
        if (buffer[0] == '\n' || nbBytesRead == 0)
            break;
        lineLength++;
    }
    return (lineLength);
}

int ft_get_number_of_lines(int fd)
{
    char buffer[1];
    int nbLines;

    nbLines = 0;

    while (read(fd, buffer, 1) != 0)
    {
        if ((buffer[0] == '\n'))
        {
            nbLines++;
        }
    }
    nbLines++;
    return (nbLines);
}

int main(int argc, char **argv)
{
    int fd;

    if (argc != 2)
        return (0);
    fd = open(fd, O_RDONLY);
    if (fd < 0)
        return (0);
    
    close(fd);
    
}