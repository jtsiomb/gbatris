#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "image.h"
#include "dynarr.h"

void print_usage(const char *argv0);
int proc_image(const char *fname);
void wrtiles_carr(FILE *fp, const char *name, struct image *timg, int ntiles);
void wrtilemap_carr(FILE *fp, const char *name, int *tmap, int xtiles, int ytiles);
void wrpalette_carr(FILE *fp, const char *name, struct cmapent *cmap, int ncol);

enum { OUT_IMG, OUT_C, OUT_ASM } outmode;
static const char *otype_suffix[] = {".png", ".c", ".s"};

int tile_xsz = 8, tile_ysz = 8;
const char *output_filename;

int main(int argc, char **argv)
{
	int i;

	for(i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(argv[i][2] == 0) {
				switch(argv[i][1]) {
				case 'o':
					output_filename = argv[++i];
					break;

				case 't':
					if(sscanf(argv[++i], "%dx%d", &tile_xsz, &tile_ysz) != 2) {
						fprintf(stderr, "-t must be followed by the tile size (WxH)\n");
						return 1;
					}
					break;

				case 'c':
					outmode = OUT_C;
					break;

				case 's':
					outmode = OUT_ASM;
					break;

				case 'h':
					print_usage(argv[0]);
					return 0;

				default:
					fprintf(stderr, "invalid option: %s\n", argv[i]);
					print_usage(argv[0]);
					return 1;
				}
			} else {
				fprintf(stderr, "invalid option: %s\n", argv[i]);
				print_usage(argv[0]);
				return 1;
			}

		} else {
			if(proc_image(argv[i]) == -1) {
				return 1;
			}
		}
	}

	return 0;
}

void print_usage(const char *argv0)
{
	printf("Usage: %s [options] <img1> [<img2> ... <imgN>]\n", argv0);
	printf("Options:\n");
	printf(" -o <file>: output file\n");
	printf(" -t WxH: tile size (default 8x8)\n");
	printf(" -c: output C array\n");
	printf(" -s: output GNU assembler data\n");
	printf(" -h: print usage and exit\n\n");
}

int find_tile(struct image *tile, struct image *tiles)
{
	int i, count;
	count = dynarr_size(tiles);
	for(i=0; i<count; i++) {
		if(cmp_image(tile, tiles + i) == 0) {
			return i;
		}
	}
	return -1;
}

int proc_image(const char *fname)
{
	int i, j, k, idx, xtiles, ytiles, ntiles, result = -1;
	FILE *fp;
	struct image img, tile;
	struct image *tiles = 0;
	unsigned char *tiles_pixels, *tptr;
	unsigned char *sptr;
	int *tilemap = 0, *mapptr;
	char *basename, *suffix, *outfile;

	if(load_image(&img, fname) == -1) {
		fprintf(stderr, "failed to load image: %s\n", fname);
		return -1;
	}
	basename = alloca(strlen(fname) + 1);
	strcpy(basename, fname);
	if((suffix = strrchr(basename, '/'))) {
		basename = suffix + 1;
	}
	if((suffix = strchr(basename, '.'))) {
		*suffix = 0;
	}

	xtiles = img.width / tile_xsz;
	ytiles = img.height / tile_ysz;
	if(img.width % tile_xsz != 0 || img.height % tile_ysz != 0) {
		fprintf(stderr, "image size (%dx%d) not evenly divisible into %dx%d tiles\n",
				img.width, img.height, tile_xsz, tile_ysz);
		goto err;
	}

	if(!(tilemap = malloc(xtiles * ytiles * sizeof *tilemap))) {
		fprintf(stderr, "failed to allocate %dx%d tilemap\n", xtiles, ytiles);
		goto err;
	}
	mapptr = tilemap;

	if(!(tiles = dynarr_alloc(0, sizeof *tiles))) {
		fprintf(stderr, "failed to allocate tile array\n");
		goto err;
	}
	/* alloc a contiguous buffer for the full tileset pixels, to make it easier to write it
	 * out as a single image in the end
	 */
	if(!(tiles_pixels = dynarr_alloc(0, tile_xsz * tile_ysz * img.bpp / 8))) {
		fprintf(stderr, "failed to allocate tile pixel buffer\n");
		goto err;
	}

	tile = img;
	tile.width = tile_xsz;
	tile.height = tile_ysz;
	tile.scansz = tile_xsz * tile.bpp / 8;

	sptr = img.pixels;
	for(i=0; i<ytiles; i++) {
		for(j=0; j<xtiles; j++) {
			tile.pixels = sptr;

			if((idx = find_tile(&tile, tiles)) == -1) {
				/* we don't have a duplicate of this tile */
				idx = dynarr_size(tiles);

				if(!(tiles = dynarr_push(tiles, 0))) {
					goto err;
				}
				if(!(tptr = dynarr_push(tiles_pixels, 0))) {
					goto err;
				}

				tiles[idx] = tile;
				tiles[idx].scansz = tiles[idx].pitch = tile.scansz;

				/* did the array get relocated? */
				if(tptr != tiles_pixels) {
					tiles_pixels = tptr;
					/* make each tile's pixels pointer point to the right place in the large pixelbuffer */
					for(k=0; k<idx+1; k++) {
						tiles[k].pixels = tptr;
						tptr += tile_ysz * tiles[idx].pitch;
					}
				} else {
					/* otherwise just set the new one */
					tiles[idx].pixels = tiles_pixels + idx * tile_ysz * tiles[idx].pitch;
				}

				blit(&tile, 0, 0, tile_xsz, tile_ysz, tiles + idx, 0, 0);
			}

			*mapptr++ = idx;

			sptr += tile.scansz;
		}
		sptr += (tile_ysz - 1) * tile.pitch;
	}

	ntiles = dynarr_size(tiles);
	fprintf(stderr, "%s (%dx%d) -> %d tiles, %d unique\n", fname, img.width, img.height,
			xtiles * ytiles, ntiles);

	assert(ntiles > 0);

	/* make a big image out of the tiles and write it out */
	tile = tiles[0];
	tile.height = ntiles * tile_ysz;

	if(output_filename) {
		outfile = (char*)output_filename;
	} else {
		outfile = alloca(strlen(basename) + 5);
		sprintf(outfile, "%s%s", basename, otype_suffix[outmode]);
	}

	switch(outmode) {
	case OUT_IMG:
		if(save_image(&tile, outfile) == -1) {
			fprintf(stderr, "failed to write output image\n");
			goto err;
		}
		break;

	case OUT_C:
		if(!(fp = fopen(outfile, "w"))) {
			fprintf(stderr, "failed to open output file: %s: %s\n", outfile, strerror(errno));
			goto err;
		}
		wrtiles_carr(fp, basename, &tile, ntiles);
		wrtilemap_carr(fp, basename, tilemap, xtiles, ytiles);
		wrpalette_carr(fp, basename, tile.cmap, tile.cmap_ncolors);
		break;

	case OUT_ASM:
		/* TODO */
		break;
	}

	result = 0;

err:
	dynarr_free(tiles_pixels);
	dynarr_free(tiles);
	free(tilemap);
	free(img.pixels);
	return result;
}

