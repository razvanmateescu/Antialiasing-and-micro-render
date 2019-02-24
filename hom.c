#include "homework.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
	int thread_id;
	int begin;
	int final;
} clock2;

int num_threads;
int resize_factor;
image *ing, *outg;


void readInput(const char * fileName, image *img) {
	FILE *fp;
	fp=fopen(fileName,"r");
	if (fp == NULL) {
		printf("Couldn't open file\n");
		exit(-1);
	}

	fscanf(fp, "%s\n", img->type);
	fscanf(fp,"%d%d\n", &(img->width), &(img->height));
	fscanf(fp, "%d\n",&(img->valoare));
	//for (int i = 0; i < 3; i++) {
		//for (int j = 0; j < 3; j++) {
		//	img->Gaus[]
		//}
	//}
	img->Gaus[0][0] = 1;
	img->Gaus[0][1] = 2;
	img->Gaus[0][2] = 1;
	img->Gaus[1][0] = 2;
	img->Gaus[1][1] = 4;
	img->Gaus[1][2] = 2;
	img->Gaus[2][0] = 1;
	img->Gaus[2][1] = 2;
	img->Gaus[2][2] = 1;

	if (img->type[1] == '5') {
		img->mat = malloc(sizeof(unsigned char*) * img->height);
		for (int i = 0; i < img->height; i++) {
			img->mat[i] = malloc(sizeof(unsigned char) * img->width);
			fread(img->mat[i], sizeof(unsigned char), img->width, fp);
		}
	} else {
		img->mat2 = malloc(sizeof(culoare*) * img->height);
		for (int i = 0; i < img->height; i++) {
			img->mat2[i] = malloc(sizeof(culoare) * img->width);
			fread(img->mat2[i], sizeof(culoare), img->width, fp);
		}
	}
	fclose(fp);
}

void writeData(const char * fileName, image *img) {
	FILE *fp;
	fp = fopen(fileName, "w");
	fprintf(fp, "%s\n", img->type);
	fprintf(fp,"%d %d\n", img->width, img->height);
	fprintf(fp, "%d\n",img->valoare);
	
	if (img->type[1] == '5') {
		for (int i = 0; i < img->height; i++) {
			fwrite(img->mat[i], sizeof(unsigned char), img->width, fp);
		}
	} else {
		for (int i = 0; i < img->height; i++) {
			fwrite(img->mat2[i], sizeof(culoare), img->width, fp);
		}
	}

	fclose(fp);
}

void* albnegru(void *var) {
	clock2 thread_id = *(clock2*) var;
	for (int i = thread_id.begin; i < thread_id.final; i++) {
		for (int j = 0; j < outg->width; j++) {
			int sum = 0;
		if ( resize_factor % 2 == 0) {
				for (int k = i * resize_factor; k < (i + 1) * resize_factor; k++) {
					for (int l = j * resize_factor; l < (j + 1) * resize_factor; l++) {
						sum = sum + ing->mat[k][l];
					}
				}
				outg->mat[i][j] = sum / (resize_factor * resize_factor);
			}
			else {
				int a=0,b=0;
				for (int k = i * resize_factor; k < (i + 1) * resize_factor; k++) {
					for (int l = j * resize_factor; l < (j + 1) * resize_factor; l++) {
						sum = sum + ing->mat[k][l] * ing->Gaus[a][b];
						a++;
				    }
					b++;
					a = 0;
				}
				outg->mat[i][j] = sum / 16;
			}
		}

	}
	printf("%d\n", ing->Gaus[0][0]);
}

void* culoareim(void *var) {
	clock2 thread_id = *(clock2*) var;
	for (int i = thread_id.begin; i < thread_id.final; i++) {
		for (int j = 0; j < outg->width; j++) {
			int sumr = 0, sumg = 0, sumb =  0;
		if ( resize_factor % 2 == 0) {
			for (int k = i * resize_factor; k < (i + 1) * resize_factor; k++) {
				for (int l = j * resize_factor; l < (j + 1) * resize_factor; l++) {
					sumr = sumr + ing->mat2[k][l].red;
					sumg = sumg + ing->mat2[k][l].green;
					sumb = sumb + ing->mat2[k][l].blue;
				}
			}
			outg->mat2[i][j].red = sumr / (resize_factor * resize_factor);
			outg->mat2[i][j].green = sumg / (resize_factor * resize_factor);
			outg->mat2[i][j].blue = sumb / (resize_factor * resize_factor);
		}
		else {
			int a=0, b=0;
			for (int k = i * resize_factor; k < (i + 1) * resize_factor; k++) {
					for (int l = j * resize_factor; l < (j + 1) * resize_factor; l++) {
						sumr = sumr + ing->mat2[k][l].red * ing->Gaus[a][b];
						sumg = sumg + ing->mat2[k][l].green * ing->Gaus[a][b];
						sumb = sumb + ing->mat2[k][l].blue * ing->Gaus[a][b];
						a++;
					}
					b++;
					a = 0;
			}
			outg->mat2[i][j].red = sumr / 16;
			outg->mat2[i][j].green = sumg / 16;
			outg->mat2[i][j].blue = sumb / 16;

		}
		}

	}	

}

void resize(image *in, image * out) {
	ing = in;
	outg = out;
	strcpy(out->type, in->type);
	out->height = in->height / resize_factor;
	out->width = in->width / resize_factor;
	out->valoare = in->valoare;
	if (in->type[1] == '5') {
		out->mat = malloc(sizeof(unsigned char*) * out->height);
		for (int i = 0; i < out->height; i++) {
			out->mat[i] = malloc(sizeof(unsigned char) * out->width);
		}

		pthread_t tid[num_threads];
		clock2 thread_id[num_threads];
		for (int i = 0; i < num_threads; i++) {
			thread_id[i].thread_id = i;
			thread_id[i].begin = i * (out->height / num_threads);
			thread_id[i].final = (i + 1) * (out->height / num_threads);
			if (i == num_threads - 1) {
				thread_id[i].final = out->height;
			}

		}
		for (int i = 0; i < num_threads; i++) {
			pthread_create(&(tid[i]), NULL, albnegru, &(thread_id[i]));
		}

		for(int i = 0; i < num_threads; i++) {
			pthread_join(tid[i], NULL);
		}
	} else {
		out->mat2 = malloc(sizeof(culoare*) * out->height);
		for (int i = 0; i < out->height; i++) {
			out->mat2[i] = malloc(sizeof(culoare) * out->width);
		}
		pthread_t tid[num_threads];
		clock2 thread_id[num_threads];
		for (int i = 0; i < num_threads; i++) {
			thread_id[i].thread_id = i;
			thread_id[i].begin = i * (out->height / num_threads);
			thread_id[i].final = (i + 1) * (out->height / num_threads);
			if (i == num_threads - 1) {
				thread_id[i].final = out->height;
			}

		}
		for (int i = 0; i < num_threads; i++) {
			pthread_create(&(tid[i]), NULL, culoareim, &(thread_id[i]));
		}

		for(int i = 0; i < num_threads; i++) {
			pthread_join(tid[i], NULL);
		}
	}
}

