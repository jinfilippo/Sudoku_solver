#include <stdio.h>

void view(int mat[][9]);
void convert(int mat[][9], int flag[][9], int table[]);
void simple_solve(int mat[][9], int flag[][9], int table[]);
int recursive(int mat[][9], int flag[][9], int table[]);
int issolution(int mat[][9], int table[]);
int iscontradiction(int mat[][9], int flag[][9], int table[]);
void revert(int mat[][9], int table[]);

int main()
{
    int mat[9][9], flag[9][9], i, j, table[]={2,3,5,7,11,13,17,19,23};

    FILE *fp=fopen("numbers.txt", "r");
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            fscanf(fp, " %d", &mat[i][j]);
        }
    }
    fclose(fp);
    printf("\n\n\t*** GIVEN SUDOKU ***\n");
    view(mat);
    convert(mat, flag, table);
    i=recursive(mat, flag, table);
    revert(mat, table);
    printf("\t*** SOLVED SUDOKU ***\n");
    view(mat);
    return 0;
}

void view(int mat[][9])
{
    int i, j;
    printf("\n\t");
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if(i==0 || i==3 || i==6) printf("+=======");
            else printf("+-------");
        }
        printf("+\n\t");
        for(j=0; j<=9; j++)
        {
            if(j==0 || j==3 || j==6 || j==9) printf("||      ");
            else printf("|       ");
        }
        printf("\n\t");
        for(j=0; j<9; j++)
        {
            if(j==0 || j==3 || j==6) printf("||  %d   ", mat[i][j]);
            else printf("|   %d   ", mat[i][j]);
        }
        printf("||\n\t");
        for(j=0; j<=9; j++)
        {
            if(j==0 || j==3 || j==6 || j==9) printf("||      ");
            else printf("|       ");
        }
        printf("\n\t");
    }
    for(i=0; i<9; i++) printf("+=======");
    printf("+\n\n");
}


void convert(int mat[][9], int flag[][9], int table[])
{
    int i, j;

    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if(mat[i][j]==0)
            {
                mat[i][j]=223092870;
                flag[i][j]=1;
            }
            else
            {
                mat[i][j]=table[mat[i][j]-1];
                flag[i][j]=0;
            }
        }
    }
}


void simple_solve(int mat[][9], int flag[][9], int table[])
{
    int i, j, k, l, s, flag1, c=1, progress=1;

    while(progress==1)
    {
        progress=0;
        for(i=0; i<9; i++)
        {
            for(j=0; j<9; j++)
            {
                if(flag[i][j]==1)
                {
                    for(k=0; k<9; k++)
                    {
                        if(flag[i][k]==0 && mat[i][j]%mat[i][k]==0 && mat[i][j]!=mat[i][k])
                        {
                            mat[i][j]=mat[i][j]/mat[i][k];
                            progress=1;
                        }
                        if(flag[k][j]==0 && mat[i][j]%mat[k][j]==0 && mat[i][j]!=mat[k][j])
                        {
                            mat[i][j]=mat[i][j]/mat[k][j];
                            progress=1;
                        }
                    }
                    for(k=(i/3)*3; k<((i/3)*3)+3; k++)
                    {
                        for(l=(j/3)*3; l<((j/3)*3)+3; l++)
                        {
                            if(flag[k][l]==0 && mat[i][j]%mat[k][l]==0 &&mat[i][j]!=mat[k][l])
                            {
                                mat[i][j]=mat[i][j]/mat[k][l];
                                progress=1;
                            }
                        }
                    }
                }
            }
        }

        for(i=0; i<9; i++)
        {
            for(j=0; j<9; j++)
            {
                for(s=0; s<9 && flag[i][j]==1; s++)
                {
                    if(mat[i][j]==table[s])
                    {
                        printf("\n[%d]\tSolved [%d][%d]", c, i, j);
                        c++;
                        flag[i][j]=0;
                        progress=1;

                        for(k=0; k<9; k++)
                        {
                            if(flag[i][k]==1 && mat[i][k]%table[s]==0) mat[i][k]=mat[i][k]/table[s];
                            if(flag[k][j]==1 && mat[k][j]%table[s]==0) mat[k][j]=mat[k][j]/table[s];
                        }
                        for(k=(i/3)*3; k<((i/3)*3)+3; k++)
                        {
                            for(l=(j/3)*3; l<((j/3)*3)+3; l++)
                            {
                                if(flag[k][l]==1 && mat[k][l]%table[s]==0) mat[k][l]=mat[k][l]/table[s];
                            }
                        }
                    }
                }
            }
        }

        for(i=0; i<9; i++)
        {
            for(j=0; j<9; j++)
            {
                for(s=0; s<9 && flag[i][j]==1; s++)
                {
                    flag1=1;
                    for(k=0; k<9; k++)
                    {
                        if(k!=i && mat[k][j]%table[s]==0) flag1=0;
                    }
                    if(flag1==1 && mat[i][j]%table[s]==0)
                    {
                        mat[i][j]=table[s];
                        printf("\n[%d]\tSolved [%d][%d]", c, i, j);
                        c++;
                        flag[i][j]=0;
                        progress=1;
                    }

                    flag1=1;
                    for(k=0; k<9 && flag[i][j]==1; k++)
                    {
                        if(k!=j && mat[i][k]%table[s]==0) flag1=0;
                    }
                    if(flag1==1 && mat[i][j]%table[s]==0 && flag[i][j]==1)
                    {
                        mat[i][j]=table[s];
                        printf("\n[%d]\tSolved [%d][%d]", c, i, j);
                        c++;
                        flag[i][j]=0;
                        progress=1;
                    }

                    flag1=1;
                    for(k=(i/3)*3; k<((i/3)*3)+3 && flag[i][j]==1; k++)
                    {
                        for(l=(j/3)*3; l<((j/3)*3)+3 && flag[i][j]==1; l++)
                        {
                            if(k==i && l==j);
                            else if(mat[k][l]%table[s]==0) flag1=0;
                        }
                    }
                    if(flag1==1 && mat[i][j]%table[s]==0 && flag[i][j]==1)
                    {
                        mat[i][j]=table[s];
                        printf("\n[%d]\tSolved [%d][%d]", c, i, j);
                        c++;
                        flag[i][j]=0;
                        progress=1;
                    }
                }
            }
        }
    }
    printf("\n\n");
}


