#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gs = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float)3);

            image[i][j].rgbtBlue = gs;
            image[i][j].rgbtGreen = gs;
            image[i][j].rgbtRed = gs;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, k = width - 1; j < width / 2; j++, k--)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int counter;

    RGBTRIPLE tmp[height][width];

    RGBTRIPLE avg[3][3];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            counter = 9;

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    avg[x][y].rgbtBlue = 0;
                    avg[x][y].rgbtGreen = 0;
                    avg[x][y].rgbtRed = 0;
                }
            }

            for (int r = i - 1, rc = 0; rc < 3; r++, rc++)
            {
                for (int c = j - 1, cc = 0; cc < 3; c++, cc++)
                {
                    if (r < 0 || c < 0 || r >= height || c >= width)
                    {
                        counter--;
                    }
                    else
                    {
                        avg[rc][cc].rgbtBlue = image[r][c].rgbtBlue;
                        avg[rc][cc].rgbtGreen = image[r][c].rgbtGreen;
                        avg[rc][cc].rgbtRed = image[r][c].rgbtRed;
                    }
                }
            }

            tmp[i][j].rgbtBlue = round((avg[0][0].rgbtBlue + avg[0][1].rgbtBlue + avg[0][2].rgbtBlue + avg[1][0].rgbtBlue + avg[1][1].rgbtBlue +
                                        avg[1][2].rgbtBlue + avg[2][0].rgbtBlue + avg[2][1].rgbtBlue + avg[2][2].rgbtBlue) / (float)counter);
            tmp[i][j].rgbtGreen = round((avg[0][0].rgbtGreen + avg[0][1].rgbtGreen + avg[0][2].rgbtGreen + avg[1][0].rgbtGreen +
                                         avg[1][1].rgbtGreen + avg[1][2].rgbtGreen + avg[2][0].rgbtGreen + avg[2][1].rgbtGreen +
                                         avg[2][2].rgbtGreen) / (float)counter);
            tmp[i][j].rgbtRed = round((avg[0][0].rgbtRed + avg[0][1].rgbtRed + avg[0][2].rgbtRed + avg[1][0].rgbtRed + avg[1][1].rgbtRed +
                                       avg[1][2].rgbtRed + avg[2][0].rgbtRed + avg[2][1].rgbtRed + avg[2][2].rgbtRed) / (float)counter);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gx[3][3][3];
            int gy[3][3][3];

            for (int r = i - 1, rc = 0; rc < 3; r++, rc++)
            {
                for (int c = j - 1, cc = 0; cc < 3; c++, cc++)
                {
                    if (r < 0 || c < 0 || r >= height || c >= width || c == j)
                    {
                        gx[rc][cc][0] = 0;
                        gx[rc][cc][1] = 0;
                        gx[rc][cc][2] = 0;
                    }
                    else if (r != i)
                    {
                        if (c < j)
                        {
                            gx[rc][cc][0] = (int)image[r][c].rgbtBlue * -1;
                            gx[rc][cc][1] = (int)image[r][c].rgbtGreen * -1;
                            gx[rc][cc][2] = (int)image[r][c].rgbtRed * -1;
                        }
                        else if (c > j)
                        {
                            gx[rc][cc][0] = (int)image[r][c].rgbtBlue * 1;
                            gx[rc][cc][1] = (int)image[r][c].rgbtGreen * 1;
                            gx[rc][cc][2] = (int)image[r][c].rgbtRed * 1;
                        }
                    }
                    else if (r == i)
                    {
                        if (c < j)
                        {
                            gx[rc][cc][0] = (int)image[r][c].rgbtBlue * -2;
                            gx[rc][cc][1] = (int)image[r][c].rgbtGreen * -2;
                            gx[rc][cc][2] = (int)image[r][c].rgbtRed * -2;
                        }
                        else if (c > j)
                        {
                            gx[rc][cc][0] = (int)image[r][c].rgbtBlue * 2;
                            gx[rc][cc][1] = (int)image[r][c].rgbtGreen * 2;
                            gx[rc][cc][2] = (int)image[r][c].rgbtRed * 2;
                        }
                    }
                }
            }

            for (int r = i - 1, rc = 0; rc < 3; r++, rc++)
            {
                for (int c = j - 1, cc = 0; cc < 3; c++, cc++)
                {
                    if (r < 0 || c < 0 || r >= height || c >= width || r == i)
                    {
                        gy[rc][cc][0] = 0;
                        gy[rc][cc][1] = 0;
                        gy[rc][cc][2] = 0;
                    }
                    else if (c != j)
                    {
                        if (r < i)
                        {
                            gy[rc][cc][0] = (int)image[r][c].rgbtBlue * -1;
                            gy[rc][cc][1] = (int)image[r][c].rgbtGreen * -1;
                            gy[rc][cc][2] = (int)image[r][c].rgbtRed * -1;
                        }
                        else if (r > i)
                        {
                            gy[rc][cc][0] = (int)image[r][c].rgbtBlue * 1;
                            gy[rc][cc][1] = (int)image[r][c].rgbtGreen * 1;
                            gy[rc][cc][2] = (int)image[r][c].rgbtRed * 1;
                        }
                    }
                    else if (c == j)
                    {
                        if (r < i)
                        {
                            gy[rc][cc][0] = (int)image[r][c].rgbtBlue * -2;
                            gy[rc][cc][1] = (int)image[r][c].rgbtGreen * -2;
                            gy[rc][cc][2] = (int)image[r][c].rgbtRed * -2;
                        }
                        else if (r > i)
                        {
                            gy[rc][cc][0] = (int)image[r][c].rgbtBlue * 2;
                            gy[rc][cc][1] = (int)image[r][c].rgbtGreen * 2;
                            gy[rc][cc][2] = (int)image[r][c].rgbtRed * 2;
                        }
                    }
                }
            }

            int gx_total[3] = {0, 0, 0};
            int gy_total[3] = {0, 0, 0};

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    gx_total[0] += gx[x][y][0];
                    gx_total[1] += gx[x][y][1];
                    gx_total[2] += gx[x][y][2];

                    gy_total[0] += gy[x][y][0];
                    gy_total[1] += gy[x][y][1];
                    gy_total[2] += gy[x][y][2];
                }
            }

            if (sqrt((gx_total[0] * gx_total[0]) + (gy_total[0] * gy_total[0])) > 255)
            {
                tmp[i][j].rgbtBlue = 255;
            }
            else
            {
                tmp[i][j].rgbtBlue = round(sqrt((gx_total[0] * gx_total[0]) + (gy_total[0] * gy_total[0])));
            }

            if (sqrt((gx_total[1] * gx_total[1]) + (gy_total[1] * gy_total[1])) > 255)
            {
                tmp[i][j].rgbtGreen = 255;
            }
            else
            {
                tmp[i][j].rgbtGreen = round(sqrt((gx_total[1] * gx_total[1]) + (gy_total[1] * gy_total[1])));
            }

            if (sqrt((gx_total[2] * gx_total[2]) + (gy_total[2] * gy_total[2])) > 255)
            {
                tmp[i][j].rgbtRed = 255;
            }
            else
            {
                tmp[i][j].rgbtRed = round(sqrt((gx_total[2] * gx_total[2]) + (gy_total[2] * gy_total[2])));
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }

    return;
}
