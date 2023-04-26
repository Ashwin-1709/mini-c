#include "ast.h"
#include "symboltable.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/*
    type -> 0 - int
    type -> 1 - float
    type -> 2 - char
    type -> 3 - int *
    type -> 4 - int **
    type -> 5 - float *
    type -> 6 - float **
    type -> 7 - char *
    type -> 8 - char **
*/

astNode *createNode() {
    astNode *cur = (astNode *)(malloc(sizeof(astNode)));
    cur->childCnt = 0;
    for (int i = 0; i < 200; i++)
        cur->label[i] = 0;
    for (int i = 0; i < 55; i++)
        cur->child[i] = NULL;
    cur->type = -1;
    return cur;
}

astNode *createNodeByVal(float val) {
    astNode *cur = createNode();
    char buf[50];
    gcvt(val, 10, buf);
    strcpy(cur->label, buf);
    cur->type = type("float");
    return cur;
}

astNode *createNodeByIntVal(int val) {
    astNode *cur = createNode();
    char buf[50];
    sprintf(buf, "%d", val);
    strcpy(cur->label, buf);
    cur->type = type("int");
    return cur;
}

astNode *createNodeByLabel(char *label) {
    astNode *cur = createNode();
    strcpy(cur->label, label);
    return cur;
}

void addNode(astNode *parent, astNode *child) {
    if (parent == NULL) {
        printf("parent is null\n");
        return;
    } else if (child == NULL) {
        printf("Child is null\n");
        return;
    }
    int c = parent->childCnt++;
    astNode **children = parent->child;
    children[c] = child;
}

void printTree(astNode *root) {
    printf("[%s ", root->label);
    int c = root->childCnt;
    for (int i = 0; i < c; i++)
        printTree(root->child[i]);
    printf("]");
}

astNode *passNode(char *label, int count, ...) {
    va_list arglist;
    astNode *cur = createNode();
    va_start(arglist, count);
    for (int i = 0; i < count; i++)
        addNode(cur, va_arg(arglist, astNode *));
    strcpy(cur->label, label);
    va_end(arglist);
    return cur;
}

char *find_id(astNode *root) {
    if (strcmp(root->label, "id") == 0)
        return root->child[0]->label;
    char *id = "N/A";
    for (int i = 0; i < root->childCnt; i++) {
        char *child_id = find_id(root->child[i]);
        if (strcmp(child_id, id) != 0)
            return child_id;
    }
    return id;
}

void arg_dfs(astNode *root, int *cur, int *args) {
    if (root == NULL)
        return;
    if (strcmp(root->label, "declare_var") == 0) {
        int type = root->child[0]->type;
        args[*cur] = type;
        *cur = *cur + 1;
    }
    for (int i = 0; i < root->childCnt; i++)
        arg_dfs(root->child[i], cur, args);
}

int *get_args(astNode *root) {
    int *args = (int *)malloc(sizeof(int) * 50);
    for (int i = 0; i < 50; i++)
        args[i] = 100;
    int cur_arg = 0;
    arg_dfs(root, &cur_arg, args);
    return args;
}
