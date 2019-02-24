#ifndef HOMEWORK_H
#define HOMEWORK_H

typedef struct {
char type[2];
int height;
int width;
int valoare;
unsigned char mat[512][512];

}image;



void readInput(const char * fileName, image *img);
void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */
