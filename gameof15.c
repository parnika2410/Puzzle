/**
 * gameof15.c
 *
 * Data Structures
 * Mini Project
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include<stdbool.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9
#define true 1
#define false 0

/* structure for col headnode */
struct cheadnode
{
    int colno ;
    struct node *down ;
    struct cheadnode *next ;
} *tempc, *col;

/* structure for row headnode */
struct rheadnode
{
    int rowno ;
    struct node * right ;
    struct rheadnode *next ;
} *tempr,*row;

/* structure for node to store element */
struct node
{
    int row ;
    int col ;
    char val ;
    struct node *right ;
    struct node *down ;
}   *tempn,*start,*previous ,*waste;

/* structure for special headnode */
struct spmat
{
    struct rheadnode *firstrow ;
    struct cheadnode *firstcol ;
    int noofrows ;
    int noofcols ;
} *head;

int win[DIM_MAX][DIM_MAX];
int d;
int i=0;
int j=0;


// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void create(void);

int main()
{
    // greet user with instructions
    greet();

    printf("Enter the size of board\n");
    scanf("%d",&d);
    head=(struct spmat*)malloc(sizeof(struct spmat));
    head->noofcols=d;
    head->noofrows=d;
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }


    //create the board using multi- linklist
    create();

    // initialize the board
    init();

    // accept moves until game is won

    while(true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

         // log the current state of the board (for testing)
        for (i = 0; i < d; i++)
        {
            for (j = 0; j < d; j++)
            {
                fprintf(file, "%i", boardRet(i,j));
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("You Win!!!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");

        int tile;
        scanf("%d",&tile);

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }



          // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/** Creates multi linklist

*/
void create(void)
{

    for(i=1;i<=d;i++)
    {
        if(i==1)
        {
            tempc=(struct cheadnode*)malloc(sizeof(struct cheadnode));
              tempc->colno=i;
              head->firstcol=tempc;

        }
        else {
                tempc->next=(struct cheadnode*)malloc(sizeof(struct cheadnode));
                tempc=tempc->next;
        }

        tempc->next=NULL;
        tempc->colno=i;
    }

    for(i=1;i<=d;i++)
    {
        if(i==1)
        {
            tempr=(struct rheadnode*)malloc(sizeof(struct rheadnode));
              tempr->rowno=i;
              head->firstrow=tempr;

        }
        else {tempr->next=(struct rheadnode*)malloc(sizeof(struct rheadnode));
                tempr=tempr->next;}
        tempr->next=NULL;
        tempr->rowno=i;
    }
}

/** Assigns new value to the
    respective node given by m(row) and n(column)
 **/
void boardInsert(int m, int n , int data)
{
    m=m+1;
    n=n+1;
    tempr=head->firstrow;
    tempr=head->firstrow;
    while(tempr->rowno!=m)
    {
        tempr=tempr->next;
    }

    tempn=tempr->right;

    while(tempn->col!=n)
    {tempn=tempn->right;
    }
    tempn->val=data;

}

/** Returns value of node given by m(row) and column n(column)
**/
int boardRet(int m, int n)
{
    m=m+1;
    n=n+1;
    tempr=head->firstrow;

    while(tempr->rowno!=m)
    {
        tempr=tempr->next;
    }

    tempn=tempr->right;

    while(tempn->col!=n)
    {tempn=tempn->right;
    }
    return tempn->val;

}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    system("cls");
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init()
{
    int s,temp;
    s=(d*d)-1;
    tempr=head->firstrow;
    tempc=head->firstcol;
    for(i=1;i<=d;i++)
    {
        for(j=1;j<=d;j++)
        {

            if( j==1 )
            {
                tempn=(struct node*)malloc(sizeof(struct node));
                tempr->right=tempn;
                tempn->val=s;
                tempn->row=i;
                tempn->col=j;
                tempn->right=NULL;
                tempn->down=NULL;
                //printf ( "Row: %d Col: %d Val: %d\n", tempn -> row, tempn -> col,tempn -> val ) ;
                tempr=tempr->next;
            }
            else
            {
                tempn->right=(struct node*)malloc(sizeof(struct node));
                tempn=tempn->right;
                //printf("Enter Element::");
                //fflush(stdin);
                //scanf("%c",&e);
                tempn->val=s;
                tempn->row=i;
                tempn->col=j;
                tempn->right=NULL;
                tempn->down=NULL;
                //printf ( "Row: %d Col: %d Val: %d\n", tempn -> row, tempn -> col, tempn -> val ) ;
            }
            s=s-1;


        }
    }

       if(d%2==0)
    {
        temp=boardRet(d-1,d-2);
        boardInsert(d-1,d-2,boardRet(d-1,d-3));   // try swaping with 2 variables
        boardInsert(d-1,d-3,temp);
    }


}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(i=0;i<d;i++)
    {
        for(j=0;j<d;j++)
        {   int k=boardRet(i,j);
            if(k==0)
            {
                printf(" _");
                printf("\t");
            }
            else
            {
                printf("%2d",k);
                printf("\t");
            }
        }
        printf("\n");
    }
}

/**
 * Prints the board in its current state.
 */
bool won(void)
{
      int s=1,a;
    a=d*d;
    for(i=0;i<d;i++)
    {
        for(j=0;j<d;j++)
        {
            win[i][j]=s;
            s=s+1;
        }
    }
    win[d-1][d-1]=0;
    int count;
    for(i=0;i<d;i++)
    {
        count=0;
        for(j=0;j<d;j++)
        {   int p=boardRet(i,j);
            if(win[i][j]==p)
            {
                count++;
            }
        }
    }
    if(count==d)
    {
        return true;
    }
    return false;
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{

    int tposi,tposj,bposi,bposj,temp;  //t=tile,b=blank,pos=psition,i and j are variables
    for(i=0;i<d;i++)
    {
        for(j=0;j<d;j++)
        {   int p=boardRet(i,j);
            if(p==tile)
            {
                tposi=i;
                tposj=j;
            }
            if(p==0)
            {
                bposi=i;
                bposj=j;
            }

            if((tposi+1)==bposi && bposj==tposj)    //above
            {
                temp=boardRet(tposi,tposj);
                boardInsert(tposi,tposj,boardRet(bposi,bposj));
                boardInsert(bposi,bposj,temp);
                return true;
            }
            else if(tposi==(bposi+1) && bposj==tposj)  //below
            {
                temp=boardRet(tposi,tposj);
                boardInsert(tposi,tposj,boardRet(bposi,bposj));
                boardInsert(bposi,bposj,temp);
                return true;
            }
            else if(tposi==bposi && bposj==(tposj+1))  //right
            {
                temp=boardRet(tposi,tposj);
                boardInsert(tposi,tposj,boardRet(bposi,bposj));
                boardInsert(bposi,bposj,temp);
                return true;
            }
            else if(tposi==bposi && (bposj+1)==tposj)  //left
            {
                temp=boardRet(tposi,tposj);
                boardInsert(tposi,tposj,boardRet(bposi,bposj));
                boardInsert(bposi,bposj,temp);
                return true;
            }
        }
    }

    return false;
}
