/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

void free_matrix(int **mat, int rows) {
    for (int i = 0; i < rows; i++) {
        free(mat[i]);
    }
    free(mat);
}

void print_matrix(int **mat, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (j > 0)
                printf(" ");
            printf("%d", mat[i][j]);
        }
        printf("\n");
    }
}

int add_row(int **mat, int q_row, int columns) {
    int sum = 0;
    for (int i = 0; i < columns; i++) {
        sum += mat[q_row][i];
    }
    return sum;
}

int add_column(int **mat, int q_col, int rows) {
    int sum = 0;
    for (int i = 0; i < rows; i++) {
        sum += mat[i][q_col];
    }
    return sum;
}

int is_magic_square(int **mat, int rows, int columns) {

    int magic_constant = add_row(mat, 0, columns);
    // add columns
    for (int j = 0; j < columns; j++) {
        if (add_column(mat, j, rows) != magic_constant) {
            return -1;
        }
    }

    // add remaining rows
    for (int i = 0; i < rows; i++) {
        if (add_row(mat, i, columns) != magic_constant) {
            return -1;
        }
    }

    // Check diagonals
    int l_d_sum = 0, r_d_sum = 0;
    for (int i = 0; i < rows; i++) {
        l_d_sum += mat[i][i];
        r_d_sum += mat[i][columns - i - 1];
    }

    if ( (l_d_sum != magic_constant) || (r_d_sum != magic_constant) ) {
        return -1;
    }
    return magic_constant;
}

int main(int argc, char *argv[]) {

    char buffer[256];
    int rows, columns;

    scanf("%d %d", &rows, &columns);
    printf("Rows: %d, Columns: %d\n", rows, columns);

    int **matrix = (int **)malloc(sizeof(int *) * rows);
    int val;

    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(sizeof(int) * columns);
        for (int j = 0; j < columns; j++) {
            scanf("%d ", &val);
            matrix[i][j] = val;
        }
    }

    print_matrix(matrix, rows, columns);

    // Check for square being a magic square
    int magic_square_num = is_magic_square(matrix, rows, columns);
    if ( magic_square_num != -1 ) {
        printf("M is a magic square (magic constant = %d)\n", magic_square_num);
    } else {
        printf("M is NOT a magic square!\n");
    }

    free_matrix(matrix, rows);

    return 0;
}
