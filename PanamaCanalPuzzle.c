#include <stdio.h>
#include <stdlib.h>
typedef struct pnode {
   struct pnode *LCHILD; /*left child pointer for the BST*/
   struct pnode *RCHILD; /*right child pointer for the BST*/
   struct pnode *back; /*pointer to pnode parent in BST*/
   int cost;             /*number of moves to get to this position*/
   char piece;           /*piece that moved to this position*/
   int dir;              /*direction moved to enter this position*/
   char board[12];       /*this position's board*/
} PositionBody;

typedef PositionBody *TypePosition;
TypePosition NEWPOSITION() {
TypePosition p = (TypePosition) malloc(sizeof(PositionBody));
if (p==NULL) {
   printf("Malloc for a new position failed.");
   exit(1);
   }
return p;
}
/*function printPuzzle will print out the board of any pnode*/
void printPuzzle(TypePosition puzzle) {
  int i = 0;
  if (puzzle->cost != -1) {
    for (i = 0; i <12; i++) {
      if(i == 6){
        printf("\n");
      }
      printf("%c ",puzzle->board[i]);
    }
  }else {
    printf("Board is empty");
    }
}
/*Array of type TypePosition is created, which will act as the
Queue in the program. Variables are also initialized to record the front
rear, size, and the Maximum amount of pnodes in the Queue. Queue is a global
variable, since it will be the only Queue being used.*/
TypePosition Queue[50000];
int front = 0;
int rear = 0;
int size = 0;
int qMax = 0;

/*Another array of type TypePosition is created, which
acts as the Stack. The Stack will be used once the answer board
CANAMAPANAL  is found, in order to add the pnode into the stack,
and traverse the back pointers until it reaches the first position. That way,
all of the setps will be printed out in order.
*/
TypePosition Stack[50];
int stackTop = 0;
/*A third array of type TypePosition is created, acting as the
Hash Table. This HashTable will hold all recorded positions of pnodes.*/
TypePosition HashTable[3];   /*Initialize size of Table, size 3 for BST*/
int numHashElem[3];          /*Intizlize a counter for # of pnodes in each BST*/
int hashMAX = 0;             /*Counter for total # of pnodes in Hash Table at the end*/

/*PUSH function to push values into stack*/
void PUSH(TypePosition d, TypePosition stack[]) {
  if (stackTop >= 50) {
    printf("Stack overflow");
  } else {
    Stack[stackTop] = d;
    stackTop++;
  }
}

/*POP function to remove top element in the stack and return value*/
TypePosition POP(TypePosition stack[]) {
  if (stackTop <= 0){
    printf("Stack underflow");
  }else {
    stackTop--;
    return stack[stackTop];
  }
  return stack[stackTop];
}

/*ENQUEUE function used to insert pnodes into Queue*/
void ENQUEUE(TypePosition d, TypePosition queue[]) {
  if (size >= 50000) {
    size = -1;
    printf("ERROR: Queue full");
  } else {
    queue[rear] = d;
    rear = rear + 1;
    if (rear == 50000) {
      rear = 0;
    }
    size = size + 1;
    if (size > qMax) {
      qMax = size;
    }
  }
};

/*DEQUEUE function used to remove the pnode in the first vertex
of the Queue and return its value*/
TypePosition DEQUEUE(TypePosition queue[]) {
  if (size == 0) {
    printf("ERROR: Queue Empty");
    return NULL;
  } else {
    TypePosition temp = queue[front];
    front = front + 1;
    if (front == 50000) {
      front = 0;
    }
    size = size - 1;
    return temp;
  }
}

