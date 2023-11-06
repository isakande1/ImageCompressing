#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
struct Bitmap {
    int rows;
    int cols;
    short *pixels;
};
struct Node {
    short intensity;
    struct Node *children[4];
};
struct Tree {
    struct Node *root;
};
short ** dimensional(struct Bitmap *b){
    int cols = b->cols;
    short ** dimen = malloc(sizeof(short *) * cols);
    short * arr = malloc(sizeof(short) * cols);
    int i , j ,f = 0;
    for(j=0; j<cols; j++) {
        for (i = 0; i < cols; i++) {
            arr[i] = (b->pixels)[(i+f)];
        }
        dimen[j] = arr;
        arr = malloc(sizeof(short) * cols);
        f+=cols;
    }
    free(arr);
    return dimen;
}
void freedim(short ** dimen, struct Bitmap *b) {
    int i, j, f,d =0;
    int cols = b->cols;
    /*copy to bitmap*/
    for(j=0; j < cols; j++) {
        for(f= 0; f < cols; f++,d++) {
            ( b->pixels)[d] = dimen[j][f];
        }
    }
    /*end copy*/
    /*free dimen*/
    for(i =0; i < cols; i++) {
        free(dimen[i]);
    }
    free(dimen);
    dimen = NULL;
}
struct Bitmap Bitmap_new(char *fname) {
    FILE *f = fopen(fname, "r");
    int count = 2;
    short digit;
    short *arr = malloc(sizeof(short));
    int colsrows;
    short *temp;
    struct Bitmap bitmap;
    fscanf(f, "%hd", &digit);
    arr[0] = digit;
    for (; (fscanf(f, "%hd", &digit) != EOF); ++count) {
       temp = realloc(arr, (sizeof(short) * count));
       if (temp != NULL) {
          arr = temp;
          arr[(count-1)] = digit;
        }
    }
    fclose(f);
    colsrows = (int) sqrt((count-1));
    bitmap.cols = colsrows;
    bitmap.rows = colsrows;
    bitmap.pixels = arr;
    return bitmap;
}
void Bitmap_clear(struct Bitmap *b) {
    free(b->pixels);
    b->pixels = NULL;
}
void Bitmap_rotate_left(struct Bitmap *b) {
    short ** dimen = dimensional(b);
    int i , j ;
    short temp ;
    int cols = b->cols ;
    int x , left , right ;
    for(i = 0; i < cols; i++){
        for (j = i; j< cols; j++){
            temp = dimen[i][j];
            dimen[i][j] = dimen[j][i];
            dimen[j][i] = temp;
        }
    }
/*reversal*/
    for(x = 0; x < cols; x++){
        left = 0;
        right = b->cols -1;
        while(left < right){
            temp = dimen[left][x];
            dimen[left][x] = dimen[right][x];
            dimen[right][x] = temp;
            left++;
            right--;
        }
    }
    freedim(dimen,b);
}
void Bitmap_rotate_right(struct Bitmap *b) {
    int cols = b->cols;
    short ** dimen = dimensional(b);
    int i ,j=0;
    short temp ;
    int x , left , right  ;
    for(x = 0; x < cols; x++){
        left = 0;
        right = b->cols -1;
        while(left < right){
            temp = dimen[left][x];
            dimen[left][x] = dimen[right][x];
            dimen[right][x] = temp;
            left++;
            right--;
        }
    }
    /*transpose matrix*/
    for(i = 0; i < cols; i++){
        for (j = i; j< cols; j++){
            temp = dimen[i][j];
            dimen[i][j] = dimen[j][i];
            dimen[j][i] = temp;
        }
    }
    freedim(dimen,b);
}
void Bitmap_flip_vertical(struct Bitmap *b) {
    short ** dimen = dimensional(b);
    short temp = 0;
    int i , j, x = (b->cols) -1;
    for(i = 0; i <  (b->cols); i++) {
        for(j=0; (j-x) !=1 ;j++,x--) {
            temp = dimen[i][j];
            dimen[i][j] = dimen[i][x];
            dimen[i][x]= temp;
        }
        x = (b->cols) -1;
    }
    freedim(dimen,b);
}
void Bitmap_flip_horizontal(struct Bitmap *b) {
    short ** dimen = dimensional(b);
    short tem = 0;
    int i , j, x ;
    for(i = 0; i <  (b->cols); i++) {
        x = (b->cols) -1;
        for(j=0; (j- x) !=1 ;j++,x--) {
            tem = dimen[j][i];
            dimen[j][i] = dimen[x][i];
            dimen[x][i] = tem;
        }
    }
    freedim(dimen,b);
}
short isequal(short ** dimen,int cols) {
    int i , j;
    short temp;
    for(i=0; i < cols; i++) {
        for(j = 0, temp = dimen[i][j]; j < cols; j++) {
            if(temp != dimen[i][j])
                return -1;
        }
    }
    return temp;
}
void split(short **dimen,short ** nw, short **ne, short ** sw, short **se,short cols) {
    int i, j, f;
    for(f =0; f < cols; f++) {
        nw[f] = malloc(sizeof(short) * cols); ne[f] = malloc(sizeof(short) * cols);
        se[f] = malloc(sizeof(short) * cols);sw[f] = malloc(sizeof(short) * cols);
    }
    for(i =0; i < cols; i++) {
        for(j =0; j < cols; j++) {
           nw[i][j] = dimen[i][j];
           ne[i][j] = dimen[i][(j+cols)];
           se[i][j] = dimen[i+cols][j];
           sw[i][j] = dimen[i+cols][(j+cols)];
        }
    }
}
void recursive(short ** dimen, int cols,  struct Tree *tree, struct Node *node1) {
    /*cols to be divide by 2*/
    int intensity;
    struct Node *node = node1;
    short **nw = NULL , **ne = NULL , **sw = NULL, **se = NULL ;
    int i;
    node->children[0] = NULL; node->children[1] = NULL; node->children[2] = NULL, node->children[3] = NULL;
    if(tree->root == NULL)
        tree->root = node;
    if((intensity = isequal(dimen,cols)) == -1 ) {
        node->intensity = -1;
        node->children[0] = malloc(sizeof(struct Node )); node->children[1] = malloc(sizeof(struct Node ));
            node->children[2] = malloc(sizeof(struct Node )), node->children[3] = malloc(sizeof(struct Node ));
         nw = malloc(sizeof(short *) * cols); ne = malloc(sizeof(short *) * cols);
         sw = malloc(sizeof(short *) * cols); se = malloc(sizeof(short *) * cols);
        split(dimen, nw, ne, sw, se, cols = cols / 2);
    }else {
        node->intensity =intensity;
        return;
    }
    for(i =0; i < cols; i++) {
        free(dimen[i]);
    }
   free(dimen);
     recursive(nw,cols,tree,node->children[0]);
    recursive(ne,cols,tree,node->children[1]);
     recursive(se,cols, tree, node->children[2]);
    recursive(sw,cols,tree,node->children[3]);
}
struct Tree Tree_new(struct Bitmap *b) {
    struct Tree t;
    struct Node *node=malloc(sizeof(struct Node ));
    short **dimen = dimensional(b);
    /*copy to bitmap*/
    t.root = NULL;
    recursive(dimen,b->cols,&t,node);
    node = NULL;
    return t;
}
void recursiveclear(struct Node *node) {
    if(node->children[0]== NULL && node->children[1]== NULL && node->children[2]==NULL && node->children[3] == NULL) {
        free(node);
        node = NULL;
        return;
    }
    recursiveclear(node->children[0]);
    recursiveclear(node->children[1]);
    recursiveclear(node->children[2]);
    recursiveclear(node->children[3]);
}
void Tree_clear(struct Tree *t) {
    struct Node *node = t->root;
    recursiveclear(node);
}
void appendtofile(struct Node *node, FILE *f) {
    if(node== NULL) {
        return;
    }
    fprintf(f,"%d\n",node->intensity);
    appendtofile(node->children[0],f);
    appendtofile(node->children[1],f);
    appendtofile(node->children[2],f);
    appendtofile(node->children[3],f);
}
void Tree_save(struct Tree *t, char *fname) {
    /*cocat file*/
    FILE *f;
    char arr[] =".compressed";
    char * fcomp = malloc((strlen(fname) + strlen(arr) +1));
    struct Node *node = t->root;
    strcpy(fcomp,fname);
    strcat(fcomp,arr);
    f = fopen(fcomp,"a");
    appendtofile(node,f);
    fclose(f);
    free(fcomp);
    fcomp = NULL;
}
int main(int argc, char *argv[]) {
    /*ghjkilo;p'*/
    int choice;
    char * fname = argv[argc-1];
    struct Bitmap b = Bitmap_new(fname);
    struct Tree t;
do {
    printf("%s\n","Options menu:");
    printf("%s\n","1. Rotate Left (counterclockwise");
    printf("%s\n","2. Rotate Right (clockwise)");
    printf("%s\n","3. Flip Vertical");
    printf("%s\n","4. Flip Horizontal");
    printf("%s\n","5. Save and quit");
    printf("\n%s\n","Your choice?");
    scanf("%d",&choice);
    switch(choice) {
        case 1: Bitmap_rotate_left(&b); break;
        case 2: Bitmap_rotate_right(&b); break;
        case 3: Bitmap_flip_vertical(&b); break;
        case 4: Bitmap_flip_horizontal(&b); break;
        case 5 : t = Tree_new(&b); Tree_save(&t,fname); break;
    }
}while(choice != 5);
    Bitmap_clear(&b);
    Tree_clear(&t);
    return 0;
}
