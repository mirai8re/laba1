#include<stdio.h>
#include<stdlib.h>
int b=0;
int c=0;
int x,y,z,q;
int menu(){



}
void helpelf(){
	printf("Helping the succesful group of 3 elves!\n\n\n");
	c=c-3;
	printf("Ready your next queue of elves\n");
	
}

void prepsleigh(){
	printf("Preparing Sleigh.........\n\n\n");
	printf("Get ready to be hitched!\n");
}
void hitch(){
	printf("Hitching the reindeers with the sleigh....\n\n\n");
	printf("Hitching Successful!\n\n Over with the Reindeer's Task!\n");}

void santa(){
	if(b==9){
		printf("Wait reindeers let me prepare the sleigh\n");
		b=0;
		prepsleigh(); 
		hitch(); 
	    printf("Did any elf enter the queue therewhile?\n press 1 for Yes\n press 2 for No\n");
	    scanf("%d", &z);
	    if(z==1){
	    	printf("Enter the number of elves those entered the queue (max 3)\n");
	    	scanf("%d", &q);
	    	c=c+q;}
	    else	{
	    	printf("Okay moving forward\n");
		}
		}
		if(c>=3)
	    {
	    	helpelf();
	    	if(c!=0){
	    		printf("Others wait in next queue");
			}
	    	
		}
	
	else{
		printf("Let me know elves, what's the matter? (ELF)\n");
		helpelf();
		
	}	
}
void elf(){
	c++;
	printf("Entry for an elf acknowledged!\n");
	printf("Total elf in queue is : %d\n", c);
	if(c==3){
		santa();
	}
	else{
		printf("Wait, while your queue hits with a number 3, elves!\n");
		printf("Wish to renter the menu? If yes, press 1.\n Else, press 2 for EXIT\n");
	scanf("%d",&y);
	switch(y){
		case 1:
			int p;
	printf("Enter your entry for respective registration: \n");
	printf("1.Entry as a Reindeer\n");
	printf("2.Entry as an Elve\n");
	printf("press any other, to exit\n");
	scanf("%d", &p);
	switch (p) {
	case 1:
		b++;
		printf("entry ack. for reindeer!\n");
		break;
	case 2: 
		elf();
		break;
	default:
	exit(0);}
			break;
		case 2:
			printf("dropping off to the end\n");
		default: 
		exit(0);	
	}
	
}}
void reind(){
	
	b++;
		printf("Entry for Reindeer acknowledged!\n");
		printf("Total reindeer in queue is: %d\n", b);		
	if(b==9){
		santa();
	}	
	else{
		printf("Wait, while your queue hits with a number 9, reindeers!\n");
		printf("Wish to renter the menu? If yes, press 1.\n Else, press 2 for EXIT\n");
	scanf("%d",&x);
	switch(x){
		case 1:
			int p;
	printf("Enter your entry for respective registration: \n");
	printf("1.Entry as a Reindeer\n");
	printf("2.Entry as an Elve\n");
	printf("press any other, to exit\n");
	scanf("%d", &p);
	switch (p) {
	case 1:
		reind();
		break;
	case 2: 
		elf();
		break;
	default:
	exit(0);}
			break;
		case 2:
			printf("dropping off to the end\n");
		default:
		exit(0);	
	}
	}
}

int main(){
	int a;
	printf("Enter your entry for respective registration: \n");
	printf("1.Entry as a Reindeer\n");
	printf("2.Entry as an Elve\n");
	scanf("%d", &a);
	switch (a) {
	case 1:
		reind();
		break;
	case 2: 
		elf();
		break;
	default:
	exit(0);
}}