/*hash function hashes each pnode in order to direct a pnode
to a bucket in the HashTable*/
int hash(TypePosition current) {
  int i = 0;
  int Hvalue = 0;
  for (i = 0; i< 12; i++){
    Hvalue = ((Hvalue * 128) + (current->board[i] * 1)) % 3;
  }
  return Hvalue;
}
/*COPY function takes in two pnodes, one pnode that has a defined board position,
and the other a new pnode with no char board value, which will later represent a
pnode with a board position being one move away from the first pnode. As of the call
for this function, COPY will simply give the new pnode the char board position and cost
of the already defined/previous pnode.*/
void COPY(TypePosition origin, TypePosition nextStep) {
  int i;
  for (i = 0; i<=11; i++) {
    nextStep->board[i] = origin->board[i];
    nextStep->cost = origin->cost;
  }
}

/*COMPARE function will compare two pnodes and will check whether or not they have the
same char board position. This function is used with the MEMBER function.*/
int COMPARISON(TypePosition current,TypePosition traverse) {
  int increment = 0;
  int index;
  for (index = 0; index < 12; index++) {
    if (traverse->board[index] ==current->board[index]) {
      increment++;
    }
  }
  if (increment == 12) {
    return 1;
  }
  return -1;
}
/*The COMPAREVAL function compares each pnode lexicographically in order to compare to pnodes with values that
are never the same, unless of course, the order of the letters are the same. This helps with traversing and
inserting in the binary tree.*/
int COMPAREVAL(TypePosition current,TypePosition sub) {
  int i;
  for (i = 0; i< 12; i++){
    if(current->board[i] > sub->board[i]) {
      return 1;
    }else if (current->board[i] < sub->board[i]) {
      return -1;
    }
  }
  return 0;
}
/*MEMBER function will return either -1 or 1. If it returns 1, then that means
the given pnode is in the hash table. If it returns -1, than the pnode is not in the
hash table. In order to find out, the MEMBER function uses the hash function to find the
index where the pnode should be, and traverses through the BST by comparing each node lexigraphically.
This determines whether the traverse will move left or right */
int MEMBER(TypePosition current) {
  int i = hash(current);
  TypePosition traverse = NEWPOSITION();
  traverse = HashTable[i];

  if (numHashElem[i] == 0) {
  return -1;
}
    else if (numHashElem[i] == 1){
      if (COMPARISON(traverse,current) == 1){
        return 1;
    }
    return -1;
  }
  int detect = 0;
  /*This while loop will keep traversing until either the pnode is found
  or the traversal has ended to a NULL position, in which that case the INSERT function will add this pnode
  at this spot*/
    while (detect == 0) {
      int compare = COMPARISON(current,traverse);
      if (compare == 1) {
        return 1;
      }
      else if(COMPAREVAL(current, traverse) == 1 && traverse->RCHILD == NULL){

        detect = 2;
      }else if (COMPAREVAL(current, traverse) == 1){

        traverse = traverse->RCHILD;
      }else if (COMPAREVAL(current,traverse) == -1 && traverse->LCHILD == NULL) {

        detect = 2;
      }else {
        traverse = traverse->LCHILD;
      }
      if (detect == 2) {
        if (compare == -1){
          return -1;
        } else {
          return 1;
        }
      }
    }
  return -1;
}

/*INSERT function inserts the given pnode into the hash table, as long as the pnode is not
already a member.*/
void INSERT(TypePosition current) {
  TypePosition traverse = NEWPOSITION();
  int i = hash(current);
  traverse = HashTable[i];
  int detect = 0;
  if(numHashElem[i] == 0){
    HashTable[i] = current;
  } else if (MEMBER(current) == -1){
    /*This while loop works similarly to the while loop in MEMBER, except that instead it will actually
    insert the pnode at the designated location */
      while (detect != 1) {
        if(COMPAREVAL(current, traverse) == 1 && traverse->RCHILD == NULL ){
          traverse->RCHILD = current;
          detect = 1;
        }else if (COMPAREVAL(current, traverse) == 1){
          traverse = traverse->RCHILD;
        }else if (COMPAREVAL(current,traverse) == -1 && traverse->LCHILD == NULL) {
          traverse->LCHILD = current;
          detect = 1;
        }else {
          traverse = traverse->LCHILD;
        }
      }
    }
  current->LCHILD = NULL;
  current->RCHILD = NULL;
  numHashElem[i]++;
  /*keeps track of the largest number of pnodes in any bucket*/
  if (numHashElem[i] > hashMAX){
    hashMAX = numHashElem[i];
  }
}
/*function positionOfSpace simply returns the index that contains the ' ' character of any given pnode.*/
int positionOfSpace(TypePosition current) {
  int i = 0;
  for (i = 0; i < 12; i++) {
    if (((char)current->board[i]) == ' ') {
      return i;
    }
  }
  return i;
}

