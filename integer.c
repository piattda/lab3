/*
	NAME: David Piatt
	DATE: 2/11/17
	CLASS: CSE 2421, M/W 4:15p
	This is a simple program to represent an integer of arbitrary size
		as a linked list. This allows for integer addition or subtraction.
		This program only subtracts or adds integers.
		The expected inputs are from the command line.
		Line 1: Integer 1
		Line 2: Integer 2
		Line 3: + / - sign
		Each line has \n after it.
*/

#include <stdlib.h>

#include <stdio.h>

//the structures used in this lab. A linked list is just a first or last pointer. It points to links which have a next, prev, or data part. 

typedef struct link link_t;

struct link{

    int data;

    link_t * prev;

    link_t * next;

};

typedef struct linked_list{

    link_t * first;

    link_t * last;

   

}linked_list_t;

 

//function to initialize linked list

static void linked_list_init(linked_list_t * list){

    list->first = list->last = 0;

}

 

//adds a new link to the end of the list. It allocates memory for it, copies "data"

static void linked_list_add(linked_list_t * list, int data){

    link_t * link;

    link = calloc(1,sizeof(link_t));

    //calloc sets the next field to zero

    if(!link){

        fprintf(stderr, "calloc failed. \n");

        exit(EXIT_FAILURE);

    }

    link->data = data;

    if(list->last){

        //add it to the old end, then update the pointer

        list->last->next = link;

        link->prev = list->last;

        list->last = link;

    }else{

        //empty list, special case

        list->first = link;

        list->last = link;

    }

}

 
//The list is traversed backwards, therefore I found it useful to include
//a function to add a node to the beginning of the list instead of the end
//This saved me lots time, I don't think it makes the program any more inefficient. It's a modification of the previous function which was given to us.
static void linked_list_add_to_front(linked_list_t* list, int data){

  link_t * link;

  link = calloc(1,sizeof(link_t));

  //calloc sets the next field to zero.

  if(!link){

    fprintf(stderr, "calloc failed. \n");

    exit(EXIT_FAILURE);

  }

  link->data = data;

  if(list->first){

    list->first->prev = link;

    link->next = list->first;

    list->first = link;

  }else{

    //empty list, special case

    list->first = link;

    list -> last = link;

  }

}

 

//This function frees a list from memory. It is good practice since we used malloc or calloc to allocate heap memory.
static void linked_list_free(linked_list_t * list){

    link_t * link;

    link_t * next;

    for(link = list->first; link; link = next){

        //store next val so that we don't lose the link to the next

        next = link->next;

        free(link);

    }

}

 
//This function prints the linked list to the console character by character.
static void print_linked_list(linked_list_t * list){

  link_t * link;

  link_t * next;

  for(link = list->first; link; link = next){

    printf("%d",link->data);

    next = link->next;

  }

  printf("\n");
  linked_list_free(list);

}

 

 
//Adds two linked lists together into a new list called result. Resulted is printed then freed. 
static void add_linked_lists(linked_list_t * list1, linked_list_t * list2){

 

  int carry = 0, sum;

  linked_list_t result;

  linked_list_init(&result);

 

  //pointers to the last position

 

  link_t * last1 = list1->last;

  link_t * last2 = list2->last;

 

  while(last1 != NULL || last2 != NULL){//while both exist

    //Find the addition of the last digit and its result

    //this is one of two things

    // 1. carry

    // 2. next digit of first list (if there is a next digit)

    // 2.1 next digit of second list (if there is a next digit)

   

    
		sum = carry;
    if(last1 != NULL){

      sum += last1->data;

    }else{

      sum += 0;

    }

    if(last2 != NULL){

      sum += last2->data;

    }else{
    	sum += 0;
    }

    //update carry for next calculation

    if(sum >= 10){

      carry = 1;

      sum = sum % 10;

    }else{

      carry = 0;

    }

    //update sum if it is greater than 10;

    //new node with sum as the data

    linked_list_add_to_front(&result, sum);

		if(last1) last1 = last1 ->prev;
		
		if(last2) last2 = last2 ->prev;
    

  }

 

  if(carry > 0){

    linked_list_add_to_front(&result,carry);

  }

  print_linked_list(&result);

 

}

 
//Subtracts two linked lists, makes new list called result. Result is printed then deleted.
static void subtract_linked_list(linked_list_t * list1, linked_list_t * list2){

   int carry = 0, subtraction;

  linked_list_t result;

  linked_list_init(&result);

 
 	link_t * link1;
  link_t * next1;
  link_t * link2;
  link_t * next2;
	int check = 1;
  for(link1 = list1->first; link1; link1 = next1){

    for(link2 = list2->first; link2; link2 = next2){
    	
    	if(link1->data != link2->data){
    		check = 0;
    	}
    	
    	next2 = link2 ->next;
    }
    next1 = link1->next;

  }
  
  if(check){
  	printf("%d",0);
  }
 

  //pointers to the last position
  link_t * last1 = list1->last;

  link_t * last2 = list2->last;

 	int one, two;
 if(!check){
	//case where list1 =/= list2
  while(last1 != NULL || last2 != NULL){//while both exist

		//Here the last digit is taken and subtracted from list2.
    
    if(last1 != NULL){

      one = last1->data;

    }else{

      one = 0;

    }

    if(last2 != NULL){

      two = last2->data;

    }else {
    	two = 0;
    }

   subtraction = one-two;

	//if a digit was carried then the current digit must be decrememnted.
    if(carry > 0){

      subtraction --;
			
      carry = 0;

    }

    

    //if subtraction is less than 0 we have to indicate we carried a digit and update subtraction. Remember subtraction is negative so we have to add it.

    if(subtraction < 0){

      carry = 1;

      subtraction = 10 + subtraction;

    }else{

      carry = 0;

    }

    //This is to prevent 0's in front of the output. If there is going to be more subtraction, ie there is another node after last1 then it doesn't add the useless 0.
		
		//if the space after the last digit has nothing and there was a carried digit we don't want to add the useless 0 to our linked list
    if(subtraction == 0 && last2 == NULL) {
    	//do nothing
    }else{
    
    linked_list_add_to_front(&result, subtraction);
	}
   
		//update the nodes.
    if(last1) last1 = last1 ->prev;

    if(last2) last2 = last2 ->prev;

  }
  }

  print_linked_list(&result);

}

 

 

int main(int argc, char *argv[]){

    int digit, sign;

    linked_list_t list1;

    linked_list_init(&list1);

   

    linked_list_t list2;

    linked_list_init(&list2);

 

    //get first digit

    digit = getc(stdin);
		
		//get first int
    while(digit != '\n'){

      linked_list_add(&list1, digit-'0');

      digit = getc(stdin);

    }

   

    //get second digit

    digit = getc(stdin);
		
		//get second int
    while(digit != '\n'){

      linked_list_add(&list2, digit-'0');

      digit = getc(stdin);

    }

   

    //get the operator

    sign = getc(stdin);

    if(sign == '+'){

      add_linked_lists(&list1,&list2);

    }else if(sign == '-'){

      subtract_linked_list(&list1, &list2);

    }

   

    //TODO: free lists.   
		//the resultant list was freed in the print function since we don't have reference to it here
		//free list 1
		linked_list_free(&list1);
		linked_list_free(&list2);
    

 

	return 0;

 

}
