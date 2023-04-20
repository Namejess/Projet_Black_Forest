void iso_3d_to_2d(int x, int y, int z, int *i, int *j, int tile_width, int tile_height)
{
    *i = (x - y) * tile_width / 2;
    *j = (x + y) * tile_height / 2 - z * tile_height;
}