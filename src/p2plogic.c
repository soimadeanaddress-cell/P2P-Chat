#include <process.h>
#include <rpc.h>

typedef struct{
    UUID uuid;
    p2pMesh* left;
    p2pMesh* right;
    p2pMesh* up;
    p2pMesh* down;
    char* addr;
} p2pMesh;

p2pMesh* sortMesh(UUID* myUUID, p2pMesh* mesh){
    
}

int main(){

}