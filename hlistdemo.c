/*
 *  Description : linux应用层编程之哈希链表hlist的使用
 *  Date        ：20180713
 *  Author      ：fuyuande
 *  Mail        : mrsonko@126.com
 *
 */


#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "hlistdemo.h"

#define log(fmt, arg...) printf(""fmt,##arg)
#define MAX_ADDR 256

void main(int argc, char **argv){
    struct hlist_head htable[MAX_ADDR];
    struct hdata_node *hnode = NULL;
    struct hlist_node *hlist = NULL, *n = NULL;
    int i = 0, quit = 1, opt = 0, key;
    unsigned int data;

    /* hlist_head 动态初始化 */
    for(i = 0; i < MAX_ADDR; i++)
        INIT_HLIST_HEAD(&htable[i]);
        
    log("*********************\n\n"
        "input options:\n"
        "1: insert\n"           //插入
        "2: serach\n"           //查询
        "3: delete\n"           //删除
        "0: quit\n"
        "\n*********************\n");  
        
    while(quit != 0){
        log("\ninput options:");
        scanf("%d",  &opt);
        switch(opt){
            //插入
            case  1:
                //分配结点
                hnode = calloc(1, sizeof(struct hdata_node));
                if(!hnode){
                    log("insert fail \n");
                    break;
                }
                //hlist_node 初始化
                INIT_HLIST_NODE(&hnode->list);
                
                log("\ninput data:");
                scanf("%d",  &hnode->data);
                key = hnode->data % MAX_ADDR;
                //添加到链表首部
                hlist_add_head(&hnode->list, &htable[key]);
                break;

            //查询
            case  2:
                log("\ninput data:");
                scanf("%d",  &data);         
                key = data % MAX_ADDR;
                if(hlist_empty(&htable[key]))
                    log("data not exist \n");
                else{
                    //遍历对应的槽，匹配值就打印
                    hlist_for_each_entry(hnode, hlist, &htable[key], list){
                        if(hnode->data == data)
                            log("find data : %u \n", hnode->data);
                    }
                }                
                break;

            //删除
            case  3:
                log("\ninput data:");
                scanf("%d",  &data);         
                key = data % MAX_ADDR;
                if(hlist_empty(&htable[key]))
                    log("data not exist ，delete fail \n");
                else{
                    //遍历对应的槽，匹配值就删除
                    hlist_for_each_entry_safe(hnode, hlist, n, &htable[key], list){
                        if(hnode->data == data){
                            hlist_del(&hnode->list);
                            break;
                        }
                    }
                }                            
                log("delete fail\n");                
                break;
            case 0:
                quit = 0;
                break;
            default:
                log("unrecognized option!");
                break;                
        }
    }
    for(i=0; i < MAX_ADDR; i++){
        //遍历每一个槽，有结点就删除
        hlist_for_each_entry_safe(hnode, hlist, n, &htable[i], list){           
                hlist_del(&hnode->list); 
                log("delete %u \n", hnode->data);
                free(hnode);
                hnode = NULL;
        }
    }
    log("exit\n");
}
