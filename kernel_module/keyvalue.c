//////////////////////////////////////////////////////////////////////
//                             North Carolina State University
//
//
//
//                             Copyright 2016
//
////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
//
////////////////////////////////////////////////////////////////////////
//
//   Author:  Hung-Wei Tseng
//
//   Description:
//     Skeleton of KeyValue Pseudo Device
//
////////////////////////////////////////////////////////////////////////

#include "keyvalue.h"

#include <asm/uaccess.h>
#include <linux/slab.h> 
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/poll.h>


#define NULL (void*)0 

unsigned transaction_id;

struct node
{
    int key;
    char *data;
    struct node *left;
    struct node *right;
    int height;
};
 

 

int height(struct node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
 

int max(int a, int b)
{
    return (a > b)? a : b;
}
 

struct node* newNode(int key,char* data)
{
    struct node* node = (struct node*)
                        kmalloc(sizeof(struct node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->data =   data;
    node->height = 1; 
    return(node);
}
 

struct node *rightRotate(struct node *y)
{
    struct node *x = y->left;
    struct node *T2 = x->right;
 
 
    x->right = y;
    y->left = T2;
 
 
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
 

    return x;
}
 

struct node *leftRotate(struct node *x)
{
    struct node *y = x->right;
    struct node *T2 = y->left;
 
 
    y->left = x;
    x->right = T2;
 
   
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
 
   
    return y;
}
 

int getBalance(struct node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
 
struct node* insert(struct node* node, int key,char *value)
{
   
    if (node == NULL)
        return(newNode(key,value));

 
    if (key < node->key)
        node->left  = insert(node->left, key,value);
    else
        node->right = insert(node->right, key,value);
 

    node->height = max(height(node->left), height(node->right)) + 1;
 
   
    int balance = getBalance(node);
 
   
 

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
 
   
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
 

    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
   
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
   
    return node;
}
 

struct node * minValueNode(struct node* node)
{
    struct node* current = node;
 
    
    while (current->left != NULL)
        current = current->left;
 
    return current;
}
 
struct node* deleteNode(struct node* root, int key)
{
   
 
    if (root == NULL)
        return root;
 

    if ( key < root->key )
        root->left = deleteNode(root->left, key);

 

    else if( key > root->key )
        root->right = deleteNode(root->right, key);
 
 
    else
    {
       
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct node *temp = root->left ? root->left : root->right;
 
   
            if(temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else 
             *root = *temp; 
 
            free(temp);
        }
        else
        {
           
            struct node* temp = minValueNode(root->right);
 
           
            root->key = temp->key;
 
           
            root->right = deleteNode(root->right, temp->key);
        }
    }
 
   
    if (root == NULL)
      return root;
 
    
    root->height = max(height(root->left), height(root->right)) + 1;
 
    
    int balance = getBalance(root);
 
    
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
 

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
 

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
 

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
 
    return root;
}

 


 
struct node* search(struct node* root,int key)
{
if(root==NULL)
{
return NULL;
}
if(root->key==key)
{
return root;
}
if(root -> key < key )
{
return search(root->right,key );
}
if(root -> key > key )
{
return search(root->left,key );
}

}



static void free_callback(void *data)
{
}

static long keyvalue_get(struct keyvalue_get __user *ukv)
{
    struct keyvalue_get kv;
    
    printk("%d",*ukv-> key)
    

    return transaction_id++;
}

static long keyvalue_set(struct keyvalue_set __user *ukv)
{
    struct keyvalue_set kv;

    return transaction_id++;
}

static long keyvalue_delete(struct keyvalue_delete __user *ukv)
{
    struct keyvalue_delete kv;

    return transaction_id++;
}

//Added by Hung-Wei
     
unsigned int keyvalue_poll(struct file *filp, struct poll_table_struct *wait)
{
    unsigned int mask = 0;
    printk("keyvalue_poll called. Process queued\n");
    return mask;
}

static long keyvalue_ioctl(struct file *filp, unsigned int cmd,
                                unsigned long arg)
{
    switch (cmd) {
    case KEYVALUE_IOCTL_GET:
        return keyvalue_get((void __user *) arg);
    case KEYVALUE_IOCTL_SET:
        return keyvalue_set((void __user *) arg);
    case KEYVALUE_IOCTL_DELETE:
        return keyvalue_delete((void __user *) arg);
    default:
        return -ENOTTY;
    }
}

static int keyvalue_mmap(struct file *filp, struct vm_area_struct *vma)
{
    return 0;
}

static const struct file_operations keyvalue_fops = {
    .owner                = THIS_MODULE,
    .unlocked_ioctl       = keyvalue_ioctl,
    .mmap                 = keyvalue_mmap,
//    .poll		  = keyvalue_poll,
};

static struct miscdevice keyvalue_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "keyvalue",
    .fops = &keyvalue_fops,
};

static int __init keyvalue_init(void)
{
    int ret;

    if ((ret = misc_register(&keyvalue_dev)))
        printk(KERN_ERR "Unable to register \"keyvalue\" misc device\n");
    return ret;
}

static void __exit keyvalue_exit(void)
{
    misc_deregister(&keyvalue_dev);
}

MODULE_AUTHOR("Hung-Wei Tseng <htseng3@ncsu.edu>");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
module_init(keyvalue_init);
module_exit(keyvalue_exit);