void wrtiles_carr(FILE *fp, const char *name, struct image *timg, int ntiles)
{
	int i, j, curx = 0;
	unsigned char *ptr = timg->pixels;

	fprintf(fp, "\nint %s_num_tiles = %d;\n", name, ntiles);
	fprintf(fp, "int %s_tiles_width = %d;\n", name, timg->width);
	fprintf(fp, "int %s_tiles_height = %d;\n", name, timg->height);
	fprintf(fp, "int %s_tiles_bpp = %d;\n", name, timg->bpp);
	fprintf(fp, "unsigned char %s_tiles[] = {\n", name);

	for(i=0; i<timg->height; i++) {
		for(j=0; j<timg->scansz; j++) {
			if(curx == 0) {
				curx = 3 + fprintf(fp, "\t%u", (unsigned int)*ptr++);
			} else {
				curx += fprintf(fp, ", %u", (unsigned int)*ptr++);
			}
			if(curx >= 80) {
				fprintf(fp, ",\n");
				curx = 0;
			}
		}
		ptr += timg->pitch - timg->scansz;
	}

	fprintf(fp, "\n};\n\n");
}

void wrtilemap_carr(FILE *fp, const char *name, int *tmap, int xtiles, int ytiles)
{
	int i, sz, curx = 0;

	fprintf(fp, "\nint %s_tilemap_cols = %d;\n", name, xtiles);
	fprintf(fp, "int %s_tilemap_rows = %d;\n", name, ytiles);
	fprintf(fp, "unsigned int %s_tilemap[] = {\n", name);

	sz = xtiles * ytiles;
	for(i=0; i<sz; i++) {
		if(curx == 0) {
			curx = 3 + fprintf(fp, "\t%u", (unsigned int)tmap[i]);
		} else {
			curx += fprintf(fp, ", %u", (unsigned int)tmap[i]);
		}
		if(curx >= 80) {
			fprintf(fp, ",\n");
			curx = 0;
		}
	}

	fprintf(fp, "\n};\n\n");
}

void wrpalette_carr(FILE *fp, const char *name, struct cmapent *cmap, int ncol)
{
	int i;

	fprintf(fp, "\nint %s_cmap_colors = %d;\n", name, ncol);
	fprintf(fp, "unsigned char %s_cmap[][3] = {\n", name);
	for(i=0; i<ncol; i++) {
		fprintf(fp, "\t{%u, %u, %u}", cmap[i].r, cmap[i].g, cmap[i].b);
		if(i < ncol - 1) {
			fputs(",\n", fp);
		} else {
			fputc('\n', fp);
		}
	}
	fprintf(fp, "};\n\n");
}