/*function adjustPath takes in a pnode that has already copied the board of the previous
pnode, and is adjusted to make the make a new board position madde by one character move*/
void adjustPath(TypePosition current, int origin, int indexSwitch,int dir) {
  char temp;
  temp = current->board[indexSwitch];
  current->piece = temp;
  current->board[indexSwitch] = current->board[origin];
  current->board[origin] = temp;
  current->cost = current->cost + 1;
  current->dir = dir;
}
/*findNextPath determines which parameters to pass in to adjustPath in order to
always make all of the possible moves that can be made from a given position.*/
void findNextPath(TypePosition current, int index,int i) {
  if (i == 0) {
    if (index == 0 || index == 6) {
      adjustPath(current, index, index + 1, 3);
    }else if (index == 5 || index == 11){
      adjustPath(current,index,index -1, 2);
    }else {
      adjustPath(current,index,index +1,3);
    }
  } else if (i == 1) {
    if (index == 0 || index == 5) {
      adjustPath(current, index, index + 6,0);
    }else if (index == 6 || index == 11) {
      adjustPath(current,index,index - 6, 1);
    }else {
      adjustPath(current, index, index - 1,2);
    }
  } else {
    if (index < 6) {
      adjustPath(current, index, index + 6, 0);
    }else if (index > 6) {
      adjustPath(current,index,index-6,1);
    }
  }
}

