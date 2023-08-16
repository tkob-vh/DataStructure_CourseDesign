/********************************************************************
* FILE: tree.cxx
* DESCRIPTION:implementing the data structure of multi tree using the 
* binary tree ,whose left pointer points to the first child and right
* pointer points to the next brother.
* AUTHOR: yyx
* LASTREVISED: 2023.8.16
********************************************************************/




#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "tree.h"


//函数定义



status CreateBiTree(BiTree& T, TElemType definition[])
/*根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，
如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{

    static int i;
    static BiTree root ;
    static int bucket[500] ;
    static int flag , tag;
    p1 = &i;
    p2 = &flag;
    p3 = &tag;
    p4 = &root;
    p5 = bucket;
    if (definition[i].key == 0) {
        T = NULL;
        ++i;
    }
    else if (definition[i].key == -1) {
        T = root;
    }
    else {
        ++bucket[definition[i].key];
        for (int k = 1; k < 500; k++) if (bucket[k] > 1) tag = 1;
        T = (BiTree)malloc(sizeof(BiTNode));
        T->data.key = definition[i].key;
        int j;
        for (j = 0; definition[i].others[j] != '\0'; ++j)
            T->data.others[j] = definition[i].others[j];
        T->data.others[j] = '\0';
        ++i;

        if (flag == 0) {
            root = T;
            flag = 1;
        }
        CreateBiTree(T->lchild, definition);
        CreateBiTree(T->rchild, definition);
    }
    if (tag == 1) return ERROR;
    return OK;
}

status ClearBiTree(BiTree& T)
//将二叉树设置成空，并删除所有结点，释放结点空间
{
    if (T == NULL) return ERROR;
    if (T->lchild != NULL) ClearBiTree(T->lchild);
    if (T->rchild != NULL) ClearBiTree(T->rchild);
    free(T);
    T = NULL;
    return OK;
}

int BiTreeDepth(BiTree T)
//求二叉树T的深度
{
    int leftHeight, rightHeight, maxHeight;//左子树，右子树，最大深度
    if (T != NULL)
    {
        leftHeight = BiTreeDepth(T->lchild);//左子树深度
        rightHeight = BiTreeDepth(T->rchild);//右子树深度
        maxHeight = leftHeight > rightHeight ? leftHeight : rightHeight;//最大深度
        return maxHeight + 1;//二叉树深度=最大深度+1
    }
    else
    {
        return 0;
    }
}

BiTNode* LocateNode(BiTree T, KeyType e)
//查找结点
{
    if (T == NULL) return NULL;
    if (T->data.key == e) return T;
    BiTNode* p = LocateNode(T->lchild, e);
    if (p) return p;
    return LocateNode(T->rchild, e);
}

status Assign(BiTree& T, KeyType e, TElemType value)
//实现结点赋值。此题允许通过增加其它函数辅助实现本关任务
{
    int i;
    BiTree temp = LocateNode(T, e);
    BiTree temp1 = LocateNode(T, value.key);
    if (temp == NULL) return ERROR;
    if (temp1 != NULL && temp1 != temp) return ERROR;
    temp->data.key = value.key;
    for (i = 0; value.others[i] != '\0'; i++)
        temp->data.others[i] = value.others[i];
    temp->data.others[i] = '\0';
    return OK;
}

BiTNode* GetSibling(BiTree T, KeyType e)
//实现获得兄弟结点
{
    if (T != NULL) {
        if (T->lchild && T->lchild->data.key == e) return T->rchild;
        else if (T->rchild && T->rchild->data.key == e) return T->lchild;
        else {
            BiTNode* p;
            p = GetSibling(T->lchild, e);
            if (p)
                return p;
            else
                return GetSibling(T->rchild, e);
        }
    }
    else return NULL;
}

status InsertNode(BiTree& T, KeyType e, int LR, TElemType c)
//插入结点。此题允许通过增加其它函数辅助实现本关任务
{
    int i;
    BiTree temp = LocateNode(T, e);
    if (temp == NULL && LR != -1) return ERROR;
    if (LocateNode(T, c.key) != NULL) return ERROR;
    BiTree cur = (BiTree)malloc(sizeof(BiTNode));
    cur->data.key = c.key;
    for (i = 0; c.others[i] != '\0'; i++) cur->data.others[i] = c.others[i];
    cur->data.others[i] = '\0';
    cur->lchild = NULL;
    if (LR == 0) {
        BiTree temp1 = temp->lchild;
        temp->lchild = cur;
        cur->rchild = temp1;
    }
    else if (LR == 1) {
        BiTree temp1 = temp->rchild;
        temp->rchild = cur;
        cur->rchild = temp1;
    }
    else if (LR == -1) {
        cur->rchild = T;
        T = cur;
    }
    return OK;
}

BiTree ParentNode(BiTree& T, KeyType e) {
    if (T == NULL) return NULL;
    if (T->lchild != NULL && T->lchild->data.key == e) return T;
    if (T->rchild != NULL && T->rchild->data.key == e) return T;
    BiTree temp = ParentNode(T->lchild, e);
    if (temp != NULL) return temp;
    return ParentNode(T->rchild, e);
}

status DeleteNode(BiTree& T, KeyType e)
//删除结点。此题允许通过增加其它函数辅助实现本关任务
{
    BiTree pte = LocateNode(T, e);
    BiTree par = ParentNode(T, e);
    if (pte == NULL) return ERROR;
    if (pte->lchild == NULL && pte->rchild == NULL) {
        if (par == NULL) {
            free(pte);
            //T==NULL;
        }
        else if (par != NULL) {
            if (par->lchild != NULL && par->lchild->data.key == e) {
                par->lchild = NULL;
                free(pte);
                // pte=NULL;
            }
            else if (par->rchild != NULL && par->rchild->data.key == e) {
                par->rchild = NULL;
                free(pte);
                // pte=NULL;
            }
        }
    }
    else if (pte->lchild != NULL && pte->rchild == NULL) {
        if (par == NULL) {
            T = T->lchild;
            free(pte);
            //pte=NULL;
        }
        else if (par != NULL) {
            if (pte == par->lchild) {
                par->lchild = pte->lchild;
                free(pte);
                // pte=NULL;
            }
            else if (pte == par->rchild) {
                par->rchild = pte->lchild;
                free(pte);
                // pte=NULL;
            }
        }
    }
    else if (pte->lchild == NULL && pte->rchild != NULL) {
        if (par == NULL) {
            T = T->rchild;
            free(pte);
            // pte=NULL;
        }
        else if (par != NULL) {
            if (pte == par->lchild) {
                par->lchild = pte->rchild;
                free(pte);
                // pte=NULL;
            }
            else if (pte == par->rchild) {
                par->rchild = pte->rchild;
                free(pte);
                //  pte=NULL;
            }
        }
    }
    else if (pte->lchild != NULL && pte->rchild != NULL) {
        if (par == NULL) {
            T = T->lchild;
            BiTree temp = T;
            while (temp->rchild != NULL) temp = temp->rchild;
            temp->rchild = pte->rchild;
            free(pte);
            //  pte=NULL;
        }
        else if (par != NULL) {
            if (pte == par->lchild) {
                par->lchild = pte->lchild;
                BiTree temp = pte->lchild;
                while (temp->rchild != NULL) temp = temp->rchild;
                temp->rchild = pte->rchild;
                free(pte);
                //    pte=NULL;
            }
            else if (pte == par->rchild) {
                par->rchild = pte->lchild;
                BiTree temp = pte->lchild;
                while (temp->rchild != NULL) temp = temp->rchild;
                temp->rchild = pte->rchild;
                free(pte);
                //   pte=NULL;
            }
        }
    }
    return OK;
}

void PrintNode(BiTree T) {
    printf("%d %s\n", T->data.key, T->data.others);
}

status PreOrderTraverse(BiTree T, void (*visit)(BiTree))
//先序遍历二叉树T
{
  
    if (T == NULL) return ERROR;
    visit(T);
    PreOrderTraverse(T->lchild, visit);
    PreOrderTraverse(T->rchild, visit);
    return OK;

}

status InOrderTraverse(BiTree T, void (*visit)(BiTree))
//中序遍历二叉树T
{
  
    if (T == NULL) return ERROR;
    InOrderTraverse(T->lchild, visit);
    visit(T);
    InOrderTraverse(T->rchild, visit);
    return OK;

}

status PostOrderTraverse(BiTree T, void (*visit)(BiTree))
//后序遍历二叉树T
{
   
    if (T == NULL) return ERROR;
    PostOrderTraverse(T->lchild, visit);
    PostOrderTraverse(T->rchild, visit);
    visit(T);
    return OK;


}

status LevelOrderTraverse(BiTree T, void (*visit)(BiTree))
//按层遍历二叉树T
{
  
    if (T == NULL) return ERROR;
    BiTree p;
    BiTree q[100];
    int front = 0, rear = 0;
    q[rear++] = T;
    while (front != rear) {
        p = q[front++];
        visit(p);
        if (p->lchild != NULL) q[rear++] = p->lchild;
        if (p->rchild != NULL) q[rear++] = p->rchild;
    }
    return OK;

}

void ReadNode(BiTree T) {
    fprintf(fp, "%d %s\n", T->data.key, T->data.others);
}

status PreOrderTraverse1(BiTree T, void (*visit)(BiTree))
//先序遍历二叉树T
{
  
    if (T == NULL) {
        fprintf(fp, "0 null\n");
        return ERROR;
    }
    visit(T);
    PreOrderTraverse1(T->lchild, visit);
    PreOrderTraverse1(T->rchild, visit);
    return OK;

}

status SaveBiTree(BiTree T, char FileName[])
//将二叉树的结点数据写入到文件FileName中
{
  
    fp = fopen(FileName, "w");
    PreOrderTraverse1(T, ReadNode);
    fprintf(fp, "-1 null\n");
    fclose(fp);
    return OK;

}

status LoadBiTree(BiTree& T, char FileName[])
//读入文件FileName的结点数据，创建二叉树
{
   
    if (T != NULL) DestroyBiTree(T);
    FILE* fp = fopen(FileName, "r");
    TElemType definition[100] = { 0,'\0' };
    int i = 0;
    if (p1 != NULL) {
        *p1 = 0;
        *p2 = 0;
        *p3 = 0;
        *p4 = NULL;
        for (int i = 0; i < 500; ++i) *(p5 + i) = 0;
    }
   
    while (fscanf(fp, "%d %s", &definition[i].key, definition[i].others) != EOF) {
        ++i;
    }
    CreateBiTree(T, definition);
    fclose(fp);
    return OK;

}

int MaxPathSum(BiTree T) {
    /*（1）最大路径和：函数名称是MaxPathSum(T)，初始条件是二叉树T存在；
    操作结果是返回根节点到叶子结点的最大路径和；*/
    if (T == NULL) return 0;
    int left = MaxPathSum(T->lchild);
    int right = MaxPathSum(T->rchild);
    return (left > right ? left : right) + T->data.key;
}

