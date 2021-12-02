//
// Created by clousty8 on 01/12/2021.
//

#ifndef INC_42SHEEEE_EXEC_STRUCT_H
#define INC_42SHEEEE_EXEC_STRUCT_H

struct exec_struct{
    struct var_list *v_l;
    int v_l_size;
};

struct var_list{
    char *name;
    char *value;
    int name_l;
    int value_l;
};

#endif // INC_42SHEEEE_EXEC_STRUCT_H
