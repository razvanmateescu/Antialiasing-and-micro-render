#include "homework1.h"
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
int resolution;
image *outg;

void initialize(image *im) {
	strcpy(im->type, "P5");
	im->width = resolution;
	im->valoare = 255;

	im->mat = malloc(sizeof(unsigned char*) * im->width);
		for (int i = 0; i < im->width; i++) {
			im->mat[i] = malloc(sizeof(unsigned char) * im->width);
		}

}

void* dunga(void *var) {
	int d;
	clock2 thread_id = *(clock2*) var;
	for (int i = thread_id.begin; i < thread_id.final; i++) {
		for (int j = 0; j < outg->width; j++) {
			d = abs(2 *(outg->width -( i + 0.5)) * 100 / outg->width -1 * (j + 0.5) * 100 / outg->width ) / sqrt(5);
			if (d < 3)
			{
				
				outg->mat[i][j] = 0;

				
			}
			else 
			{
				
				outg->mat[i][j] = 255;
				
				
			}
			d=0;

		}
	}
}

void render(image *im) {
	outg = im;

	pthread_t tid[num_threads];
	clock2 thread_id[num_threads];

	for (int i = 0; i < num_threads; i++) {
			thread_id[i].thread_id = i;
			thread_id[i].begin = i * (im->width / num_threads);
			thread_id[i].final = (i + 1) * (im->width / num_threads);
			if (i == num_threads - 1) {
				thread_id[i].final = im->width;
			}

		}
		for (int i = 0; i < num_threads; i++) {
			pthread_create(&(tid[i]), NULL, dunga, &(thread_id[i]));
		}

		for(int i = 0; i < num_threads; i++) {
			pthread_join(tid[i], NULL);
		}

		
}

void writeData(const char * fileName, image *img) {
	FILE *fp;
	fp = fopen(fileName, "w");

	
	fprintf(fp, "%s\n", img->type);
	fprintf(fp,"%d %d\n", img->width, img->width);
	fprintf(fp, "%d\n",img->valoare);

	for (int i = 0; i < img->width; i++) {
			fwrite(img->mat[i], sizeof(unsigned char), img->width, fp);
			
		}

fclose(fp);	
}
