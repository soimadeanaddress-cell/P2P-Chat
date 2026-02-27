#include <process.h>
#include <rpc.h>

typedef enum{
    e_PORTLIST = 1,
    e_CMP_DATA = 2,
    e_IMAGEFILE = 3,
    e_UUID = 4,
}Headers;

typedef struct{
    char* addr;
    UUID uuid;
    UserList* prev;
    UserList* next;
} UserList;

UserList* SortList(UserList* List, UUID* myUuid){
    
    if(List==NULL) return;
    UserList* sorted = NULL;
    UserList* current = List;

    while (current!=NULL){
        UserList* next = current->next;

        if(sorted==NULL || UuidEqual(&current->uuid, &sorted->uuid, NULL) || UuidCompare(&current->uuid, &sorted->uuid, NULL) == -1){

            current->next = sorted;
            if(sorted!=NULL)sorted->prev = current;

            sorted = current;
            sorted->prev = NULL;
        }else{
            UserList* currSorted = sorted;
            while(currSorted->next != NULL && UuidCompare(&currSorted->next->uuid, &current->uuid, NULL) == -1){
                currSorted = currSorted->next;
            }

            current->next = currSorted->next;

            if(currSorted->next!=NULL)currSorted->next->prev = current;

            currSorted->next = current;
            current->prev = currSorted;
        }

        current = next;
    }

    UserList* sortedStartAtMyUuid = sorted;
    UserList* tail = sorted; 
    while (tail->next != NULL) {
        tail = tail->next;
        if(UuidEqual(&tail->uuid, myUuid, NULL)) sortedStartAtMyUuid = tail;
    }
    tail->next = sorted; sorted->prev = tail;

    return sortedStartAtMyUuid;
}

int main(){

}