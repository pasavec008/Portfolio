/*prevzate z:
https://github.com/prabaprakash/Data-Structures-and-Algorithms-Programs/blob/master/Hashing%20-%20Linear%20Probing%20(Open%20addressing).c
*/
int arr[SIZE]={};                           
int count;

int CalculateHash(int key)
{
    return key%SIZE;
}

void Insert(int element)
{
    if(count==SIZE){
        printf("Error.\nTable if FULL\n");
        exit(EXIT_FAILURE);
    }
    int probe=CalculateHash(element);
    while(arr[probe]!=0 && arr[probe]!=-1)              //tu som zmenil NULL na 0 kvoli warningu
    {
        probe=(probe+1)%SIZE;
    }
    arr[probe]=element;
    count++;
}

int Search(int element)
{
    if(count==0){
        printf("Error.\nTable if EMPTY\n");
        exit(EXIT_FAILURE);
    }
    int probe=CalculateHash(element);
    while(arr[probe]!=0)
    {
        if(arr[probe]==element){
            //printf("Prevzata hash nasla prvok\n");
            return probe;
        }    
        probe=(probe+1)%SIZE;
    }
    //printf("\nElement %d not found. (prevzata_hash)\n",element);
    return -1;
}

void PrintList()
{
    int i;
    for(i=0;i<SIZE;i++)
        printf("%d%c",arr[i],' ');
}