int main(){
  /*Intitialize start position and its char board position*/
  TypePosition startPos = NEWPOSITION();
  startPos->board[0] = 'C';
  startPos->board[1] = 'A';
  startPos->board[2] = 'N';
  startPos->board[3] = 'A';
  startPos->board[4] = 'M';
  startPos->board[5] = 'A';
  startPos->board[6] = 'P';
  startPos->board[7] = 'A';
  startPos->board[8] = 'N';
  startPos->board[9] = 'A';
  startPos->board[10] = 'L';
  startPos->board[11] = ' ';
  startPos->cost = 0;
  startPos->back = NULL;
  int initialize;
  /*for loop gives NULL or 0 values to TypePosition's in hash table*/
    for (initialize = 0; initialize < 3; initialize++) {
      HashTable[initialize] = NEWPOSITION();
      HashTable[initialize]->cost = -1;
      HashTable[initialize]->LCHILD = NULL;
      HashTable[initialize]->RCHILD = NULL;
      HashTable[initialize]->back = NULL;
  }
  /*TypePosition answer created with board being equal to answer value, or "PANAMACANAL"*/
  TypePosition answer = NEWPOSITION();
  COPY(startPos,answer);
  char switchTwo = answer->board[0]; /*switchTwo char switches two characters to create answer board*/
  answer->board[0] = answer->board[6];
  answer->board[6] = switchTwo;
  ENQUEUE(startPos, Queue);
  TypePosition first;/*TypePositions first, second, and third created to represent all possible one move outcomes*/
  TypePosition second;
  TypePosition third;
  /*While loop works so that while the PANAMACANAL position is not in the hash table,
  the Queue will keep calling ENQUEUE and DEQUEUE in order to for new positions, adding new ones to the hash table*/
   while(MEMBER(answer) == -1) {
    TypePosition removed = NEWPOSITION();
    removed = DEQUEUE(Queue);
     first = NEWPOSITION();
    COPY(removed,first);
    second = NEWPOSITION();
    COPY(removed,second);
    int index =0;
    index = positionOfSpace(removed);
    int i = 0;
    for (i = 0;i < 3; i++) {
      if (i == 0) {
       findNextPath(first,index,i);
       if (MEMBER(first) == -1) {
          ENQUEUE(first,Queue);
          INSERT(first);
          first->back = removed;
        }
      }
      else if (i == 1) {
        findNextPath(second,index,i);
       if (MEMBER(second) == -1) {
          ENQUEUE(second,Queue);
          INSERT(second);
          second->back = removed;
        }
      }else {
        if (index != 0 || index!= 5 || index!= 6 || index !=11){
          third = NEWPOSITION();
          COPY(removed,third);
          findNextPath(third,index,i);
         if (MEMBER(third) == -1){
           ENQUEUE(third,Queue);
            INSERT(third);
            third->back = removed;
          }
        }
      }
    }
  }
  TypePosition backPath = NEWPOSITION();
  backPath = HashTable[hash(answer)];
  /*following while loop will find the answer in the binary search tree that it should be located in.
  The hash table index that it is located in is determined by hashing the answer and getting the int value.
  The backPath variable at the end of the loop will thus equal the answer, and back pointers will be used to find
  the path*/
  int index2 = hash(answer);
  backPath = HashTable[index2];
  while(COMPARISON(backPath,answer) != 1 ) {
    if (COMPAREVAL(answer,backPath) == 1) {
      backPath = backPath->RCHILD;
    }
    else {
      backPath = backPath->LCHILD;
    }
  }
  /*next for loop counts all buckets that only point to one node rather than a linked list of more, and
  also finds the number of  empty buckets*/
  int emptyBuckets = 0;
  int oneBuckets = 0;
  int j;
  for (j = 0; j < 3 ;j++){
    if (numHashElem[j] == 1) {
      oneBuckets++;
    }
    if (HashTable[j]-> cost == -1){
      emptyBuckets++;
    }
  }
  /*This while loop will keep adding every pnode that is part of the path to the answer to a Stack,
  starting from the answer position PANAMACANAL all the way to the start position, CANAMAPANAL.
  That way, popping the stack until it is empty and printing every pnode that is popped will show the
  order and steps of the paths.*/
  while (backPath->back != NULL) {
    PUSH(backPath, Stack);
    backPath = backPath->back;
  }
  PUSH(backPath,Stack);
  /*next while loop POPS the top, and prints out the char board of the given pnode, including
  the character moved to reach this position, as well as the direction that this character moved, in
  order to print all of the steps from the first position to the last.*/
  while(stackTop > 0) {
    backPath = POP(Stack);
    if (stackTop < 26) {
      printf("\n\nStep %d, move %c ",26 - stackTop,backPath->piece);
        if (backPath->dir == 0) {
          printf("North\n");
        }else if (backPath->dir == 1){
          printf("South\n");
        }else if (backPath->dir == 2) {
          printf("East\n");
        }else {
          printf("West\n");
        }
    }else {
      printf("\n\nStep %d:\n",26 - stackTop);
    }
    printPuzzle(backPath);
  }
  /*print statements for statistics*/
  printf("\n\nNumber of Items in the hash table: %d\n\n", numHashElem[0] + numHashElem[1] + numHashElem[2]);
  printf("Number of empty hash table buckets at the end: %d\n\n", emptyBuckets);
  printf("Number of hash table buckets with only one item in them: %d\n\n",oneBuckets);
  printf("Maximum number of items in a largest hash table bucket at the end: %d\n\n", hashMAX);
  printf("Number of positions in the Queue at the end: %d\n\n", size);
  printf("Maximum number of positions ever in the Queue: %d\n\n", qMax);
  return 0;
}
