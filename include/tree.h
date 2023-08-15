/********************************************************************
* FILE: LexicalAnalyzer.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/
#pragma once

// variables in tree.cxx
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int status;
typedef int KeyType;
typedef struct {
    KeyType  key;
    char others[20];
} TElemType;    //二叉树结点的数据类型
typedef struct BiTNode {
    TElemType  data;
    struct BiTNode* lchild, * rchild;
} BiTNode, * BiTree; //二叉树的结点类型
typedef struct {
    char Name[20];
    BiTree T;
}Trees; //全局变量，用于存储二叉树

extern FILE* fp;
extern Trees Set[10];
extern int* p1, * p2, * p3;
extern BiTree* p4;
extern int* p5;





//函数声明
status CreateBiTree(BiTree& T, TElemType definition[]);//根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务
status ClearBiTree(BiTree& T);//将二叉树设置成空，并删除所有结点，释放结点空间
int BiTreeDepth(BiTree T);//求二叉树T的深度
BiTNode* LocateNode(BiTree T, KeyType e);//查找结点
status Assign(BiTree& T, KeyType e, TElemType value);//实现结点赋值。此题允许通过增加其它函数辅助实现本关任务
BiTNode* GetSibling(BiTree T, KeyType e);//实现获得兄弟结点
status InsertNode(BiTree& T, KeyType e, int LR, TElemType c);  //插入结点。此题允许通过增加其它函数辅助实现本关任务
BiTree ParentNode(BiTree& T, KeyType e);//实现获得父结点
status DeleteNode(BiTree& T, KeyType e);//删除结点。此题允许通过增加其它函数辅助实现本关任务
status PreOrderTraverse(BiTree T, void (*visit)(BiTree));//先序遍历二叉树T
status InOrderTraverse(BiTree T, void (*visit)(BiTree));//中序遍历二叉树T
status PostOrderTraverse(BiTree T, void (*visit)(BiTree));//后序遍历二叉树T
status LevelOrderTraverse(BiTree T, void (*visit)(BiTree));//按层遍历二叉树T
void ReadNode(BiTree T);//辅助函数，用于将结点数据写入文件
status PreOrderTraverse1(BiTree T, void (*visit)(BiTree));//先序遍历二叉树T
status SaveBiTree(BiTree T, char FileName[]);//将二叉树的结点数据写入到文件FileName中
status LoadBiTree(BiTree& T, char FileName[]);//读入文件FileName的结点数据，创建二叉树
int MaxPathSum(BiTree T);//最大路径和
BiTNode* LowestCommonAncestor(BiTree T, KeyType e1, KeyType e2);//最近公共祖先
void InvertTree(BiTree T);//翻转二叉树
void PrintNode(BiTree T); //辅助函数，用于输出结点数据
status AddtoSet(char TreeName[], BiTree& T);//将二叉树T添加到集合Set中，集合名称为TreeName
status ChooseFromSet(char TreeName[], BiTree& T);//从集合Set中选择名称为TreeName的二叉树，赋值给T
status DestroyBiTree(BiTree& T);//销毁二叉树T
status Empty(BiTree T);
