#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXN 20 //for max board size
#define POPSIZE 100//num of individuals in pops
#define MAXGEN 1000//limit for num of generation
#define MUTATIONRATE 0.05 //probability for mutation 5%

//strcut to represent the each solution
typedef struct {
    int genes[MAXN];//position for each queen in each row of col (i to MAXN)
    int fitness;//conflict check
} Individual;

void boarddisplay(Individual *ind);
void printarray(Individual *ind);
void mutate(Individual *ind);
void cross(Individual p1, Individual p2,Individual *c1, Individual *c2);
Individual pickParent();
void intPopu();
int calcFit(Individual *ind);
int absval(int x);


int N;//NxN board size
Individual pop[POPSIZE];//current population
Individual newPopu[POPSIZE];//generating population of next


int main()
{
    int gen,i;//generation counter and loop variable
    Individual p1,p2;//to store parent selected from the population
    srand(time(NULL));//seeding random number generator

    printf("Enter N between 4 to 20: ");//asking the user to enter a num between 4 to 20
    scanf("%d",&N);//storing the user input
    if(N<4 || N>20)// input by user less than 3 or more than 20
    {
        printf("Invalid input N");//letting the user know
        return 0;//exiting the program
    }

    intPopu();//creating an random population of the soln
    for(gen=0;gen<MAXGEN;gen++)//looping till the max generation is reached
        {
            for (i=0;i<POPSIZE; i+=2)//generating 2 children at a time
                {
                    p1 =pickParent();//select the 1st parent
                    p2 =pickParent();//selecting 2nd parent
                    cross(p1, p2,&newPopu[i],&newPopu[i + 1]);//calling func cross to perform cross between p1 and p2
                    mutate(&newPopu[i]);//randomly mutate 1st child
                    mutate(&newPopu[i+1]);//randomly mutate 2nd child
                    newPopu[i].fitness = calcFit(&newPopu[i]);//calculating fitness for 1st child
                    newPopu[i + 1].fitness = calcFit(&newPopu[i+1]);//calculating fitness for 2ns child

                    if (newPopu[i].fitness ==0)//if no conflicts then we have a valid soln
                        {
                            printf("\nSoln found in %d generation",gen);//letting the user in which generation the solution was found
                            printarray(&newPopu[i]);//printing the array
                            printf("\nChess Board:\n");//printing the chessboard
                            boarddisplay(&newPopu[i]);
                            return 0;//exiting the program
                        }
                }
        //replacing the old population with new generation
        for(i=0;i<POPSIZE;i++)
            { pop[i] = newPopu[i];}
    }
    printf("\nMax num of generation crossed - no soln");//if no perfect solution found
    return 0;//exiting the program
}


//returning the absolute value to detect diagonal queen conflicts
int absval(int x)
{
    if(x<0)
        return -x;
    return x;
}

//func to calculate fitness of an individual
int calcFit(Individual *ind)
{
    int conflict=0;//conflict count set to 0
    //conflict used to count the num of times queens were attacked diagonally
    int i,j;//variables for loop
    for(i=0;i<N;i++)//looping thru to check all pairs of queens - col i
        {
            for(j=i+1;j<N;j++)//col j
            {
                if (absval(ind-> genes[i]-ind ->genes[j])== absval(i-j))
                //abs diff of row == abs diff of col - then 2 queens attack each other diagonally
                    {conflict++;}//incrementing the conflict count by 1 each time
        }
    }
    return -conflict;//returning the negative val of conflicts
    //if 0 returned - fitness=0, valid solution
}

//fucn to initialize population
void intPopu()
{
    int i,j,k,temp;//loop variables
    for(i=0;i<POPSIZE;i++)//loopinf thru each population
        {
            for(j=0;j<N;j++)//initializing chromosomes from 0 to N-1
            //only 1 queen per row and col
            {pop[i].genes[j] = j;}
    //shuffling the chromosomes to create a random permutation
        for(j=0;j<N;j++)
            {
                k = rand()%N;//selecting a random index between 0 to N-1
                temp = pop[i].genes[j];//storing the gene value in a temp vaiable
                pop[i].genes[j]= pop[i].genes[k];//replacing the gene at position j with the one at k
                pop[i].genes[k]= temp;//putting the original gene into position k
            }
        pop[i].fitness =calcFit(&pop[i]);//calling func to calc and store the fitness
    }
}

//func to select 1 parent frm the population - tournament selection
Individual pickParent()
{
    int a,b;//variables to store
    a = rand()%POPSIZE;//randomly select the index of the 1st individual
    b = rand()%POPSIZE;//randomly select the index of the 2nd indiv.

    if (pop[a].fitness>pop[b].fitness)//comparing the fitness of a and b
        return pop[a];//returning a if a is better
    else
        return pop[b];//else returning b
}

//func to preform crossig parent to make children
void cross(Individual p1, Individual p2,Individual *c1, Individual *c2)
{
    int cut,i,idx1,idx2;//cross point and loop varaibles
    int used1[MAXN], used2[MAXN];//store row values from child1 and 2

    cut= rand()%N;//randomly selecting a cross point between 0 and N-1
    for(i=0;i<N;i++)//initializing the arrs to 0
        {
            used1[i] = 0;
            used2[i] = 0;
        }
    for(i=0;i<cut;i++)//
        {
            c1->genes[i] =p1.genes[i];//copying the left part of the parent1 to child1
            c2->genes[i] =p2.genes[i];//copying the left part of parent2 to child2
            used1[c1->genes[i]]=1; //marking the used gene values with 1
            used2[c2->genes[i]]=1;
        }

    idx1=cut;//filling the remaining portions
    idx2=cut;

    for(i=0;i<N;i++)
        {
            if (!used1[p2.genes[i]]) {c1->genes[idx1++] =p2.genes[i];}//filling the child1 with unused genes from parent 2
            if (!used2[p1.genes[i]]) {c2->genes[idx2++] =p1.genes[i];}//filling th child2 with unused genes from parent1
    }
}

//func to perform the mutation on one individual
void mutate(Individual *ind)
{
    double r;//variable to store the random value between 0 and 1
    int a,b,temp;//variables from swapping
    r = (double)rand() / RAND_MAX;// generating a random real number between 0 and 1

    if (r < MUTATIONRATE)//performing mutation only with a small probability
        {
            a= rand()%N;//picking a random col index a form N
            b= rand()%N;//picking a random col index b from N
            temp = ind->genes[a];//storing the gene at position a in temp
            ind->genes[a] = ind->genes[b];//swapping the gene at b to a
            ind->genes[b] = temp;//swapping the gene stored in temp to b
        }
}


//func to print the chessboard
void boarddisplay(Individual *ind)
{
    int r,c;//looping variables
    for (r=0;r<N;r++)//looping thru every row
        {
            for(c=0;c<N;c++)//looping thru every col
            {
                if (ind->genes[c] ==r)//gene[c] gives the row number of the queen in that col
                    printf("Q ");//if queen exists then print Q
                else
                    printf(". ");//else print .
            }
            printf("\n");//going to the next line
         }
}

//func to print the array
void printarray(Individual *ind)
{
    int i;//variable for loop
    printf("\nArray: [");//printing the label
    for (i=0;i<N;i++)//looping thru all the cols
        {
        printf("%d ", ind->genes[i]);//printing the row position of the queen in col i
        }
    printf("]\n");
}