int issolution(int mat[][9], int table[])
{
    int i, j, k, l, s, flag1=1;

    for(i=0; i<9 && flag1==1; i++)
    {
        for(j=0; j<9 && flag1==1; j++)
        {
            flag1=0;
            for(s=0; s<9 && flag1==0; s++)
            {
                if(mat[i][j]==table[s]) flag1=1;
            }
        }
    }

    for(i=0; i<9 && flag1==1; i++)
    {
        for(j=0; j<9 && flag1==1; j++)
        {
            for(k=0; k<9; k++)
            {
                if(k!=i && mat[i][j]==mat[k][j]) flag1=0;
                if(k!=j && mat[i][j]==mat[i][k]) flag1=0;
            }
            for(k=(i/3)*3; k<((i/3)*3)+3 && flag1==1; k++)
            {
                for(l=(j/3)*3; l<((j/3)*3)+3 && flag1==1; l++)
                {
                    if(k==i && l==j);
                    else
                    {
                        if(mat[i][j]==mat[k][l]) flag1=0;
                    }
                }
            }
        }
    }
    return flag1;
}


int iscontradiction(int mat[][9], int flag[][9], int table[])
{
    int i, j, k, l, s, contradiction=0;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            for(k=0; k<9; k++)
            {
                if(flag[i][j]==0 && k!=j && mat[i][j]==mat[i][k]) contradiction=1;
                if(flag[i][j]==0 && k!=i && mat[i][j]==mat[k][j]) contradiction=1;
            }
            for(k=(i/3)*3; k<((i/3)*3)+3; k++)
            {
                for(l=(j/3)*3; l<((j/3)*3)+3; l++)
                {
                    if(k==i && l==j);
                    else
                    {
                        if(flag[i][j]==0 && mat[i][j]==mat[k][l]) contradiction=1;
                    }
                }
            }
        }
    }
    return contradiction;
}


int recursive(int mat[][9], int flag[][9], int table[])
{
    int i, j, k, l, s, solved, contradiction, lead[9][9], lead_flag[9][9];

    simple_solve(mat, flag, table);
    solved=issolution(mat, table);
    contradiction=iscontradiction(mat, flag, table);
    if(solved);
    else if(contradiction);
    else
    {
        for(i=0; i<9 && solved==0 && contradiction==0; i++)
        {
            for(j=0; j<9 && solved==0 && contradiction==0; j++)
            {
                if(flag[i][j]==0);
                else
                {
                    for(s=0; s<9 && solved==0; s++)
                    {
                        if(mat[i][j]%table[s]==0)
                        {
                            for(k=0; k<9; k++)
                            {
                                for(l=0; l<9; l++)
                                {
                                    lead[k][l]=mat[k][l];
                                    lead_flag[k][l]=flag[k][l];
                                }
                            }
                            mat[i][j]=table[s];
                            flag[i][j]=0;
                            contradiction=recursive(mat, flag, table);
                            solved=issolution(mat, table);
                            if(contradiction)
                            {
                                for(k=0; k<9; k++)
                                {
                                    for(l=0; l<9; l++)
                                    {
                                        mat[k][l]=lead[k][l];
                                        flag[k][l]=lead_flag[k][l];
                                    }
                                }
                            }
                        }
                    }
                    if(solved==0) contradiction=1;
                }
            }
        }
    }
    return contradiction;
}


void revert(int mat[][9], int table[])
{
    int i, j, s;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            for(s=0; s<9; s++)
            {
                if(mat[i][j]==table[s]) mat[i][j]=s+1;
            }
        }
    }
}