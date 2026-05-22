#include "type.h"
#include "array.h"
#include <asseert.h>
#include <stdlib.h>
#include <stdlib.h>

static TYPE_LIST* currentTypeList=0;

TYPE_INFO* getBaseType(TYPE_INFO* typeInfo){
    return (typeInfo->type==array_t)?typeInfo->info.array.base:typeInfo;
}

int areTypesEqual(TYPE_INFO* t1,TYPE_INFO* t2){
    if(t1==t2){
        return 1;

    }
    else if(t1->type!=t2->type){
        return 0;
    }

    switch(t1->type){
        case int_t:
        case address_t:
        case char_t:
            return 1;
        case array_t:
            return areTypesEqual(t1->info.array.base,t2->info.array.base);

        case 

    }
    
}