BiTNode* LowestCommonAncestor(BiTree T, KeyType e1, KeyType e2) {
    /*（2）最近公共祖先：函数名称是LowestCommonAncestor(T,e1,e2)；
    初始条件是二叉树T存在；操作结果是该二叉树中e1节点和e2节点的最近公共祖先；*/
    if (T == NULL) return NULL;
    if (T->data.key == e1 || T->data.key == e2) return T;
    BiTNode* left = LowestCommonAncestor(T->lchild, e1, e2);
    BiTNode* right = LowestCommonAncestor(T->rchild, e1, e2);
    if (left != NULL && right != NULL) return T;
    else if (left != NULL) return left;
    else if (right != NULL) return right;
    else return NULL;
}

void InvertTree(BiTree T) {
    /*翻转二叉树：函数名称是InvertTree(T)，初始条件是线性表L已存在；
    操作结果是将T翻转，使其所有节点的左右节点互换；*/
    if (T == NULL) return;
    BiTree temp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = temp;
    InvertTree(T->lchild);
    InvertTree(T->rchild);
}

status AddtoSet(char TreeName[], BiTree& T) {   
            SaveBiTree(T, TreeName);
            return OK; 
}

status ChooseFromSet(char TreeName[], BiTree& T) {
            LoadBiTree(T, TreeName);
            return OK;
}

status DestroyBiTree(BiTree& T)
{
    if (T)
    {
        DestroyBiTree(T->lchild);
        DestroyBiTree(T->rchild);
        free(T);
        T = NULL;
    }
    return OK;
}

status Empty(BiTree T)
{
	if (T == NULL)
		return TRUE;
	else
		return FALSE;
}